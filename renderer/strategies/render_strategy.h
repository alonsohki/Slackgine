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
// FILE:        render_strategy.h
// PURPOSE:     Interface for rendering strategies.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#include "core/slackgine.h"

namespace Renderer
{

class IRenderStrategy
{
public:
    virtual             ~IRenderStrategy        () {}
    
    //--------------------------------------------------------------------------
    // Method to render the scene
    virtual bool        render                  ( Core::Slackgine* sg ) = 0;
    
    //--------------------------------------------------------------------------
    // Error management
    virtual void        getError                ( char* dest ) const = 0;
};

}
