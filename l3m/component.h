#pragma once

#include <cstdarg>
#include <cstdio>
#include "l3m/stream.h"

namespace l3m
{

class IComponent
{
public:
                        IComponent      ( const char* type, float version ): m_type ( type ), m_version ( version ) {}
    virtual             ~IComponent     () {}
    
    virtual bool        Load            ( l3m::IStream& fp, float version ) = 0;
    virtual bool        Save            ( l3m::OStream& fp ) = 0;
    
    const char*         type            () const { return m_type; }
    float               version         () const { return m_version; }
    const char*         error           () const { return m_error; }
    
protected:
    bool                SetError        ( const char* msg, ... )
    {
        va_list vl;
        va_start ( vl, msg );
        vsnprintf ( m_error, sizeof(m_error), msg, vl );
        va_end ( vl );
        return false;
    }
    
private:
    const char*         m_type;
    float               m_version;
    char                m_error [ 256 ];
};

}
