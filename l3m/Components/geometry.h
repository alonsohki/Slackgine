#pragma once

#include <list>
#include <map>
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
    struct VertexLayer
    {
        Renderer::Vertex*       vertices;
        u32                     numVertices;
    };
    typedef std::map < std::string, VertexLayer > layerMap;

private:
    std::string         m_name;
    VertexLayer         m_mainVertexLayer;
    layerMap            m_mapVertexLayers;
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
    
    bool        Load            ( l3m::IStream& fp, f32 version );
    bool        Save            ( l3m::OStream& fp );
    
    void        LoadMesh        ( Renderer::Mesh* mesh );
    
    void        Load            ( const f32* pVertices, u32 flags,
                                  u32 stride, u32 vertexCount );
    void        Set             ( Renderer::Vertex* pVertices, u32 vertexCount );
    
    // Vertex layers
    bool        CreateVertexLayer       ( const std::string& name, Renderer::Vertex* pVertices, u32 vertexCount );
    bool        DeleteVertexLayer       ( const std::string& name );
    
    // Accessors
public:
    const std::string&          name            () const { return m_name; }
    const Renderer::Vertex*     vertices        () const { return m_mainVertexLayer.vertices; }
    const u32&                  numVertices     () const { return m_mainVertexLayer.numVertices;; }
    const meshList&             meshes          () const { return m_meshes; }
    
    std::string&                name            () { return m_name; }
    Renderer::Vertex*&          vertices        () { return m_mainVertexLayer.vertices; }
    meshList&                   meshes          () { return m_meshes; }
    
    // Accessors for vertex layers
    Renderer::Vertex*           vertices        ( const std::string& layerName ) const;
    u32                         numVertices     ( const std::string& layerName ) const;
};

}


