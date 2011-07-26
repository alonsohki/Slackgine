#include <cstdio>
#include <cstdlib>
#include "l3m/l3m.h"

extern bool import_blender ( int, const char**, const char*, l3m::Model*, l3m::Model* );

int main(int argc, const char **argv)
{
    if ( argc < 3 )
    {
        fprintf ( stderr, "Usage: %s [input file] [model name]\n", argv[0] );
        return EXIT_FAILURE;
    }
    
    l3m::Model model;
    l3m::Model txd;
    if ( ! import_blender ( argc, argv, argv[1], &model, &txd ) )
    {
        fprintf ( stderr, "Unable to import the blender model\n" );
        return EXIT_FAILURE;
    }
    
    char filename [ 64 ];
    snprintf ( filename, sizeof(filename), "%s.l3m", argv[2] );
    if ( !model.Save(filename) )
    {
        fprintf ( stderr, "Unable to save the model: %s\n", model.error() );
        return EXIT_FAILURE;
    }
    
    if ( txd.numComponents() >  0 )
    {
        snprintf ( filename, sizeof(filename), "%s.txd", argv[2] );
        if ( !txd.Save(filename) )
        {
            fprintf ( stderr, "Unable to save the model textures: %s\n", txd.error() );
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}

