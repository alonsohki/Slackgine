#ifndef OPENGL3_RENDERER_H
#define	OPENGL3_RENDERER_H

#include <cstring>
#include "opengl3.h"
#include "math/matrix.h"

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
    Matrix      m_matrix;

public:
                OpenGL3_Renderer        ();
                ~OpenGL3_Renderer       ();
    
    bool        Initialize              ();
    bool        SetupModel              ( const l3m::Model* model );
    
    bool        BeginScene              ( const Matrix& matProjection = IdentityMatrix(), const Matrix& matLookat = IdentityMatrix() );
    bool        Render                  ( const l3m::Model* model, const Matrix& mat = IdentityMatrix() );
    bool        EndScene                ();
    
    void        GetError                ( char* dest ) const { strcpy(dest, m_error); }
};

}


#endif	/* OPENGL3_RENDERER_H */

