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
// FILE:        shader.h
// PURPOSE:     Interface for the renderer shaders.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#include <iostream>
#include <fstream>

namespace Renderer
{
    
class IShader
{
public:
    enum Type
    {
        VERTEX_SHADER,
        FRAGMENT_SHADER,
        GEOMETRY_SHADER
    };

public:
    virtual             ~IShader        () {}
    
    bool                Load            ( const char* file )
    {
        std::ifstream fp ( file, std::ios::in | std::ios::binary );
        if ( fp.fail() )
            return false;
        return Load ( fp );
    }
    virtual bool        Load            ( std::istream& fp ) = 0;
    virtual bool        Ok              () const = 0;
    virtual void        GetError        ( char* dest ) const = 0;
};

}

#include "renderer.h"
