/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        entity.cpp
// PURPOSE:     Entity base class. Component management.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include <set>
#include "entity.h"
#include "slackgine.h"

using namespace Core;

Entity::Entity ( l3m::Model* pModel, Entity* parent )
: Transformable ( IdentityTransform() )
, m_parent ( 0 )
, m_beingDeleted ( false )
{
    setParent ( parent );
    setModel ( pModel );
}

Entity::Entity ( Entity* parent )
: Transformable ( IdentityTransform() )
, m_parent ( 0 )
, m_beingDeleted ( false )
{
    setParent ( parent );
    setModel ( 0 );
}

Entity::~Entity()
{
    m_beingDeleted = true;
    
    // Recursively delete all the child entities
    for ( EntityVector::iterator iter = m_children.begin(); iter != m_children.end(); ++iter )
    {
        delete *iter;
    }
    
    // Unlink us from our parent
    if ( m_parent )
        m_parent->unlinkChild( this );

    // Find all the -unique- components.
    std::set<Entities::IComponent*> knownComponents;
    for ( ComponentVector::const_iterator iter = m_tickableComponents.begin();
          iter != m_tickableComponents.end();
          ++iter )
    {
        knownComponents.insert ( *iter );
    }
    for ( ComponentVector::const_iterator iter = m_renderableComponents.begin();
          iter != m_renderableComponents.end();
          ++iter )
    {
        knownComponents.insert ( *iter );
    }
    
    // Delete them
    for ( std::set<Entities::IComponent*>::const_iterator iter = knownComponents.begin();
          iter != knownComponents.end();
          ++iter )
    {
        delete *iter;
    }
}

void Entity::unlinkChild ( Entity* entity )
{
    if ( !m_beingDeleted )
    {
        for ( EntityVector::iterator iter = m_children.begin();
              iter != m_children.end();
              ++iter )
        {
            if ( *iter == entity )
            {
                m_children.erase ( iter );
                break;
            }
        }
    }
}

void Entity::linkChild ( Entity* entity )
{
    if ( entity != this )
    {
        m_children.push_back ( entity );
    }
}

void Entity::setParent ( Entity* parent )
{
    // If there was a previously set parent, unlink from it.
    if ( m_parent != 0 )
        m_parent->unlinkChild ( this );
    
    // Assign the new parent
    m_parent = parent;
    
    // Relink to the new parent
    if ( m_parent != 0 )
        m_parent->linkChild ( this );
}

void Entity::setModel ( l3m::Model* pModel )
{
    m_model = pModel;
}

void Entity::tick ()
{
    Transformable::tick ();

    for ( ComponentVector::const_iterator iter = m_tickableComponents.begin();
          iter != m_tickableComponents.end();
          ++iter )
    {
        (*iter)->tick ();
    }
    
    // Tick all the child entities
    for ( EntityVector::iterator iter = m_children.begin();
          iter != m_children.end();
          ++iter )
    {
        (*iter)->tick ();
    }
}

#if 0
void Entity::render ( Renderer::IRenderer* renderer )
{
    // Render all the child entities
    for ( EntityVector::iterator iter = m_children.begin();
          iter != m_children.end();
          ++iter )
    {
        (*iter)->render ( renderer );
    }
    
    // Render this
    renderer->pushState ();

    for ( ComponentVector::const_iterator iter = m_renderableComponents.begin();
          iter != m_renderableComponents.end();
          ++iter )
    {
        (*iter)->preRender( renderer );
        (*iter)->render ( renderer );
    }
    
    if ( m_modelRenderer != 0 )
        m_modelRenderer->render ( renderer, transform() );
    
    for ( ComponentVector::const_iterator iter = m_renderableComponents.begin();
      iter != m_renderableComponents.end();
      ++iter )
    {
        (*iter)->postRender( renderer );
    }
    
    renderer->popState ();
}
#endif

bool Entity::addComponent( Entities::IComponent* component )
{
    if ( !component->isTickable() && !component->isRenderable() )
        return false;
    
    if ( component->isTickable() )
        m_tickableComponents.push_back ( component );
    if ( component->isRenderable() )
        m_renderableComponents.push_back ( component );
    
    component->parent() = this;
    
    return true;
}
