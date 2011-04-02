#include <arpa/inet.h>
#include <cerrno>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include "l3m.h"

l3m::l3m ( const std::string& type )
: m_type ( type )
{
    InitializeEndianness ();
}

l3m::~l3m()
{
    for ( groupMap::iterator iter = m_groups.begin (); iter != m_groups.end(); ++iter )
    {
        for ( meshList::iterator iter2 = iter->second.begin(); iter2 != iter->second.end(); ++iter2 )
        {
            delete *iter2;
        }
    }
}

bool l3m::SaveToFile ( const char* path ) const
{
    FILE* fp = fopen ( path, "wb" );
    bool status;
    if ( !fp ) return false;
    
    status = SaveToFile(fp);
    fclose ( fp );
    return status;
}

bool l3m::LoadFromFile ( const char* path )
{
    FILE* fp = fopen ( path, "rb" );
    bool status;
    if ( !fp ) return false;
    
    status = LoadFromFile(fp);
    fclose(fp);
    return status;
}

l3m::meshList* l3m::FindGroup ( const std::string& name )
{
    l3m::groupMap::iterator iter = m_groups.find ( name );
    if ( iter != m_groups.end () )
        return &(iter->second);
    return 0;
}

void l3m::LoadMesh(Mesh* mesh, const std::string& group)
{
    meshList* list = FindGroup ( group );
    if ( list == 0 )
    {
        meshList newList;
        newList.push_back ( mesh );
        m_groups.insert ( groupMap::value_type(group, newList) );
    }
    else
    {
        list->push_back ( mesh );
    }
}




// Endianness functions
template < typename T >
static size_t identityWrite ( const T* v, uint32_t count, FILE* fp )
{
    return fwrite ( v, sizeof(T), count, fp );
}

template < typename T >
static size_t identityRead ( T* v, uint32_t count, FILE* fp )
{
    return fread ( v, sizeof(T), count, fp );
}

static size_t swap16Write ( const uint16_t* v, uint32_t count, FILE* fp )
{
    uint16_t current;
    for ( uint32_t i = 0; i < count; ++i )
    {
        current = ( ( v[i] >> 8 ) & 0x00FF ) | ( v[i] << 8 & 0xFF00 );
        if ( fwrite ( &current, sizeof(uint16_t), 1, fp ) < 1 )
            return i;
    }
    return count;
}

static size_t swap16Read ( uint16_t* v, uint32_t count, FILE* fp )
{
    uint16_t current;
    for ( uint32_t i = 0; i < count; ++i )
    {
        if ( fread ( &current, sizeof(uint16_t), 1, fp ) < 1 )
            return i;
        v[i] = ( ( current >> 8 ) & 0x00FF ) | ( current << 8 & 0xFF00 );
    }
    return count;
}

static size_t swap32Write ( const uint32_t* v, uint32_t count, FILE* fp )
{
    uint32_t current;
    for ( uint32_t i = 0; i < count; ++i )
    {
        current =  ( ( v[i] >> 24 ) & 0x000000FF ) |
                   ( ( v[i] >> 8  ) & 0x0000FF00 ) |
                   ( ( v[i] << 8  ) & 0x00FF0000 ) |
                   ( ( v[i] << 24 ) & 0xFF000000 );
        if ( fwrite ( &current, sizeof(uint32_t), 1, fp ) < 1 )
            return i;
    }
    return count;
}

static size_t swap32Read ( uint32_t* v, uint32_t count, FILE* fp )
{
    uint32_t current;
    for ( uint32_t i = 0; i < count; ++i )
    {
        if ( fread ( &current, sizeof(uint32_t), 1, fp ) < 1 )
            return i;
        v[i] =  ( ( current >> 24 ) & 0x000000FF ) |
                ( ( current >> 8  ) & 0x0000FF00 ) |
                ( ( current << 8  ) & 0x00FF0000 ) |
                ( ( current << 24 ) & 0xFF000000 );
    }
    return count;
}

void l3m::InitializeEndianness()
{
    // Check if this machine is big endian
    unsigned char thisMachineIsBigEndian = htons(0xFFF1) == 0xFFF1;
    
    // Check if we must write big endian files
    unsigned char targetIsBigEndian;
    if ( L3M_SAVE_ENDIANNESS == L3M_MACHINE_ENDIAN )
        targetIsBigEndian = thisMachineIsBigEndian;
    else
        targetIsBigEndian = ( L3M_SAVE_ENDIANNESS == L3M_BIG_ENDIAN );
    
    // Choose the endianness swapping strategy
    if ( targetIsBigEndian != thisMachineIsBigEndian )
    {
        m_endian16writer = swap16Write;
        m_endian32writer = swap32Write;
    }
    else
    {
        m_endian16writer = identityWrite<uint16_t>;
        m_endian32writer = identityWrite<uint32_t>;
    }
}

