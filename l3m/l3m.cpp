#include <arpa/inet.h>
#include <cerrno>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include "l3m.h"

l3m::l3m ( const std::string& type )
: m_type ( type )
{
}

l3m::l3m(const l3m& orig)
: m_type ( orig.m_type )
{
    // TODO
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

// Endianness functions
template < typename T >
static size_t identity ( T* v, uint32_t count, FILE* fp )
{
    return fwrite ( v, sizeof(T), count, fp );
}

size_t swap16 ( uint16_t* v, uint32_t count, FILE* fp )
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
size_t swap32 ( uint32_t* v, uint32_t count, FILE* fp )
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

static bool WriteData ( const void* data, size_t size, unsigned int nmemb, FILE* fp )
{
    size_t s = fwrite ( data, size, nmemb, fp );
    if ( s < nmemb )
        return false;
    return true;
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

bool l3m::SaveToFile ( FILE* fp ) const
{
    unsigned int npos = (unsigned int)-1;
    unsigned int zero = 0;
    unsigned int i;
    size_t (*endian16writer)(uint16_t*, uint32_t, FILE*);
    size_t (*endian32writer)(uint32_t*, uint32_t, FILE*);
    
    #define FWRITE(data, size, nmemb, fp) if ( ! WriteData(data, size, nmemb, fp) ) return false
    #define FWRITE16(data, nmemb, fp) if ( endian16writer(reinterpret_cast<uint16_t*>(data), nmemb, fp) < nmemb ) return false;
    #define FWRITE32(data, nmemb, fp) if ( endian32writer(reinterpret_cast<uint32_t*>(data), nmemb, fp) < nmemb ) return false;
    #define FWRITEF(data, nmemb, fp) FWRITE32(reinterpret_cast<uint32_t*>(data),nmemb,fp)
    #define FWRITE_STR(str, fp) { \
                unsigned int __size__ = str .length(); \
                FWRITE32(&__size__, 1, fp ); \
            } \
            FWRITE ( str .c_str(), sizeof(char), str .length(), fp )

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
    
    // Choose the endianness swapping strategy
    
    if ( targetIsBigEndian != thisMachineIsBigEndian )
    {
        endian16writer = swap16;
        endian32writer = swap32;
    }
    else
    {
        endian16writer = identity<uint16_t>;
        endian32writer = identity<uint32_t>;
    }
    
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
