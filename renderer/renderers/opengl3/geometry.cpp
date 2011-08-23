/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011 - Alberto Alonso
//
// Redistribution of this software, including any of its
// files, in either binary or source code form is totally
// forbidden without the permission of its owners.
//
// Owners reserve their right to legal action against any
// person or company that fails to do so.
//
// For more detailed information, see the LICENSE file in
// the top-level directory.
//
// FILE:        geometry.cpp
// PURPOSE:     OpenGL3 geometries.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include <vector>

#include "geometry.h"
#include "renderer.h"

using namespace Renderer;

Geometry::Geometry ()
: GeometryBase ()
, m_vertexBuffer ( 0 )
, m_elementBuffer ( 0 )
, m_initialized ( false )
{
}

Geometry::~Geometry ()
{
    if ( m_vertexBuffer != 0 )
    {
        glDeleteBuffers ( 1, &m_vertexBuffer );
        eglGetError ();
        m_vertexBuffer = 0;
    }
    if ( m_elementBuffer != 0 )
    {
        glDeleteBuffers ( 1, &m_elementBuffer );
        eglGetError ();
        m_elementBuffer = 0;
    }
}

bool Geometry::Initialize ()
{
    GLuint buffers [ 2 ];
    glGenBuffers ( 2, buffers );
    eglGetError ();
    
    // Prepare the vertex buffer
    m_vertexBuffer = buffers[0];
    
    // Calculate the total size of the vertex buffer, including layers, and leave
    // the layer offsets calculated.
    u32 size = numVertices()*sizeof(Vertex);
    const GeometryBase::layerMap& layers = vertexLayers();
    for ( GeometryBase::layerMap::const_iterator iter = layers.begin();
          iter != layers.end();
          ++iter )
    {
        m_offsets [ iter->first ] = size;
        size += numVertices() * iter->second.elementSize;
    }

    // Upload the vertex data
    glBindBuffer ( GL_ARRAY_BUFFER, m_vertexBuffer );
    eglGetError ();
    glBufferData ( GL_ARRAY_BUFFER, size, 0, GL_STATIC_DRAW );
    eglGetError ();
    glBufferSubData ( GL_ARRAY_BUFFER, 0, numVertices()*sizeof(Vertex), vertices() );
    for ( GeometryBase::layerMap::const_iterator iter = layers.begin();
          iter != layers.end();
          ++iter )
    {
        glBufferSubData ( GL_ARRAY_BUFFER, m_offsets [ iter->first ],
                          numVertices()*iter->second.elementSize, iter->second.data );
    }
    
    // Generate the mesh nodes based on the geometry meshes.
    // Precalculate the number of indices.
    u32 numIndices = 0;
    const GeometryBase::meshList& meshes = this->meshes ();
    for ( GeometryBase::meshList::const_iterator iter = meshes.begin();
          iter != meshes.end();
          ++iter )
    {
        numIndices += (*iter)->numIndices();
    }
    // Allocate space for a buffer containing all the indices.
    m_elementBuffer = buffers [ 1 ];
    glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, m_elementBuffer );
    glBufferData ( GL_ELEMENT_ARRAY_BUFFER, sizeof(u32)*numIndices, 0, GL_STATIC_DRAW );
    
    // Fill it, creating in the while the mesh nodes.
    u32 offset = 0;
    for ( GeometryBase::meshList::const_iterator iter = meshes.begin();
          iter != meshes.end();
          ++iter )
    {
        glBufferSubData ( GL_ELEMENT_ARRAY_BUFFER, sizeof(u32)*offset,
                          sizeof(u32)*(*iter)->numIndices(), (*iter)->indices() );
        MeshNode node;
        node.offset = offset;
        node.mesh = *iter;
        m_meshNodes.push_back(node);
    }
    

    m_initialized = true;
    return true;
}
