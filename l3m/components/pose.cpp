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
#ifdef DEBUG
    for ( u32 i = 0; i < MAX_JOINTS; ++i )
        matrices()[i] = IdentityMatrix ();
#endif
}

Pose::~Pose ()
{
}

bool Pose::load(l3m::Model*, l3m::IStream& stream, float version)
{
    if ( stream.readStr ( m_name ) < 1 )
        return setError ( "Error reading the pose name" );
    
    if ( stream.read32(&m_numJoints, 1) != 1 )
        return setError ( "Error reading the pose '%s' joint count", m_name.c_str() );
    
    for ( u32 i = 0; i < m_numJoints; ++i )
    {
        if ( stream.readStr(m_jointNames[i]) < 0 )
            return setError ( "Error reading the pose '%s' joint names", m_name.c_str() );
    }
    
    if ( stream.readMatrix(&m_matrices[0], m_numJoints) != m_numJoints )
        return setError ( "Error reading the pose '%s' matrices", m_name.c_str() );
    
    return true;
}

bool Pose::save(l3m::Model*, l3m::OStream& stream)
{
    if ( ! stream.writeStr ( m_name ) )
        return setError ( "Error writing the pose '%s' name", m_name.c_str() );
    
    if ( !stream.write32 ( m_numJoints ) )
        return setError ( "Error writing the pose '%s' joint count", m_name.c_str() );
    
    for ( u32 i = 0; i < m_numJoints; ++i )
    {
        if ( !stream.writeStr(m_jointNames[i]) )
            return setError ( "Error writing the pose '%s' joint names", m_name.c_str() );
    }
    
    if ( !stream.writeMatrix(&m_matrices[0], m_numJoints) )
        return setError ( "Error writing the pose '%s' matrices", m_name.c_str() );

    return true;
}
