#include "geometry.h"

using namespace Renderer;

Geometry::Geometry ()
: m_vertices ( 0 )
, m_numVertices ( 0 )
{
}

Geometry::~Geometry ()
{
    for ( meshList::iterator iter = m_meshes.begin(); iter != m_meshes.end(); ++iter )
        delete *iter;
    FreeVertices ();
    
    // Delete all the vertex layers
    for ( layerMap::iterator iter = m_mapVertexLayers.begin ();
          iter != m_mapVertexLayers.end ();
          ++iter )
    {
        free ( iter->second.data );
    }
}

void Geometry::FreeVertices()
{
    if ( m_vertices != 0 )
        free ( m_vertices );
    m_vertices = 0;
    m_numVertices = 0;
}

void Geometry::Load ( const float* pVertices, unsigned int flags, unsigned int stride, unsigned int vertexCount )
{
    FreeVertices ();
    m_vertices = Vertex::LoadAllocating(pVertices, flags, stride, vertexCount);
    m_numVertices = vertexCount;
}

void Geometry::Set ( Vertex* pVertices, unsigned int vertexCount )
{
    if ( pVertices != m_vertices )
    {
        FreeVertices ();
        m_vertices = pVertices;
    }
    m_numVertices = vertexCount;
}

void Geometry::LoadMesh ( Renderer::Mesh* mesh )
{
    m_meshes.push_back(mesh);
}
