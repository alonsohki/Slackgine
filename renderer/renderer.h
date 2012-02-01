/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        renderer.h
// PURPOSE:     Interface for the renderers.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#ifndef RENDERER_H
#define	RENDERER_H

#include "shared/platform.h"
#include "shared/FastDelegate.h"
#include "math/transform.h"
#include "geometry.h"
#include "program.h"
#include "texture.h"

namespace Renderer { class Geometry; }

namespace Renderer
{

class IRenderer
{
public:
    typedef FastDelegate1<Mesh*, bool> MeshRenderFn;
    typedef FastDelegate1<const std::string&, ITexture*> TextureLookupFn;
    
public:
    virtual bool        initialize      () = 0;
    
    virtual bool        beginScene      ( const Matrix& matProjection,
                                          const Matrix& matLookat,
                                          TextureLookupFn fn ) = 0;
    virtual void        setProgram      ( IProgram* program ) = 0;
    virtual void        pushState       () = 0;
    virtual bool        render          ( Geometry* geometry, const Transform& transform = IdentityTransform(), MeshRenderFn fn = 0 ) = 0;
    virtual void        popState        () = 0;
    virtual bool        endScene        () = 0;
    
    virtual void        getError        ( char* dest ) const = 0;
};

}

#include "renderers/renderer_selection.h"

#endif	/* RENDERER_H */

