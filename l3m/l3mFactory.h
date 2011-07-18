#ifndef L3MFACTORY_H
#define	L3MFACTORY_H

#include <map>
#include "l3m.h"

class l3mFactory
{
public:
    static l3m::l3m*    CreateAndLoad   ( std::istream& is, l3m::l3m::ErrorCode* errorCode = 0 );
    static l3m::l3m*    CreateAndLoad   ( const char* path, l3m::l3m::ErrorCode* errorCode = 0 );
    static bool         RegisterType    ( const std::string& type, l3mComponent* (*instantiatorFn)() );
    static l3m::l3m*    CreateOfType    ( const std::string& type );

private:
    typedef std::map<std::string, l3mComponent* (*)()> mapType;
    static mapType ms_instantiators;
};

#endif	/* L3MFACTORY_H */

