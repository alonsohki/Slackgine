#ifndef OPENGL3_PROGRAM_H
#define	OPENGL3_PROGRAM_H

#include "opengl3.h"

class OpenGL3_Program : public IProgram
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
                        OpenGL3_Program         ();
                        ~OpenGL3_Program        ();
    
    bool                AttachShader            ( const IShader* shader );
    bool                DetachShader            ( const IShader* shader );    
    bool                Link                    ();
    bool                Use                     ();
    
    bool                Ok                      () const { return m_linked && m_handler > 0; }
    
    GLuint&             handler                 () { return m_handler; }
    const GLuint&       handler                 () const { return m_handler; }
};

#endif	/* OPENGL3_PROGRAM_H */

