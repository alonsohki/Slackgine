#pragma once

#include "l3m/l3m.h"
#include "component.h"

namespace Core { namespace Entities {
   
class ModelRenderer : public IComponent
{
public:
                ModelRenderer       ( Entity* parent );
                ~ModelRenderer      ();
                
    void        Render              ( Renderer::IRenderer* renderer );
    
private:
    void        Initialize          ();
    
private:
    l3m::Model*         m_model;
};

} }