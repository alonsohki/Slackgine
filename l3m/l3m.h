#ifndef L3M_H
#define L3M_H

#include <string>
#include <map>
#include <vector>
#include <list>
#include <iostream>
#include <stdint.h>
#include "../shared/platform.h"
#include "math/vector.h"
#include "vertex.h"
#include "mesh.h"
#include "geometry.h"

static const char* const L3M_BOM = "L3M\x01";
static const float L3M_VERSION = 0.79f;
static const enum
{
    L3M_MACHINE_ENDIAN,
    L3M_LOW_ENDIAN,
    L3M_BIG_ENDIAN
} L3M_SAVE_ENDIANNESS = L3M_LOW_ENDIAN;

class l3m
{
public:
    class IRendererData { public: virtual ~IRendererData() {} };

public:
    // Save flags
    enum SaveFlags
    {
        SAVE_COMPRESSED         = 0x001
    };
    
    // Error codes
    enum ErrorCode
    {
        OK = 0,
        
        // File saving
        UNABLE_TO_OPEN_FILE_FOR_WRITING,
        ERROR_WRITING_BOM,
        ERROR_WRITING_FLAGS,
        ERROR_WRITING_VERSION,
        ERROR_WRITING_VERTEX_VERSION,
        ERROR_WRITING_TYPE,
        ERROR_WRITING_NUMBER_OF_GEOMETRIES,
        ERROR_WRITING_GEOMETRY_NAME,
        ERROR_WRITING_GEOMETRY_MATRIX,
        ERROR_WRITING_NUMBER_OF_MESHES,
        ERROR_WRITING_MESH_NAME,
        ERROR_WRITING_POLYGON_TYPE,
        ERROR_WRITING_MATERIAL_COLORS,
        ERROR_WRITING_VERTEX_COUNT,
        ERROR_WRITING_VERTEX_DATA,
        ERROR_WRITING_INDEX_COUNT,
        ERROR_WRITING_INDEX_DATA,
        ERROR_WRITING_TXD_COUNT,
        ERROR_WRITING_METADATAS_COUNT,
        ERROR_WRITING_META_NAME,
        ERROR_WRITING_METADATA,
                
        // FIle loading
        UNABLE_TO_OPEN_FILE_FOR_READING,
        INVALID_STREAM,
        ERROR_READING_BOM,
        INVALID_BOM,
        ERROR_READING_FLAGS,
        ERROR_READING_VERSION,
        INVALID_VERSION,
        ERROR_READING_VERTEX_VERSION,
        INVALID_VERTEX_VERSION,
        ERROR_READING_TYPE,
        INVALID_TYPE,
        ERROR_READING_GEOMETRY_COUNT,
        ERROR_READING_GEOMETRY_NAME,
        ERROR_READING_GEOMETRY_MATRIX,
        ERROR_READING_MESH_COUNT,
        ERROR_READING_MESH_NAME,
        ERROR_READING_POLYGON_TYPE,
        ERROR_READING_MATERIAL_COLORS,
        ERROR_READING_VERTEX_COUNT,
        ERROR_READING_VERTEX_DATA,
        ERROR_READING_INDEX_COUNT,
        ERROR_READING_INDEX_DATA,
        ERROR_READING_TXD_COUNT,
        ERROR_READING_METADATAS_COUNT,
        ERROR_READING_META_NAME,
        ERROR_READING_METADATA,
        
        MAX_ERROR_CODE
    };

public:
    typedef std::list<Geometry*> geometryList;
private:
    geometryList        m_geometries;
    std::string         m_type;
    ErrorCode           m_errorCode;
    int                 m_errno;
    char                m_error [ 256 ];
    bool                m_isDynamic;
    
    mutable IRendererData*
                        m_rendererData;
    
    std::vector<std::string>    m_metadatas;
    

public:
                l3m ();
protected:
                l3m ( const std::string& type );
public:
    virtual     ~l3m();
    
    // Accessors
public:
    const std::string&  type            () const { return m_type; }
    const ErrorCode&    errorCode       () const { return m_errorCode; }
    const int&          getErrno        () const { return m_errno; }
    const char*         error           () const { return m_error; }
    IRendererData*&     rendererData    () const { return m_rendererData; }
    const geometryList& geometries      () const { return m_geometries; }
    
private:
    ErrorCode&          errorCode       () { return m_errorCode; }
    int&                getErrno        () { return m_errno; }

    // Files
public:
    ErrorCode           Save            ( const char* path, unsigned int flags = 0 );
    ErrorCode           Save            ( std::ostream& os, unsigned int flags = 0 );
    ErrorCode           Load            ( const char* path );
    ErrorCode           Load            ( std::istream& is );
private:
    friend class l3mFactory;
    static l3m*         CreateAndLoad   ( std::istream& is, ErrorCode* errcode = 0 );
    
private:
    ErrorCode           InternalLoad    ( std::istream& is, bool doEndianSwapping );
    
    // Error handling
private:
    ErrorCode           SetError                ( ErrorCode err );
public:
    static const char*  TranslateErrorCode      ( ErrorCode err );

    // Metadatas
protected:
    void                DeclareMetadata ( const std::string& name );
    virtual bool        SaveMetadata    ( const std::string& name, std::ostream& fp ) { return true; }
    virtual bool        LoadMetadata    ( const std::string& name, std::istream& fp ) { return true; }

    // Endianness
private:
    void                InitializeEndianness ();
    size_t (*m_endian16writer)(const u16*, u32, std::ostream&);
    size_t (*m_endian32writer)(const u32*, u32, std::ostream&);
    size_t (*m_endian64writer)(const u64*, u32, std::ostream&);
    size_t (*m_endian16reader)(u16*, u32, std::istream&);
    size_t (*m_endian32reader)(u32*, u32, std::istream&);
    size_t (*m_endian64reader)(u64*, u32, std::istream&);
protected:
    bool                Write16         ( const u16* v, u32 nmemb, std::ostream& fp ) const;
    bool                Write32         ( const u32* v, u32 nmemb, std::ostream& fp ) const;
    bool                Write64         ( const u64* v, u32 nmemb, std::ostream& fp ) const;
    bool                WriteFloat      ( const float* v, u32 nmemb, std::ostream& fp ) const;
    bool                WriteStr        ( const std::string& str, std::ostream& fp ) const;
    bool                WriteData       ( const void* data, size_t size, unsigned int nmemb, std::ostream& fp ) const;
    size_t              Read16          ( u16* v, u32 nmemb, std::istream& fp ) const;
    size_t              Read32          ( u32* v, u32 nmemb, std::istream& fp ) const;
    size_t              Read64          ( u64* v, u32 nmemb, std::istream& fp ) const;
    size_t              ReadFloat       ( float* v, u32 nmemb, std::istream& fp ) const;
    size_t              ReadStr         ( std::string& str, std::istream& fp ) const;
    size_t              ReadData        ( char* dest, size_t size, u32 nmemb, std::istream& fp ) const;
    
public:
    void                LoadMesh        ( Mesh* mesh, const std::string& geometryName = "" );
    Geometry*           CreateGeometry  ( const std::string& name );
    Geometry*           FindGeometry    ( const std::string& name );
    
    // Dynamic models
public:
    bool                isDynamic       () const { return m_isDynamic; }
    void                setDynamic      ( bool dynamic ) { m_isDynamic = dynamic; }
};

#endif
