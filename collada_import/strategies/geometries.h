#ifndef DEFAULT_H
#define	DEFAULT_H

#include "../strategy.h"

class GeometriesStrategy : public IStrategy
{
public:
    GeometriesStrategy ()
    {
    }
    
    ~GeometriesStrategy ()
    {
    }
    
    bool ParseData ( TiXmlElement& element, l3m::Model& model, const char** err );
};

#endif	/* DEFAULT_H */

