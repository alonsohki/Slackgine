/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        mesh.cpp
// PURPOSE:     Geometry meshes.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include <cstring>
#include "mesh.h"

using namespace Renderer;

Mesh::Mesh()
: m_indices ( 0 )
, m_numIndices ( 0 )
, m_polyType ( TRIANGLES )
, m_material ( 0 )
{}

Mesh::~Mesh ()
{
    freeIndices ();
}

void Mesh::freeIndices()
{
    if ( m_indices != 0 )
        sgFree ( m_indices );
    m_indices = 0;
    m_numIndices = 0;
}

void Mesh::load(const unsigned int* pIndices, unsigned int indexCount, PolygonType polyType)
{
    freeIndices ();
    m_indices = ( unsigned int * )sgMalloc(sizeof(unsigned int) * indexCount);
    memcpy ( m_indices, pIndices, sizeof(unsigned int) * indexCount );
    m_numIndices = indexCount;
    m_polyType = polyType;
}

void Mesh::set(unsigned int* pIndices, unsigned int indexCount, PolygonType polyType)
{
    if ( pIndices != m_indices )
    {
        freeIndices ();
        m_indices = pIndices;
    }
    m_numIndices = indexCount;
    m_polyType = polyType;
}
