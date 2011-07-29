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
    puts ( "\t-h|--help\t\tShows this help" );
    puts ( "\t-d|--delete-duplicates\tDeletes duplicate vertices for each geometry" );
    puts ( "\t-f|--filter type\tFilters out components of that type" );
}

int main ( int argc, char* const * argv )
{
    // Parse the command-line parameters.
    bool do_delete_duplicates = false;
    std::vector < std::string > vecFilterTypes;
    const char* filepath = 0;
    
    while ( 1 )
    {
        static struct option long_options[] = {
           { "help",                0, 0, 'h' },
           { "delete-duplicates",   0, 0, 'd' },
           { "filter",              1, 0, 'f' },
           { 0,                     0, 0, 0   }
        };

        int c;
        int option_index;
        
        c = getopt_long(argc, argv, "hdf:", long_options, &option_index);
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
    if ( !model.Load(*fp) )
    {
        fprintf ( stderr, "Unable to load the model: %s\n", model.error() );
        return EXIT_FAILURE;
    }
    
    // Optimize this model.
    if ( do_delete_duplicates && delete_duplicates ( &model ) == false )
        return EXIT_FAILURE;
    if ( vecFilterTypes.size() > 0 && filter_types ( &model, vecFilterTypes ) == false )
        return EXIT_FAILURE;
    
    // Write the result to the standard output.
    if ( !model.Save(std::cout) )
    {
        fprintf ( stderr, "\nUnable to save the model: %s\n", model.error() );
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}