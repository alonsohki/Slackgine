#ifndef GLES20_RENDERER_H
#define	GLES20_RENDERER_H

#include <sstream>
#include "gles20.h"

namespace Renderer
{
    
class GLES20_Renderer : public IRenderer
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
                GLES20_Renderer        ();
                ~GLES20_Renderer       ();
    
    bool        initialize              ();

    bool        beginScene              ();
    bool        render                  ( Geometry* geometry, const Transform& transform );
    bool        endScene                ();

    void        setCamera               ( const Matrix& matProjection, const Matrix& matLookat );
    void        pushState               ();
    void        popState                ();

    void        getError                ( char* dest ) const { strcpy(dest, m_error); }
};

};

#endif	/* GLES20_RENDERER_H */

