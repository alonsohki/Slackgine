/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        texture.h
// PURPOSE:     OpenGL3 textures.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#include <cstring>
#include <string>
#include "shared/platform.h"
#include "shared/pixmap.h"
#include "gles20.h"

namespace Renderer
{
    
class GLES20_Texture : public ITexture
{
public:
                        GLES20_Texture          ( u32 width, u32 height, ITexture::Format format );
                        ~GLES20_Texture         ();
    
    void                loadPixmap              ( const Pixmap& pix );
    void                storePixmap             ( Pixmap* output ) const;
    bool                bind                    ();
    
private:
    static GLenum       convertFormat           ( ITexture::Format format );
    
private:
    GLuint      m_handler;
    u32         m_width;
    u32         m_height;
};

}
