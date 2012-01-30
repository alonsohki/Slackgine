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

#include <cstring>
#include "l3m.h"
#include "Components/factory.h"
#include "Components/unknown.h"

using namespace l3m;

static const char* const L3M_BOM = "L3M\x01";
static const float L3M_VERSION = 2.0f;
static const enum
{
    L3M_MACHINE_ENDIAN,
    L3M_LOW_ENDIAN,
    L3M_BIG_ENDIAN
} L3M_SAVE_ENDIANNESS = L3M_LOW_ENDIAN;

bool Model::Load(std::istream& fp)
{
    // Check BOM
    char BOM [ 8 ];
    fp.read ( BOM, sizeof(char)*strlen(L3M_BOM) );
    if ( memcmp ( BOM, L3M_BOM, strlen(L3M_BOM) ) != 0 )
        return SetError ( "Unable to read the BOM" );

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
    if ( is.Read32 ( &readFlags, 1 ) != 1 )
        return SetError ( "Unable to read the flags" );
    readFlags &= ~IOStream::ENDIAN_SWAPPING;
    readFlags |= flags & IOStream::ENDIAN_SWAPPING;
    is.SetFlags ( readFlags );
    
    // Read and check the version
    float version;
    if ( is.ReadFloat ( &version, 1 ) != 1 )
        return SetError ( "Unable to read the L3M version" );
    if ( version > L3M_VERSION )
        return SetError ( "Unsupported L3M version" );
    
    // Read the number of components
    u32 numComponents;
    if ( is.Read32(&numComponents, 1) != 1 )
        return SetError ( "Unable to read the number of components" );

    for ( u32 i = 0; i < numComponents; ++i )
    {
        // Read the component type
        std::string type;
        if ( is.ReadStr ( type ) <= 0 )
            return SetError ( "Unable to read the component type" );
        if ( type == "unknown" )
            return SetError ( "Invalid component type 'unknown'" );
        
        // Read the component version
        if ( is.ReadFloat ( &version, 1 ) != 1 )
            return SetError ( "Unable to read the component version for a component of type '%s'", type.c_str() );
        
        // Load the component data length
        u32 len;
        if ( is.Read32 ( &len, 1 ) != 1 )
            return SetError ( "Unable to read the component data length" );
        
        IComponent* component = ComponentFactory::Create( type );
        if ( component == 0 )
        {
            // If we don't know how to handle this component type, create an unknown component.
            component = new l3m::UnknownComponent ( type, version, len );
        }
        
        // Load the component data
        if ( component->Load ( this, is, version ) == false )
            return SetError ( "Unable to load a component of type '%s': %s", type.c_str(), component->error() );
        
        m_vecComponents.push_back ( component );
    }
    
    m_size = is.totalIn ();
    
    ResolveDeltas ();
    
    return true;
}

bool Model::Save(std::ostream& fp)
{
    OStream os ( &fp, IOStream::NONE );
    
    // BOM
    os.WriteData ( L3M_BOM, sizeof(char), strlen(L3M_BOM) );
    
    // Write the BOM endianness identifier, based on the machine endianness and desired configuration
    u8 thisMachineIsBigEndian = detectBigEndian ();
    u8 targetIsBigEndian;
    
    if ( L3M_SAVE_ENDIANNESS == L3M_MACHINE_ENDIAN )
        targetIsBigEndian = thisMachineIsBigEndian;
    else
        targetIsBigEndian = ( L3M_SAVE_ENDIANNESS == L3M_BIG_ENDIAN );
    os.WriteData ( reinterpret_cast<char*>(&targetIsBigEndian), sizeof(u8), 1 );
    
    // Flags
    u32 flags = os.flags() & ~IOStream::ENDIAN_SWAPPING;
    if ( os.Write32 ( &flags, 1 ) != 1 )
        return SetError ( "Unable to write the stream flags" );
    
    // Versión
    float fVersion = L3M_VERSION;
    if ( os.WriteFloat ( &fVersion, 1 ) != 1 )
        return SetError ( "Unable to write the L3M version" );
    
    // Number of components
    u32 numComponents = m_vecComponents.size ();
    if ( os.Write32 ( &numComponents, 1 ) != 1 )
        return SetError ( "Unable to write the number of components" );
    
    // Write each component
    for ( u32 i = 0; i < numComponents; ++i )
    {
        // Write the component type
        os.WriteStr ( m_vecComponents[i]->type() );
        
        // Write the component version
        float version = m_vecComponents[i]->version();
        if ( os.WriteFloat ( &version, 1 ) != 1 )
            return SetError ( "Unable to write the component version" );
        
        // Write the component to a temporary buffer
        std::ostringstream oss;
        OStream data ( &oss, IOStream::NONE );
        if ( !m_vecComponents[i]->Save ( this, data ) )
            return SetError ( m_vecComponents[i]->error() );
        
        // Write this buffer length
        u32 len = oss.str().length();
        if ( !os.Write32 ( &len, 1 ) )
            return SetError ( "Error writing the component data length" );
        os.WriteData ( oss.str().c_str(), len, 1 );
    }
    
    return true;
}

void Model::ResolveDeltas ()
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