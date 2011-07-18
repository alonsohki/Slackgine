#ifndef L3MFACTORY_H
#define	L3MFACTORY_H

#include <map>
#include "l3m.h"

class l3mFactory
{
public:
    static l3mComponent*         CreateAndLoad   ( std::istream& is, l3mComponent::ErrorCode* errorCode = 0 );
    static l3mComponent*         CreateAndLoad   ( const char* path, l3mComponent::ErrorCode* errorCode = 0 );
    static bool         RegisterType    ( const std::string& type, l3mComponent* (*instantiatorFn)() );
    static l3mComponent*         CreateOfType    ( const std::string& type );

private:
    typedef std::map<std::string, l3mComponent* (*)()> mapType;
    static mapType ms_instantiators;
};

#endif	/* L3MFACTORY_H */

