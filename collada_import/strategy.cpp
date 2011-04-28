#include "strategy.h"
#include "strategies/default.h"

IStrategy::mapType IStrategy::ms_strategies;

void IStrategy::InitializeStrategies()
{
    if ( ms_strategies.size() > 0 )
        return;
    
    ms_strategies["default"] = new DefaultStrategy ();
}

void IStrategy::CleanupStrategies()
{
    for ( mapType::iterator iter = ms_strategies.begin(); iter != ms_strategies.end(); ++iter )
        delete iter->second;
    ms_strategies.clear ();
}

IStrategy* IStrategy::GetStrategy(const std::string& type)
{
    InitializeStrategies();
    
    mapType::const_iterator iter = ms_strategies.find(type);
    if ( iter == ms_strategies.end() )
        return 0;
    return iter->second;
}