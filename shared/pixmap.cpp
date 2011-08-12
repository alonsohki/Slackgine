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
{
    Create ( width, height );
}

Pixmap::Pixmap ( const Pixmap& other )
{
    operator= ( other );
}

Pixmap::~Pixmap ()
{
    CleanupData ();
}

void Pixmap::CleanupData ()
{
    if ( m_pixels != 0 )
        delete [] m_pixels;
    m_pixels = 0;
    m_width = 0;
    m_height = 0;
    m_error[0] = '\0';
}

void Pixmap::Create ( u32 width, u32 height, const Color* data )
{
    CleanupData ();
    m_width = width;
    m_height = height;
    m_pixels = new Color [ m_width * m_height ] ();
    if ( data != 0 )
        memcpy ( m_pixels, data, sizeof(Color)*m_width*m_height );
}

Pixmap& Pixmap::operator= ( const Pixmap& other )
{
    CleanupData ();
    m_width = other.m_width;
    m_height = other.m_height;
    memcpy ( m_error, other.m_error, sizeof(m_error) );
    m_pixels = new Color [ m_width * m_height ] ();
    memcpy ( m_pixels, other.m_pixels, sizeof(Color)*m_width*m_height );
    return *this;
}

bool Pixmap::operator== ( const Pixmap& other ) const
{
    return m_width == other.m_width &&
           m_height == other.m_height &&
           memcmp ( m_pixels, other.m_pixels, sizeof(Color)*m_width*m_height ) == 0;
}

bool Pixmap::operator!= ( const Pixmap& other ) const
{
    return !operator== ( other );
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
    CleanupData ();
    
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

static void write_to_png ( png_structp png_ptr, png_bytep data, png_size_t length )
{
    std::ostream* stream = (std::ostream *)png_get_io_ptr ( png_ptr );
    stream->write ( (char*)data, length );
}

static void flush_png ( png_structp png_ptr )
{
    std::ostream* stream = (std::ostream *)png_get_io_ptr ( png_ptr );
    stream->flush();
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

bool Pixmap::SavePNG ( const char* filename )
{
    std::ofstream fp;
    fp.open ( filename, std::ios::out | std::ios::binary );
    if ( !fp.is_open() )
        return SetError ( "Unable to open the file for writing" );
    return SavePNG ( fp );
}

bool Pixmap::SavePNG ( std::ostream& stream )
{
    png_byte** row_pointers = new png_byte*[m_height];
    png_structp png_ptr = png_create_write_struct (PNG_LIBPNG_VER_STRING, 0, 0, 0);
    if ( !png_ptr )
       return SetError ( "Error creating the PNG write struct" );

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
    {
       png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
       return SetError ( "Error creating the PNG write info struct" );
    }
    
    if (setjmp(png_jmpbuf(png_ptr)))
    {
       delete [] row_pointers;
       png_destroy_write_struct(&png_ptr, &info_ptr);
       return SetError ( "An error occured when writing the PNG data" );
    }
    
    png_set_IHDR(png_ptr,
                 info_ptr,
                 m_width,
                 m_height,
                 8,
                 PNG_COLOR_TYPE_RGB_ALPHA,
                 PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_DEFAULT,
                 PNG_FILTER_TYPE_DEFAULT);
    
    png_set_write_fn ( png_ptr, &stream, write_to_png, flush_png );
    
    for ( u32 i = 0; i < m_height; ++i )
        row_pointers[i] = (png_byte *)&m_pixels[i*m_width];
    png_set_rows(png_ptr, info_ptr, row_pointers);
    png_write_png(png_ptr, info_ptr, detectBigEndian() ? PNG_TRANSFORM_IDENTITY : PNG_TRANSFORM_BGR|PNG_TRANSFORM_SWAP_ALPHA, 0);     
    png_destroy_write_struct(&png_ptr, &info_ptr);

    delete [] row_pointers;
    
    return true;
}