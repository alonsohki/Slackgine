#ifndef OPENGL3_RENDERER_H
#define	OPENGL3_RENDERER_H

#include <sstream>
#include "opengl3.h"

class OpenGL3_Renderer : public IRenderer
{
private:
    IShader*    m_vertexShader;
    IShader*    m_fragmentShader;
    IProgram*   m_program;

public:
                OpenGL3_Renderer        ();
                ~OpenGL3_Renderer       ();
    
    bool        Initialize              ();
    bool        SetupModel              ( const l3m* model );
    bool        BeginScene              ();
    bool        RenderEntity            ( const Entity* entity );
    bool        EndScene                ();
};


#endif	/* OPENGL3_RENDERER_H */

