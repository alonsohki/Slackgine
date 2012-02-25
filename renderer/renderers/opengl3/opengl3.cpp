/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        opengl3.cpp
// PURPOSE:     Specific functions for the OpenGL3 element factory.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include "opengl3.h"

using namespace Renderer;

IRenderer* Factory::createRenderer()
{
    return sgNew OpenGL3_Renderer ();
}

IProgram* Factory::createProgram()
{
    return sgNew OpenGL3_Program ();
}

IShader* Factory::createShader(IShader::Type type, const char* file)
{
    IShader* shader = sgNew OpenGL3_Shader ( type );
    shader->load ( file );
    return shader;
}

IShader* Factory::createShader(IShader::Type type, std::istream& fp)
{
    IShader* shader = sgNew OpenGL3_Shader ( type );
    shader->load ( fp );
    return shader;
}

ITexture* Factory::createTexture (u32 width, u32 height, ITexture::Format format)
{
    return sgNew OpenGL3_Texture ( width, height, format );
}
