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
// FILE:        clean_texture_ids.cpp
// PURPOSE:     Strategy to clean the texture ids.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include <cstdlib>
#include <cstring>
#include "clean_texture_ids.h"
#include "l3m/Components/texture.h"
#include "l3m/Components/scene.h"

bool clean_texture_ids ( l3m::Model* model )
{
    using l3m::Model;
    using l3m::Texture;
    using l3m::IComponent;
    using l3m::Scene;
    
    Model::componentVector& components = model->components();
    for ( Model::componentVector::iterator iter = components.begin();
          iter != components.end();
          ++iter )
    {
        IComponent* comp = *iter;
        if ( comp->type() == "texture" )
        {
            Texture* tex = static_cast < Texture* > ( comp );
            char* id_copy = strdup ( tex->id().c_str() );
            char* old_id = strdup ( id_copy );
            
            char* p = strrchr ( id_copy, '_' );
            if ( p )
            {
                *p = '\0';
                while ( ( p = strchr ( id_copy, '-' ) ) != 0 )
                    strcpy ( p, p+1 );
                tex->id() = id_copy;
                
                // Update the scene uris
                for ( Model::componentVector::iterator iter2 = components.begin();
                      iter2 != components.end();
                      ++iter2 )
                {
                    IComponent* comp2 = *iter2;
                    if ( comp2->type() == "scene" )
                    {
                        Scene* sce = static_cast < Scene* > ( comp2 );
                        // Loop all the geometry nodes
                        for ( Scene::nodesVector::iterator iter3 = sce->geometryNodes ().begin ();
                              iter3 != sce->geometryNodes().end();
                              ++iter3 )
                        {
                            Scene::Node& node = *iter3;
                            // Loop all the textures
                            for ( std::vector<std::string>::iterator iter4 = node.textures.begin();
                                  iter4 != node.textures.end();
                                  ++iter4 )
                            {
                                std::string& url = *iter4;
                                if ( url == old_id )
                                    url = id_copy;
                            }
                        }
                    }
                }
            }
            free ( id_copy );
            free ( old_id );
        }
    }
    
    return true;
}
