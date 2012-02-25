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
