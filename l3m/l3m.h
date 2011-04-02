#ifndef L3M_H
#define L3M_H

#include <string>
#include "vector.h"
#include "vertex.h"

static const char* const L3M_BOM = "L3M\x01";
static const float L3M_VERSION = 10.0f; // Las versiones se representan por 1/x. A más reciente, más cercano a 0.

class l3m
{
private:
    struct __VertexGroup
    {
        std::string     name;
        Vertex*         vertices;
        __VertexGroup*  next;

        __VertexGroup  ( const std::string& p_name )
        : name ( p_name ) {}
    }*                  m_groups;
    std::string         m_type;
    
public:
    typedef struct __VertexGroup VertexGroup;

public:
    l3m ( const std::string& type = "default" );
    l3m ( const l3m& orig );
    virtual     ~l3m();
    
    // Accessors
public:
    const Vertex*       group           ( const std::string& name ) const { VertexGroup* g = FindGroup(name); return g ? g->vertices : 0; }
    Vertex*             group           ( const std::string& name ) { VertexGroup* g = FindGroup(name); return g ? g->vertices : 0; }
    
    const std::string&  type            () const { return m_type; }
protected:
    std::string&        type            () { return m_type; }
public:
    
public:
    // Ficheros
    bool                SaveToFile      ( FILE* fp ) const;
    bool                SaveToFile      ( const char* path ) const;
    
    // Grupos
    void                LoadGroup       ( const std::string& name, const Vertex* pVertices, unsigned int count );
    void                LoadGroup       ( const std::string& name, const float* pVertices, unsigned int flags, unsigned int stride, unsigned int count );
private:
    VertexGroup*        FindGroup       ( const std::string& name ) const;
};

#endif
