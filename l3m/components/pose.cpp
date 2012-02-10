/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        pose.cpp
// PURPOSE:     Armature pose component.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include "pose.h"

using namespace l3m;

Pose::Pose ()
: IComponent ( "pose", 1.0f )
{
}

Pose::~Pose ()
{
}

bool Pose::load(l3m::Model*, l3m::IStream& stream, float version)
{
    if ( stream.readStr ( m_pose.name() ) < 1 )
        return setError ( "Error reading the pose name" );
    
    if ( stream.read32(&m_pose.numJoints(), 1) != 1 )
        return setError ( "Error reading the pose '%s' joint count", m_pose.name().c_str() );
    
    for ( u32 i = 0; i < m_pose.numJoints(); ++i )
    {
        if ( stream.readStr(m_pose.jointNames()[i]) < 0 )
            return setError ( "Error reading the pose '%s' joint names", m_pose.name().c_str() );
    }
    
    if ( stream.readQTransform(&m_pose.transforms()[0], (u32)m_pose.numJoints()) != (u32)m_pose.numJoints() )
        return setError ( "Error reading the pose '%s' transforms", m_pose.name().c_str() );
    
    return true;
}

bool Pose::save(l3m::Model*, l3m::OStream& stream)
{
    if ( ! stream.writeStr ( m_pose.name() ) )
        return setError ( "Error writing the pose '%s' name", m_pose.name().c_str() );
    
    if ( !stream.write32 ( m_pose.numJoints() ) )
        return setError ( "Error writing the pose '%s' joint count", m_pose.name().c_str() );
    
    for ( u32 i = 0; i < m_pose.numJoints(); ++i )
    {
        if ( !stream.writeStr(m_pose.jointNames()[i]) )
            return setError ( "Error writing the pose '%s' joint names", m_pose.name().c_str() );
    }
    
    if ( !stream.writeQTransform(&m_pose.transforms()[0], m_pose.numJoints()) )
        return setError ( "Error writing the pose '%s' transforms", m_pose.name().c_str() );

    return true;
}
