#include <set>
#include "entity.h"

using namespace Core;

Entity::Entity ( l3m::Model* pModel )
: m_modelRenderer ( 0 )
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
        delete m_modelRenderer;
        m_modelRenderer = 0;
    }
    m_model = pModel;
    
    if ( m_model != 0 )
    {
        m_modelRenderer = new Entities::ModelRenderer ( this );
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
    
    m_modelRenderer->Render ( renderer );
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
