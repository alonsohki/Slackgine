/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        main.cpp
// PURPOSE:     Main file of the model optimizer.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include <cstdio>
#include <cstdlib>
#include <getopt.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "l3m/l3m.h"
#include "optimizations.h"

using namespace l3m;

static void print_help ( const char* path )
{
    printf ( "Syntax: %s [options] [model]\n\n", path );
    puts ( "Applies the optimizations specified in 'options' to the given 'model' file." );
    puts ( "If the 'model' parameter is not specified, it will be read from the standard input." );
    puts ( "" );
    puts ( "Available options are:" );
    puts ( "\t-h|--help\t\t\tShows this help" );
    puts ( "\t-d|--delete-duplicates\t\tDeletes duplicate vertices for each geometry" );
    puts ( "\t-f|--filter type\t\tFilters out components of that type" );
    puts ( "\t-T|--clean-texture-ids\t\tFilters the texture ids to make them a clean name" );
    puts ( "\t-U|--delete-unused\t\tFilters out the unused materials" );
    puts ( "\t-t|--delete-unused-textures\tFilters out textures not being used by this model materials" );
    puts ( "\t-z|--compression-level\t\tSets the compression level of the output model (0-9)" );
}

int main ( int argc, char* const * argv )
{
    // Parse the command-line parameters.
    bool do_delete_duplicates = false;
    std::vector < std::string > vecFilterTypes;
    const char* filepath = 0;
    bool do_clean_texture_ids = false;
    bool do_delete_unused = false;
    bool do_delete_unused_textures = false;
    u32 compressionLevel = 0;
    bool compressionLevelSet = false;
    
    while ( 1 )
    {
        static struct option long_options[] = {
           { "help",                    0, 0, 'h' },
           { "delete-duplicates",       0, 0, 'd' },
           { "filter",                  1, 0, 'f' },
           { "clean-texture-ids",       0, 0, 'T' },
           { "delete-unused",           0, 0, 'U' },
           { "delete-unused-textures",  0, 0, 't' },
           { "compression-level",       0, 0, 'z' },
           { 0,                         0, 0, 0   }
        };

        int c;
        int option_index;
        
        c = getopt_long(argc, argv, "hdf:TUtz:", long_options, &option_index);
        if ( c == -1 )
            break;
        
        switch ( c )
        {
            case 'h':
                print_help(argv[0]);
                return EXIT_SUCCESS;
                break;

            case 'd':
                do_delete_duplicates = true;
                break;
                
            case 'f':
                vecFilterTypes.push_back (optarg);
                break;
                
            case '?':
                return EXIT_FAILURE;
                break;
                
            case 'T':
                do_clean_texture_ids = true;
                break;
                
            case 'U':
                do_delete_unused = true;
                break;

            case 't':
                do_delete_unused_textures = true;
                break;
                
            case 'z':
                compressionLevel = atoi(optarg);
                compressionLevelSet = true;
                break;

            default:
                break;
        }
    }
    
    // Model file
    if ( optind < argc )
        filepath = argv[optind++];
    
    // Adjust the input stream
    std::istream* fp = &std::cin;
    std::ifstream file;
    
    if ( filepath != 0 )
    {
        file.open ( filepath, std::ios::binary | std::ios::in );
        if ( file.fail() )
        {
            fprintf ( stderr, "Unable to open the file\n" );
            return EXIT_FAILURE;
        }
        fp = &file;
    }

    // Load the model.
    Model model;
    if ( !model.load(*fp) )
    {
        fprintf ( stderr, "Unable to load the model: %s\n", model.error() );
        return EXIT_FAILURE;
    }
    
    // Optimize this model.
    if ( do_delete_duplicates && delete_duplicates ( &model ) == false )
        return EXIT_FAILURE;
    if ( vecFilterTypes.size() > 0 && filter_types ( &model, vecFilterTypes ) == false )
        return EXIT_FAILURE;
    if ( do_clean_texture_ids && clean_texture_ids( &model ) == false )
        return EXIT_FAILURE;
    if ( do_delete_unused && delete_unused ( &model ) == false )
        return EXIT_FAILURE;
    if ( do_delete_unused_textures && delete_unused_textures ( &model ) == false )
        return EXIT_FAILURE;
    
    if ( compressionLevelSet )
        model.setCompressionLevel( compressionLevel );
    
    // Write the result to the standard output.
    if ( !model.save(std::cout) )
    {
        fprintf ( stderr, "\nUnable to save the model: %s\n", model.error() );
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}