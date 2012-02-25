/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        pose.h
// PURPOSE:     Armature pose component.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#include "../component.h"
#include "renderer/pose.h"

namespace l3m
{
    
class Pose : public IComponent
{
public:
    static IComponent*  create          ()
    {
        return sgNew Pose ();
    }
    
public:
                        Pose            ();
                        ~Pose           ();

    //--------------------------------------------------------------------------
    // Functions to load/save the model data, inherited from IComponent.
    bool                load            ( l3m::Model*, l3m::IStream& stream, float version );
    bool                save            ( l3m::Model*, l3m::OStream& stream );
    
    //--------------------------------------------------------------------------
    // Public accessors
    Renderer::Pose&             pose        () { return m_pose; }
    const Renderer::Pose&       pose        () const { return m_pose; }
    
private:
    Renderer::Pose      m_pose;
};

}
