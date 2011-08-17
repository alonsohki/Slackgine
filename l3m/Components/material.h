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
// FILE:        material.h
// PURPOSE:     Materials component.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#include "../component.h"
#include "renderer/material.h"

namespace l3m
{

class Material : public IComponent
{
public:
    static IComponent* Create ()
    {
        return new Material ();
    }
    
public:
                                Material        ();
                                ~Material       ();
                
    bool                        Load            ( l3m::IStream& fp, float version );
    bool                        Save            ( l3m::OStream& fp );
    
    std::string&                name            () { return m_name; }
    Renderer::Material&         material        () { return m_material; }
    
    const std::string&          name            () const { return m_name; }
    const Renderer::Material&   material        () const { return m_material; }
    
private:
    std::string         m_name;
    Renderer::Material  m_material;
};

}
