/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        main.cpp
// PURPOSE:     Model splitter main file.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include <cstdio>
#include <cstdlib>
#include "l3m/l3m.h"
#include "l3m/Components/require.h"
#include "l3m/Components/texture.h"

int main ( int argc, const char** argv )
{
    using l3m::Model;
    using l3m::Texture;
    using l3m::Require;
    std::vector<std::string> requirements;
    
    if ( argc < 3 )
    {
        fprintf ( stderr, "Syntax: %s [model file] [output file]\n", argv[0] );
        return EXIT_FAILURE;
    }
    
    Model model;
    if ( model.Load(argv[1]) == false )
    {
        fprintf ( stderr, "Error loading the model: %s\n", model.error() );
        return EXIT_FAILURE;
    }

    Model::componentVector& components = model.components();
    for ( Model::componentVector::iterator iter = components.begin ();
          iter != components.end(); )
    {
        l3m::IComponent* comp = *iter;
        if ( comp->type () == "texture" )
        {
            Model texModel;
            char outname [ 512 ];
            
            Texture* tex = static_cast < Texture* > ( *iter );
            texModel.components ().push_back ( tex );
            snprintf ( outname, sizeof(outname), "%s.ltm", tex->id().c_str() );
            texModel.Save ( outname );
            
            iter = components.erase ( iter );
            requirements.push_back ( tex->id() );
        }
        else
            ++iter;
    }
    
    // Add the requirements
    for ( std::vector<std::string>::iterator iter = requirements.begin();
          iter != requirements.end();
          ++iter )
    {
        Require* req = model.CreateComponent<Require>("require");
        req->path() = *iter;
        req->reqType() = Require::REQUIRE_TEXTURE;
    }
    
    if ( model.Save(argv[2]) == false )
    {
        fprintf ( stderr, "Error saving the model: %s\n", model.error() );
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}

