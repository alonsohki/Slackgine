#include <cerrno>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include "l3m.h"
#include "l3mFactory.h"

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
    for ( geometryList::iterator iter = m_geometries.begin(); iter != m_geometries.end(); ++iter )
        delete *iter;
    
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

Geometry* l3m::CreateGeometry ( const std::string& name )
{
    Geometry* geometry = FindGeometry ( name );
    if ( !geometry )
    {
        geometry = new Geometry ( name );
        m_geometries.push_back ( geometry );
    }
    return geometry;
}

Geometry* l3m::FindGeometry ( const std::string& name )
{
    for ( geometryList::iterator iter = m_geometries.begin ();
          iter != m_geometries.end();
          ++iter )
    {
        if ( (*iter)->name() == name )
            return *iter;
    }
    return 0;
}

void l3m::LoadMesh(Mesh* mesh, const std::string& geometryName )
{
    Geometry* geometry = FindGeometry ( geometryName );
    if ( geometry == 0 )
    {
        geometry = new Geometry ( geometryName );
        m_geometries.push_back ( geometry );
    }
    geometry->LoadMesh ( mesh );
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
    fp.read ( reinterpret_cast<char *>(v), sizeof(T)*count );
    size_t s = fp.gcount () / sizeof(T);
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
    unsigned char thisMachineIsBigEndian = detectBigEndian ();
    
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
    size_t readLength = ReadData ( buffer, sizeof(char), length, fp );
    if ( readLength != length )
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
    fp.read ( dest, size*nmemb );
    return fp.gcount () / size;
}

