#ifndef OPENGL3_RENDERER_H
#define	OPENGL3_RENDERER_H

#include <cstring>
#include "opengl3.h"
#include "math/matrix.h"
#include "../../renderer.h"

namespace Renderer
{

class OpenGL3_Renderer : public IRenderer
{
private:
    bool        m_initialized;
    IShader*    m_vertexShader;
    IShader*    m_fragmentShader;
    IProgram*   m_program;
    char        m_error [ 512 ];
    Matrix      m_matProjection;
    Matrix      m_matLookat;

public:
                OpenGL3_Renderer        ();
                ~OpenGL3_Renderer       ();
    
    bool        Initialize              ();
    
    bool        BeginScene              ( const Matrix& matProjection = IdentityMatrix(), const Matrix& matLookat = IdentityMatrix() );
    void        PushState               ();
    bool        Render                  ( Geometry* geometry, const Matrix& mat = IdentityMatrix() );
    void        PopState                ();
    bool        EndScene                ();
    
    void        GetError                ( char* dest ) const { strcpy(dest, m_error); }
};

}


#endif	/* OPENGL3_RENDERER_H */

