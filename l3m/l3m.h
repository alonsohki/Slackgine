#ifndef L3M_H
#define L3M_H

#include <string>
#include <map>
#include <list>
#include "vector.h"
#include "vertex.h"
#include "mesh.h"

static const char* const L3M_BOM = "L3M\x01";
static const float L3M_VERSION = 0.00000001f;
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
    l3m ( const l3m& orig );
    virtual     ~l3m();
    
    // Accessors
public:
    const std::string&  type            () const { return m_type; }
protected:
    std::string&        type            () { return m_type; }
public:
    
public:
    // Files
    bool                SaveToFile      ( FILE* fp ) const;
    bool                SaveToFile      ( const char* path ) const;

public:
    // Grouped meshes
    void                LoadMesh        ( Mesh* mesh, const std::string& group = "" );
private:
    meshList*           FindGroup       ( const std::string& name );
};

#endif
