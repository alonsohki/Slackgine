/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2012
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        delete_unused.cpp
// PURPOSE:     Strategy to filter unused components.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include <l3m/components/components.h>
#include "delete_unused.h"
#include <map>
#include <vector>

bool delete_unused ( l3m::Model* model )
{
    typedef std::map < std::string, bool > CheckMap;
    CheckMap materials;
    CheckMap armatures;
    
    // Check for the used materials
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
                    materials[m->material()->name()] = true;
            }
            if ( g->geometry().pose() != 0 )
                armatures[g->geometry().pose()->name()] = true;
        }
    }
    
    for ( l3m::Model::ComponentVector::iterator iter = components.begin();
          iter != components.end();
        )
    {
        if ( (*iter)->type() == "material" )
        {
            l3m::Material* m = static_cast<l3m::Material*>(*iter);
            if ( materials.find(m->material().name()) == materials.end() )
            {
                fprintf ( stderr, "Deleting unused material: %s\n", m->material().name().c_str() );
                iter = components.erase(iter);
                continue;
            }
        }
        
        ++iter;
    }    
    
    for ( l3m::Model::ComponentVector::iterator iter = components.begin();
          iter != components.end();
        )
    {
        if ( (*iter)->type() == "pose" )
        {
            l3m::Pose* p = static_cast<l3m::Pose*>(*iter);
            if ( armatures.find(p->pose().name()) == armatures.end() )
            {
                fprintf ( stderr, "Deleting unused armature: %s\n", p->pose().name().c_str() );
                iter = components.erase(iter);
                continue;
            }
        }
        
        ++iter;
    }
    
    return true;
}