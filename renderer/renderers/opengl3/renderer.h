#ifndef OPENGL3_RENDERER_H
#define	OPENGL3_RENDERER_H

#include <cstring>
#include "opengl3.h"

class OpenGL3_Renderer : public IRenderer
{
private:
    IShader*    m_vertexShader;
    IShader*    m_fragmentShader;
    IProgram*   m_program;
    char        m_error [ 512 ];

public:
                OpenGL3_Renderer        ();
                ~OpenGL3_Renderer       ();
    
    bool        Initialize              ();
    bool        SetupModel              ( const l3m* model );
    
    bool        BeginScene              ();
    bool        Render                  ( const l3m* model );
    bool        EndScene                ();
    
    void        GetError                ( char* dest ) const { strcpy(dest, m_error); }
};


#endif	/* OPENGL3_RENDERER_H */

