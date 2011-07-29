#pragma once

#include <vector>
#include "l3m/l3m.h"
#include "l3m/Components/scene.h"
#include "component.h"

namespace Core { namespace Entities {
   
class ModelRenderer : public IComponent
{
public:
    struct Node
    {
        Renderer::Geometry*     geometry;
        Matrix                  transform;
    };
    typedef std::vector < Node > nodesVector;
    
public:
                ModelRenderer       ( Entity* parent );
                ~ModelRenderer      ();
                
    void        Render              ( Renderer::IRenderer* renderer );
    
private:
    void        Initialize          ();
    
private:
    l3m::Model*         m_model;
    l3m::Scene*         m_scene;
    nodesVector         m_vecNodes;
};

} }