bool l3m::Write16 ( const uint16_t* v, uint32_t nmemb, FILE* fp ) const
{
    return m_endian16writer ( v, nmemb, fp) >= nmemb;
}
size_t l3m::Read16 ( uint16_t* v, uint32_t nmemb, FILE* fp ) const
{
    return m_endian16reader ( v, nmemb, fp );
}
bool l3m::Write32 ( const uint32_t* v, uint32_t nmemb, FILE* fp ) const
{
    return m_endian32writer ( v, nmemb, fp ) >= nmemb;
}
size_t l3m::Read32 ( uint32_t* v, uint32_t nmemb, FILE* fp ) const
{
    return m_endian32reader ( v, nmemb, fp );
}
bool l3m::WriteFloat ( const float* v, uint32_t nmemb, FILE* fp ) const
{
    return m_endian32writer ( reinterpret_cast<const uint32_t*>(v), nmemb, fp ) >= nmemb;
}
size_t l3m::ReadFloat ( float* v, uint32_t nmemb, FILE* fp ) const
{
    return m_endian32writer ( reinterpret_cast<const uint32_t*>(v), nmemb, fp );
}
bool l3m::WriteStr ( const std::string& str, FILE* fp ) const
{
    uint32_t length = str.length ();
    if ( !Write32 ( &length, 1, fp ) )
        return false;
    return WriteData ( str.c_str(), sizeof(char), length, fp );
}
size_t l3m::ReadStr ( std::string& dest, FILE* fp ) const
{
    uint32_t length;
    if ( Read32( &length, 1, fp ) != 1 )
        return -1;
    char* buffer = new char [ length ];
    if ( ReadData ( buffer, sizeof(char), length, fp ) != length )
    {
        delete [] buffer;
        return -1;
    }
    dest.assign ( buffer, length );
    delete [] buffer;
    return length;
}

bool l3m::WriteData ( const void* data, size_t size, unsigned int nmemb, FILE* fp ) const
{
    size_t s = fwrite ( data, size, nmemb, fp );
    if ( s < nmemb )
        return false;
    return true;
}

size_t l3m::ReadData ( char* dest, size_t size, unsigned int nmemb, FILE* fp ) const
{
    return fread ( dest, size, nmemb, fp );
}

