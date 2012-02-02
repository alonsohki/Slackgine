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
#include "math/matrix.h"

namespace l3m
{
    
class Pose : public IComponent
{
public:
    //--------------------------------------------------------------------------
    // Configuration stuff
    enum
    {
        MAX_JOINTS = 64
    };
    
public:
    static IComponent*  create          ()
    {
        return new Pose ();
    }
    
public:
                        Pose            ();
                        ~Pose           ();

    bool                load            ( l3m::Model*, l3m::IStream& stream, float version );
    bool                save            ( l3m::Model*, l3m::OStream& stream );
    
    //--------------------------------------------------------------------------
    // Public accessors
    std::string&        name            () { return m_name; }
    std::string*        jointNames       () { return &m_jointNames[0]; }
    Matrix*             matrices        () { return &m_matrices[0]; }
    u32&                numJoints       () { return m_numJoints; }
    
    const std::string&  name            () const { return m_name; }
    const std::string*  jointNames       () const { return &m_jointNames[0]; }
    const Matrix*       matrices        () const { return &m_matrices[0]; }
    const u32&          numJoints       () const { return m_numJoints; }
    
private:
    //--------------------------------------------------------------------------
    // Name (unique identifier) for this pose
    std::string     m_name;
    
    //--------------------------------------------------------------------------
    // Names for each bone
    std::string     m_jointNames [ MAX_JOINTS ];

    //--------------------------------------------------------------------------
    // Matrices defining the transform of each joint
    Matrix          m_matrices [ MAX_JOINTS ];
    
    //--------------------------------------------------------------------------
    // Number of joints
    u32             m_numJoints;
};

}
