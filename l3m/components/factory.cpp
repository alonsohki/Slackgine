/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        factory.cpp
// PURPOSE:     Factory to create components based on their type.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include "factory.h"
#include "components.h"

l3m::ComponentFactory::typesMap l3m::ComponentFactory::ms_types;
bool l3m::ComponentFactory::ms_initialized = false;

void l3m::ComponentFactory::Initialize()
{
    ms_initialized = true;
    Register ( "geometry", Geometry::Create );
    Register ( "scene", Scene::Create );
    Register ( "texture", Texture::Create );
    Register ( "require", Require::Create );
    Register ( "camera", Camera::Create );
    Register ( "material", Material::Create );
}