bool l3m::SaveToFile ( FILE* fp ) const
{
    unsigned int npos = (unsigned int)-1;
    unsigned int zero = 0;
    unsigned int i;
    
    #define FWRITE(data, size, nmemb, fp) if ( ! WriteData(data, size, nmemb, fp) ) return false
    #define FWRITE16(data, nmemb, fp) if ( ! Write16(reinterpret_cast<uint16_t*>(data), nmemb, fp) ) return false
    #define FWRITE32(data, nmemb, fp) if ( ! Write32(reinterpret_cast<uint32_t*>(data), nmemb, fp) ) return false
    #define FWRITEF(data, nmemb, fp) if ( ! WriteData(reinterpret_cast<float*>(data), sizeof(float), nmemb, fp) ) return false
    #define FWRITE_STR(str, fp) if ( ! WriteStr(str,fp) ) return false

    // BOM
    FWRITE ( L3M_BOM, sizeof(char), strlen(L3M_BOM), fp );
    
    // Write the BOM endianness identifier, based on the machine endianness and desired configuration
    unsigned char thisMachineIsBigEndian = htons(0xFFF1) == 0xFFF1;
    unsigned char targetIsBigEndian;
    
    if ( L3M_SAVE_ENDIANNESS == L3M_MACHINE_ENDIAN )
        targetIsBigEndian = thisMachineIsBigEndian;
    else
        targetIsBigEndian = ( L3M_SAVE_ENDIANNESS == L3M_BIG_ENDIAN );
    FWRITE ( &targetIsBigEndian, sizeof(unsigned char), 1, fp );
    
    // Versión
    float fVersion = L3M_VERSION;
    FWRITEF ( &fVersion, 1, fp );
    
    // Type
    FWRITE_STR ( type(), fp );

    // Offset 2 TXD
    long off2TXD = ftell ( fp );
    FWRITE32 ( &npos, 1, fp );
    
    // Offset 2 Meta
    long off2Meta = ftell ( fp );
    FWRITE32 ( &npos, 1, fp );
    
    // Groups
    unsigned int numGroups = m_groups.size();
    FWRITE32 ( &numGroups, 1, fp );
    
    // Write each group
    long* groupOffsetRefs = new long [ m_groups.size() ];
    i = 0;
    for ( groupMap::const_iterator iter = m_groups.begin(); iter != m_groups.end(); ++iter )
    {
        // Group name
        FWRITE_STR ( iter->first, fp );
        
        // Save offset position
        groupOffsetRefs[i] = ftell ( fp );
        ++i;
        FWRITE32 ( &npos, 1, fp );
    }
    
    // For each group, write its meshes and fill the ref
    unsigned int group = 0;
    for ( groupMap::const_iterator iter = m_groups.begin(); iter != m_groups.end(); ++iter )
    {
        // Fill the ref
        long ref = ftell ( fp );
        fseek ( fp, groupOffsetRefs[group], SEEK_SET );
        FWRITE32 ( &ref, 1, fp );
        fseek ( fp, 0, SEEK_END );
        
        // Write the meshes headers
        const meshList& meshes = iter->second;
        unsigned int numMeshes = meshes.size ();
        FWRITE32 ( &numMeshes, 1, fp );
        long* meshOffsetRefs = new long [ numMeshes ];
        
        i = 0;
        for ( meshList::const_iterator iter2 = meshes.begin(); iter2 != meshes.end(); ++iter2 )
        {
            Mesh* mesh = *iter2;
            
            // Write the mesh name
            FWRITE_STR ( mesh->name(), fp );
            
            // Keep the mesh offset position
            meshOffsetRefs[i] = ftell ( fp );
            ++i;
            FWRITE32 ( &npos, 1, fp );
        }
        
        
        // Write the mesh data
        unsigned int currentMesh = 0;
        
        for ( meshList::const_iterator iter2 = meshes.begin(); iter2 != meshes.end(); ++iter2 )
        {
            Mesh* mesh = *iter2;
            
            // Fill the ref
            ref = ftell(fp);
            fseek ( fp, meshOffsetRefs [ currentMesh ], SEEK_SET );
            FWRITE32 ( &ref, 1, fp );
            fseek ( fp, 0, SEEK_END );
            
            // Write the vertex data
            unsigned int num = mesh->numVertices();
            FWRITE32 ( &num, 1, fp );
            FWRITEF ( mesh->vertices(), (num * sizeof(Vertex)) / sizeof(float), fp );
            
            // Write the face data
            num = mesh->numFaces();
            FWRITE32 ( &num, 1, fp );
            FWRITEF ( mesh->faces(), (num * sizeof(Face)) / sizeof(unsigned int), fp );
        }
        
        delete [] meshOffsetRefs;
    }
    delete [] groupOffsetRefs;
    
    
    // Write the ref to the TXDs
    long ref = ftell ( fp );
    fseek ( fp, off2TXD, SEEK_SET );
    FWRITE32 ( &ref, 1, fp );
    fseek ( fp, 0, SEEK_END );
    // TODO: Implement TXD support
    FWRITE32 ( &zero, 1, fp );
    
    // Write the ref to the metadata
    ref = ftell ( fp );
    fseek ( fp, off2Meta, SEEK_SET );
    FWRITE32 ( &ref, 1, fp );
    fseek ( fp, 0, SEEK_END );
    // TODO: Implement metadata support
    FWRITE32 ( &zero, 1, fp );
    
#undef FWRITE
#undef FWRITE_STR
    
    return true;
}


bool l3m::LoadFromFile ( FILE* fp )
{
    char buffer [ 1024 ];
    size_t size;
    
    #define FREAD(data, _size, nmemb, fp) if ( ( size = ReadData(data, _size, nmemb, fp) ) != nmemb ) return false
    #define FREAD16(data, nmemb, fp) if ( ! ( size = Read16(reinterpret_cast<uint16_t*>(data), nmemb, fp) ) != nmemb ) return false
    #define FREAD32(data, nmemb, fp) if ( ! ( size = Read32(reinterpret_cast<uint32_t*>(data), nmemb, fp) ) != nmemb ) return false
    #define FREADF(data, nmemb, fp) if ( ! ( size = ReadData(reinterpret_cast<char*>(data), sizeof(float), nmemb, fp) ) != nmemb ) return false
    #define FREAD_STR(str, fp) if ( ! ( size = ReadStr(str,fp) ) != nmemb ) return false

    // Read out the BOM marker
    FREAD ( buffer, sizeof(char), strlen(L3M_BOM), fp );
    if ( memcmp ( buffer, L3M_BOM, strlen(L3M_BOM) ) != 0 )
        return false;
    FREAD ( buffer, sizeof(char), 1, fp );
 
    // Choose the endianness strategy
    unsigned char thisMachineIsBigEndian = htons(0xFFF1) == 0xFFF1;
    unsigned char targetIsBigEndian = buffer[0];
    
    if ( thisMachineIsBigEndian != targetIsBigEndian )
    {
        m_endian16reader = swap16Read;
        m_endian32reader = swap32Read;
    }
    else
    {
        m_endian16reader = identityRead<uint16_t>;
        m_endian32reader = identityRead<uint32_t>;
    }

    float fVersion;
    FREADF(&fVersion, 1, fp);
    printf ( "Versión leída: %f\n", fVersion );
    return true;
}