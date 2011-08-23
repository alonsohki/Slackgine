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
// FILE:        texture.h
// PURPOSE:     OpenGL3 textures.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#include <cstring>
#include <string>
#include "shared/platform.h"
#include "shared/pixmap.h"
#include "opengl3.h"

namespace Renderer
{
    
class OpenGL3_Texture : public ITexture
{
public:
                        OpenGL3_Texture         ( u32 width, u32 height, ITexture::Format format );
                        ~OpenGL3_Texture        ();
    
    void                LoadPixmap              ( const Pixmap& pix );
    void                StorePixmap             ( Pixmap* output ) const;
    bool                Bind                    ();
    
private:
    static GLenum       ConvertFormat           ( ITexture::Format format );
    
private:
    GLuint      m_handler;
    u32         m_width;
    u32         m_height;
};

}
