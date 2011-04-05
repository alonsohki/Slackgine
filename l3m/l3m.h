#ifndef L3M_H
#define L3M_H

#include <string>
#include <map>
#include <vector>
#include <list>
#include <stdint.h>
#include "vector.h"
#include "vertex.h"
#include "mesh.h"

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
        // Error codes
    enum ErrorCode
    {
        OK = 0,
        
        // File saving
        UNABLE_TO_OPEN_FILE_FOR_WRITING,
        ERROR_WRITING_BOM,
        ERROR_WRITING_VERSION,
        ERROR_WRITING_TYPE,
        ERROR_ALLOCATING_TXD_OFFSET,
        ERROR_ALLOCATING_METADATAS_OFFSET,
        ERROR_WRITING_NUMBER_OF_GROUPS,
        ERROR_WRITING_GROUP_NAME,
        ERROR_ALLOCATING_GROUP_OFFSET,
        ERROR_WRITING_GROUP_OFFSET,
        ERROR_WRITING_NUMBER_OF_MESHES,
        ERROR_WRITING_MESH_NAME,
        ERROR_ALLOCATING_MESH_OFFSET,
        ERROR_WRITING_MESH_OFFSET,
        ERROR_WRITING_VERTEX_COUNT,
        ERROR_WRITING_VERTEX_DATA,
        ERROR_WRITING_FACE_COUNT,
        ERROR_WRITING_FACE_DATA,
        ERROR_WRITING_TXD_OFFSET,
        ERROR_WRITING_TXD_COUNT,
        ERROR_WRITING_METADATAS_OFFSET,
        ERROR_WRITING_METADATAS_COUNT,
        ERROR_WRITING_META_NAME,
        ERROR_ALLOCATING_META_OFFSET,
        ERROR_WRITING_META_OFFSET,
        ERROR_WRITING_METADATA,
                
        // FIle loading
        UNABLE_TO_OPEN_FILE_FOR_READING,
        ERROR_READING_BOM,
        INVALID_BOM,
        ERROR_READING_VERSION,
        INVALID_VERSION,
        INVALID_TYPE,
        
        MAX_ERROR_CODE
    };

private:
    typedef std::list<Mesh *> meshList;
    typedef std::map<std::string, meshList> groupMap;
    groupMap            m_groups;
    std::string         m_type;
    ErrorCode           m_errorCode;
    int                 m_errno;
    char                m_error [ 256 ];
    
    std::vector<std::string>    m_metadatas;
    
public:
    typedef struct __VertexGroup VertexGroup;

public:
                l3m ( const std::string& type = "default" );
    virtual     ~l3m();
    
    // Accessors
public:
    const std::string&  type            () const { return m_type; }
    const ErrorCode&    errorCode       () const { return m_errorCode; }
    const int&          getErrno           () const { return m_errno; }
    const char*         error           () const { return m_error; }
    
protected:
    std::string&        type            () { return m_type; }
private:
    ErrorCode&          errorCode       () { return m_errorCode; }
    int&                getErrno        () { return m_errno; }

    // Files
public:
    ErrorCode           SaveToFile      ( FILE* fp );
    ErrorCode           SaveToFile      ( const char* path );
    ErrorCode           LoadFromFile    ( FILE* fp );
    ErrorCode           LoadFromFile    ( const char* path );
    
    // Error handling
private:
    ErrorCode           SetError                ( ErrorCode err );
    const char*         TranslateErrorCode      ( ErrorCode err ) const;

    // Metadatas
protected:
    void                DeclareMetadata ( const std::string& name );
    virtual bool        SaveMetadata    ( const std::string& name, FILE* fp ) { return true; }

    // Endianness
private:
    void                InitializeEndianness ();
    size_t (*m_endian16writer)(const uint16_t*, uint32_t, FILE*);
    size_t (*m_endian32writer)(const uint32_t*, uint32_t, FILE*);
    size_t (*m_endian16reader)(uint16_t*, uint32_t, FILE*);
    size_t (*m_endian32reader)(uint32_t*, uint32_t, FILE*);
protected:
    bool                Write16         ( const uint16_t* v, uint32_t nmemb, FILE* fp ) const;
    bool                Write32         ( const uint32_t* v, uint32_t nmemb, FILE* fp ) const;
    bool                WriteFloat      ( const float* v, uint32_t nmemb, FILE* fp ) const;
    bool                WriteStr        ( const std::string& str, FILE* fp ) const;
    bool                WriteData       ( const void* data, size_t size, unsigned int nmemb, FILE* fp ) const;
    size_t              Read16          ( uint16_t* v, uint32_t nmemb, FILE* fp ) const;
    size_t              Read32          ( uint32_t* v, uint32_t nmemb, FILE* fp ) const;
    size_t              ReadFloat       ( float* v, uint32_t nmemb, FILE* fp ) const;
    size_t              ReadStr         ( std::string& str, FILE* fp ) const;
    size_t              ReadData        ( char* dest, size_t size, uint32_t nmemb, FILE* fp ) const;
    
public:
    // Grouped meshes
    void                LoadMesh        ( Mesh* mesh, const std::string& group = "" );
private:
    meshList*           FindGroup       ( const std::string& name );
};

#endif
