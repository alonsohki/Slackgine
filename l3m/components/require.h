/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        require.h
// PURPOSE:     Special component that tells the engine a model dependency.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#include "../component.h"

namespace l3m
{
    
class Require : public IComponent
{
public:
    enum RequireType
    {
        REQUIRE_MODEL,
        REQUIRE_TEXTURE
    };
    
public:
    static IComponent*  Create          ()
    {
        return new Require ();
    }
    
public:
                        Require         ();
                        ~Require        ();

    bool                load            ( l3m::Model*, l3m::IStream& stream, float version );
    bool                save            ( l3m::Model*, l3m::OStream& stream );
    
    std::string&        path            () { return m_path; }
    RequireType&        reqType         () { return m_reqType;  }
    
    const std::string&  path            () const { return m_path; }
    const RequireType&  reqType         () const { return m_reqType; }
    
    std::string         makepath        () const
    {
        switch ( m_reqType )
        {
            case REQUIRE_MODEL:
                return path() + ".l3m";
                break;
            case REQUIRE_TEXTURE:
                return path() + ".ltm";
                break;
        }
        
        return "";
    }
    
private:
    std::string         m_path;
    RequireType         m_reqType;
};

}
