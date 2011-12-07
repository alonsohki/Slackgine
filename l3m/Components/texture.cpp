/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        texture.cpp
// PURPOSE:     Texture component.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include <sstream>
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
    
    // Read the image size
    u32 size;
    if ( stream.Read32 ( &size, 1 ) != 1 )
        return SetError ( "Error reading the texture image size" );

    // Create a buffer for this
    char* buffer = new char [ size ];
    // Fill it
    if ( stream.ReadData ( buffer, size, 1 ) != 1 )
        return SetError ( "Error reading the texture pixel data" );

    std::istringstream is ( buffer );
    m_image.Load ( is );

    delete [] buffer;

    return true;
}

bool Texture::Save ( OStream& stream )
{
    // Write the texture id
    if ( !stream.WriteStr ( m_id ) )
        return SetError ( "Error writing the texture ID" );

    // Compress the pixmap
    std::ostringstream os;
    m_image.SavePNG ( os );
    
    // Write the result size
    u32 len = os.str().length ();
    if ( !stream.Write32(&len, 1) )
        return SetError ( "Error writing the texture image size" );
    
    // Write the image data
    if ( !stream.WriteData(os.str().c_str(), len, 1) )
        return SetError ( "Error writing the texture pixel data" );

    return true;
}

