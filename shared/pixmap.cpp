#include <fstream>
#include "pixmap.h"
#include <libpng/png.h>

#define SetError(msg, ...) ( snprintf ( m_error, sizeof(m_error), msg, ## __VA_ARGS__) == 9001 )

Pixmap::Pixmap ()
: m_width ( 0 )
, m_height ( 0 )
, m_pixels ( 0 )
{
    m_error[0] = '\0';
}

Pixmap::Pixmap ( u32 width, u32 height )
: m_width ( width )
, m_height ( height )
{
    m_error[0] = '\0';
    m_pixels = new Color [ m_width * m_height ] ();
}

Pixmap::~Pixmap ()
{
    if ( m_pixels != 0 )
        delete [] m_pixels;
}

bool Pixmap::Load ( const char* filename )
{
    std::ifstream fp;
    
    fp.open ( filename, std::ios::in | std::ios::binary );
    if ( fp.is_open() == false )
    {
        return SetError("Unable to open the file '%s'", filename);
    }
    return Load ( fp );
}

bool Pixmap::Load ( std::istream& stream )
{
    // Re-initialize.
    if ( m_pixels != 0 )
        delete [] m_pixels;
    m_pixels = 0;
    m_width = 0;
    m_height = 0;
    m_error[0] = '\0';
    
    // Check for the file type to load
    char header [ 8 ];
    stream.read ( header, 8 );
    if ( stream.gcount() != 8 )
        return SetError("Invalid file");
    
    if ( !png_sig_cmp ((png_byte*)header, 0, 8) )
        return LoadPNG ( stream );
    else
        return SetError ( "Unsupported file type" );
}

static void read_from_png ( png_structp png_ptr, png_bytep data, png_size_t length )
{
    std::istream* stream = (std::istream *)png_get_io_ptr ( png_ptr );
    stream->read ( (char *)data, length );
}

bool Pixmap::LoadPNG ( std::istream& stream )
{
    png_structp png_ptr = png_create_read_struct (PNG_LIBPNG_VER_STRING,
                                                  0, 0, 0 );
    if ( !png_ptr )
        return SetError ( "Unable to create the PNG context" );
    
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if ( !info_ptr )
    {
        png_destroy_read_struct(&png_ptr, 0, 0);
        return SetError ( "Unable to create the PNG info struct" );
    }
    
    png_infop end_info = png_create_info_struct(png_ptr);
    if ( !end_info )
    {
        png_destroy_read_struct(&png_ptr, &info_ptr, 0 );
        return SetError ( "Unable to create the PNG end info struct" );
    }
    
    if (setjmp(png_jmpbuf(png_ptr)))
    {
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        return SetError ( "An error occured while reading the PNG file" );
    }
    
    png_set_sig_bytes ( png_ptr, 8 );
    png_set_read_fn ( png_ptr, &stream, read_from_png );
    png_read_png ( png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, 0 );
    
    m_height = info_ptr->height;
    m_width = info_ptr->width;
    m_pixels = new Color [ m_width * m_height ] ();
    png_bytep* row_pointers = png_get_rows( png_ptr, info_ptr );
    
    for ( u32 h = 0; h < m_height; ++h )
    {
        png_bytep row = row_pointers[h];
        
        for ( u32 w = 0; w < m_width; ++w )
        {
            m_pixels [ w + h*m_width ] = Color ( row[0], row[1], row[2], row[3] );
            row += 4;
        }
    }
    
    return true;
}
