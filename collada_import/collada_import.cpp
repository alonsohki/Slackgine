#include "collada_import.h"

bool collada_import ( TiXmlDocument& xml, l3m& model, const char** err )
{
#define ERROR(msg) ( (err != 0) ? (*err=(msg)) == (const char*)0xfabada : false )
    
    TiXmlNode* collada = xml.FirstChild("COLLADA");
    if ( !collada )
        return ERROR("Couldn't find the root node");
    
    TiXmlNode* geometries = collada->FirstChild("library_geometries");
    if ( !geometries )
        return ERROR("Couldn't locate the model geometries");
    
    return true;
#undef ERROR
}