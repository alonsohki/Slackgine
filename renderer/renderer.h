/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011 - Alberto Alonso
//
// Redistribution of this software, including any of its
// files, in either binary or source code form is totally
// forbidden without the permission of its owners.
//
// Owners reserve their right to legal action against any
// person or company that fails to do so.
//
// For more detailed information, see the LICENSE file in
// the top-level directory.
//
// FILE:        renderer.h
// PURPOSE:     Interface for the renderers.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#ifndef RENDERER_H
#define	RENDERER_H

#include "shared/platform.h"
#include "math/matrix.h"
#include "geometry.h"

namespace Renderer { class Geometry; }

namespace Renderer
{

class IRenderer
{
public:
    virtual bool        Initialize      () = 0;
    
    virtual bool        BeginScene      ( const Matrix& matProjection = IdentityMatrix(), const Matrix& matLookat = IdentityMatrix() ) = 0;
    virtual void        PushState       () = 0;
    virtual bool        Render          ( Geometry* geometry, const Matrix& mat = IdentityMatrix() ) = 0;
    virtual void        PopState        () = 0;
    virtual bool        EndScene        () = 0;
    
    virtual void        GetError        ( char* dest ) const = 0;
};

}

#include "renderers/renderer_selection.h"

#endif	/* RENDERER_H */

