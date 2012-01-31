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

    // Initialize the main shaders
    static const char* const s_defaultVertexShader =
        "precision mediump float;\n"
        "attribute vec3 in_Position;\n"
        "attribute vec3 in_Normal;\n"
        "attribute vec2 in_Tex2D;\n"
        "\n"
        "uniform mat4 un_Matrix;\n"
        "uniform mat4 un_ProjectionMatrix;\n"
        "uniform mat4 un_LookatMatrix;\n"
        "uniform mat4 un_ModelviewMatrix;\n"
        "\n"
        "uniform mat4 un_NormalMatrix;\n"
        "\n"
        "varying vec3 ex_Normal;\n"
        "void main(void)\n"
        "{\n"
        "    gl_Position = un_Matrix * vec4(in_Position, 1.0);"
        "    ex_Normal = (un_NormalMatrix * vec4(in_Normal, 1.0)).xyz;\n"
        "}\n";

    static const char* const s_defaultFragmentShader =
        "precision mediump float;\n"
        "varying vec3 ex_Normal;\n"
        "\n"
        "void main(void)\n"
        "{\n"
        "    gl_FragColor = vec4((ex_Normal + 1.0) * 0.5, 1.0);\n"
        "}\n";

    std::istringstream vertexShaderSource ( s_defaultVertexShader );
    std::istringstream fragmentShaderSource ( s_defaultFragmentShader );

    m_program = new GLES20_Program ();
    m_vertexShader = new GLES20_Shader (IShader::VERTEX_SHADER);
    m_fragmentShader = new GLES20_Shader (IShader::FRAGMENT_SHADER);

    strcpy ( m_error, "Success" );
    
    char error [ 1024 ];
    if ( !m_vertexShader->Load (vertexShaderSource) )
    {
        m_vertexShader->GetError ( error );
        snprintf ( m_error, sizeof(m_error), "Error loading the vertex shader: %s", error );
    }
    else if ( !m_fragmentShader->Load ( fragmentShaderSource ) )
    {
        m_fragmentShader->GetError( error );
        snprintf ( m_error, sizeof(m_error), "Error loading the fragment shader: %s", error );
    }
    else if ( !m_program->AttachShader(m_vertexShader) || !m_program->AttachShader(m_fragmentShader) )
    {
        m_program->GetError ( error );
        snprintf ( m_error, sizeof(m_error), "Error attaching the shaders: %s", error );
    }
    else if ( !m_program->Link() )
    {
        m_program->GetError( error );
        snprintf ( m_error, sizeof(m_error), "Error linking the shaders: %s", error );
    }

    m_initialized = ( strcmp(m_error, "Success") == 0 );
    return m_initialized;
}

bool GLES20_Renderer::beginScene ( const Matrix& matProjection, const Matrix& matLookat )
{
    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    eglGetError();

    if ( !m_program->Use () )
        return false;
    
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
    
    return true;
}

bool GLES20_Renderer::render ( Geometry* geometry, const Transform& transform, MeshRenderFn fn )
{
    if ( !geometry->initialized() )
        if ( !geometry->Initialize() )
            return false;
    
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
                m_program->SetUniform("un_ProjectionMatrix", m_matProjection );
                m_program->SetUniform("un_LookatMatrix", m_matLookat );
                m_program->SetUniform("un_ModelviewMatrix", mat);
                m_program->SetUniform("un_NormalMatrix", matNormals);
                m_program->SetUniform("un_Matrix", matGeometry );
                glDrawElements ( polyType, mesh->numIndices(), GL_UNSIGNED_INT, reinterpret_cast<const GLvoid *>((*iter).offset * sizeof(u32)) );
                eglGetError();
            }
        }
    }
    
    return true;
}

bool GLES20_Renderer::endScene()
{
    return true;
}

void GLES20_Renderer::pushState ()
{
}

void GLES20_Renderer::popState ()
{
}

