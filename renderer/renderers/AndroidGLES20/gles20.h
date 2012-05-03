#ifndef GLES20_H
#define	GLES20_H

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <GLES2/gl2platform.h>
#include <jni.h>

#define LOG_GL_ERRORS 0

#if LOG_GL_ERRORS
  void logGlError( GLenum errNum, const char * file, int line );
#endif

#if LOG_GL_ERRORS
# define eglGetError( ) logGlError( glGetError(), __FILE__, __LINE__ )
#else
# define eglGetError()
#endif

#include "../../texture.h"
#include "../../shader.h"
#include "../../program.h"
#include "../../renderer.h"
#include "shader.h"
#include "program.h"
#include "renderer.h"
#include "texture.h"

namespace Renderer
{

    class Factory
    {
    public:
        static IRenderer*   createRenderer  ();
        static IProgram*    createProgram   ();
        static IShader*     createShader    ( IShader::Type type, const char* file );
        static IShader*     createShader    ( IShader::Type type, std::istream& fp );

        static ITexture*    createTexture   ( u32 width, u32 height, ITexture::Format format = ITexture::RGBA8 );
    };
    
}
    
// Geometry definition
#include "geometry.h"

#endif

