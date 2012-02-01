/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        main.cpp
// PURPOSE:     Main file of the model importer. Setup IO and write results.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//


#include <cstdio>
#include <cstdlib>
#include "l3m/l3m.h"
#include "post_process.h"

extern bool import_blender ( int, const char**, const char*, l3m::Model* );
//extern bool import_blender ( int, const char**, std::istream&, l3m::Model* );

int main(int argc, const char **argv)
{
    l3m::Model model;
    if ( argc == 1 )
    {
        fprintf ( stderr, "Unsupported behavior. Use %s [model file]\n", argv[0] );
        return EXIT_FAILURE;
    }
    else
    {
        if ( ! import_blender ( argc, argv, argv[1], &model ) )
        {
            fprintf ( stderr, "Unable to import the blender model\n" );
            return EXIT_FAILURE;
        }
    }
    
    // Post-process the model after importing to match the requirements.
    const char* error;
    if ( !PostProcess ( &model, &error ) )
    {
        fprintf ( stderr, "Error while post-processing the model: %s\n", error );
        return EXIT_FAILURE;
    }
    
    if ( !model.save(std::cout) )
    {
        fprintf ( stderr, "Unable to save the model: %s\n", model.error() );
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}

