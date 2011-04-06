#ifndef MESH_H
#define	MESH_H

#include <string>
#include <cstdlib>
#include "vertex.h"
#include "face.h"

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
    Face*               m_faces;
    unsigned int        m_numFaces;
    PolygonType         m_polyType;
    
public:
    Mesh ( const std::string& name )
    : m_name ( name )
    , m_vertices ( 0 )
    , m_numVertices ( 0 )
    , m_faces ( 0 )
    , m_numFaces ( 0 )
    , m_polyType ( TRIANGLES )
    {}
    
    ~Mesh ()
    {
        FreeVertices ();
        FreeFaces ();
    }
    
private:
    void FreeVertices ()
    {
        if ( m_vertices != 0 )
            free ( m_vertices );
        m_vertices = 0;
        m_numVertices = 0;
    }
    void FreeFaces ()
    {
        if ( m_faces != 0 )
            free ( m_faces );
        m_faces = 0;
        m_numFaces = 0;
    }
    
    // Accessors
public:
    const std::string&          name            () const { return m_name; }
    const Vertex*               vertices        () const { return m_vertices; }
    const Face*                 faces           () const { return m_faces; }
    const unsigned int&         numVertices     () const { return m_numVertices; }
    const unsigned int&         numFaces        () const { return m_numFaces; }
    const PolygonType&          polyType        () const { return m_polyType; }
    
    std::string&                name            () { return m_name; }
    Vertex*                     vertices        () { return m_vertices; }
    Face*                       faces           () { return m_faces; }
    PolygonType&                polyType        () { return m_polyType; }

    // Loads the vertex and face data, allocating space for it.
    void Load ( const float* pVertices, unsigned int flags, unsigned int stride, unsigned int vertexCount,
                const unsigned int* pIndices, unsigned int faceCount, PolygonType polyType = TRIANGLES )
    {
        FreeVertices ();
        FreeFaces ();
        m_vertices = Vertex::LoadAllocating(pVertices, flags, stride, vertexCount);
        m_numVertices = vertexCount;
        m_faces = Face::LoadAllocating(pIndices, faceCount);
        m_numFaces = faceCount;
        m_polyType = polyType;
    }
    
    // Loads the vertex and face data, storing the pointer to the previously allocated data.
    void Set ( Vertex* pVertices, unsigned int vertexCount,
               Face* pFaces, unsigned int faceCount,
               PolygonType polyType = TRIANGLES )
    {
        FreeVertices ();
        FreeFaces ();
        m_vertices = pVertices;
        m_numVertices = vertexCount;
        m_faces = pFaces;
        m_numFaces = faceCount;
        m_polyType = polyType;
    }
    
    static Mesh* LoadAllocating ( const std::string& name,
                                  const float* pVertices, unsigned int flags, unsigned int stride, unsigned int vertexCount,
                                  const unsigned int* pIndices, unsigned int faceCount, PolygonType polyType = TRIANGLES )
    {
        Mesh* mesh = new Mesh ( name );
        mesh->Load ( pVertices, flags, stride, vertexCount, pIndices, faceCount, polyType );
        return mesh;
    }
};


#endif

