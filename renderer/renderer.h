#ifndef RENDERER_H
#define	RENDERER_H

#include "entity.h"

class IRenderer
{
public:
    virtual bool        Initialize      () = 0;
    
    virtual bool        BeginScene      () = 0;
    virtual bool        RenderEntity    ( Entity* pEntity ) = 0;
    virtual bool        EndScene        () = 0;
};

#include "renderers/renderer_selection.h"

#endif	/* RENDERER_H */

