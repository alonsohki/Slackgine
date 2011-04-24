#ifndef RENDERER_H
#define	RENDERER_H

#include "shared/platform.h"
#include "core/entity.h"

class IRenderer
{
public:
    virtual bool        Initialize      () = 0;
    virtual bool        SetupModel      ( const l3m* model ) = 0;
    
    virtual bool        BeginScene      () = 0;
    virtual bool        RenderEntity    ( const Entity* pEntity ) = 0;
    virtual bool        EndScene        () = 0;
    
    virtual void        GetError        ( char* dest ) const = 0;
};

#include "renderers/renderer_selection.h"

#endif	/* RENDERER_H */

