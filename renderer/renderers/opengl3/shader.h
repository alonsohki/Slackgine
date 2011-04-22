#ifndef OPENGL3_SHADER_H
#define	OPENGL3_SHADER_H

#include <string>
#include "opengl3.h"

class OpenGL3_Shader : public IShader
{
    bool        m_loaded;
    GLuint      m_handler;
    
public:
    OpenGL3_Shader ( IShader::Type type )
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
            eglGetError();
        }
        else
            m_handler = 0;
    }
    
    ~OpenGL3_Shader ()
    {
        if ( m_handler > 0 )
        {
            glDeleteShader ( m_handler );
            eglGetError();
        }
    }
    
    bool Load ( std::istream& fp )
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
        return m_loaded;
    }
    
    bool Ok () const
    {
        return m_handler > 0 && m_loaded;
    }
    
    GLuint&             handler () { return m_handler; }
    const GLuint&       handler () const { return m_handler; }
};

#endif	/* OPENGL3_SHADER_H */

