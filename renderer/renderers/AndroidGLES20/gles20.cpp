/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        gles20.cpp
// PURPOSE:     Specific functions for the OpenGL ES 2.0 element factory.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include "gles20.h"
#include "shared/log.h"

using namespace Renderer;

IRenderer* Factory::createRenderer()
{
    return sgNew GLES20_Renderer ();
}

IProgram* Factory::createProgram()
{
    return sgNew GLES20_Program ();
}

IShader* Factory::createShader(IShader::Type type, const char* file)
{
    IShader* shader = sgNew GLES20_Shader ( type );
    shader->load ( file );
    return shader;
}

IShader* Factory::createShader(IShader::Type type, std::istream& fp)
{
    IShader* shader = sgNew GLES20_Shader ( type );
    shader->load ( fp );
    return shader;
}

ITexture* Factory::createTexture (u32 width, u32 height, ITexture::Format format)
{
    return sgNew GLES20_Texture ( width, height, format );
}

#if LOG_GL_ERRORS
void logGlError( GLenum errNum, const char * file, int line )
{
  if ( errNum == GL_NO_ERROR )
    return;
  
  static const char * errMsgTable[] = {
    "Invalid Enum",                 // 0x500  GL_INVALID_ENUM
    "Invalid Value",                // 0x501  GL_INVALID_VALUE
    "Invalid Operation",            // 0x502  GL_INVALID_OPERATION
    "Stack Overflow",               // 0x503  GL_STACK_OVERFLOW
    "Stack Underflow",              // 0x504  GL_STACK_UNDERFLOW
    "Out of Memory",                // 0x505  GL_OUT_OF_MEMORY
    "Invalid Framebuffer Operation" // 0x506  GL_INVALID_FRAMEBUFFER_OPERATION
  };  
  static const int errMsgTableSize = (int) (sizeof(errMsgTable) / sizeof(const char *));

  const char * errMsg = "Unknown Error";  
  const int errMsgTableIndex = (int) (errNum - GL_INVALID_ENUM);
  
  if ((errMsgTableIndex >= 0) && (errMsgTableIndex < errMsgTableSize))
    errMsg = errMsgTable[errMsgTableIndex];
  
  LOG_E ( "OpenGL", "%s at %s:%d\n", errMsg, file, line );
}
#endif
