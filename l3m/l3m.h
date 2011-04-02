#ifndef L3M_H
#define L3M_H

#include <string>
#include "vector.h"
#include "vertex.h"

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
    }*          groups;
public:
    typedef struct __VertexGroup VertexGroup;

public:
    l3m();
    l3m(const l3m& orig);
    virtual     ~l3m();
    
    // Accessors
public:
    const Vertex*       group           ( const std::string& name ) const { VertexGroup* g = FindGroup(name); return g ? g->vertices : 0; }
    Vertex*             group           ( const std::string& name ) { VertexGroup* g = FindGroup(name); return g ? g->vertices : 0; }
    
    void                LoadGroup       ( const std::string& name, const Vertex* pVertices, unsigned int count );
    void                LoadGroup       ( const std::string& name, const float* pVertices, unsigned int flags, unsigned int stride, unsigned int count );
    
private:
    VertexGroup*        FindGroup       ( const std::string& name ) const;
};

#endif
