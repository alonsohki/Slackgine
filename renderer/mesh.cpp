#include <cstring>
#include "mesh.h"

using namespace Renderer;

__MeshBase::__MeshBase()
: m_vertices ( 0 )
, m_numVertices ( 0 )
, m_indices ( 0 )
, m_numIndices ( 0 )
, m_polyType ( TRIANGLES )
{}

__MeshBase::~__MeshBase ()
{
    FreeVertices ();
    FreeIndices ();
}

void __MeshBase::FreeVertices()
{
    if ( m_vertices != 0 )
        free ( m_vertices );
    m_vertices = 0;
    m_numVertices = 0;
}

void __MeshBase::FreeIndices()
{
    if ( m_indices != 0 )
        free ( m_indices );
    m_indices = 0;
    m_numIndices = 0;
}

void __MeshBase::Load(const float* pVertices, unsigned int flags, unsigned int stride, unsigned int vertexCount, const unsigned int* pIndices, unsigned int indexCount, PolygonType polyType)
{
    FreeVertices ();
    FreeIndices ();
    m_vertices = Vertex::LoadAllocating(pVertices, flags, stride, vertexCount);
    m_numVertices = vertexCount;
    m_indices = ( unsigned int * )malloc(sizeof(unsigned int) * indexCount);
    memcpy ( m_indices, pIndices, sizeof(unsigned int) * indexCount );
    m_numIndices = indexCount;
    m_polyType = polyType;
}

void __MeshBase::Set(Vertex* pVertices, unsigned int vertexCount, unsigned int* pIndices, unsigned int indexCount, PolygonType polyType)
{
    if ( pVertices != m_vertices )
    {
        FreeVertices ();
        m_vertices = pVertices;
    }
    if ( pIndices != m_indices )
    {
        FreeIndices ();
        m_indices = pIndices;
    }
    m_numVertices = vertexCount;
    m_numIndices = indexCount;
    m_polyType = polyType;
}