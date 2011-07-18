#ifndef L3M_H
#define	L3M_H

#include <iostream>
#include <fstream>
#include "l3mComponent.h"
#include "l3mStream.h"

namespace l3m
{
    
class Model
{
public:
    bool        Load            ( const char* filepath )
    {
        std::fstream fp;
        fp.open(filepath, std::ios::in | std::ios::binary);
        if ( fp.fail() )
            return false;
        return Load ( fp );
    }
    bool        Save            ( const char* filepath )
    {
        std::fstream fp;
        fp.open(filepath, std::ios::out | std::ios::binary);
        if ( fp.fail() )
            return false;
        return Load ( fp );
    }
    
    bool        Load            ( const std::istream& fp );
    bool        Save            ( const std::ostream& fp );
};


};

#endif	/* L3M_H */

