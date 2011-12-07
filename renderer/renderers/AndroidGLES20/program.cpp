#include "gles20.h"

using namespace Renderer;

GLES20_Program::GLES20_Program ()
: m_linked ( false )
{
    m_handler = glCreateProgram ();
    eglGetError();
    strcpy ( m_error, "Success" );
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
    m_linked = ( linked == GL_TRUE );

    if ( !m_linked )
        glGetShaderInfoLog ( m_handler, sizeof(m_error), 0, m_error );
    
    return m_linked;
}

bool GLES20_Program::Use ()
{
    {
        glUseProgram ( m_handler );
        eglGetError();
    }
    return Ok ();
}

bool GLES20_Program::SetUniform(const std::string& name, f32 value)
{
    GLint loc = glGetUniformLocation ( handler(), name.c_str() );
    eglGetError();
    if ( loc == -1 )
        return false;
    glUniform1f ( loc, value );
    eglGetError();
    return true;
}

bool GLES20_Program::SetUniform(const std::string& name, i32 value)
{
    GLint loc = glGetUniformLocation ( handler(), name.c_str() );
    eglGetError();
    if ( loc == -1 )
        return false;
    glUniform1i ( loc, value );
    eglGetError();
    return true;
}

bool GLES20_Program::SetUniform(const std::string& name, const Vector2& vec)
{
    GLint loc = glGetUniformLocation ( handler(), name.c_str() );
    eglGetError();
    if ( loc == -1 )
        return false;
    glUniform2fv ( loc, 2, vec.vector() );
    eglGetError();
    return true;
}

bool GLES20_Program::SetUniform(const std::string& name, const Vector3& vec)
{
    GLint loc = glGetUniformLocation ( handler(), name.c_str() );
    eglGetError();
    if ( loc == -1 )
        return false;
    glUniform3fv ( loc, 3, vec.vector() );
    eglGetError();
    return true;
}

bool GLES20_Program::SetUniform(const std::string& name, const Matrix& mat)
{
    GLint loc = glGetUniformLocation ( handler(), name.c_str() );
    eglGetError();
    if ( loc == -1 )
        return false;
    glUniformMatrix4fv ( loc, 1, GL_FALSE, mat.vector() );
    eglGetError();
    return true;
}

