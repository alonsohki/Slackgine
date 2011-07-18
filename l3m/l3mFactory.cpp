#include <fstream>
#include "l3mFactory.h"

l3mFactory::mapType l3mFactory::ms_instantiators;

l3mComponent* l3mFactory::CreateAndLoad(const char* path, l3mComponent::ErrorCode* errorCode )
{
    std::fstream fp;
    fp.open ( path, std::ios::in | std::ios::binary );
    if ( fp.fail() )
        return 0;
    return CreateAndLoad ( fp, errorCode );
}

l3mComponent* l3mFactory::CreateAndLoad(std::istream& fp, l3mComponent::ErrorCode* errorCode )
{
    return l3mComponent::CreateAndLoad(fp, errorCode);
}

l3mComponent* l3mFactory::CreateOfType(const std::string& type)
{
    if ( type == "default" )
        return new l3mComponent ();
    
    mapType::const_iterator iter = ms_instantiators.find(type);
    if ( iter == ms_instantiators.end() )
        return 0;
    return iter->second();
}

bool l3mFactory::RegisterType(const std::string& type, l3mComponent*(*instantiatorFn)() )
{
    mapType::const_iterator iter = ms_instantiators.find(type);
    if ( iter != ms_instantiators.end() )
        return false;
    ms_instantiators[type] = instantiatorFn;
    return true;
}