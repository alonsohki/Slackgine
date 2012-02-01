/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        renderer.cpp
// PURPOSE:     Android OpenGL ES 2.0 renderer.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include <sstream>
#include "gles20.h"

using namespace Renderer;

GLES20_Renderer::GLES20_Renderer()
: m_initialized(false)
, m_vertexShader(0)
, m_fragmentShader(0)
, m_program(0)
{
    strcpy ( m_error, "Success" );
}

GLES20_Renderer::~GLES20_Renderer()
{
    if ( m_program != 0 )
    {
        delete m_vertexShader;
        delete m_fragmentShader;
        delete m_program;
    }
}

bool GLES20_Renderer::initialize()
{
    if ( m_initialized )
        return true;
    strcpy ( m_error, "Success" );

    m_initialized = ( strcmp(m_error, "Success") == 0 );
    return m_initialized;
}

void GLES20_Renderer::setProgram ( IProgram* program )
{
    m_program = program;
}

bool GLES20_Renderer::beginScene ( const Matrix& matProjection, const Matrix& matLookat, TextureLookupFn texLookup )
{
    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    eglGetError();
    
    glEnable ( GL_DEPTH_TEST );
    glCullFace ( GL_BACK );
    
    // Change the basis to the OpenGL basis
    static const f32 m [ 16 ] = {
        1.0f,   0.0f,   0.0f,   0.0f,
        0.0f,   0.0f,  -1.0f,   0.0f,
        0.0f,   1.0f,   0.0f,   0.0f,
        0.0f,   0.0f,   0.0f,   1.0f
    };
    static const Matrix s_matBasisChanger ( m );
    
    const f32* col0 = &matProjection.m[0][0];
    const f32* col1 = &matProjection.m[1][0];
    const f32* col2 = &matProjection.m[2][0];
    const f32* col3 = &matProjection.m[3][0];
    const f32 projectionM [ 16 ] = {
         col0[0],  col0[2], -col0[1],  col0[3],
         col2[0],  col2[2], -col2[1],  col2[3],
        -col1[0], -col1[2], -col1[1], -col1[3],
         col3[0],  col3[2], -col3[1],  col3[3]
    };
    
    m_matProjection = Matrix ( projectionM );
    m_matLookat = s_matBasisChanger * matLookat;
    
    m_viewVector = Vector3 ( 0.0f, 1.0f, 0.0 ) * m_matLookat;
    m_viewVector.Normalize();
    
    m_texLookup = texLookup;
    
    return true;
}

void GLES20_Renderer::setupLighting()
{
    m_program->setUniform("un_Lights[0].diffuse", Color(255, 255, 255, 255), false );
    m_program->setUniform("un_Lights[0].ambient", Color(0, 0, 0, 255), false );
    m_program->setUniform("un_Lights[0].specular", Color(255, 255, 255, 255), false );
    m_program->setUniform("un_Lights[0].position", Vector3(0, -2, 0) );
    m_program->setUniform("un_Lights[0].direction", Vector3(0, 1, 0) );
}

