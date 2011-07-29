#include "filter.h"

using namespace l3m;

bool filter_types ( Model* model, const std::vector < std::string >& types )
{
    Model::componentVector& components = model->components();
    
    for ( Model::componentVector::iterator iter = components.begin();
          iter != components.end (); )
    {
        const std::string& type = (*iter)->type ();
        
        bool found = false;
        for ( std::vector<std::string>::const_iterator s = types.begin ();
              !found && s != types.end ();
              ++s )
        {
            if ( *s == type )
                found = true;
        }
        
        if ( found )
            iter = components.erase ( iter );
        else
            ++iter;
    }
    
    return true;
}
