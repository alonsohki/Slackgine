#ifndef RENDERER_H
#define	RENDERER_H

#include "l3m/l3m.h"
#include "shared/platform.h"
#include "core/entity.h"

class IRenderer
{
public:
    virtual bool        Initialize      () = 0;
    virtual bool        SetupModel      ( const l3m* model ) = 0;
    
    virtual bool        BeginScene      () = 0;
    virtual bool        Render          ( const l3m* model ) = 0;
    virtual bool        EndScene        () = 0;
    
    virtual void        GetError        ( char* dest ) const = 0;
};

#include "renderers/renderer_selection.h"

#endif	/* RENDERER_H */

