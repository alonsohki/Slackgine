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
}
