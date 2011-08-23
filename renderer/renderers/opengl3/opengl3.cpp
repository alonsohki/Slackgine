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
// FILE:        opengl3.cpp
// PURPOSE:     Specific functions for the OpenGL3 element factory.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include "opengl3.h"

using namespace Renderer;

IRenderer* Factory::CreateRenderer()
{
    return new OpenGL3_Renderer ();
}

IProgram* Factory::CreateProgram()
{
    return new OpenGL3_Program ();
}

IShader* Factory::CreateShader(IShader::Type type, const char* file)
{
    IShader* shader = new OpenGL3_Shader ( type );
    shader->Load ( file );
    return shader;
}

IShader* Factory::CreateShader(IShader::Type type, std::istream& fp)
{
    IShader* shader = new OpenGL3_Shader ( type );
    shader->Load ( fp );
    return shader;
}

ITexture* Factory::CreateTexture (u32 width, u32 height, ITexture::Format format)
{
    return new OpenGL3_Texture ( width, height, format );
}
