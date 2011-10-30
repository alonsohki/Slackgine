/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        celshading.h
// PURPOSE:     Cel-shading strategy.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#include "render_strategy.h"

namespace Renderer { namespace Strategy
{
    
class Celshading : public RenderStrategy
{
public:
    //--------------------------------------------------------------------------
    // Constructor/Destructor
    Celshading  ();
    ~Celshading ();
};
    
}
}