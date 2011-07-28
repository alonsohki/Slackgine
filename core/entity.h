#pragma once

#include "math/matrix.h"
#include "l3m/l3m.h"
#include "entity_components/component.h"
#include "entity_components/model_renderer.h"
#include "renderer/renderer.h"

namespace Core {

class Entity
{
public:
    typedef std::vector < Entities::IComponent* > componentVector;
    
public:
                                Entity          ( l3m::Model* pModel = 0 );
    virtual                     ~Entity         ();
    
    l3m::Model*                 GetModel        () const { return m_model; }
    void                        SetModel        ( l3m::Model* pModel );
    
    void                        Tick            ();
    void                        Render          ( Renderer::IRenderer* renderer );
    
    bool                        AddComponent    ( Entities::IComponent* component );

private:
    Matrix                      m_matrix;
    l3m::Model*                 m_model;
    
    componentVector             m_tickableComponents;
    componentVector             m_renderableComponents;
    Entities::ModelRenderer*    m_modelRenderer;
};

}
