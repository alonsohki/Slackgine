/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        shader.h
// PURPOSE:     Class defining a handle to the shaders loaded by the shader
//              manager.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#include <string>
#include "renderer/shader.h"

namespace Core
{
    
class Shader
{
    //--------------------------------------------------------------------------
    // This class can only be created by the shader manager.
    friend class ShaderManager;
private:
    Shader      ( const std::string& name );
    ~Shader     ();
    
    //--------------------------------------------------------------------------
    // Public accessors
public:
    const std::string&          name            () const { return m_name; }
    const Renderer::IShader*    vert            () const { return m_vert; }
    const Renderer::IShader*    frag            () const { return m_frag; }
    
    std::string&                name            () { return m_name; }
    Renderer::IShader*&         vert            () { return m_vert; }
    Renderer::IShader*&         frag            () { return m_frag; }
    
private:
    //--------------------------------------------------------------------------
    // Shader name
    // This is expected to be the name of the shader main function as well.
    std::string         m_name;
    
    //--------------------------------------------------------------------------
    // Handler for the vertex shader
    Renderer::IShader*  m_vert;
    
    //--------------------------------------------------------------------------
    // Handler for the fragment shader
    Renderer::IShader*  m_frag;
};
    
}
