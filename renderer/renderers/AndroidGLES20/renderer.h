#ifndef GLES20_RENDERER_H
#define	GLES20_RENDERER_H

#include <sstream>
#include "gles20.h"

class GLES20_Renderer : public IRenderer
{
private:
    bool        m_initialized;
    IShader*    m_vertexShader;
    IShader*    m_fragmentShader;
    IProgram*   m_program;
    char        m_error [ 512 ];

public:
                GLES20_Renderer        ();
                ~GLES20_Renderer       ();
    
    bool        Initialize              ();
    bool        SetupModel              ( const l3mComponent* model );

    bool        BeginScene              ();
    bool        Render                  ( const l3mComponent* model );
    bool        EndScene                ();

    void        GetError                ( char* dest ) const { strcpy(dest, m_error); }
};


#endif	/* GLES20_RENDERER_H */

