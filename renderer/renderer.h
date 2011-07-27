#ifndef RENDERER_H
#define	RENDERER_H

#include "l3m/l3m.h"
#include "shared/platform.h"
#include "core/entity.h"
#include "math/matrix.h"

namespace Renderer
{
    
class IRenderer
{
public:
    virtual bool        Initialize      () = 0;
    
    virtual bool        BeginScene      ( const Matrix& matProjection = IdentityMatrix(), const Matrix& matLookat = IdentityMatrix() ) = 0;
    virtual bool        Render          ( const l3m::Model* model, const Matrix& mat = IdentityMatrix() ) = 0;
    virtual bool        EndScene        () = 0;
    
    virtual void        GetError        ( char* dest ) const = 0;
};

}

#include "renderers/renderer_selection.h"

#endif	/* RENDERER_H */

