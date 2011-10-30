/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        texture.h
// PURPOSE:     Interface for the renderer textures.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#include "shared/pixmap.h"

namespace Renderer
{
    
class ITexture
{
public:
    enum Format
    {
        ALPHA4,
        ALPHA8,
        ALPHA12,
        ALPHA16,
        RGB4,
        RGBA4,
        RGB5,
        RGB5_A1,
        RGB8,
        RGBA8,
        RGB10_A2,
        RGBA12,
        RGB16,
        RGBA16
    };

public:
    virtual             ~ITexture       () {}
    
    virtual void        LoadPixmap      ( const Pixmap& pix ) = 0;
    virtual void        StorePixmap     ( Pixmap* output ) const = 0;
    
    virtual bool        Bind            () = 0;
};

}
