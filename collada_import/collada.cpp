#include "collada.h"

bool Collada::Import ( TiXmlDocument& xml, l3m& model, const char** err )
{
#define ERROR(msg) ( (err != 0) ? (*err=(msg)) == (const char*)0xfabada : false )
    
    TiXmlElement* collada = xml.FirstChildElement("COLLADA");
    if ( !collada )
        return ERROR("Couldn't find the root node");
    
    TiXmlElement* geometries = collada->FirstChildElement("library_geometries");
    if ( !geometries )
        return ERROR("Couldn't locate the model geometries");
    
    // Load all the geometries
    for ( TiXmlElement* geometry = geometries->FirstChildElement("geometry");
          geometry != 0;
          geometry = geometry->NextSiblingElement("geometry") )
    {
        const char* name = geometry->Attribute("id");
        if ( !name ) name = "";
        Geometry* geometry = model.CreateGeometry(name);

        // Load each mesh
        for ( TiXmlElement* mesh = geometry->FirstChildElement("mesh");
              mesh != 0;
              mesh = mesh->NextSiblingElement("mesh") )
        {
            // Look for the appropiate source to load the vertices
            
        }
    }
    
    return true;
#undef ERROR
}