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
    if ( stream.Read32 ( &(m_image.pixels()->value()), size[0]*size[1] ) != size[0]*size[1] )
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
    u32* values = &(m_image.pixels()->value());
    if ( !stream.Write32(values, size[0]*size[1]) )
        return SetError ( "Error writing the texture pixel data" );

    return true;
}
