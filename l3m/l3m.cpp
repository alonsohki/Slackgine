#include <arpa/inet.h>
#include <cerrno>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include "l3m.h"

l3m::l3m ()
: m_type ( "default" )
, m_isDynamic ( false )
, m_rendererData ( 0 )
{
    InitializeEndianness ();
    SetError ( OK );
}

l3m::l3m ( const std::string& type )
: m_type ( type )
, m_isDynamic ( false )
, m_rendererData ( 0 )
{
    InitializeEndianness ();
    SetError ( OK );
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
    
    if ( m_rendererData != 0 )
        delete m_rendererData;
}

void l3m::DeclareMetadata(const std::string& name)
{
    m_metadatas.push_back ( name );
}

l3m::ErrorCode l3m::Save ( const char* path, unsigned int flags )
{
    std::ofstream fp;
    fp.open ( path, std::ios::out | std::ios::binary );
    if ( fp.fail() )
        return SetError(UNABLE_TO_OPEN_FILE_FOR_WRITING);
    return Save(fp, flags);
}

l3m::ErrorCode l3m::Load ( const char* path )
{
    std::ifstream fp;
    fp.open ( path, std::ios::in | std::ios::binary );
    if ( fp.fail() )
        return SetError(UNABLE_TO_OPEN_FILE_FOR_READING);
    return Load(fp);
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
static size_t identityWrite ( const T* v, u32 count, std::ostream& fp )
{
    fp.write(reinterpret_cast<const char*>(v), sizeof(T)*count);
    if ( !fp.fail() )
        return count;
    return -1;
}

template < typename T >
static size_t identityRead ( T* v, u32 count, std::istream& fp )
{
    size_t s = fp.readsome ( reinterpret_cast<char *>(v), sizeof(T)*count ) / sizeof(T);
    if ( ! fp.fail() )
        return s;
    return -1;
}

static size_t swap16Write ( const u16* v, u32 count, std::ostream& fp )
{
    u16 current;
    for ( u32 i = 0; i < count; ++i )
    {
        current = ( ( v[i] >> 8 ) & 0x00FF ) | ( v[i] << 8 & 0xFF00 );
        fp.write(reinterpret_cast<const char*>(&current), sizeof(u16));
        if ( fp.fail() )
            return i;
    }
    return count;
}

static size_t swap16Read ( u16* v, u32 count, std::istream& fp )
{
    u16 current;
    for ( u32 i = 0; i < count; ++i )
    {
        fp.read ( reinterpret_cast<char*>(&current), sizeof(u16) );
        if ( fp.gcount() < (ssize_t)sizeof(u16) )
            return i;
        v[i] = ( ( current >> 8 ) & 0x00FF ) | ( current << 8 & 0xFF00 );
    }
    return count;
}

static size_t swap32Write ( const u32* v, u32 count, std::ostream& fp )
{
    u32 current;
    for ( u32 i = 0; i < count; ++i )
    {
        current =  ( ( v[i] >> 24 ) & 0x000000FF ) |
                   ( ( v[i] >> 8  ) & 0x0000FF00 ) |
                   ( ( v[i] << 8  ) & 0x00FF0000 ) |
                   ( ( v[i] << 24 ) & 0xFF000000 );
        fp.write(reinterpret_cast<const char*>(&current), sizeof(u32));
        if ( fp.fail() )
            return i;
    }
    return count;
}

static size_t swap32Read ( u32* v, u32 count, std::istream& fp )
{
    u32 current;
    for ( u32 i = 0; i < count; ++i )
    {
        fp.read ( reinterpret_cast<char*>(&current), sizeof(u32) );
        if ( fp.gcount() < (ssize_t)sizeof(u32) )
            return i;
        v[i] =  ( ( current >> 24 ) & 0x000000FF ) |
                ( ( current >> 8  ) & 0x0000FF00 ) |
                ( ( current << 8  ) & 0x00FF0000 ) |
                ( ( current << 24 ) & 0xFF000000 );
    }
    return count;
}

static size_t swap64Write ( const u64* v, u32 count, std::ostream& fp )
{
    u64 current;
    for ( u32 i = 0; i < count; ++i )
    {
        current =  ( ( v[i] >> 56 ) & 0x00000000000000FFLL ) |
                   ( ( v[i] >> 40 ) & 0x000000000000FF00LL ) |
                   ( ( v[i] >> 24 ) & 0x0000000000FF0000LL ) |
                   ( ( v[i] >> 8  ) & 0x00000000FF000000LL ) |
                   ( ( v[i] << 8  ) & 0x000000FF00000000LL ) |
                   ( ( v[i] << 24 ) & 0x0000FF0000000000LL ) |
                   ( ( v[i] << 40 ) & 0x00FF000000000000LL ) |
                   ( ( v[i] << 56 ) & 0xFF00000000000000LL );
        fp.write(reinterpret_cast<const char*>(&current), sizeof(u64));
        if ( fp.fail() )
            return i;
    }
    return count;
}

static size_t swap64Read ( u64* v, u32 count, std::istream& fp )
{
    u64 current;
    for ( u32 i = 0; i < count; ++i )
    {
        fp.read ( reinterpret_cast<char*>(&current), sizeof(u64) );
        if ( fp.gcount() < (ssize_t)sizeof(u64) )
            return i;
        v[i] =  ( ( current >> 56 ) & 0x00000000000000FFLL ) |
                ( ( current >> 40 ) & 0x000000000000FF00LL ) |
                ( ( current >> 24 ) & 0x0000000000FF0000LL ) |
                ( ( current >> 8  ) & 0x00000000FF000000LL ) |
                ( ( current << 8  ) & 0x000000FF00000000LL ) |
                ( ( current << 24 ) & 0x0000FF0000000000LL ) |
                ( ( current << 40 ) & 0x00FF000000000000LL ) |
                ( ( current << 56 ) & 0xFF00000000000000LL );
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
        m_endian64writer = swap64Write;
    }
    else
    {
        m_endian16writer = identityWrite<u16>;
        m_endian32writer = identityWrite<u32>;
        m_endian64writer = identityWrite<u64>;
    }
}

bool l3m::Write16 ( const u16* v, u32 nmemb, std::ostream& fp ) const
{
    return m_endian16writer ( v, nmemb, fp) >= nmemb;
}
size_t l3m::Read16 ( u16* v, u32 nmemb, std::istream& fp ) const
{
    return m_endian16reader ( v, nmemb, fp );
}
bool l3m::Write32 ( const u32* v, u32 nmemb, std::ostream& fp ) const
{
    return m_endian32writer ( v, nmemb, fp ) >= nmemb;
}
size_t l3m::Read32 ( u32* v, u32 nmemb, std::istream& fp ) const
{
    return m_endian32reader ( v, nmemb, fp );
}
bool l3m::Write64 ( const u64* v, u32 nmemb, std::ostream& fp ) const
{
    return m_endian64writer ( v, nmemb, fp ) >= nmemb;
}
size_t l3m::Read64 ( u64* v, u32 nmemb, std::istream& fp ) const
{
    return m_endian64reader ( v, nmemb, fp );
}
bool l3m::WriteFloat ( const float* v, u32 nmemb, std::ostream& fp ) const
{
    return m_endian32writer ( reinterpret_cast<const u32*>(v), nmemb, fp ) >= nmemb;
}
size_t l3m::ReadFloat ( float* v, u32 nmemb, std::istream& fp ) const
{
    return m_endian32reader ( reinterpret_cast<u32*>(v), nmemb, fp );
}
bool l3m::WriteStr ( const std::string& str, std::ostream& fp ) const
{
    u32 length = str.length ();
    if ( !Write32 ( &length, 1, fp ) )
        return false;
    return WriteData ( str.c_str(), sizeof(char), length, fp );
}
size_t l3m::ReadStr ( std::string& dest, std::istream& fp ) const
{
    u32 length;
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

bool l3m::WriteData ( const void* data, size_t size, u32 nmemb, std::ostream& fp ) const
{
    fp.write ( reinterpret_cast<const char*>(data), size*nmemb );
    return true;
}

size_t l3m::ReadData ( char* dest, size_t size, u32 nmemb, std::istream& fp ) const
{
    return fp.readsome(dest, size*nmemb) / size;
}

l3m::ErrorCode l3m::Save ( std::ostream& fp, u32 flags )
{
    u64 npos = (u64)-1;
    unsigned int i;
    
    #define FWRITE(data, size, nmemb, fp, err) if ( ! WriteData(data, size, nmemb, fp) ) return SetError(err)
    #define FWRITE16(data, nmemb, fp, err) if ( ! Write16(reinterpret_cast<u16*>(data), nmemb, fp) ) return SetError(err)
    #define FWRITE32(data, nmemb, fp, err) if ( ! Write32(reinterpret_cast<u32*>(data), nmemb, fp) ) return SetError(err)
    #define FWRITE64(data, nmemb, fp, err) if ( ! Write64(reinterpret_cast<u64*>(data), nmemb, fp) ) return SetError(err)
    #define FWRITEF(data, nmemb, fp, err) if ( ! WriteData(reinterpret_cast<float*>(data), sizeof(float), nmemb, fp) ) return SetError(err)
    #define FWRITE_STR(str, fp, err) if ( ! WriteStr(str,fp) ) return SetError(err)

    // BOM
    FWRITE ( L3M_BOM, sizeof(char), strlen(L3M_BOM), fp, ERROR_WRITING_BOM );
    
    // Write the BOM endianness identifier, based on the machine endianness and desired configuration
    u8 thisMachineIsBigEndian = htons(0xFFF1) == 0xFFF1;
    u8 targetIsBigEndian;
    
    if ( L3M_SAVE_ENDIANNESS == L3M_MACHINE_ENDIAN )
        targetIsBigEndian = thisMachineIsBigEndian;
    else
        targetIsBigEndian = ( L3M_SAVE_ENDIANNESS == L3M_BIG_ENDIAN );
    FWRITE ( &targetIsBigEndian, sizeof(u8), 1, fp, ERROR_WRITING_BOM );
    
    // Write the file flags.
    FWRITE32 ( &flags, 1, fp, ERROR_WRITING_FLAGS );
    
    // Versión
    float fVersion = L3M_VERSION;
    FWRITEF ( &fVersion, 1, fp, ERROR_WRITING_VERSION );
    u32 vertexVersion = Vertex::VERSION;
    FWRITE32 ( &vertexVersion, 1, fp, ERROR_WRITING_VERTEX_VERSION );
    
    // Type
    FWRITE_STR ( type(), fp, ERROR_WRITING_TYPE );

    // Offset 2 TXD
    u64 off2TXD = fp.tellp ();
    FWRITE64 ( &npos, 1, fp, ERROR_ALLOCATING_TXD_OFFSET );
    
    // Offset 2 Meta
    u64 off2Meta = fp.tellp ();
    FWRITE64 ( &npos, 1, fp, ERROR_ALLOCATING_METADATAS_OFFSET );
    
    // Groups
    unsigned int numGroups = m_groups.size();
    FWRITE32 ( &numGroups, 1, fp, ERROR_WRITING_NUMBER_OF_GROUPS );
    
    // Write each group
    u64 groupOffsetRefs [ m_groups.size() ];
    i = 0;
    for ( groupMap::const_iterator iter = m_groups.begin(); iter != m_groups.end(); ++iter )
    {
        // Group name
        FWRITE_STR ( iter->first, fp, ERROR_WRITING_GROUP_NAME );
        
        // Save offset position
        groupOffsetRefs[i] = fp.tellp  ();
        ++i;
        FWRITE64 ( &npos, 1, fp, ERROR_ALLOCATING_GROUP_OFFSET );
    }
    
    // For each group, write its meshes and fill the ref
    u32 group = 0;
    for ( groupMap::const_iterator iter = m_groups.begin(); iter != m_groups.end(); ++iter )
    {
        // Fill the ref
        u64 ref = fp.tellp ();
        fp.seekp ( groupOffsetRefs[group], std::ios::beg );
        FWRITE64 ( &ref, 1, fp, ERROR_WRITING_GROUP_OFFSET );
        fp.seekp ( 0, std::ios::end );
        
        // Write the meshes headers
        const meshList& meshes = iter->second;
        u32 numMeshes = meshes.size ();
        FWRITE32 ( &numMeshes, 1, fp, ERROR_WRITING_NUMBER_OF_MESHES );
        u64 meshOffsetRefs [ numMeshes ];
        
        i = 0;
        for ( meshList::const_iterator iter2 = meshes.begin(); iter2 != meshes.end(); ++iter2 )
        {
            Mesh* mesh = *iter2;
            
            // Write the mesh name
            FWRITE_STR ( mesh->name(), fp, ERROR_WRITING_MESH_NAME );
            
            // Keep the mesh offset position
            meshOffsetRefs[i] = fp.tellp ();
            ++i;
            FWRITE64 ( &npos, 1, fp, ERROR_ALLOCATING_MESH_OFFSET );
        }
        
        
        // Write the mesh data
        u32 currentMesh = 0;
        
        for ( meshList::const_iterator iter2 = meshes.begin(); iter2 != meshes.end(); ++iter2 )
        {
            Mesh* mesh = *iter2;
            
            // Fill the ref
            ref = fp.tellp ();
            fp.seekp ( meshOffsetRefs [ currentMesh ], std::ios::beg );
            FWRITE64 ( &ref, 1, fp, ERROR_WRITING_MESH_OFFSET );
            fp.seekp ( 0, std::ios::end );
            
            // Write the polygon type
            FWRITE32 ( &(mesh->polyType()), 1, fp, ERROR_WRITING_POLYGON_TYPE );
            
            // Write the vertex data
            unsigned int num = mesh->numVertices();
            FWRITE32 ( &num, 1, fp, ERROR_WRITING_VERTEX_COUNT );
            FWRITEF ( mesh->vertices(), (num * sizeof(Vertex)) / sizeof(float), fp, ERROR_WRITING_VERTEX_DATA );
            
            // Write the index data
            num = mesh->numIndices();
            FWRITE32 ( &num, 1, fp, ERROR_WRITING_INDEX_COUNT );
            FWRITE32 ( mesh->indices(), (num * sizeof(unsigned int)) / sizeof(unsigned int), fp, ERROR_WRITING_INDEX_DATA );
            
            ++currentMesh;
        }
        
        ++group;
    }
    
    // Write the ref to the TXDs
    u64 ref = fp.tellp ();
    fp.seekp( off2TXD, std::ios::beg );
    FWRITE64 ( &ref, 1, fp, ERROR_WRITING_TXD_OFFSET );
    fp.seekp ( 0, std::ios::end );
    // TODO: Implement TXD support
    ref = 0;
    FWRITE64 ( &ref, 1, fp, ERROR_WRITING_TXD_COUNT );
    
    // Write the ref to the metadata
    ref = fp.tellp();
    fp.seekp ( off2Meta, std::ios::beg );
    FWRITE64 ( &ref, 1, fp, ERROR_WRITING_METADATAS_OFFSET );
    fp.seekp ( 0, std::ios::end );
    
    // Write the metadatas
    u32 metadataCount = m_metadatas.size ();
    FWRITE32 ( &metadataCount, 1, fp, ERROR_WRITING_METADATAS_COUNT );

    // Allocate space for the metadata refs
    u64 metaOffsetRefs [ metadataCount ];
    for ( i = 0; i < metadataCount; ++i )
    {
        // Write the metadata name
        FWRITE_STR ( m_metadatas[i], fp, ERROR_WRITING_META_NAME );
        
        // Allocate space for the metadata offset
        metaOffsetRefs[i] = fp.tellp();
        FWRITE64 ( &npos, 1, fp, ERROR_ALLOCATING_META_OFFSET );
    }
    
    // Write the metadata data
    for ( i = 0; i < metadataCount; ++i )
    {
        // Write the ref
        ref = fp.tellp();
        fp.seekp ( metaOffsetRefs[i], std::ios::beg );
        FWRITE64 ( &ref, 1, fp, ERROR_WRITING_META_OFFSET );
        fp.seekp ( 0, std::ios::end );
        
        // Write the metadata into.
        if ( SaveMetadata ( m_metadatas[i], fp ) == false )
                return SetError ( ERROR_WRITING_METADATA );
    }
    
#undef FWRITE
#undef FWRITE16
#undef FWRITE32
#undef FWRITEF
#undef FWRITE_STR
    
    return SetError(OK);
}


l3m::ErrorCode l3m::Load ( std::istream& fp )
{
    char buffer [ 1024 ];
    size_t size;
    u32 i;
    u64 refBack;
    
    #define FREAD(data, _size, nmemb, fp, err) if ( ( ( size = ReadData(data, _size, (nmemb), fp) ) != (nmemb) ) || fp.fail() ) return SetError(err)
    #define FREAD16(data, nmemb, fp, err) if ( ( ( size = Read16(reinterpret_cast<u16*>(data), (nmemb), fp) ) != (nmemb) ) || fp.fail() ) return SetError(err)
    #define FREAD32(data, nmemb, fp, err) if ( ( ( size = Read32(reinterpret_cast<u32*>(data), (nmemb), fp) ) != (nmemb) ) || fp.fail() ) return SetError(err)
    #define FREAD64(data, nmemb, fp, err) if ( ( ( size = Read64(reinterpret_cast<u64*>(data), (nmemb), fp) ) != (nmemb) ) || fp.fail() ) return SetError(err)
    #define FREADF(data, nmemb, fp, err) if ( ( ( size = ReadFloat(reinterpret_cast<float*>(data), (nmemb), fp) ) != (nmemb) ) || fp.fail() ) return SetError(err)
    #define FREAD_STR(str, fp, err) if ( ( ( size = ReadStr(str,fp) ) < 0 ) || fp.fail() ) return SetError(err)

    // Read out the BOM marker
    FREAD ( buffer, sizeof(char), strlen(L3M_BOM), fp, ERROR_READING_BOM );
    if ( memcmp ( buffer, L3M_BOM, strlen(L3M_BOM) ) != 0 )
        return SetError(INVALID_BOM);

    FREAD ( buffer, sizeof(char), 1, fp, ERROR_READING_BOM );
    
    // Choose the endianness strategy
    u8 thisMachineIsBigEndian = htons(0xFFF1) == 0xFFF1;
    u8 targetIsBigEndian = buffer[0];
    
    if ( thisMachineIsBigEndian != targetIsBigEndian )
    {
        m_endian16reader = swap16Read;
        m_endian32reader = swap32Read;
        m_endian64reader = swap64Read;
    }
    else
    {
        m_endian16reader = identityRead<u16>;
        m_endian32reader = identityRead<u32>;
        m_endian64reader = identityRead<u64>;
    }
    
    // Read the flags
    u32 flags;
    FREAD32(&flags, 1, fp, ERROR_READING_FLAGS);

    // Load and check version
    float fVersion;
    FREADF(&fVersion, 1, fp, ERROR_READING_VERSION);
    if ( fVersion > L3M_VERSION )
        return SetError ( INVALID_VERSION );
    u32 vertexVersion;
    FREAD32(&vertexVersion, 1, fp, ERROR_READING_VERTEX_VERSION);
    if ( vertexVersion > Vertex::VERSION )
        return SetError ( INVALID_VERTEX_VERSION );
    
    // Load and check type
    std::string strType;
    FREAD_STR ( strType, fp, ERROR_READING_TYPE );
    if ( strType != type() )
        return SetError ( INVALID_TYPE );
    
    // Get the ref to the txds
    u64 off2TXD;
    FREAD64(&off2TXD, 1, fp, ERROR_READING_TXD_OFFSET );
    
    // Get the ref to the metadata
    u64 off2Meta;
    FREAD64(&off2Meta, 1, fp, ERROR_READING_METADATAS_OFFSET);
    
    // Load groups
    u32 numGroups;
    FREAD32(&numGroups, 1, fp, ERROR_READING_GROUP_COUNT);
    
    std::string groupName;
    u64 ref2Group;
    for ( i = 0; i < numGroups; ++i )
    {
        FREAD_STR ( groupName, fp, ERROR_READING_GROUP_NAME );
        FREAD64 ( &ref2Group, 1, fp, ERROR_READING_GROUP_OFFSET );
        refBack = fp.tellg ();
        
        fp.seekg ( ref2Group, std::ios::beg );
        
        // Read out the number of meshes
        u32 numMeshes;
        FREAD32 ( &numMeshes, 1, fp, ERROR_READING_MESH_COUNT );
        
        for ( unsigned int j = 0; j < numMeshes; ++j )
        {
            // Read out the mesh name
            std::string meshName;
            FREAD_STR ( meshName, fp, ERROR_READING_MESH_NAME );
            
            // Create the Mesh
            Mesh* mesh = new Mesh ( meshName );
            
            // Read the mesh offset
            u64 off2Mesh;
            u64 refMeshBack;
            FREAD64 ( &off2Mesh, 1, fp, ERROR_READING_MESH_OFFSET );
            refMeshBack = fp.tellg();
            
            fp.seekg ( off2Mesh, std::ios::beg );
            
            // Read the polygon type
            FREAD32 ( &(mesh->polyType()), 1, fp, ERROR_READING_POLYGON_TYPE );
            
            // Read the vertex count
            u32 vertexCount;
            FREAD32 ( &vertexCount, 1, fp, ERROR_READING_VERTEX_COUNT );
            
            // Read the vertices
            Vertex* vertices = Vertex::Allocate ( vertexCount );
            FREADF (vertices->base(), vertexCount * (sizeof(Vertex) / sizeof(float)), fp, ERROR_READING_VERTEX_DATA );
            
            // Read the index count
            u32 indexCount;
            FREAD32 ( &indexCount, 1, fp, ERROR_READING_INDEX_COUNT );
            
            // Read the indices
            unsigned int* indices = (unsigned int *)malloc(sizeof(unsigned int)*indexCount);
            FREAD32 ( indices, indexCount, fp, ERROR_READING_INDEX_DATA );
            
            // Load the mesh
            mesh->Set(vertices, vertexCount, indices, indexCount, mesh->polyType());
            this->LoadMesh(mesh, groupName);
            
            fp.seekg ( refMeshBack, std::ios::beg );
        }
        
        fp.seekg ( refBack, std::ios::beg );
    }
    
    // TODO: Load TXDs
    
    // META-DATA
    // Load the meta-data count
    fp.seekg ( off2Meta, std::ios::beg );
    u32 metadataCount;
    FREAD32 ( &metadataCount, 1, fp, ERROR_READING_METADATAS_COUNT );
    
    for ( u32 i = 0; i < metadataCount; ++i )
    {
        // Load the metadata name
        std::string metaName;
        FREAD_STR ( metaName, fp, ERROR_READING_META_NAME );
        
        // Load the metadata offset
        u64 refMeta;
        FREAD64 ( &refMeta, 1, fp, ERROR_READING_META_OFFSET );

        // Jump there!
        refBack = fp.tellg ();
        fp.seekg ( refMeta, std::ios::beg );
        
        if ( LoadMetadata ( metaName, fp ) == false )
            return SetError ( ERROR_READING_METADATA );
        
        fp.seekg ( refBack, std::ios::beg );
    }

#undef FREAD
#undef FREAD16
#undef FREAD32
#undef FREADF
#undef FREAD_STR

    return SetError(OK);
}

l3m::ErrorCode l3m::SetError ( l3m::ErrorCode err )
{
    errorCode() = err;
    if ( err == OK )
    {
        getErrno() = 0;
        m_error[0] = '\0';
    }
    else
    {
        getErrno() = errno;
        snprintf ( m_error, sizeof(m_error)-1, "%s: %s", TranslateErrorCode(errorCode()), strerror(getErrno()) );
    }
    
    return err;
}

const char* l3m::TranslateErrorCode ( l3m::ErrorCode err ) const
{
    switch ( err )
    {
        case OK: return "Success";
        
        // File saving
        case UNABLE_TO_OPEN_FILE_FOR_WRITING: return "Unable to open file for writing";
        case ERROR_WRITING_BOM: return "Error writing BOM";
        case ERROR_WRITING_FLAGS: return "Error writing flags";
        case ERROR_WRITING_VERSION: return "Error writing version number";
        case ERROR_WRITING_VERTEX_VERSION: return "Error writing vertex version number";
        case ERROR_WRITING_TYPE: return "Error writing model type";
        case ERROR_ALLOCATING_TXD_OFFSET: return "Error allocating space for the TXD offset";
        case ERROR_ALLOCATING_METADATAS_OFFSET: return "Error allocating space for the meta-data offset";
        case ERROR_WRITING_NUMBER_OF_GROUPS: return "Error writing the number of groups";
        case ERROR_WRITING_GROUP_NAME: return "Error writing the group name";
        case ERROR_ALLOCATING_GROUP_OFFSET: return "Error allocating space for the group offset";
        case ERROR_WRITING_GROUP_OFFSET: return "Error writing the group offset";
        case ERROR_WRITING_NUMBER_OF_MESHES: return "Error writing the number of meshes";
        case ERROR_WRITING_MESH_NAME: return "Error writing the mesh name";
        case ERROR_ALLOCATING_MESH_OFFSET: return "Error allocating space for the mesh offset";
        case ERROR_WRITING_MESH_OFFSET: return "Error writing the mesh offset";
        case ERROR_WRITING_POLYGON_TYPE: return "Error writing the mesh polygon type";
        case ERROR_WRITING_VERTEX_COUNT: return "Error writing the vertex count";
        case ERROR_WRITING_VERTEX_DATA: return "Error writing the vertex data";
        case ERROR_WRITING_INDEX_COUNT: return "Error writing the index count";
        case ERROR_WRITING_INDEX_DATA: return "Error writing the index data";
        case ERROR_WRITING_TXD_OFFSET: return "Error writing the TXD offseT";
        case ERROR_WRITING_TXD_COUNT: return "Error writing the TXD count";
        case ERROR_WRITING_METADATAS_OFFSET: return "Error writing metadatas offset";
        case ERROR_WRITING_METADATAS_COUNT: return "Error writing metadatas count";
        case ERROR_WRITING_META_NAME: return "Error writing metadata name";
        case ERROR_ALLOCATING_META_OFFSET: return "Error allocating metadata offset";
        case ERROR_WRITING_META_OFFSET: return "Error writing metadata offset";
        case ERROR_WRITING_METADATA: return "Error writing metadata";
        
        // FIle loading
        case UNABLE_TO_OPEN_FILE_FOR_READING: return "Unable to open file for reading";
        case ERROR_READING_BOM: return "Error reading BOM";
        case INVALID_BOM: return "Invalid BOM";
        case ERROR_READING_FLAGS: return "Error reading flags";
        case ERROR_READING_VERSION: return "Error reading version";
        case INVALID_VERSION: return "Invalid version";
        case ERROR_READING_VERTEX_VERSION: return "Error reading vertex version";
        case INVALID_VERTEX_VERSION: return "Invalid vertex version";
        case ERROR_READING_TYPE: return "Error reading type";
        case INVALID_TYPE: return "Invalid model type";
        case ERROR_READING_TXD_OFFSET: return "Error reading TXDs offset";
        case ERROR_READING_METADATAS_OFFSET: return "Error reading metadatas offset";
        case ERROR_READING_GROUP_COUNT: return "Error reading group count";
        case ERROR_READING_GROUP_NAME: return "Error reading group name";
        case ERROR_READING_GROUP_OFFSET: return "Error reading group offset";
        case ERROR_READING_MESH_COUNT: return "Error reading mesh count";
        case ERROR_READING_MESH_NAME: return "Error reading mesh name";
        case ERROR_READING_MESH_OFFSET: return "Error reading mesh offset";
        case ERROR_READING_POLYGON_TYPE: return "Error reading the mesh polygon type";
        case ERROR_READING_VERTEX_COUNT: return "Error reading vertex count";
        case ERROR_READING_VERTEX_DATA: return "Error reading vertex data";
        case ERROR_READING_INDEX_COUNT: return "Error reading index count";
        case ERROR_READING_INDEX_DATA: return "Error reading index data";
        case ERROR_READING_METADATAS_COUNT: return "Error reading metadatas count";
        case ERROR_READING_META_NAME: return "Error reading metadata name";
        case ERROR_READING_META_OFFSET: return "Error reading metadata offset";
        case ERROR_READING_METADATA: return "Error reading metadata";
                
        default: return "Unknown";
    }
}