l3m::ErrorCode l3m::Save ( std::ostream& fp, u32 flags )
{
    u32 zero = 0;
    u32 i;
    
    #define FWRITE(data, size, nmemb, fp, err) if ( ! WriteData(data, size, nmemb, fp) ) return SetError(err)
    #define FWRITE16(data, nmemb, fp, err) if ( ! Write16(reinterpret_cast<const u16*>(data), nmemb, fp) ) return SetError(err)
    #define FWRITE32(data, nmemb, fp, err) if ( ! Write32(reinterpret_cast<const u32*>(data), nmemb, fp) ) return SetError(err)
    #define FWRITE64(data, nmemb, fp, err) if ( ! Write64(reinterpret_cast<const u64*>(data), nmemb, fp) ) return SetError(err)
    #define FWRITEF(data, nmemb, fp, err) if ( ! WriteData(reinterpret_cast<const float*>(data), sizeof(float), nmemb, fp) ) return SetError(err)
    #define FWRITE_STR(str, fp, err) if ( ! WriteStr(str,fp) ) return SetError(err)

    // BOM
    FWRITE ( L3M_BOM, sizeof(char), strlen(L3M_BOM), fp, ERROR_WRITING_BOM );
    
    // Write the BOM endianness identifier, based on the machine endianness and desired configuration
    u8 thisMachineIsBigEndian = detectBigEndian ();
    u8 targetIsBigEndian;
    
    if ( L3M_SAVE_ENDIANNESS == L3M_MACHINE_ENDIAN )
        targetIsBigEndian = thisMachineIsBigEndian;
    else
        targetIsBigEndian = ( L3M_SAVE_ENDIANNESS == L3M_BIG_ENDIAN );
    FWRITE ( &targetIsBigEndian, sizeof(u8), 1, fp, ERROR_WRITING_BOM );
    
    // Versión
    float fVersion = L3M_VERSION;
    FWRITEF ( &fVersion, 1, fp, ERROR_WRITING_VERSION );
    u32 vertexVersion = Vertex::VERSION;
    FWRITE32 ( &vertexVersion, 1, fp, ERROR_WRITING_VERTEX_VERSION );
    
    // Type
    FWRITE_STR ( type(), fp, ERROR_WRITING_TYPE );

    // Write the file flags.
    FWRITE32 ( &flags, 1, fp, ERROR_WRITING_FLAGS );

    // Geometries
    unsigned int numGeometries = m_geometries.size();
    FWRITE32 ( &numGeometries, 1, fp, ERROR_WRITING_NUMBER_OF_GEOMETRIES );
    
    // Write each geometry
    for ( geometryList::const_iterator iter = m_geometries.begin(); iter != m_geometries.end(); ++iter )
    {
        const Geometry* geometry = *iter;
        
        // Geometry name
        FWRITE_STR ( geometry->name(), fp, ERROR_WRITING_GEOMETRY_NAME );
        
        // Geometry matrix
        FWRITEF ( geometry->matrix().vector(), 16, fp, ERROR_WRITING_GEOMETRY_MATRIX );

        // Write the meshes headers
        const Geometry::meshList& meshes = geometry->meshes();
        u32 numMeshes = meshes.size ();
        FWRITE32 ( &numMeshes, 1, fp, ERROR_WRITING_NUMBER_OF_MESHES );
        
        for ( Geometry::meshList::const_iterator iter2 = meshes.begin(); iter2 != meshes.end(); ++iter2 )
        {
            Mesh* mesh = *iter2;
            
            // Write the mesh name
            FWRITE_STR ( mesh->name(), fp, ERROR_WRITING_MESH_NAME );
            
            // Write the polygon type
            FWRITE32 ( &(mesh->polyType()), 1, fp, ERROR_WRITING_POLYGON_TYPE );
            
            // Write the material data
            const Material& mat = mesh->material();
            FWRITE32 ( (u32*)&mat, 4, fp, ERROR_WRITING_MATERIAL_COLORS );
            
            // Write the vertex data
            unsigned int num = mesh->numVertices();
            FWRITE32 ( &num, 1, fp, ERROR_WRITING_VERTEX_COUNT );
            FWRITEF ( mesh->vertices(), (num * sizeof(Vertex)) / sizeof(float), fp, ERROR_WRITING_VERTEX_DATA );
            
            // Write the index data
            num = mesh->numIndices();
            FWRITE32 ( &num, 1, fp, ERROR_WRITING_INDEX_COUNT );
            FWRITE32 ( mesh->indices(), (num * sizeof(u32)) / sizeof(u32), fp, ERROR_WRITING_INDEX_DATA );
        }
    }
    
    // TODO: Implement TXD support
    FWRITE32 ( &zero, 1, fp, ERROR_WRITING_TXD_COUNT );
    
    // Write the metadatas
    u32 metadataCount = m_metadatas.size ();
    FWRITE32 ( &metadataCount, 1, fp, ERROR_WRITING_METADATAS_COUNT );

    for ( i = 0; i < metadataCount; ++i )
    {
        // Write the metadata name
        FWRITE_STR ( m_metadatas[i], fp, ERROR_WRITING_META_NAME );

        // Write the metadata info
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

l3m* l3m::CreateAndLoad ( std::istream& fp, ErrorCode* code )
{
#define SET_ERROR(x) if ( code != 0 ) *code = x ;
    char buffer [ 256 ];
    size_t (*endian16reader)(u16*, u32, std::istream&);
    size_t (*endian32reader)(u32*, u32, std::istream&);
    size_t (*endian64reader)(u64*, u32, std::istream&);
    
    // Choose the endianness strategy
    fp.read ( buffer, sizeof(char)*strlen(L3M_BOM) );
    if ( memcmp ( buffer, L3M_BOM, strlen(L3M_BOM) ) != 0 )
    {
        SET_ERROR(INVALID_BOM);
        return 0;
    }

    // Check for endianness swapping
    fp.read ( buffer, sizeof(char) );
    u8 thisMachineIsBigEndian = detectBigEndian();
    u8 targetIsBigEndian = buffer[0];
    bool doEndianSwapping = ( thisMachineIsBigEndian != targetIsBigEndian );
    if ( doEndianSwapping )
    {
        endian16reader = swap16Read;
        endian32reader = swap32Read;
        endian64reader = swap64Read;
    }
    else
    {
        endian16reader = identityRead<u16>;
        endian32reader = identityRead<u32>;
        endian64reader = identityRead<u64>;
    }
    
    // Load and check version
    float fVersion;
    if ( endian32reader((u32 *)&fVersion, 1, fp) != 1 )
    {
        SET_ERROR ( ERROR_READING_VERSION );
        return 0;
    }
    if ( fVersion > L3M_VERSION )
    {
        SET_ERROR ( INVALID_VERSION );
        return 0;
    }
    
    u32 vertexVersion;
    if ( endian32reader(&vertexVersion, 1, fp) != 1 )
    {
        SET_ERROR ( ERROR_READING_VERTEX_VERSION );
        return 0;
    }
    if ( vertexVersion > Vertex::VERSION )
    {
        SET_ERROR ( INVALID_VERTEX_VERSION );
        return 0;
    }
    
    // Load the type
    i32 strLength;
    std::string strType;
    if ( endian32reader((u32*)&strLength, 1, fp) != 1 )
    {
        SET_ERROR ( ERROR_READING_TYPE );
        return 0;
    }
    char* temp = new char [ strLength ] ();
    fp.read ( temp, strLength );
    if ( fp.gcount() != strLength )
    {
        SET_ERROR ( ERROR_READING_TYPE );
        return 0;
    }
    strType.assign ( temp, strLength );
    delete [] temp;
    
    // Generate an instance of this type
    l3m* instance = l3mFactory::CreateOfType(strType);
    if ( !instance )
    {
        SET_ERROR ( INVALID_TYPE );
        return 0;
    }
    
    ErrorCode err = instance->InternalLoad ( fp, doEndianSwapping );
    SET_ERROR ( err );
    return instance;
#undef SET_ERROR
}

l3m::ErrorCode l3m::Load ( std::istream& fp )
{
    char buffer [ 2 ];
    size_t size;
    bool doEndianSwapping;
    
    if ( fp.fail() || fp.eof() )
        return SetError(INVALID_STREAM);
    
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
    u8 thisMachineIsBigEndian = detectBigEndian ();
    u8 targetIsBigEndian = buffer[0];
    
    if ( thisMachineIsBigEndian != targetIsBigEndian )
    {
        m_endian16reader = swap16Read;
        m_endian32reader = swap32Read;
        m_endian64reader = swap64Read;
        doEndianSwapping = true;
    }
    else
    {
        m_endian16reader = identityRead<u16>;
        m_endian32reader = identityRead<u32>;
        m_endian64reader = identityRead<u64>;
        doEndianSwapping = false;
    }
    
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
    
    return InternalLoad ( fp, doEndianSwapping );
}

l3m::ErrorCode l3m::InternalLoad ( std::istream& fp, bool doEndianSwapping )
{
    size_t size;
    u32 i;
    
    if ( doEndianSwapping )
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

    // Load geometries
    u32 numGeometries;
    FREAD32(&numGeometries, 1, fp, ERROR_READING_GEOMETRY_COUNT);
    
    std::string geometryName;
    for ( i = 0; i < numGeometries; ++i )
    {
        // Load the geometry name
        FREAD_STR ( geometryName, fp, ERROR_READING_GEOMETRY_NAME );
        Geometry* geometry = new Geometry ( geometryName );
        this->m_geometries.push_back ( geometry );
        
        // Load the geometry matrix
        FREADF ( geometry->matrix().vector(), 16, fp, ERROR_READING_GEOMETRY_MATRIX );

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
            
            // Read the polygon type
            FREAD32 ( &(mesh->polyType()), 1, fp, ERROR_READING_POLYGON_TYPE );
            
            // Read the material
            FREAD32 ( &(mesh->material()), 4, fp, ERROR_READING_MATERIAL_COLORS );
            
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
            geometry->LoadMesh(mesh);
        }
    }
    
    // TODO: Load TXDs
    u32 txdCount;
    FREAD32 ( &txdCount, 1, fp, ERROR_READING_TXD_COUNT );
    
    // META-DATA
    // Load the meta-data count
    u32 metadataCount;
    FREAD32 ( &metadataCount, 1, fp, ERROR_READING_METADATAS_COUNT );
    
    for ( u32 i = 0; i < metadataCount; ++i )
    {
        // Load the metadata name
        std::string metaName;
        FREAD_STR ( metaName, fp, ERROR_READING_META_NAME );
        
        if ( LoadMetadata ( metaName, fp ) == false )
            return SetError ( ERROR_READING_METADATA );
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

const char* l3m::TranslateErrorCode ( l3m::ErrorCode err )
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
        case ERROR_WRITING_NUMBER_OF_GEOMETRIES: return "Error writing the number of geometries";
        case ERROR_WRITING_GEOMETRY_NAME: return "Error writing the geometry name";
        case ERROR_WRITING_GEOMETRY_MATRIX: return "Error writing the geometry matrix";
        case ERROR_WRITING_NUMBER_OF_MESHES: return "Error writing the number of meshes";
        case ERROR_WRITING_MESH_NAME: return "Error writing the mesh name";
        case ERROR_WRITING_POLYGON_TYPE: return "Error writing the mesh polygon type";
        case ERROR_WRITING_MATERIAL_COLORS: return "Error writing the mesh material colors";
        case ERROR_WRITING_VERTEX_COUNT: return "Error writing the vertex count";
        case ERROR_WRITING_VERTEX_DATA: return "Error writing the vertex data";
        case ERROR_WRITING_INDEX_COUNT: return "Error writing the index count";
        case ERROR_WRITING_INDEX_DATA: return "Error writing the index data";
        case ERROR_WRITING_TXD_COUNT: return "Error writing the TXD count";
        case ERROR_WRITING_METADATAS_COUNT: return "Error writing metadatas count";
        case ERROR_WRITING_META_NAME: return "Error writing metadata name";
        case ERROR_WRITING_METADATA: return "Error writing metadata";
        
        // FIle loading
        case UNABLE_TO_OPEN_FILE_FOR_READING: return "Unable to open file for reading";
        case INVALID_STREAM: return "Invalid stream";
        case ERROR_READING_BOM: return "Error reading BOM";
        case INVALID_BOM: return "Invalid BOM";
        case ERROR_READING_FLAGS: return "Error reading flags";
        case ERROR_READING_VERSION: return "Error reading version";
        case INVALID_VERSION: return "Invalid version";
        case ERROR_READING_VERTEX_VERSION: return "Error reading vertex version";
        case INVALID_VERTEX_VERSION: return "Invalid vertex version";
        case ERROR_READING_TYPE: return "Error reading type";
        case INVALID_TYPE: return "Invalid model type";
        case ERROR_READING_GEOMETRY_COUNT: return "Error reading geometry count";
        case ERROR_READING_GEOMETRY_NAME: return "Error reading geometry name";
        case ERROR_READING_GEOMETRY_MATRIX: return "Error reading the geometry matrix";
        case ERROR_READING_MESH_COUNT: return "Error reading mesh count";
        case ERROR_READING_MESH_NAME: return "Error reading mesh name";
        case ERROR_READING_POLYGON_TYPE: return "Error reading the mesh polygon type";
        case ERROR_READING_MATERIAL_COLORS: return "Error reading the mesh material colors";
        case ERROR_READING_VERTEX_COUNT: return "Error reading vertex count";
        case ERROR_READING_VERTEX_DATA: return "Error reading vertex data";
        case ERROR_READING_INDEX_COUNT: return "Error reading index count";
        case ERROR_READING_INDEX_DATA: return "Error reading index data";
        case ERROR_READING_METADATAS_COUNT: return "Error reading metadatas count";
        case ERROR_READING_TXD_COUNT: return "Error reading TXD count";
        case ERROR_READING_META_NAME: return "Error reading metadata name";
        case ERROR_READING_METADATA: return "Error reading metadata";
                
        default: return "Unknown";
    }
}
