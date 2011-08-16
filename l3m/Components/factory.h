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
// FILE:        factory.h
// PURPOSE:     Factory to create models based on their type.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#include <map>
#include "l3m/component.h"

namespace l3m
{

class ComponentFactory
{
public:
    typedef IComponent* (*instantiatorFn) ();
private:
    typedef std::map<std::string, instantiatorFn> typesMap;
    
public:
    static void                 Register        ( const std::string& type, instantiatorFn fn )
    {
        ms_types [ type ] = fn;
    }
    
    static IComponent*          Create          ( const std::string& type )
    {
        if ( !ms_initialized )
            Initialize ();
        
        typesMap::const_iterator iter = ms_types.find ( type );
        if ( iter != ms_types.end() )
            return iter->second ();
        return 0;
    }
    
private:
    static void                 Initialize      ();
    
private:
    static typesMap     ms_types;
    static bool         ms_initialized;
};

}
