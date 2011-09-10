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
// FILE:        opengl3.h
// PURPOSE:     Specific functions for the OpenGL3 element factory.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glut.h>

#if defined(DEBUG)
// This code Copyright (c) Max Rupp
#define eglError( Error, File, Line )\
{\
fprintf(stderr, "(%s:%u) OpenGL Error: %s\n", File, Line, Error );\
\
exit( -1 );\
}

#define eglGetError( )\
switch ( glGetError() )\
{\
case GL_INVALID_ENUM:      eglError( "GL_INVALID_ENUM"      , __FILE__, __LINE__ ); break;\
case GL_INVALID_VALUE:     eglError( "GL_INVALID_VALUE"     , __FILE__, __LINE__ ); break;\
case GL_INVALID_OPERATION: eglError( "GL_INVALID_OPERATION" , __FILE__, __LINE__ ); break;\
case GL_STACK_OVERFLOW:    eglError( "GL_STACK_OVERFLOW"    , __FILE__, __LINE__ ); break;\
case GL_STACK_UNDERFLOW:   eglError( "GL_STACK_UNDERFLOW"   , __FILE__, __LINE__ ); break;\
case GL_OUT_OF_MEMORY:     eglError( "GL_OUT_OF_MEMORY"     , __FILE__, __LINE__ ); break;\
case GL_NO_ERROR:                                              break; \
default:                                                       break;\
}

// End of (c) Max Rupp code.
#else
#define eglGetError()
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
    static IRenderer*   CreateRenderer  ();
    static IProgram*    CreateProgram   ();
    static IShader*     CreateShader    ( IShader::Type type, const char* file );
    static IShader*     CreateShader    ( IShader::Type type, std::istream& fp );
    
    static ITexture*    CreateTexture   ( u32 width, u32 height, ITexture::Format format = ITexture::RGBA8 );
};

}

// Geometry definition
#include "geometry.h"
