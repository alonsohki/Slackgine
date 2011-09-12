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
#include "math/transform.h"
#include "geometry.h"

namespace Renderer { class Geometry; }

namespace Renderer
{

class IRenderer
{
public:
    virtual bool        initialize      () = 0;
    
    virtual bool        beginScene      () = 0;
    virtual void        pushState       () = 0;
    virtual void        setCamera       ( const Matrix& matProjection = IdentityMatrix(), const Matrix& matLookat = IdentityMatrix() ) = 0;
    virtual bool        render          ( Geometry* geometry, const Transform& transform = IdentityTransform() ) = 0;
    virtual void        popState        () = 0;
    virtual bool        endScene        () = 0;
    
    virtual void        getError        ( char* dest ) const = 0;
};

}

#include "renderers/renderer_selection.h"

#endif	/* RENDERER_H */

