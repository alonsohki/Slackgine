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

void l3m::ComponentFactory::initialize()
{
    ms_initialized = true;
    registerType ( "geometry", Geometry::create );
    registerType ( "scene", Scene::create );
    registerType ( "texture", Texture::create );
    registerType ( "require", Require::create );
    registerType ( "camera", Camera::create );
    registerType ( "material", Material::create );
    registerType ( "pose", Pose::create );
}

void l3m::ComponentFactory::registerType(const std::string& type, instantiatorFn fn)
{
    ms_types [ type ] = fn;
}

l3m::IComponent* l3m::ComponentFactory::create(const std::string& type)
{
    if ( !ms_initialized )
        initialize ();

    typesMap::const_iterator iter = ms_types.find ( type );
    if ( iter != ms_types.end() )
        return iter->second ();
    return 0;
}

bool l3m::ComponentFactory::isValidType(const std::string& type)
{
    if ( !ms_initialized )
        initialize ();
    
    typesMap::const_iterator iter = ms_types.find ( type );
    return ( iter != ms_types.end() );
}
