#ifndef MESH_H
#define	MESH_H

#include <string>
#include <cstdlib>
#include "vertex.h"
#include "material.h"

class Mesh
{
public:
    enum PolygonType
    {
        TRIANGLES,
        TRIANGLE_STRIP,
        TRIANGLE_FAN,
        QUADS
    };

private:
    std::string         m_name;
    Vertex*             m_vertices;
    unsigned int        m_numVertices;
    unsigned int*       m_indices;
    unsigned int        m_numIndices;
    PolygonType         m_polyType;
    Material            m_material;
    
public:
    Mesh ( const std::string& name );
    
    ~Mesh ();
    
private:
    void FreeVertices ();
    void FreeIndices ();
    
    // Accessors
public:
    const std::string&          name            () const { return m_name; }
    const Vertex*               vertices        () const { return m_vertices; }
    const unsigned int*         indices         () const { return m_indices; }
    const unsigned int&         numVertices     () const { return m_numVertices; }
    const unsigned int&         numIndices      () const { return m_numIndices; }
    const PolygonType&          polyType        () const { return m_polyType; }
    const Material&             material        () const { return m_material; }
    
    std::string&                name            () { return m_name; }
    Vertex*&                    vertices        () { return m_vertices; }
    unsigned int*&              indices         () { return m_indices; }
    PolygonType&                polyType        () { return m_polyType; }
    Material&                   material        () { return m_material; }

    // Loads the vertex and face data, allocating space for it.
    void Load ( const float* pVertices, unsigned int flags, unsigned int stride, unsigned int vertexCount,
                const unsigned int* pIndices, unsigned int indexCount, PolygonType polyType = TRIANGLES );
    
    // Loads the vertex and face data, storing the pointer to the previously allocated data.
    void Set ( Vertex* pVertices, unsigned int vertexCount,
               unsigned int* pIndices, unsigned int indexCount,
               PolygonType polyType = TRIANGLES );
    
    static Mesh* LoadAllocating ( const std::string& name,
                                  const float* pVertices, unsigned int flags, unsigned int stride, unsigned int vertexCount,
                                  const unsigned int* pIndices, unsigned int indexCount, PolygonType polyType = TRIANGLES )
    {
        Mesh* mesh = new Mesh ( name );
        mesh->Load ( pVertices, flags, stride, vertexCount, pIndices, indexCount, polyType );
        return mesh;
    }
};


#endif

