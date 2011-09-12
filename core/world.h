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
