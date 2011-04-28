#include <cstdlib>
#include <iostream>
#include <string>
#include <fstream>
#include "tinyxml/tinyxml.h"
#include "l3m/l3m.h"
#include "l3m/l3mFactory.h"
#include "strategy.h"
#include "collada_import.h"

int main(int argc, char** argv)
{
    char buffer [ 512 ];
    std::string data;
    std::ifstream fp_in;
    std::ofstream fp_out;
    std::istream* input = &std::cin;;
    std::ostream* output = &std::cout;
    
    if ( argc < 2 )
    {
        fprintf(stderr, "Usage: %s <model type> [input file] [output file]\n", argv[0]);
        return -1;
    }
    const char* type = argv[1];
    
    if ( argc >= 3 )
    {
        fp_in.open ( argv[2] );
        if ( fp_in.fail() )
        {
            fprintf(stderr, "Unable to open the input file\n" );
            return -1;
        }
        input = &fp_in;
    }
    
    if ( argc >= 4 )
    {
        fp_out.open ( argv[3] );
        if ( fp_out.fail() )
        {
            fprintf(stderr, "Unable to open the output file\n" );
            return -1;
        }
        output = &fp_out;
    }
    
    while ( input->eof() == false )
    {
        input->read ( buffer, sizeof(buffer) );
        data.append ( buffer, input->gcount() );
    }
    
    // Register all the strategies
    IStrategy* strategy = IStrategy::GetStrategy ( type );
    if ( !strategy )
    {
        fprintf(stderr, "Couldn't find a strategy for the type '%s'\n", type);
        return -1;
    }
    
    // Parse the XML document
    TiXmlDocument xml;
    xml.Parse ( data.c_str() );
    
    if ( xml.Error() )
    {
        fprintf(stderr, "Unable to parse the input document in %s: %s\n", xml.Value(), xml.ErrorDesc());
        return -1;
    }
    
    // Load the model
    l3m* model = l3mFactory::CreateOfType(type);
    if ( !model )
    {
        fprintf(stderr, "Unknown l3m type: %s\n", type);
        return -1;
    }
    
    // Load the model data
    const char* err;
    if ( !collada_import(xml, *model, &err) )
    {
        fprintf(stderr, "Unable to load the collada model: %s.\n", err );
        return -1;
    }
    
    
    if ( !strategy->ParseData(xml, *model) )
    {
        fprintf(stderr, "Unable to load the specific data for the type %s\n", type);
        return -1;
    }
    
    // Save the model
    if ( model->Save(*output) != l3m::OK )
    {
        fprintf(stderr, "Unable to save the model data: %s\n", model->error() );
        return -1;
    }
    
    // Cleanup
    delete model;
    IStrategy::CleanupStrategies();

    return 0;
}

