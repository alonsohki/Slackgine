#include "entity.h"

Entity::Entity ( const l3m::Model* pModel )
{
    SetModel ( pModel );
}

Entity::~Entity()
{}

void Entity::SetModel ( const l3m::Model* pModel )
{
    m_model = pModel;
}