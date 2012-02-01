/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        material.h
// PURPOSE:     Material component.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#include "l3m/component.h"
#include "renderer/material.h"

namespace l3m
{

class Material : public IComponent
{
private:
    Renderer::Material  m_material;
    
public:
    //--------------------------------------------------------------------------
    // Static constructor of camera entities for the factory
    static IComponent*  Create          ()
    {
        return new Material ();
    }

public:
    //--------------------------------------------------------------------------
    // Default constructor/destructor
    Material        ();
    ~Material       ();
    
    //--------------------------------------------------------------------------
    // Accessors
    const Renderer::Material&       material    () const { return m_material; }
    Renderer::Material&             material    () { return m_material; }
    
    //--------------------------------------------------------------------------
    // Inherited methods from IComponent
    bool                Load            ( l3m::Model*, l3m::IStream& stream, float version );
    bool                Save            ( l3m::Model*, l3m::OStream& stream );
};

}
