/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        geometry.cpp
// PURPOSE:     OpenGL ES 2.0 geometries.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include <vector>

#include "geometry.h"
#include "renderer.h"

using namespace Renderer;

Geometry::Geometry ()
: GeometryBase ()
, m_initialized ( false )
, m_vertexBuffer ( 0 )
, m_elementBuffer ( 0 )
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

bool Geometry::initialize ()
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

bool Geometry::bindVertexLayer ( IProgram* program,
                                 const std::string& attributeName,
                                 const std::string& layerName,
                                 u32 level,
                                 DataType type,
                                 bool normalize,
                                 u32 count,
                                 u32 offset )
{
    GLES20_Program* prog = static_cast < GLES20_Program* > ( program );
    GLint id = glGetAttribLocation ( prog->handler(), attributeName.c_str () );
    eglGetError ();
    if ( id == -1 )
        return false;
    GLchar* ptr = getVertexLayer <GLchar> ( layerName, level );
    if ( ptr == 0 )
        return false;

    GLenum glType = GL_NONE;
    switch ( type )
    {
        case FLOAT:
            glType = GL_FLOAT;
            break;
        case INT:
            glType = GL_INT;
            break;
        case UNSIGNED_INT:
            glType = GL_UNSIGNED_INT;
            break;
    }
    
    if ( glType == GL_NONE )
        return false;
    
    u32 stride = getVertexLayerElementSize ( layerName );
    offset += m_offsets [ layerName ] + level*numVertices()*stride;
    
    glBindBuffer ( GL_ARRAY_BUFFER, m_vertexBuffer );
    eglGetError();
    glVertexAttribPointer ( id, count, glType, normalize?GL_TRUE:GL_FALSE, stride, reinterpret_cast<GLchar *>(offset) );
    eglGetError();
    glEnableVertexAttribArray ( id );
    eglGetError();

    return true;
}

bool Geometry::unbindAttribute ( IProgram* program, const std::string& attrName )
{
    GLES20_Program* prog = static_cast < GLES20_Program* > ( program );
    GLint id = glGetAttribLocation ( prog->handler(), attrName.c_str () );
    eglGetError ();
    if ( id == -1 )
        return false;
    
    glDisableVertexAttribArray ( id );
    eglGetError ();
    
    return true;
}
