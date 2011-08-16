/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011 - Alberto Alonso
//
// Redistribution of this software, including any of its
// files, in either binary or source code form is totally
// forbidden without the permission of its owners.
//
// Owners reserve their right to legal action against any
// person or company that fails to do so.
//
// For more detailed information, see the LICENSE file in
// the top-level directory.
//
// FILE:        entity.h
// PURPOSE:     Entity base class. Component management.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

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
    
    // Accessors
public:
    Matrix&                     matrix          () { return m_matrix; }
    const Matrix&               matrix          () const { return m_matrix; }

private:
    Matrix                      m_matrix;
    l3m::Model*                 m_model;
    
    componentVector             m_tickableComponents;
    componentVector             m_renderableComponents;
    Entities::ModelRenderer*    m_modelRenderer;
};

}
