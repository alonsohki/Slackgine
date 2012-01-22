/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        default.cpp
// PURPOSE:     CDefault strategy.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include "default.h"
#include "slackgine.h"

using namespace Renderer::Strategy;
using Core::Entity;

Default::Default ()
{
}

Default::~Default ()
{
}

bool Default::execute (Core::Slackgine* sg)
{
    std::deque < Entity* > entities;
    
    // Begin the scene and push the world root to the deque
    sg->getRenderer()->beginScene();
    entities.push_back( &sg->getWorld() );
    
    // Repeat until no more entities
    while ( entities.size() > 0 )
    {
        // Get the current entity to render
        Entity* cur = entities.front ();
        entities.pop_front ();
        
        // Push its children to the back of the deque
        for ( Entity::EntityVector::iterator iter = cur->getChildren().begin(); iter != cur->getChildren().end(); ++iter )
            entities.push_back ( *iter );
        
        // Render it!
        cur->render(sg->getRenderer());
    }
    
    // End the scene
    sg->getRenderer()->endScene();

    return true;
}
