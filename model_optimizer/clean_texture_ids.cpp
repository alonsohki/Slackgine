/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        clean_texture_ids.cpp
// PURPOSE:     Strategy to clean the texture ids.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include <cstdlib>
#include <cstring>
#include "clean_texture_ids.h"
#include "l3m/components/texture.h"
#include "l3m/components/scene.h"
#include "l3m/components/material.h"

static std::string cleanTextureID ( const std::string texID )
{
    char* id_copy = strdup ( texID.c_str() );
    std::string ret;
    
    char* p = strrchr ( id_copy, '_' );
    if ( p != 0 )
    {
        *p = '\0';
        while ( ( p = strchr ( id_copy, '-' ) ) != 0 )
            strcpy ( p, p+1 );
        ret = id_copy;
    }
    else
        ret = texID;
    
    free ( id_copy );
    return ret;
}

bool clean_texture_ids ( l3m::Model* model )
{
    using l3m::Model;
    using l3m::Texture;
    using l3m::IComponent;
    using l3m::Scene;
    using l3m::Material;
    
    Model::ComponentVector& components = model->components();
    for ( Model::ComponentVector::iterator iter = components.begin();
          iter != components.end();
          ++iter )
    {
        IComponent* comp = *iter;
        if ( comp->type() == "texture" )
        {
            Texture* tex = static_cast < Texture* > ( comp );
            std::string new_id = cleanTextureID ( tex->id() );
            std::string old_id = tex->id();
            
            if ( old_id != new_id )
            {
                tex->id() = new_id;
                
                // Update the scene uris
                for ( Model::ComponentVector::iterator iter2 = components.begin();
                      iter2 != components.end();
                      ++iter2 )
                {
                    IComponent* comp2 = *iter2;
                    if ( comp2->type() == "scene" )
                    {
                        Scene* sce = static_cast < Scene* > ( comp2 );
                        // Loop all the geometry nodes
                        for ( Scene::NodesVector::iterator iter3 = sce->geometryNodes ().begin ();
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
                                    url = new_id;
                            }
                        }
                    }
                }
            }
        }
        
        else if ( comp->type() == "material" )
        {
            Material* mat = static_cast < Material* > ( comp );
            if ( mat->material().texture() != "" )
            {
                mat->material().texture() = cleanTextureID(mat->material().texture());
            }
        }
    }
    
    return true;
}
