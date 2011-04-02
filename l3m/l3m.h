#ifndef L3M_H
#define L3M_H

#include <string>
#include <map>
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
private:
    typedef std::list<Mesh *> meshList;
    typedef std::map<std::string, meshList> groupMap;
    groupMap            m_groups;
    std::string         m_type;
    
public:
    typedef struct __VertexGroup VertexGroup;

public:
                l3m ( const std::string& type = "default" );
    virtual     ~l3m();
    
    // Accessors
public:
    const std::string&  type            () const { return m_type; }
protected:
    std::string&        type            () { return m_type; }

    // Files
public:
    bool                SaveToFile      ( FILE* fp ) const;
    bool                SaveToFile      ( const char* path ) const;
    bool                LoadFromFile    ( FILE* fp );
    bool                LoadFromFile    ( const char* path );


    // Endianness
private:
    void                InitializeEndianness ();
    size_t (*m_endian16writer)(const uint16_t*, uint32_t, FILE*);
    size_t (*m_endian32writer)(const uint32_t*, uint32_t, FILE*);
    size_t (*m_endian16reader)(uint16_t*, uint32_t, FILE*);
    size_t (*m_endian32reader)(uint32_t*, uint32_t, FILE*);
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
