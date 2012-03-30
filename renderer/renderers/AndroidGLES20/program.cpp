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

bool GLES20_Program::attachShader(const IShader* shader_)
{
    const GLES20_Shader* shader = static_cast<const GLES20_Shader*>(shader_);
    if ( m_handler == 0 || shader->ok() == false )
        return false;

    glAttachShader ( m_handler, shader->handler() );
    eglGetError();
    return true;
}

bool GLES20_Program::detachShader(const IShader* shader_)
{
    const GLES20_Shader* shader = static_cast<const GLES20_Shader*>(shader_);
    if ( m_handler == 0 || shader->ok() == false )
        return false;

    glDetachShader ( m_handler, shader->handler() );
    eglGetError();
    return true;
}

bool GLES20_Program::link()
{
    if ( m_handler == 0 )
        return false;

    glBindAttribLocation ( m_handler, POSITION, "in_Position" );
    eglGetError();
    glBindAttribLocation ( m_handler, NORMAL, "in_Normal" );
    eglGetError();
    glBindAttribLocation ( m_handler, TEX2D, "in_TexCoord" );
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

bool GLES20_Program::use ()
{
    {
        glUseProgram ( m_handler );
        eglGetError();
    }
    return ok ();
}

bool GLES20_Program::setUniform(const std::string& name, b8 value)
{
    GLint loc = glGetUniformLocation ( handler(), name.c_str() );
    eglGetError();
    if ( loc == -1 )
        return false;
    glUniform1i ( loc, value );
    eglGetError();
    return true;
}

bool GLES20_Program::setUniform(const std::string& name, f32 value)
{
    GLint loc = glGetUniformLocation ( handler(), name.c_str() );
    eglGetError();
    if ( loc == -1 )
        return false;
    glUniform1f ( loc, value );
    eglGetError();
    return true;
}

bool GLES20_Program::setUniform(const std::string& name, i32 value)
{
    GLint loc = glGetUniformLocation ( handler(), name.c_str() );
    eglGetError();
    if ( loc == -1 )
        return false;
    glUniform1i ( loc, value );
    eglGetError();
    return true;
}

bool GLES20_Program::setUniform(const std::string& name, const Vector2& vec)
{
    GLint loc = glGetUniformLocation ( handler(), name.c_str() );
    eglGetError();
    if ( loc == -1 )
        return false;
    glUniform2fv ( loc, 1, vec.vector() );
    eglGetError();
    return true;
}

bool GLES20_Program::setUniform(const std::string& name, const Vector3& vec)
{
    GLint loc = glGetUniformLocation ( handler(), name.c_str() );
    eglGetError();
    if ( loc == -1 )
        return false;
    glUniform3fv ( loc, 1, vec.vector() );
    eglGetError();
    return true;
}

bool GLES20_Program::setUniform(const std::string& name, const Matrix& mat)
{
    GLint loc = glGetUniformLocation ( handler(), name.c_str() );
    eglGetError();
    if ( loc == -1 )
        return false;
    glUniformMatrix4fv ( loc, 1, GL_FALSE, mat.vector() );
    eglGetError();
    return true;
}

bool GLES20_Program::setUniform(const std::string& name, const Matrix* mat, u32 count)
{
    GLint loc = glGetUniformLocation ( handler(), name.c_str() );
    eglGetError();
    if ( loc == -1 )
        return false;
    glUniformMatrix4fv ( loc, count, GL_FALSE, mat->vector() );
    eglGetError();
    return true;
}

bool GLES20_Program::setUniform(const std::string& name, const Color& col, bool includeAlpha)
{
    GLint loc = glGetUniformLocation ( handler(), name.c_str() );
    eglGetError();
    if ( loc == -1 )
        return false;
    
    float values [ 4 ];
    values[0] = col.r() / 255.0f;
    values[1] = col.g() / 255.0f;
    values[2] = col.b() / 255.0f;
    values[3] = col.a() / 255.0f;
    
    if ( includeAlpha )
        glUniform4fv ( loc, 1, &values[0] );
    else
        glUniform3fv ( loc, 1, &values[0] );
    eglGetError();
    return true;
}

bool GLES20_Program::setUniform(const std::string& name, f32* values, u32 count)
{
    GLint loc = glGetUniformLocation ( handler(), name.c_str() );
    eglGetError();
    if ( loc == -1 )
        return false;
    glUniform1fv ( loc, count, values );
    eglGetError();
    return true;
}