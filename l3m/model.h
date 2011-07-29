#ifndef L3M_H
#define	L3M_H

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdarg>
#include "l3m/component.h"
#include "l3m/stream.h"
#include "Components/factory.h"

namespace l3m
{
    
class Model
{
public:
    typedef std::vector < IComponent* > componentVector;
    
public:
    virtual     ~Model          ()
    {
        for ( componentVector::const_iterator iter = m_vecComponents.begin();
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
            return SetError ( "Unable to open '%s' for reading", filepath );
        return Load ( fp );
    }
    bool        Save            ( const char* filepath )
    {
        std::fstream fp;
        fp.open(filepath, std::ios::out | std::ios::binary);
        if ( fp.fail() )
            return SetError ( "Unable to open '%s' for writing", filepath );
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
    componentVector&            components      () { return m_vecComponents; }
    const componentVector&      components      () const { return m_vecComponents; }
    
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
    componentVector             m_vecComponents;
    char                        m_error [ 1024 ];
};


};

#endif	/* L3M_H */

