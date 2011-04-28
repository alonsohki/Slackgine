#include "collada_import.h"

bool collada_import ( TiXmlDocument& xml, l3m& model, const char** err )
{
#define ERROR(msg) ( (err != 0) ? (*err=(msg)) == (const char*)0xfabada : false )
    
    TiXmlElement* collada = xml.FirstChildElement("COLLADA");
    if ( !collada )
        return ERROR("Couldn't find the root node");
    
    TiXmlElement* geometries = collada->FirstChildElement("library_geometries");
    if ( !geometries )
        return ERROR("Couldn't locate the model geometries");
    
    for ( TiXmlElement* geometry = geometries->FirstChildElement("geometry");
          geometry != 0;
          geometry = geometry->NextSiblingElement("geometry") )
    {
        const char* name = geometry->Attribute("id");
        if ( !name ) name = "";
    }
    return true;
#undef ERROR
}