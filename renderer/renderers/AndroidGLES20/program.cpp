#include "gles20.h"

using namespace Renderer;

#if 0

GLES20_Program::GLES20_Program ()
: m_linked ( false )
{
    m_handler = glCreateProgram ();
    eglGetError();
    if ( m_handler == 0 )
        strcpy ( m_error, "Invalid handler" );
}

GLES20_Program::~GLES20_Program()
{
    if ( m_handler > 0 )
    {
        glDeleteProgram ( m_handler );
        eglGetError();
    }
}

bool GLES20_Program::AttachShader(const IShader* shader_)
{
    const GLES20_Shader* shader = static_cast<const GLES20_Shader*>(shader_);
    if ( m_handler == 0 || shader->Ok() == false )
        return false;

    glAttachShader ( m_handler, shader->handler() );
    eglGetError();
    return true;
}

bool GLES20_Program::DetachShader(const IShader* shader_)
{
    const GLES20_Shader* shader = static_cast<const GLES20_Shader*>(shader_);
    if ( m_handler == 0 || shader->Ok() == false )
        return false;

    glDetachShader ( m_handler, shader->handler() );
    eglGetError();
    return true;
}

bool GLES20_Program::Link()
{
    if ( m_handler == 0 )
        return false;

    glBindAttribLocation ( m_handler, POSITION, "in_Position" );
    eglGetError();
    glBindAttribLocation ( m_handler, NORMAL, "in_Normal" );
    eglGetError();
    glBindAttribLocation ( m_handler, TEX2D, "in_Tex2D" );
    eglGetError();

    glLinkProgram ( m_handler );
    eglGetError();

    GLint linked;
    glGetProgramiv ( m_handler, GL_LINK_STATUS, &linked );
    eglGetError();
    m_linked = !!linked;

    if ( !m_linked )
        glGetShaderInfoLog ( m_handler, sizeof(m_error), 0, m_error );

    return m_linked;
}

bool GLES20_Program::Use ()
{
    if ( Ok() )
    {
        glUseProgram ( m_handler );
        eglGetError();
    }
    return Ok ();
}

#endif
