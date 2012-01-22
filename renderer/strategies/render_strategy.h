/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        render_strategy.h
// PURPOSE:     Interface for rendering strategies.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#include <vector>
#include "core/slackgine.h"
#include "renderer/shader.h"

namespace Core { class Slackgine; }

namespace Renderer
{

class RenderStrategy
{
public:
    //--------------------------------------------------------------------------
    // Constructor / Destructor
                    RenderStrategy          ();
    virtual         ~RenderStrategy         ();
    
public:
    
    //--------------------------------------------------------------------------
    // Method to execute this strategy
    virtual bool    execute                 ( Core::Slackgine* sg );
    
    //--------------------------------------------------------------------------
    // Error management
    void            getError                ( char* dest ) const;
    
private:
    //--------------------------------------------------------------------------
    // Private class attributes
    char        m_error [ 256 ];
};

}
