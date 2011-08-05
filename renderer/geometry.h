#pragma once

#include <map>
#include <list>
#include <string>
#include "mesh.h"
#include "vertex.h"

namespace Renderer
{

class GeometryBase
{
public:
    typedef std::list<Mesh *> meshList;
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
    layerMap            m_mapVertexLayers;
    meshList            m_meshes;
    
private:
    void        FreeVertices    ();
    
public:
                GeometryBase        ();
    virtual     ~GeometryBase       ();


    void        LoadMesh        ( Mesh* mesh );
    void        Load            ( const float* pVertices, unsigned int flags, unsigned int stride, unsigned int vertexCount );
    void        Set             ( Renderer::Vertex* pVertices, u32 vertexCount );
    
    // Vertex layers
public:
    template < typename T >
    T*          CreateVertexLayer       ( const std::string& name, T* data = 0 )
    {
        return reinterpret_cast < T* > ( CreateVertexLayer ( name, data, sizeof(T) ) );
    }
    
    void*       CreateVertexLayer       ( const std::string& name, void* data, u32 elementSize )
    {
        void* destination = GetVertexLayer<void *> ( name );

        if ( destination == 0 )
        {
            VertexLayer layer;
            layer.elementSize = elementSize;
            layer.data = malloc ( elementSize * numVertices() );
            m_mapVertexLayers.insert ( layerMap::value_type ( name, layer ) );
            
            destination = layer.data;
        }
        
        if ( data != 0 )
            memcpy ( destination, data, elementSize * numVertices() );
        
        return destination;
    }

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
    const layerMap&             vertexLayers    () const { return m_mapVertexLayers; }
    
    std::string&                name            () { return m_name; }
    Renderer::Vertex*&          vertices        () { return m_vertices; }
    meshList&                   meshes          () { return m_meshes; }
    layerMap&                   vertexLayers    () { return m_mapVertexLayers; }
};

}

// Include the renderer selection through the renderer header to get the actual geometry definition
#include "renderer.h"