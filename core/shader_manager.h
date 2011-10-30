/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        shader_manager.h
// PURPOSE:     Shader manager, with the ability to load shaders from files
//              and generate the shader main function to link them.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#include <string>
#include <vector>
#include "shader.h"

namespace Core
{
    
class ShaderManager
{
    //--------------------------------------------------------------------------
    // Define the types to be used by the storage
    typedef std::vector < std::string > PathVector;
    typedef std::map < std::string, Shader* > ShaderMap;
    
    //--------------------------------------------------------------------------
    // Use this only from the Slackgine context.
    friend class Slackgine;

private:
    ShaderManager       ();
    ~ShaderManager      ();
    
public:
    //--------------------------------------------------------------------------
    // addLookupPath
    // Add a path to look for shaders inside it.
    bool        addLookupPath           ( const std::string& path );
    
    //--------------------------------------------------------------------------
    // load
    // Looks up for the given shader in the search paths and loads it. You will
    // still be expected you activate it when you require to use it.
    Shader*     load                    ( const std::string& shader );
    
    
private:
    //--------------------------------------------------------------------------
    // Private utility functions.
    bool        getShaderAccessPath     ( const std::string& shader, std::string* output = 0 ) const;
    
private:
    //--------------------------------------------------------------------------
    // Private attributes
    PathVector          m_paths;
    ShaderMap           m_shaders;
};
    
}