bool GLES20_Renderer::render ( Geometry* geometry, const Transform& transform, MeshRenderFn fn )
{
    if ( m_program == 0 || m_program->ok() == false )
    {
        if ( !m_program )
            strcpy ( m_error, "Invalid program" );
        else
            m_program->getError ( m_error );
        return false;
    }
    if ( !m_program->use () )
    {
        m_program->getError ( m_error );
        return false;
    }
    
    if ( !geometry->initialized() )
        if ( !geometry->initialize() )
            return false;
    
    setupLighting ();
    
    Matrix mat = Transform2Matrix ( transform );
    Matrix matNormals = MatrixForNormals ( mat );
    Matrix matGeometry = m_matProjection * m_matLookat * mat;

    // Use vertex buffers
    const Vertex* v = 0;
    glBindBuffer ( GL_ARRAY_BUFFER, geometry->m_vertexBuffer );
    
    glVertexAttribPointer ( GLES20_Program::POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (char *)&(v->pos()) );
    eglGetError();
    glVertexAttribPointer ( GLES20_Program::NORMAL, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex), (char *)&(v->norm()) );
    eglGetError();
    glEnableVertexAttribArray ( GLES20_Program::POSITION );
    eglGetError();
    eglGetError();

    // Bind the indices buffer
    glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, geometry->m_elementBuffer );
    
    for ( Geometry::meshNodeVector::iterator iter = geometry->m_meshNodes.begin();
          iter != geometry->m_meshNodes.end();
          ++iter )
    {
        Mesh* mesh = (*iter).mesh;

        if ( !fn || fn(mesh) == true )
        {
            
            m_program->setUniform("un_ProjectionMatrix", m_matProjection );
            m_program->setUniform("un_LookatMatrix", m_matLookat );
            m_program->setUniform("un_ModelviewMatrix", mat);
            m_program->setUniform("un_NormalMatrix", matNormals);
            m_program->setUniform("un_Matrix", matGeometry );
            m_program->setUniform("un_ViewVector", m_viewVector );
    
            GLenum polyType = GL_INVALID_ENUM;
            switch ( mesh->polyType() )
            {
                case Mesh::TRIANGLES: polyType = GL_TRIANGLES; break;
                case Mesh::TRIANGLE_STRIP: polyType = GL_TRIANGLE_STRIP; break;
                case Mesh::TRIANGLE_FAN: polyType = GL_TRIANGLE_FAN; break;
                default: break;
            }

            if ( polyType != GL_INVALID_ENUM )
            {
                // Set the material
                Material* mat = mesh->material();
                if ( mat != 0 )
                {
                    m_program->setUniform( "un_Material.diffuse", mat->diffuse(), true );
                    m_program->setUniform( "un_Material.ambient", mat->ambient(), false );
                    m_program->setUniform( "un_Material.specular", mat->specular(), false );
                    m_program->setUniform( "un_Material.emission", mat->emission(), false );
                    m_program->setUniform( "un_Material.shininess", mat->shininess() );
                    m_program->setUniform( "un_Material.isShadeless", mat->isShadeless() );
                    
                    // Setup the texturing
                    b8 doTexturing = false;
                    if ( m_texLookup != 0 && mat->texture() != "" )
                    {
                        ITexture* tex = m_texLookup ( mat->texture() );
                        if ( tex != 0 && tex->bind() &&
                             geometry->bindVertexLayer(m_program, "in_TexCoord", "uv", 0, Geometry::FLOAT, false, 2, 0) )
                        {
                            doTexturing = true;
                        }
                    }
                    
                    if ( doTexturing )
                    {
                        m_program->setUniform( "un_Material.textureLevels", 1 );
                        m_program->setUniform( "un_Samplers0", 0 );
                    }
                    else
                    {
                        m_program->setUniform( "un_Material.textureLevels", 0 );
                        geometry->unbindAttribute(m_program, "in_TexCoord");
                    }
                }
                else
                {
                    m_program->setUniform( "un_Material.diffuse", Color(255*0.7f, 255*0.7f, 255*0.7f, 255) );
                    m_program->setUniform( "un_Material.ambient", Vector3(0.7f, 0.7f, 0.7f) );
                    m_program->setUniform( "un_Material.specular", Vector3(0.0f, 0.0f, 0.0f) );
                    m_program->setUniform( "un_Material.emission", Vector3(0.0f, 0.0f, 0.0f) );
                    m_program->setUniform( "un_Material.shininess", 0.0f );
                    m_program->setUniform( "un_Material.isShadeless", false );
                    geometry->unbindAttribute(m_program, "in_TexCoord");
                }

                glDrawElements ( polyType, mesh->numIndices(), GL_UNSIGNED_INT, reinterpret_cast<const GLvoid *>((*iter).offset * sizeof(u32)) );
                eglGetError();
            }
        }
    }
    
    return true;
}

bool GLES20_Renderer::endScene()
{
    glUseProgram ( 0 );
    return true;
}

void GLES20_Renderer::pushState ()
{
}

void GLES20_Renderer::popState ()
{
}

