/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        default.h
// PURPOSE:     Default strategy.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#include "render_strategy.h"

namespace Renderer { namespace Strategy
{
    
class Default : public RenderStrategy
{
public:
    //--------------------------------------------------------------------------
    // Constructor/Destructor
                Default         ();
                ~Default        ();
    
    //--------------------------------------------------------------------------
    // Execute
    bool        execute         ( Core::Slackgine* sg );
};
    
} }
