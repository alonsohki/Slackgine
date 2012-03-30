/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        morph.h
// PURPOSE:     Morph component.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//              Rayco Hernandez Garcia <raycohg@gmail.com>

#pragma once

#include "../component.h"
#include "renderer/morph.h"

namespace l3m
{
    
class Morph : public IComponent
{
public:
    static IComponent* create ()
    {
        return sgNew Morph ();
    }
    
public:
                        Morph     ();
                        ~Morph    ();

    //--------------------------------------------------------------------------
    // Functions to load/save the model data, inherited from IComponent.
    bool                load            ( l3m::Model*, l3m::IStream& stream, float version );
    bool                save            ( l3m::Model*, l3m::OStream& stream );
    
    //--------------------------------------------------------------------------
    // Public accessors
    Renderer::Morph&         morph () { return m_morph; }
    const Renderer::Morph&   morph () const { return m_morph; }
    
private:
    Renderer::Morph      m_morph;
};

}
