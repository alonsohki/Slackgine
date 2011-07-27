#include "factory.h"
#include "components.h"

l3m::ComponentFactory::typesMap l3m::ComponentFactory::ms_types;
bool l3m::ComponentFactory::ms_initialized = false;

void l3m::ComponentFactory::Initialize()
{
    ms_initialized = true;
    Register ( "geometry", Geometry::Create );
    Register ( "scene", Scene::Create );
}
