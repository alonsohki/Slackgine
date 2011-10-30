/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        geometry.cpp
// PURPOSE:     Base geometry functions, to be extended by the rendering engine specific data.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include "geometry.h"

using namespace Renderer;

GeometryBase::GeometryBase ()
: m_vertices ( 0 )
, m_numVertices ( 0 )
{
}

GeometryBase::~GeometryBase ()
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

void GeometryBase::FreeVertices()
{
    if ( m_vertices != 0 )
        free ( m_vertices );
    m_vertices = 0;
    m_numVertices = 0;
}

void GeometryBase::Set ( Vertex* pVertices, unsigned int vertexCount )
{
    if ( pVertices != m_vertices )
    {
        FreeVertices ();
        m_vertices = pVertices;
    }
    m_numVertices = vertexCount;
}

void GeometryBase::LoadMesh ( Renderer::Mesh* mesh )
{
    m_meshes.push_back(mesh);
}
