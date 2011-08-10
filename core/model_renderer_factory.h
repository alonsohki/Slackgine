#pragma once

#include <map>
#include "entity_components/model_renderer.h"
#include "l3m/l3m.h"

namespace Core
{

class ModelRendererFactory
{
public:
    struct Node
    {
        Entities::ModelRenderer*  component;
        u32                       refs;
    };
    typedef std::map < l3m::Model*, Node > nodeMap;
    
public:
    static Entities::ModelRenderer*     Create          ( l3m::Model* model );
    static void                         Release         ( Entities::ModelRenderer* component );
    
private:
    static nodeMap      ms_mapNodes;
};

}