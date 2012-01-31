/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        entity.h
// PURPOSE:     Entity base class. Component management.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#include "math/matrix.h"
#include "l3m/l3m.h"
#include "entity_components/component.h"
#include "renderer/renderer.h"
#include "transformable.h"

namespace Core {

class Entity : public Transformable
{
    friend class World;
    
public:
    typedef std::vector < Entities::IComponent* > ComponentVector;
    typedef std::vector < Entity* > EntityVector;
    
public:
                                Entity          ( l3m::Model* pModel = 0, Entity* parent = 0 );
                                Entity          ( Entity* parent );
    virtual                     ~Entity         ();
    
    void                        setParent       ( Entity* parent );
    
    l3m::Model*                 getModel        () const { return m_model; }
    void                        setModel        ( l3m::Model* pModel );
    
    void                        tick            ();
    
    bool                        addComponent    ( Entities::IComponent* component );
    
    Entity*                     getParent       () { return m_parent; }
    const Entity*               getParent       () const { return m_parent; }
    EntityVector&               getChildren     () { return m_children; }
    const EntityVector&         getChildren     () const { return m_children; }
    
private:
    //--------------------------------------------------------------------------
    // Child entities linkage.
    void                        linkChild       ( Entity* entity );
    void                        unlinkChild     ( Entity* entity );

private:
    Entity*                     m_parent;
    EntityVector                m_children;
    l3m::Model*                 m_model;
    bool                        m_beingDeleted;
    
    ComponentVector             m_tickableComponents;
    ComponentVector             m_renderableComponents;
};

}
