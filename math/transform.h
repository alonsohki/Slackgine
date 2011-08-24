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
// FILE:        transform.h
// PURPOSE:     Rigid body transformations
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#include "matrix.h"
#include "quaternion.h"
#include "vector.h"

class Transform
{
private:
    Quaternion  m_orientation;
    Vector3     m_translation;
    // Pad it to 32 bytes
    float       m_padding;
    
public:
    Transform ()
    : m_padding ( 0.0f )
    {}
    
    Transform ( const Vector3& translation, const Quaternion& orientation )
    : m_orientation ( orientation )
    , m_translation ( translation )
    , m_padding ( 0.0f )
    {}
    
    Quaternion&         orientation () { return m_orientation; }
    const Quaternion&   orientation () const { return m_orientation; }
    Vector3&            translation () { return m_translation; }
    const Vector3&      translation () const { return m_translation; }
};

class IdentityTransform : public Transform
{
public:
    IdentityTransform ()
    : Transform ( ZeroVector3(), IdentityQuaternion() )
    {}
};

#include <cstdio>
class Transform2Matrix : public Matrix
{
public:
    Transform2Matrix ( const Transform& transform )
    {
        const Quaternion& quat = transform.orientation();
        const Vector3& vec = transform.translation();
        const f32& qw = quat.w();
        const f32& qx = quat.x();
        const f32& qy = quat.y();
        const f32& qz = quat.z();
        const f32& vx = vec.x();
        const f32& vy = vec.y();
        const f32& vz = vec.z();
        
        f32 m[16] = {
            1 - 2*qy*qy - 2*qz*qz,      2*qx*qy + 2*qw*qz,      2*qx*qz - 2*qw*qy,      0.0f,
            2*qx*qy - 2*qw*qz,          1 - 2*qx*qx + 2*qz*qz,  2*qy*qz + 2*qw*qx,      0.0f,
            2*qx*qz + 2*qw*qy,          2*qy*qz - 2*qw*qx,      1 - 2*qx*qx - 2*qy*qy,  0.0f,
            vx,                         vy,                     vz,                     1.0f
        };
        
        Matrix::operator= ( m );
    }
};
