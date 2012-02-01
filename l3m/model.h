/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        model.h
// PURPOSE:     l3m models.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#include "component.h"
#include "components/factory.h"
#include "shared/FastDelegate.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdarg>
#include "l3m/stream.h"

namespace l3m
{
    
class Model
{
public:
    typedef std::vector < IComponent* > ComponentVector;
    
    //--------------------------------------------------------------------------
    // Delta resolver datatypes
private:
    typedef FastDelegate3 < IComponent*, Model*, void*, bool > DeltaResolverFn;
    struct DeltaData
    {
        DeltaData () : data(0) {}
 
        DeltaResolverFn fn;
        void* data;
    };
    typedef std::map < IComponent*, std::vector<DeltaData> > DeltaResolverMap;

    
public:
    //--------------------------------------------------------------------------
    // Constructor/Destructor
    Model   ();
    ~Model  ();


    //--------------------------------------------------------------------------
    // Model loading/saving functions
    bool        load            ( const std::string& filepath );
    bool        save            ( const std::string& filepath );
    bool        load            ( std::istream& fp );
    bool        save            ( std::ostream& fp );
    

    //--------------------------------------------------------------------------
    // Functions to manage/access the model components
    IComponent*                 createComponent ( const std::string& type );
    unsigned int                numComponents   () const { return m_vecComponents.size(); }
    ComponentVector&            components      () { return m_vecComponents; }
    const ComponentVector&      components      () const { return m_vecComponents; }
    
    
    //--------------------------------------------------------------------------
    // Function to get the size of this component. Mainly used by the model
    // manager garbage collector.
    const u32&                  size            () const { return m_size; }
    
    
    //--------------------------------------------------------------------------
    // Function to register a delta resolver function, which will be called
    // at the end of the model loading to make it resolve all unresolved deltas.
    // A delta is a reference from a component to another component that must be
    // resolved after the model has been completely loaded.
    void        registerDeltaResolver   ( IComponent* comp, DeltaResolverFn fn, void* data )
    {
        DeltaData d;
        d.fn = fn;
        d.data = data;
        m_deltas [ comp ].push_back(d);
    }

private:
    void        resolveDeltas       ();
    
    
private:
    bool        setError        ( const char* msg, ... )
    {
        va_list vl;
        va_start ( vl, msg );
        vsnprintf ( m_error, sizeof(m_error), msg, vl );
        va_end ( vl );
        return false;
    }
public:
    const char* error           () const { return m_error; }
    
private:
    DeltaResolverMap            m_deltas;
    ComponentVector             m_vecComponents;
    char                        m_error [ 1024 ];
    u32                         m_size;
};


};

