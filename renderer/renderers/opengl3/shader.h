#ifndef OPENGL3_SHADER_H
#define	OPENGL3_SHADER_H

#include <cstring>
#include <string>
#include "opengl3.h"

namespace Renderer
{
    
class OpenGL3_Shader : public IShader
{
    bool        m_loaded;
    GLuint      m_handler;
    char        m_error [ 512 ];
    
public:
                        OpenGL3_Shader          ( IShader::Type type );
                        ~OpenGL3_Shader         ();
    
    bool                Load                    ( std::istream& fp );
    bool                Ok                      () const { return m_handler > 0 && m_loaded; }
    void                GetError                ( char* dest ) const { strcpy(dest, m_error); }
    
    GLuint&             handler                 () { return m_handler; }
    const GLuint&       handler                 () const { return m_handler; }
};

}

#endif	/* OPENGL3_SHADER_H */

