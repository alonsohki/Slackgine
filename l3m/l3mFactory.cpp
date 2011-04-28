#include <fstream>
#include "l3mFactory.h"

l3mFactory::mapType l3mFactory::ms_instantiators;

l3m* l3mFactory::CreateAndLoad(const char* path, l3m::ErrorCode* errorCode )
{
    std::fstream fp;
    fp.open ( path, std::ios::in | std::ios::binary );
    if ( fp.fail() )
        return 0;
    return CreateAndLoad ( fp, errorCode );
}

l3m* l3mFactory::CreateAndLoad(std::istream& fp, l3m::ErrorCode* errorCode )
{
    return l3m::CreateAndLoad(fp, errorCode);
}

l3m* l3mFactory::CreateOfType(const std::string& type)
{
    if ( type == "default" )
        return new l3m ();
    
    mapType::const_iterator iter = ms_instantiators.find(type);
    if ( iter == ms_instantiators.end() )
        return 0;
    return iter->second();
}

bool l3mFactory::RegisterType(const std::string& type, l3m*(*instantiatorFn)() )
{
    mapType::const_iterator iter = ms_instantiators.find(type);
    if ( iter != ms_instantiators.end() )
        return false;
    ms_instantiators[type] = instantiatorFn;
    return true;
}