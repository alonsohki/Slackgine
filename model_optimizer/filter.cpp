/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2012
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        filter.h
// PURPOSE:     Strategy to filter certain component types.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include "filter.h"

using namespace l3m;

bool filter_types ( Model* model, const std::vector < std::string >& types )
{
    Model::ComponentVector& components = model->components();
    
    for ( Model::ComponentVector::iterator iter = components.begin();
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
