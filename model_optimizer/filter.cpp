/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011 - Alberto Alonso
//
// Redistribution of this software, including any of its
// files, in either binary or source code form is totally
// forbidden without the permission of its owners.
//
// Owners reserve their right to legal action against any
// person or company that fails to do so.
//
// For more detailed information, see the LICENSE file in
// the top-level directory.
//
// FILE:        filter.h
// PURPOSE:     Strategy to filter certain component types.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

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
