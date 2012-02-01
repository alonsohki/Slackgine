/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
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
    //--------------------------------------------------------------------------
    // registerType
    // Registers a new component type, with a given function to create new
    // instances of it.
    static void                 registerType    ( const std::string& type, instantiatorFn fn );
    

    //--------------------------------------------------------------------------
    // create
    // Creates a new componeot of that type, or returns null if it's an unknown
    // type.
    static IComponent*          create          ( const std::string& type );
    
private:
    static void                 initialize      ();
    
private:
    static typesMap     ms_types;
    static bool         ms_initialized;
};

}
