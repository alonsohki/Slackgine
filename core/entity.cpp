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
// FILE:        entity.cpp
// PURPOSE:     Entity base class. Component management.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include <set>
#include "entity.h"
#include "model_renderer_factory.h"

using namespace Core;

Entity::Entity ( l3m::Model* pModel )
: m_modelRenderer ( 0 )
, m_matrix ( IdentityMatrix() )
{
    SetModel ( pModel );
}

Entity::~Entity()
{
    // Find all the -unique- components.
    std::set<Entities::IComponent*> knownComponents;
    for ( componentVector::const_iterator iter = m_tickableComponents.begin();
          iter != m_tickableComponents.end();
          ++iter )
    {
        knownComponents.insert ( *iter );
    }
    for ( componentVector::const_iterator iter = m_renderableComponents.begin();
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
    
    // Delete the model renderer component
    delete m_modelRenderer;
}

void Entity::SetModel ( l3m::Model* pModel )
{
    if ( m_modelRenderer != 0 )
    {
        ModelRendererFactory::Release ( m_modelRenderer );
        m_modelRenderer = 0;
    }
    m_model = pModel;
    
    if ( m_model != 0 )
    {
        m_modelRenderer = ModelRendererFactory::Create ( m_model );
    }
}

void Entity::Tick ()
{
    for ( componentVector::const_iterator iter = m_tickableComponents.begin();
          iter != m_tickableComponents.end();
          ++iter )
    {
        (*iter)->Tick ();
    }
}

void Entity::Render ( Renderer::IRenderer* renderer )
{
    renderer->PushState ();

    for ( componentVector::const_iterator iter = m_renderableComponents.begin();
          iter != m_renderableComponents.end();
          ++iter )
    {
        (*iter)->Render ( renderer );
    }
    
    m_modelRenderer->Render ( renderer, matrix() );
    renderer->PopState ();
}

bool Entity::AddComponent( Entities::IComponent* component )
{
    if ( !component->isTickable() && !component->isRenderable() )
        return false;
    
    if ( component->isTickable() )
        m_tickableComponents.push_back ( component );
    if ( component->isRenderable() )
        m_renderableComponents.push_back ( component );
    
    return true;
}
