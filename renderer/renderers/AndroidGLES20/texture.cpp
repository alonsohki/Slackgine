/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        texture.cpp
// PURPOSE:     OpenGL3 textures.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include "texture.h"

using namespace Renderer;

GLES20_Texture::GLES20_Texture ( u32 width, u32 height, ITexture::Format format )
{
    GLenum glFormat = ConvertFormat (format);
    if ( glFormat != GL_INVALID_ENUM )
    {
        glGenTextures ( 1, &m_handler );
        eglGetError ();
        if ( m_handler != 0 )
        {
            glBindTexture ( GL_TEXTURE_2D, m_handler );
            eglGetError ();
            glTexImage2D ( GL_TEXTURE_2D, 0, glFormat, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0 );
            eglGetError ();
        }
    }
    else
        m_handler = 0;
    
    m_width = width;
    m_height = height;
}

GLES20_Texture::~GLES20_Texture ()
{
    if ( m_handler != 0 )
        glDeleteTextures ( 1, &m_handler );
}

void GLES20_Texture::LoadPixmap (const Pixmap& pix)
{
    Pixmap pixCopy = pix;
    pixCopy.Resample ( m_width, m_height );
    if ( m_handler != 0 )
    {
        glBindTexture ( GL_TEXTURE_2D, m_handler );
        eglGetError ();
        glTexSubImage2D ( GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, pixCopy.pixels () );
        eglGetError ();
    }
}

void GLES20_Texture::StorePixmap (Pixmap* output) const
{
    // TODO
}

bool GLES20_Texture::Bind ()
{
    if ( m_handler != 0 )
    {
        glBindTexture ( GL_TEXTURE_2D, m_handler );
        return true;
    }
    return false;
}

GLenum GLES20_Texture::ConvertFormat (ITexture::Format format)
{
    GLenum ret;
    
    switch ( format )
    {
        case RGB8: ret = GL_RGB; break;
        case RGBA8: ret = GL_RGBA; break;
        case RGBA4: ret = GL_RGBA4; break;
        case RGB5_A1: ret = GL_RGB5_A1; break;
        default:
            ret = GL_INVALID_ENUM;
            break;
    }
    
    return ret;
}
