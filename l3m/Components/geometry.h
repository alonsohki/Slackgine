#pragma once

#include <list>
#include <string>
#include "l3m/component.h"
#include "renderer/mesh.h"
#include "renderer/vertex.h"

namespace l3m
{

class Geometry : public IComponent
{
public:
    typedef std::list<Renderer::Mesh *> meshList;

private:
    std::string         m_name;
    Renderer::Vertex*   m_vertices;
    u32                 m_numVertices;
    meshList            m_meshes;
    
private:
    void FreeVertices ();

public:
    static IComponent* Create ()
    {
        return new Geometry ( "" );
    }
    
                Geometry        ( const std::string& name );
                ~Geometry       ();
    
    bool        Load            ( l3m::IStream& fp, float version );
    bool        Save            ( l3m::OStream& fp );
    
    void        LoadMesh        ( Renderer::Mesh* mesh );
    
    void        Load            ( const float* pVertices, unsigned int flags,
                                  unsigned int stride, unsigned int vertexCount );
    void        Set             ( Renderer::Vertex* pVertices, unsigned int vertexCount );
    
    // Accessors
public:
    const std::string&          name            () const { return m_name; }
    const Renderer::Vertex*     vertices        () const { return m_vertices; }
    const unsigned int&         numVertices     () const { return m_numVertices; }
    const meshList&             meshes          () const { return m_meshes; }
    
    std::string&                name            () { return m_name; }
    Renderer::Vertex*&          vertices        () { return m_vertices; }
    meshList&                   meshes          () { return m_meshes; }
};

}


