/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        shader.cpp
// PURPOSE:     OpenGL3 shaders.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include "opengl3.h"

using namespace Renderer;

OpenGL3_Shader::OpenGL3_Shader ( IShader::Type type )
: m_loaded ( false )
{
    GLenum glType = GL_INVALID_ENUM;

    switch ( type )
    {
        case VERTEX_SHADER:
            glType = GL_VERTEX_SHADER;
            break;
        case FRAGMENT_SHADER:
            glType = GL_FRAGMENT_SHADER;
            break;
        case GEOMETRY_SHADER:
        default:
            break;
    }

    if ( glType != GL_INVALID_ENUM )
    {
        m_handler = glCreateShader ( glType );
        strcpy ( m_error, "Success" );
        eglGetError();
    }
    else
    {
        m_handler = 0;
        strcpy ( m_error, "Invalid or unsupported shader type" );
    }
}

OpenGL3_Shader::~OpenGL3_Shader()
{
    if ( m_handler > 0 )
    {
        glDeleteShader ( m_handler );
        eglGetError();
    }
}

bool OpenGL3_Shader::load ( std::istream& fp )
{
    if ( m_handler == 0 )
        return false;

    char buffer [ 512 ];
    std::string out;

    while ( fp.eof() == false )
    {
        fp.read( buffer, sizeof(buffer) );
        out.append ( buffer, fp.gcount() );
    }

    const GLchar* source = static_cast < const GLchar* > ( out.c_str() );
    glShaderSource ( m_handler, 1, &source, 0 );
    eglGetError();
    glCompileShader ( m_handler );
    eglGetError();

    GLint status;
    glGetShaderiv ( m_handler, GL_COMPILE_STATUS, &status );
    eglGetError();
    m_loaded = ( status == GL_TRUE );
    
    if ( !m_loaded )
        glGetShaderInfoLog(m_handler, sizeof(m_error), 0, m_error );
    
    return m_loaded;
}

void OpenGL3_Renderer::pushState ()
{
    
}

void OpenGL3_Renderer::popState ()
{
    
}
