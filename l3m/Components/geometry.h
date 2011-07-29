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
        void*   data;
        u32     elementSize;
    };
    typedef std::map < std::string, VertexLayer > layerMap;

private:
    std::string         m_name;
    Renderer::Vertex*   m_vertices;
    u32                 m_numVertices;
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
public:
    template < typename T >
    T*          CreateVertexLayer       ( const std::string& name, T* data = 0 )
    {
        T* destination = GetVertexLayer<T> ( name );

        if ( destination == 0 )
        {
            VertexLayer layer;
            layer.elementSize = sizeof(T);
            layer.data = malloc ( sizeof(T) * numVertices() );
            m_mapVertexLayers.insert ( layerMap::value_type ( name, layer ) );
            
            destination = layer.data;
        }
        
        if ( data != 0 )
            memcpy ( destination, data, sizeof(T) * numVertices() );
        
        return destination;
    }
    
private:
    void        CreateVertexLayer       ( const std::string& name, void* data, u32 elementSize )
    {
        VertexLayer layer;
        layer.elementSize = elementSize;
        layer.data = data;
        m_mapVertexLayers.insert ( layerMap::value_type ( name, layer ) );
    }
public:
    
    template < typename T >
    T*          GetVertexLayer          ( const std::string& name )
    {
        T* layerData = 0;
        layerMap::iterator iter = m_mapVertexLayers.find ( name );
        if ( iter != m_mapVertexLayers.end() )
            layerData = reinterpret_cast < T* > ( iter->second.data );
        return layerData;
    }
    
    bool        DeleteVertexLayer       ( const std::string& name )
    {
        layerMap::iterator iter = m_mapVertexLayers.find ( name );
        if ( iter != m_mapVertexLayers.end() )
        {
            free ( iter->second.data );
            m_mapVertexLayers.erase ( iter );
            return true;
        }
        return false;
    }
    
    // Accessors
public:
    const std::string&          name            () const { return m_name; }
    const Renderer::Vertex*     vertices        () const { return m_vertices; }
    const u32&                  numVertices     () const { return m_numVertices;; }
    const meshList&             meshes          () const { return m_meshes; }
    
    std::string&                name            () { return m_name; }
    Renderer::Vertex*&          vertices        () { return m_vertices; }
    meshList&                   meshes          () { return m_meshes; }
};

}


