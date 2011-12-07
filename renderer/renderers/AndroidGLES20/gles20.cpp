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

IRenderer* Factory::CreateRenderer()
{
    return new GLES20_Renderer ();
}

IProgram* Factory::CreateProgram()
{
    return new GLES20_Program ();
}

IShader* Factory::CreateShader(IShader::Type type, const char* file)
{
    IShader* shader = new GLES20_Shader ( type );
    shader->Load ( file );
    return shader;
}

IShader* Factory::CreateShader(IShader::Type type, std::istream& fp)
{
    IShader* shader = new GLES20_Shader ( type );
    shader->Load ( fp );
    return shader;
}

ITexture* Factory::CreateTexture (u32 width, u32 height, ITexture::Format format)
{
    return new GLES20_Texture ( width, height, format );
}
