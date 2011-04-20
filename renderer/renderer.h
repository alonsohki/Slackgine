#ifndef RENDERER_H
#define	RENDERER_H

#include "entity.h"

class IRenderer
{
public:
    virtual bool        Initialize      () = 0;
    
    virtual bool        BeginScene      () = 0;
    virtual bool        RenderEntity    ( const Entity* pEntity ) = 0;
    virtual bool        EndScene        () = 0;
    
    static IRenderer*   GetRenderer     ();
};

#endif	/* RENDERER_H */

