#include "entity.h"

Entity::Entity ( const l3m::Model* pModel )
: m_model ( pModel )
{}

Entity::~Entity()
{}

void Entity::Update ( Slackgine* ctx )
{
    if ( m_model != 0 )
        ctx->renderer()->Render( m_model );
}
