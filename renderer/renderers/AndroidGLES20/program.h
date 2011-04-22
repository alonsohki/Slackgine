#ifndef GLES20_PROGRAM_H
#define	GLES20_PROGRAM_H

#include "gles20.h"

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
    
public:
                        GLES20_Program         ();
                        ~GLES20_Program        ();
    
    bool                AttachShader            ( const IShader* shader );
    bool                DetachShader            ( const IShader* shader );    
    bool                Link                    ();
    bool                Use                     ();
    
    bool                Ok                      () const { return m_linked && m_handler > 0; }
    
    GLuint&             handler                 () { return m_handler; }
    const GLuint&       handler                 () const { return m_handler; }
};

#endif	/* GLES20_PROGRAM_H */

