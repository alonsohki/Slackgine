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

bool Texture::load ( l3m::Model*, IStream& stream, float version )
{
    // Load the texture string
    if ( stream.readStr ( &m_id ) <= 0 )
        return setError ( "Error reading the texture ID" );
    
    // Read the image size
    u32 size;
    if ( stream.read32 ( &size, 1 ) != 1 )
        return setError ( "Error reading the texture image size" );

    // Create a buffer for this
    char* buffer = new char [ size ];
    // Fill it
    if ( stream.readData ( buffer, size, 1 ) != 1 )
        return setError ( "Error reading the texture pixel data" );
    
    std::istringstream is ( std::string(buffer, size) );
    m_image.load ( is );

    delete [] buffer;

    return true;
}

bool Texture::save ( l3m::Model*, OStream& stream )
{
    // Write the texture id
    if ( !stream.writeStr ( m_id ) )
        return setError ( "Error writing the texture ID" );

    // Compress the pixmap
    std::ostringstream os;
    m_image.savePNG ( os );
    
    // Write the result size
    u32 len = os.str().length ();
    if ( !stream.write32(&len, 1) )
        return setError ( "Error writing the texture image size" );
    
    // Write the image data
    if ( !stream.writeData(os.str().c_str(), len, 1) )
        return setError ( "Error writing the texture pixel data" );

    return true;
}

