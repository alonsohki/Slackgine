#pragma once

#include <map>
#include "l3m/l3mComponent.h"

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
    static void                 Initialize      ()
    {
        ms_initialized = true;
        Register ( "geometry", Geometry::Create );
    }
    
private:
    static typesMap     ms_types;
    static bool         ms_initialized;
};

}
