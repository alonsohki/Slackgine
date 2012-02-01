/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        pixmap.h
// PURPOSE:     Pixel maps (pictures).
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#include <iostream>
#include "color.h"

class Pixmap
{
public:
                        Pixmap          ();
                        Pixmap          ( u32 width, u32 height );
                        Pixmap          ( const Pixmap& other );
                        ~Pixmap         ();
private:
    void                cleanupData     ();
    
public:
    Pixmap&             operator=       ( const Pixmap& other );
    bool                operator==      ( const Pixmap& other ) const;
    bool                operator!=      ( const Pixmap& other ) const;
    
    void                create          ( u32 width, u32 height, const Color* data = 0 );
    void                resample        ( u32 newWidth, u32 newHeight );
private:
    void                resample_X      ( u32 newWidth );
    void                resample_Y      ( u32 newHeight );
public:

    bool                load            ( const char* filename );
    bool                load            ( std::istream& stream );
    bool                savePNG         ( const char* filename );
    bool                savePNG         ( std::ostream& stream );

    const char*         error           () const { return m_error; }
    const u32&          width           () const { return m_width; }
    const u32&          height          () const { return m_height; }
    const Color*        pixels          () const { return m_pixels; }
    Color*              pixels          () { return m_pixels; }
    
private:
    bool                loadPNG         ( std::istream& stream );

private:
    u32         m_width;
    u32         m_height;
    Color*      m_pixels;
    char        m_error [ 512 ];
};