#ifndef GLES20_SHADER_H
#define	GLES20_SHADER_H

#include <string>
#include "gles20.h"


namespace Renderer
{

class GLES20_Shader : public IShader
{
    bool        m_loaded;
    GLuint      m_handler;
    char        m_error [ 512 ];
    
public:
                        GLES20_Shader          ( IShader::Type type );
                        ~GLES20_Shader         ();
    
    bool                load                    ( std::istream& fp );
    bool                ok                      () const { return m_handler > 0 && m_loaded; }
    void                getError                ( char* dest ) const { strcpy(dest, m_error); }
    
    GLuint&             handler                 () { return m_handler; }
    const GLuint&       handler                 () const { return m_handler; }
};

};

#endif	/* GLES20_SHADER_H */

