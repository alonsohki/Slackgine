/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        model.cpp
// PURPOSE:     l3m models.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>
#include <cstring>
#include <zlib.h>
#include "l3m.h"
#include "components/factory.h"
#include "components/unknown.h"

using namespace l3m;

static const char* const L3M_BOM = "L3M\x01";
static const float L3M_VERSION = 2.0f;
static const enum
{
    L3M_MACHINE_ENDIAN,
    L3M_LOW_ENDIAN,
    L3M_BIG_ENDIAN
} L3M_SAVE_ENDIANNESS = L3M_LOW_ENDIAN;

Model::Model ()
{
    m_size = 0;
    mCompressionLevel = 9;
}

Model::~Model ()
{
    for ( ComponentVector::const_iterator iter = m_vecComponents.begin();
            iter != m_vecComponents.end();
            ++iter )
    {
        sgDelete *iter;
    }
    m_vecComponents.clear ();
}


//------------------------------------------------------------------------------
// Model load/save
bool Model::load ( const std::string& filepath )
{
    std::fstream fp;
    fp.open(filepath.c_str(), std::ios::in | std::ios::binary);
    if ( fp.fail() )
        return setError ( "Unable to open '%s' for reading", filepath.c_str() );
    return load ( fp );
}

bool Model::save(const std::string& filepath)
{
    std::fstream fp;
    fp.open(filepath.c_str(), std::ios::out | std::ios::binary);
    if ( fp.fail() )
        return setError ( "Unable to open '%s' for writing", filepath.c_str() );
    return save ( fp );
}

bool Model::load(std::istream& fp)
{
    // Make sure there aren't any unallowed delta resolvers.
    m_deltas.clear();
    
    // Check BOM
    char BOM [ 8 ];
    fp.read ( BOM, sizeof(char)*strlen(L3M_BOM) );
    if ( memcmp ( BOM, L3M_BOM, strlen(L3M_BOM) ) != 0 )
        return setError ( "Unable to read the BOM" );

    // Check for endianness swapping
    u8 targetIsBigEndian;
    fp.read ( reinterpret_cast<char*>(&targetIsBigEndian), 1 );
    u8 thisMachineIsBigEndian = detectBigEndian();
    bool doEndianSwapping = ( thisMachineIsBigEndian != targetIsBigEndian );
    
    u32 flags = IOStream::NONE;
    if ( doEndianSwapping )
        flags |= IOStream::ENDIAN_SWAPPING;
    
    // Generate the input stream.
    IStream is ( &fp, flags );
    u32 readFlags = IOStream::NONE;
    if ( is.read32 ( &readFlags, 1 ) != 1 )
        return setError ( "Unable to read the flags" );
    readFlags &= ~IOStream::ENDIAN_SWAPPING;
    readFlags |= flags & IOStream::ENDIAN_SWAPPING;
    is.setFlags ( readFlags );
    
    // Read and check the version
    float version;
    if ( is.readFloat ( &version, 1 ) != 1 )
        return setError ( "Unable to read the L3M version" );
    if ( version > L3M_VERSION )
        return setError ( "Unsupported L3M version" );
    
    // Read the number of components
    u32 numComponents;
    if ( is.read32(&numComponents, 1) != 1 )
        return setError ( "Unable to read the number of components" );

    for ( u32 i = 0; i < numComponents; ++i )
    {
        // Read the component type
        std::string type;
        if ( is.readStr ( &type ) <= 0 )
            return setError ( "Unable to read the component type" );
        if ( type == "unknown" )
            return setError ( "Invalid component type 'unknown'" );
        
        // Read the component version
        if ( is.readFloat ( &version, 1 ) != 1 )
            return setError ( "Unable to read the component version for a component of type '%s'", type.c_str() );
        
        // Load the component data length
        u32 len;
        if ( is.read32 ( &len, 1 ) != 1 )
            return setError ( "Unable to read the component data length" );
        
        // Check if the chunk is compressed
        b8 isCompressed;
        if ( is.readBoolean(&isCompressed) == false )
            return setError ( "Error reading the compression marker" );
        
        // Reserve memory for the component data
        u32 compressedLength = 0;
        char* data;
        if ( isCompressed == true )
        {
            if ( is.read32 ( &compressedLength ) == false )
                return setError ( "Error reading the compressed component original length" );
            data = new char [ len + compressedLength ];
            if ( is.readData( &data[len], compressedLength, 1 ) != 1 )
            {
                delete [] data;
                return setError ( "Error reading the compressed chunk" );
            }
            uLongf destLen = len;
            if ( uncompress((Bytef*)&data[0], &destLen, (Bytef*)&data[len], compressedLength) != Z_OK )
            {
                delete [] data;
                return setError ( "Error uncompressing the component data" );
            }
        }
        else
        {
            data = new char [ len ];
            if ( is.readData ( &data[0], len, 1 ) != 1 )
            {
                delete [] data;
                return setError ( "Error reading the component data" );
            }
        }
        
        // Wrap the buffer into a stream
        boost::iostreams::stream<boost::iostreams::array_source> iss ( data, len );
        IStream dataStream ( &iss, is.flags() );
        
        IComponent* component = ComponentFactory::create( type );
        if ( component == 0 )
        {
            // If we don't know how to handle this component type, create an unknown component.
            component = sgNew l3m::UnknownComponent ( type, version, len, isCompressed, compressedLength );
        }
        
        // Load the component data
        if ( component->load ( this, dataStream, version ) == false )
        {
            delete [] data;
            return setError ( "Unable to load a component of type '%s': %s", type.c_str(), component->error() );
        }
        delete [] data;
        m_vecComponents.push_back ( component );
    }
    
    m_size = is.totalIn ();
    
    // Proceed with the delta resolution now.
    resolveDeltas ();
    
    return true;
}

