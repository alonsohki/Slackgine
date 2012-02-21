/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        delete_unused_textures.cpp
// PURPOSE:     Strategy to filter unused textures.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include <l3m/components/components.h>
#include "delete_unused_textures.h"
#include <map>
#include <vector>

bool delete_unused_textures ( l3m::Model* model )
{
    typedef std::map < std::string, bool > CheckMap;
    CheckMap textures;
    
    // Check for the used textures
    l3m::Model::ComponentVector& components = model->components();
    for ( l3m::Model::ComponentVector::iterator iter = components.begin();
          iter != components.end();
          ++iter )
    {
        if ( (*iter)->type() == "geometry" )
        {
            l3m::Geometry* g = static_cast<l3m::Geometry*>(*iter);
            for ( Renderer::Geometry::meshList::iterator iter2 = g->geometry().meshes().begin();
                  iter2 != g->geometry().meshes().end();
                  ++iter2 )
            {
                Renderer::Mesh* m = *iter2;
                if ( m->material() != 0 )
                    textures[m->material()->texture()] = true;
            }
        }
    }
    
    // Find all the textures and delete those that didn't get marked
    for ( l3m::Model::ComponentVector::iterator iter = components.begin();
          iter != components.end();
        )
    {
        if ( (*iter)->type() == "texture" )
        {
            l3m::Texture* t = static_cast<l3m::Texture*>(*iter);
            if ( textures.find(t->id()) == textures.end() )
            {
                fprintf ( stderr, "Deleting unused texture: %s\n", t->id().c_str() );
                iter = components.erase(iter);
                continue;
            }
        }
        
        ++iter;
    }
    
    return true;
}
