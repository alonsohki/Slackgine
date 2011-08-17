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
// FILE:        texture.cpp
// PURPOSE:     Texture component.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include "texture.h"

using namespace l3m;

Texture::Texture ()
: IComponent ( "texture", 1.0f )
{
}

Texture::~Texture ()
{
}

bool Texture::Load ( IStream& stream, float version )
{
    // Load the texture string
    if ( stream.ReadStr ( m_id ) <= 0 )
        return SetError ( "Error reading the texture ID" );
    
    // Read the image size;
    u32 size [ 2 ];
    if ( stream.Read32 ( size, 2 ) != 2 )
        return SetError ( "Error reading the texture image size" );
    
    // Create the pixmap
    m_image.Create ( size[0], size[1] );
    
    // Load the pixels
    if ( stream.ReadColor ( m_image.pixels(), size[0]*size[1] ) != size[0]*size[1] )
        return SetError ( "Error reading the texture pixel data" );
    
    return true;
}

bool Texture::Save ( OStream& stream )
{
    // Write the texture id
    if ( !stream.WriteStr ( m_id ) )
        return SetError ( "Error writing the texture ID" );
    
    // Write the image size
    u32 size [ 2 ];
    size[0] = m_image.width();
    size[1] = m_image.height();
    if ( !stream.Write32(size, 2) )
        return SetError ( "Error writing the texture image size" );
    
    // Write the image data
    if ( !stream.WriteColor(m_image.pixels(), size[0]*size[1]) )
        return SetError ( "Error writing the texture pixel data" );

    return true;
}
