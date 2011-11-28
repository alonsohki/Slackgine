#ifndef GLES20_PROGRAM_H
#define	GLES20_PROGRAM_H

#include "gles20.h"

namespace Renderer
{

class GLES20_Program : public IProgram
{
public:
    enum DefaultAttributeIndex
    {
        POSITION        = 0,
        NORMAL          = 1,
        TEX2D           = 2
    };
    
private:
    GLuint      m_handler;
    bool        m_linked;
    char        m_error [ 512 ];
    
public:
                        GLES20_Program         ();
                        ~GLES20_Program        ();
    
    bool                AttachShader            ( const IShader* shader );
    bool                DetachShader            ( const IShader* shader );    
    bool                Link                    ();
    bool                Use                     ();
    
    bool                SetUniform              ( const std::string& name, f32 value );
    bool                SetUniform              ( const std::string& name, i32 value );
    bool                SetUniform              ( const std::string& name, const Vector2& vec );
    bool                SetUniform              ( const std::string& name, const Vector3& vec );
    bool                SetUniform              ( const std::string& name, const Matrix& mat );

    bool                Ok                      () const { return m_linked && m_handler > 0; }
    void                GetError                ( char* dest ) const { strcpy(dest, m_error); }
    
    GLuint&             handler                 () { return m_handler; }
    const GLuint&       handler                 () const { return m_handler; }
};

}

#endif	/* GLES20_PROGRAM_H */

