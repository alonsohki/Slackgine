#ifndef OPENGL3_SHADER_H
#define	OPENGL3_SHADER_H

#include <string>
#include "../../shader.h"

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
            m_handler = glCreateShader ( glType );
        else
            m_handler = 0;
    }
    
    ~OpenGL3_Shader ()
    {
        if ( m_handler > 0 )
            glDeleteShader ( m_handler );
    }
    
    bool Load ( std::istream& fp )
    {
        if ( m_handler == 0 )
            return false;
        
        char buffer [ 512 ];
        std::string out;
        
        while ( fp.eof() == false )
        {
            int bytes = fp.readsome( buffer, sizeof(buffer) );
            out.append ( buffer, bytes );
        }
        
        const GLchar* source = static_cast < const GLchar* > ( out.c_str() );
        glShaderSource ( m_handler, 1, &source, 0 );
        glCompileShader ( m_handler );
        
        GLint status;
        glGetShaderiv ( m_handler, GL_COMPILE_STATUS, &status );
        m_loaded = ( status == GL_TRUE );
        return m_loaded;
    }
    
    bool Ok () const
    {
        return m_handler > 0 && m_loaded;
    }
};

#endif	/* OPENGL3_SHADER_H */

