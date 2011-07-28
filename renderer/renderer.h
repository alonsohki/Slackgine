#ifndef RENDERER_H
#define	RENDERER_H

#include "shared/platform.h"
#include "math/matrix.h"
#include "mesh.h"

namespace Renderer
{

class IRenderer
{
public:
    virtual bool        Initialize      () = 0;
    
    virtual bool        BeginScene      ( const Matrix& matProjection = IdentityMatrix(), const Matrix& matLookat = IdentityMatrix() ) = 0;
    virtual void        PushState       () = 0;
    virtual bool        Render          ( const Mesh* mesh, const Matrix& mat = IdentityMatrix() ) = 0;
    virtual void        PopState        () = 0;
    virtual bool        EndScene        () = 0;
    
    virtual void        GetError        ( char* dest ) const = 0;
};

}

#include "renderers/renderer_selection.h"

#endif	/* RENDERER_H */

