#ifndef GLES20_H
#define	GLES20_H

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <GLES2/gl2platform.h>
#include <jni.h>
#include <android/log.h>

#if defined(DEBUG)
// This code Copyright (c) Max Rupp
#define eglError( Error, file, line )\
{\
LOGE ( "(%s:%d) OpenGL Error: %s\n", file, line, Error );\
}

#define eglGetError( )\
{\
for ( GLenum Error = glGetError( ); ( GL_NO_ERROR != Error ); Error = glGetError( ) )\
{\
switch ( Error )\
{\
case GL_INVALID_ENUM:      eglError( "GL_INVALID_ENUM"      , __FILE__, __LINE__ ); break;\
case GL_INVALID_VALUE:     eglError( "GL_INVALID_VALUE"     , __FILE__, __LINE__ ); break;\
case GL_INVALID_OPERATION: eglError( "GL_INVALID_OPERATION" , __FILE__, __LINE__ ); break;\
case GL_OUT_OF_MEMORY:     eglError( "GL_OUT_OF_MEMORY"     , __FILE__, __LINE__ ); break;\
default:                                                       break;\
}\
break; \
}\
}
// End of (c) Max Rupp code.
#else
#define eglGetError()
#endif

#define  LOG_TAG    "libSlackgine-jni-bindings"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

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
        static IRenderer*   CreateRenderer  ();
        static IProgram*    CreateProgram   ();
        static IShader*     CreateShader    ( IShader::Type type, const char* file );
        static IShader*     CreateShader    ( IShader::Type type, std::istream& fp );

        static ITexture*    CreateTexture   ( u32 width, u32 height, ITexture::Format format = ITexture::RGBA8 );
    };

}

// Geometry definition
#include "geometry.h"

#endif

