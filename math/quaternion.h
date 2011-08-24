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
// FILE:        quaternion.h
// PURPOSE:     Quaternions
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#include "shared/platform.h"
#include "vector.h"

class Quaternion
{
private:
    f32         m_w;
    Vector3     m_xyz;
    
public:
    Quaternion () {}
    
    Quaternion ( const f32& w, const Vector3& xyz )
    : m_w ( w )
    , m_xyz ( xyz )
    {
    }
    
    Quaternion ( const Quaternion& quat )
    {
        operator= ( quat );
    }
    
    // Operators
    Quaternion& operator= ( const Quaternion& quat )
    {
        memcpy ( this, &quat, sizeof(Quaternion) );
        return *this;
    }
    
    f32&                w       () { return m_w; }
    f32&                x       () { return m_xyz.x(); }
    f32&                y       () { return m_xyz.y(); }
    f32&                z       () { return m_xyz.z(); }
    Vector3&            xyz     () { return m_xyz; }
    const f32&          w       () const { return m_w; }
    const f32&          x       () const { return m_xyz.x(); }
    const f32&          y       () const { return m_xyz.y(); }
    const f32&          z       () const { return m_xyz.z(); }
    const Vector3&      xyz     () const { return m_xyz; }
};

class IdentityQuaternion : public Quaternion
{
public:
    IdentityQuaternion ()
    : Quaternion ( 1.0f, ZeroVector3() )
    {}
};
