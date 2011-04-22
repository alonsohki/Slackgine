#ifndef GLES20_RENDERER_H
#define	GLES20_RENDERER_H

#include <sstream>
#include "gles20.h"

class GLES20_Renderer : public IRenderer
{
private:
    IShader*    m_vertexShader;
    IShader*    m_fragmentShader;
    IProgram*   m_program;

public:
                GLES20_Renderer        ();
                ~GLES20_Renderer       ();
    
    bool        Initialize              ();
    bool        SetupModel              ( const l3m* model );
    bool        BeginScene              ();
    bool        RenderEntity            ( const Entity* entity );
    bool        EndScene                ();
};


#endif	/* GLES20_RENDERER_H */

