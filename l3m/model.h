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
#include "Components/factory.h"
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
                Model           ()
    : m_size ( 0 )
    {
    }
                ~Model          ()
    {
        for ( ComponentVector::const_iterator iter = m_vecComponents.begin();
              iter != m_vecComponents.end();
              ++iter )
        {
            delete *iter;
        }
        m_vecComponents.clear ();
    }

    bool        Load            ( const std::string& filepath )
    {
        std::fstream fp;
        fp.open(filepath.c_str(), std::ios::in | std::ios::binary);
        if ( fp.fail() )
            return SetError ( "Unable to open '%s' for reading", filepath.c_str() );
        return Load ( fp );
    }
    bool        Save            ( const std::string& filepath )
    {
        std::fstream fp;
        fp.open(filepath.c_str(), std::ios::out | std::ios::binary);
        if ( fp.fail() )
            return SetError ( "Unable to open '%s' for writing", filepath.c_str() );
        return Save ( fp );
    }
    
    bool        Load            ( std::istream& fp );
    bool        Save            ( std::ostream& fp );
    
    template < class T >
    T*          CreateComponent ( const std::string& type )
    {
        IComponent* component = ComponentFactory::Create( type );
        m_vecComponents.push_back(component);
        return static_cast<T*>(component);
    }
    
    unsigned int                numComponents   () const { return m_vecComponents.size(); }
    ComponentVector&            components      () { return m_vecComponents; }
    const ComponentVector&      components      () const { return m_vecComponents; }
    
    const u32&                  size            () const { return m_size; }
    
    //--------------------------------------------------------------------------
    // Function to register a delta resolver function, which will be called
    // at the end of the model loading to make it resolve all unresolved deltas.
    // A delta is a reference from a component to another component that must be
    // resolved after the model has been completely loaded.
    void        RegisterDeltaResolver   ( IComponent* comp, DeltaResolverFn fn, void* data )
    {
        DeltaData d;
        d.fn = fn;
        d.data = data;
        m_deltas [ comp ].push_back(d);
    }

private:
    void        ResolveDeltas       ();
    
    
private:
    bool        SetError        ( const char* msg, ... )
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

