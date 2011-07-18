#ifndef L3M_H
#define	L3M_H

#include <iostream>
#include <fstream>
#include <vector>
#include "l3mComponent.h"
#include "l3mStream.h"

namespace l3m
{
    
class Model
{
public:
    virtual     ~Model          ()
    {
        for ( std::vector<IComponent*>::const_iterator iter = m_vecComponents.begin();
              iter != m_vecComponents.end();
              ++iter )
        {
            delete *iter;
        }
        m_vecComponents.clear ();
    }
                
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
    
    bool        Load            ( std::istream& fp );
    bool        Save            ( std::ostream& fp );
    
private:
    std::vector<IComponent*>    m_vecComponents;
};


};

#endif	/* L3M_H */

