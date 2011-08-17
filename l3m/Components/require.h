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
    static IComponent*  Create          ()
    {
        return new Require ();
    }
    
public:
                        Require         ();
                        ~Require        ();

    bool                Load            ( l3m::IStream& stream, float version );
    bool                Save            ( l3m::OStream& stream );
    
    std::string&        path            () { return m_path; }
    const std::string&  path            () const { return m_path; }
    
private:
    std::string         m_path;
};

}
