/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        pose.h
// PURPOSE:     Pose definition.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#include <string>
#include "math/matrix.h"
#include "math/qtransform.h"

namespace Renderer
{
class Pose
{
public:
    //--------------------------------------------------------------------------
    // Configuration stuff
    enum
    {
        MAX_JOINTS = 64
    };

public:
            Pose    () {}
            ~Pose   () {}

public:
    //--------------------------------------------------------------------------
    // Public accessors
    std::string&        name            () { return m_name; }
    std::string*        jointNames      () { return &m_jointNames[0]; }
    QTransform*         transforms      () { return &m_transforms[0]; }
    u32&                numJoints       () { return m_numJoints; }
    
    const std::string&  name            () const { return m_name; }
    const std::string*  jointNames      () const { return &m_jointNames[0]; }
    const QTransform*   transforms      () const { return &m_transforms[0]; }
    const u32&          numJoints       () const { return m_numJoints; }
    
    //--------------------------------------------------------------------------
    // For use in the renderer
    void                calculateTransforms     ( Matrix* matrices );
    
private:
    //--------------------------------------------------------------------------
    // Name (unique identifier) for this pose
    std::string     m_name;
    
    //--------------------------------------------------------------------------
    // Names for each bone
    std::string     m_jointNames [ MAX_JOINTS ];

    //--------------------------------------------------------------------------
    // Matrices defining the transform of each joint
    QTransform      m_transforms [ MAX_JOINTS ];
    
    //--------------------------------------------------------------------------
    // Number of joints
    u32             m_numJoints;
};

}
