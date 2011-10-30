/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        world.h
// PURPOSE:     World: The container for all the entities in the... world.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#include "entity.h"
#include "renderer/renderer.h"

namespace Core
{

class World : public Entity
{
public:
    //--------------------------------------------------------------------------
    // Constructor / Destructor
    World           ();
    ~World          ();
};

}
