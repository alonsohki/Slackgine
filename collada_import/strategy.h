#ifndef STRATEGY_H
#define	STRATEGY_H

#include "tinyxml/tinyxml.h"
#include "l3m/l3m.h"

class IStrategy
{
public:
    virtual             ~IStrategy              () {}
    virtual bool        ParseData               ( TiXmlDocument& document, l3mComponent& model ) = 0;
    
    
public:
    static IStrategy*   GetStrategy                     ( const std::string& type );
    static void         CleanupStrategies               ();
private:
    static void         InitializeStrategies            ();
    
private:
    typedef std::map<std::string, IStrategy*> mapType;
    static mapType ms_strategies;
};

#endif