bool Model::save(std::ostream& fp)
{
    OStream os ( &fp, IOStream::NONE );
    
    // BOM
    os.writeData ( L3M_BOM, sizeof(char), strlen(L3M_BOM) );
    
    // Write the BOM endianness identifier, based on the machine endianness and desired configuration
    u8 thisMachineIsBigEndian = detectBigEndian ();
    u8 targetIsBigEndian;
    
    if ( L3M_SAVE_ENDIANNESS == L3M_MACHINE_ENDIAN )
        targetIsBigEndian = thisMachineIsBigEndian;
    else
        targetIsBigEndian = ( L3M_SAVE_ENDIANNESS == L3M_BIG_ENDIAN );
    os.writeData ( reinterpret_cast<char*>(&targetIsBigEndian), sizeof(u8), 1 );
    
    // Flags
    u32 flags = os.flags() & ~( IOStream::ENDIAN_SWAPPING | IOStream::COMPRESSION );
    if ( mCompressionLevel > 0 )
        flags |= IOStream::COMPRESSION;
    if ( os.write32 ( &flags, 1 ) != 1 )
        return setError ( "Unable to write the stream flags" );
    
    // Versión
    float fVersion = L3M_VERSION;
    if ( os.writeFloat ( &fVersion, 1 ) != 1 )
        return setError ( "Unable to write the L3M version" );
    
    // Number of components
    u32 numComponents = m_vecComponents.size ();
    if ( os.write32 ( &numComponents, 1 ) != 1 )
        return setError ( "Unable to write the number of components" );
    
    // Write each component
    for ( u32 i = 0; i < numComponents; ++i )
    {
        // Write the component type
        os.writeStr ( m_vecComponents[i]->type() );
        
        // Write the component version
        float version = m_vecComponents[i]->version();
        if ( os.writeFloat ( &version, 1 ) != 1 )
            return setError ( "Unable to write the component version" );
        
        // Write the component to a temporary buffer
        std::ostringstream oss;
        OStream data ( &oss, os.flags() );
        if ( !m_vecComponents[i]->save ( this, data ) )
            return setError ( m_vecComponents[i]->error() );
        
        // Write this buffer length
        u32 len = oss.str().length();
        if ( !os.write32 ( len ) )
            return setError ( "Error writing the component data length" );
        
        if ( m_vecComponents[i]->type() == "unknown" )
        {
            UnknownComponent* unk = static_cast < UnknownComponent* > ( m_vecComponents[i] );
            if ( !os.writeBoolean(unk->shouldCompress()) )
                return setError ( "Error writing the compression marker" );
            if ( !os.write32 (unk->compressedLength()) )
                return setError ( "Error writing the unknown component compressed length" );
            if ( !os.writeData ( oss.str().c_str(), len, 1 ) )
                return setError ( "Error writing the data chunk" );
        }
        if ( mCompressionLevel > 0 && m_vecComponents[i]->shouldCompress() == true )
        {
            if ( !os.writeBoolean(true) )
                return setError ( "Error writing the compression marker" );
            
            // Compress the buffer
            uLongf bufferLength = compressBound ( len );
            Bytef* buffer = new Bytef [ bufferLength ];
            if ( compress2 ( buffer, &bufferLength, (Bytef*)oss.str().c_str(), len, mCompressionLevel) != Z_OK )
            {
                delete [] buffer;
                return setError ( "Error compressing the component data chunk" );
            }
            
            // Write the compressed length
            u32 compressedLength = static_cast < u32 > ( bufferLength );
            if ( !os.write32 ( compressedLength ) )
            {
                delete [] buffer;
                return setError ( "Error writing the component compressed data chunk size" );
            }
            
            // Write the data chunk
            if ( !os.writeData ( (const char *)buffer, compressedLength, 1 ) )
            {
                delete [] buffer;
                return setError ( "Error writing the compressed data chunk" );
            }
            delete [] buffer;
        }
        else
        {
            if ( !os.writeBoolean(false) )
                return setError ( "Error writing the compression marker" );
            if ( !os.writeData ( oss.str().c_str(), len, 1 ) )
                return setError ( "Error writing the data chunk" );
        }
    }
    
    return true;
}

void Model::resolveDeltas ()
{
    for ( DeltaResolverMap::iterator iter = m_deltas.begin();
          iter != m_deltas.end();
          ++iter )
    {
        for ( std::vector<DeltaData>::iterator iter2 = iter->second.begin();
              iter2 != iter->second.end();
              ++iter2 )
        {
            DeltaData& dd = *iter2;
            dd.fn ( iter->first, this, dd.data );
        }
    }
    
    m_deltas.clear ();
}


//------------------------------------------------------------------------------
// Component stuff
IComponent* Model::createComponent(const std::string& type)
{
    IComponent* component = ComponentFactory::create( type );
    m_vecComponents.push_back(component);
    return component;
}


//------------------------------------------------------------------------------
// Compression
void Model::setCompressionLevel(u32 level)
{
    mCompressionLevel = min ( level, (u32)9 );
}
