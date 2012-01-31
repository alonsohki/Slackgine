#include "gles20.h"

using namespace Renderer;

GLES20_Shader::GLES20_Shader ( IShader::Type type )
: m_loaded ( false )
{
    GLenum glType = GL_INVALID_ENUM;

    switch ( type )
    {
        case VERTEX_SHADER:
            glType = GL_VERTEX_SHADER;
            break;
        case FRAGMENT_SHADER:
            glType = GL_FRAGMENT_SHADER;
            break;
        case GEOMETRY_SHADER:
        default:
            break;
    }

    if ( glType != GL_INVALID_ENUM )
    {
        m_handler = glCreateShader ( glType );
        eglGetError();
        if ( m_handler > 0 )
            strcpy ( m_error, "Success" );
        else
            strcpy ( m_error, "Invalid handler" );
    }
    else
    {
        m_handler = 0;
        strcpy ( m_error, "Invalid or unsupported shader type" );
    }
}

GLES20_Shader::~GLES20_Shader()
{
    if ( m_handler > 0 )
    {
        glDeleteShader ( m_handler );
        eglGetError();
    }
}

bool GLES20_Shader::Load ( std::istream& fp )
{
    static const char* prefix = "precision mediump float;\n";
    if ( m_handler == 0 )
        return false;

    char buffer [ 512 ];
    std::string out;

    while ( fp.eof() == false )
    {
        fp.read( buffer, sizeof(buffer) );
        out.append ( buffer, fp.gcount() );
    }

    int prefixLen = strlen(prefix);
    char* source = new char [ out.length() + prefixLen + 1 ];
    strncpy ( source, prefix, prefixLen );
    memcpy ( &source[prefixLen], out.c_str(), out.length() );
    glShaderSource ( m_handler, 1, (const GLchar **)&source, 0 );
    delete [] source;
    eglGetError();
    glCompileShader ( m_handler );
    eglGetError();

    GLint status;
    glGetShaderiv ( m_handler, GL_COMPILE_STATUS, &status );
    eglGetError();
    m_loaded = !!status;

    if ( !m_loaded )
    {
        GLint length;
        glGetShaderiv ( m_handler, GL_INFO_LOG_LENGTH, &length );
        glGetShaderInfoLog ( m_handler, length, &length, m_error );
    }

    return m_loaded;
}

