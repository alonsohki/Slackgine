#include "collada.h"
#include "math/util.h"
#include "strategy.h"

using namespace l3m;

bool Collada::Import ( TiXmlDocument& xml, l3m::Model& model, const char** err )
{
#define ERROR(msg) ( (err != 0) ? (*err=(msg)) == (const char*)0xfabada : false )
    
    TiXmlElement* collada = xml.FirstChildElement("COLLADA");
    if ( !collada )
        return ERROR("Couldn't find the root node");
    
    for ( TiXmlElement* elements = collada->FirstChildElement(); elements != 0; elements = elements->NextSiblingElement() )
    {
        IStrategy* strategy = IStrategy::GetStrategy(elements->Value());
        if ( strategy != 0 )
        {
            if ( !strategy->ParseData(*elements, model, err ) )
                return ERROR(*err);
        }
    }
    
    return true;
    
#if 0
    
    // Process the visual scene
    TiXmlElement* visualScene = collada->FirstChildElement("library_visual_scenes");
    if ( !visualScene )
        return ERROR("Unable to locate the visual scenes");
    visualScene = visualScene->FirstChildElement("visual_scene");
    if ( !visualScene )
        return ERROR("Unable to locate the visual scene");
    
    for ( TiXmlElement* node = visualScene->FirstChildElement("node");
          node != 0;
          node = node->NextSiblingElement("node") )
    {
        TiXmlElement* instance = node->FirstChildElement("instance_geometry");
        if ( instance != 0 )
        {
            Geometry* g = model.FindGeometry( instance->Attribute("url") + 1 );
            if ( g != 0 )
            {
                float values[4];
                const char* p;
                char* p2;
                Matrix matTransform = IdentityMatrix ();
                
#define FETCH_VALUES(x,n) p = (x)->FirstChild()->Value(); \
                values[0] = strtof(p, &p2); \
                for ( u32 i = 1; i < n; ++i ) \
                    values[i] = strtof(p2, &p2)
                
                for ( TiXmlElement* transform = node->FirstChildElement();
                      transform != 0;
                      transform = transform->NextSiblingElement() )
                {
                    if ( !strcmp(transform->Value(), "translate") )
                    {
                        FETCH_VALUES(transform, 3);
                        matTransform *= TranslationMatrix(values[0], values[1], values[2]);
                    }
                    else if ( !strcmp(transform->Value(), "rotate") )
                    {
                        FETCH_VALUES(transform, 4);
                        matTransform *= RotationMatrix(deg2rad(values[3]), values[0], values[1], values[2]);
                    }
                    else if ( !strcmp(transform->Value(), "scale") )
                    {
                        FETCH_VALUES(transform, 3);
                        matTransform *= ScalingMatrix(values[0], values[1], values[2]);
                    }
                }
                
                g->matrix() = matTransform;
#undef FETCH_VALUES
            }
        }
    } 
   
#endif
}
