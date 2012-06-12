/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2012
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        world.cpp
// PURPOSE:     World: The container for all the entities in the... world.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include "world.h"

using namespace Core;

World::World ()
: Entity ( this )
{
    transform() = IdentityTransform ();
}

World::~World ()
{
}
