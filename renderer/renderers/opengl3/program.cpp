#include "opengl3.h"

OpenGL3_Program::OpenGL3_Program ()
: m_linked ( false )
{
    m_handler = glCreateProgram ();
    eglGetError();
}

OpenGL3_Program::~OpenGL3_Program()
{
    if ( m_handler > 0 )
    {
        glDeleteProgram ( m_handler );
        eglGetError();
    }
}

bool OpenGL3_Program::AttachShader(const IShader* shader_)
{
    const OpenGL3_Shader* shader = static_cast<const OpenGL3_Shader*>(shader_);
    if ( m_handler == 0 || shader->Ok() == false )
        return false;

    glAttachShader ( m_handler, shader->handler() );
    eglGetError();
    return true;
}

bool OpenGL3_Program::DetachShader(const IShader* shader_)
{
    const OpenGL3_Shader* shader = static_cast<const OpenGL3_Shader*>(shader_);
    if ( m_handler == 0 || shader->Ok() == false )
        return false;

    glDetachShader ( m_handler, shader->handler() );
    eglGetError();
    return true;
}

bool OpenGL3_Program::Link()
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
    if ( linked == GL_TRUE )
        m_linked = true;
    else
        m_linked = false;
    return m_linked;
}

bool OpenGL3_Program::Use ()
{
    if ( Ok() )
    {
        glUseProgram ( m_handler );
        eglGetError();
    }
    return Ok ();
}
