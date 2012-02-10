/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        quaternion.h
// PURPOSE:     Quaternions
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#include "shared/platform.h"
#include "shared/log.h"
#include "euler.h"
#include "util.h"
#include "vector.h"

class Quaternion
{
private:
    f32         m_w;
    Vector3     m_xyz;
    
public:
    //--------------------------------------------------------------------------
    // Default constructor: Do nothing
    Quaternion () {}
    
    //--------------------------------------------------------------------------
    // Constructor taking the initialization values
    Quaternion ( const f32& w, const Vector3& xyz )
    : m_w ( w )
    , m_xyz ( xyz )
    {
    }
    
    //--------------------------------------------------------------------------
    // Constructor taking euler angles
    Quaternion ( const EulerAngles& rot )
    {
        setToRotateInertialToObject( rot );
    }
    
    //--------------------------------------------------------------------------
    // Copy constructor
    Quaternion ( const Quaternion& quat )
    {
        operator= ( quat );
    }
    
    //--------------------------------------------------------------------------
    // Operators
    Quaternion& operator= ( const Quaternion& quat )
    {
        m_w = quat.m_w;
        m_xyz = quat.m_xyz;
        return *this;
    }
    
    bool operator== ( const Quaternion& quat ) const
    {
        return m_w == quat.m_w && m_xyz == quat.m_xyz;
    }
    
    bool operator!= ( const Quaternion& quat ) const
    {
        return !operator== ( quat );
    }
    
    //--------------------------------------------------------------------------
    // Accessors
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

    //--------------------------------------------------------------------------
    // Interface to euler angles
    void setToRotateInertialToObject ( const EulerAngles& orientation )
    {
        // Compute since and cosing of the half angles
        f32 sp, sb, sh;
        f32 cp, cb, ch;
        sinCos ( &sp, &cp, orientation.pitch() * 0.5f );
        sinCos ( &sb, &cb, orientation.bank() * 0.5f );
        sinCos ( &sh, &ch, orientation.heading() * 0.5f );
        
        // Compute values
        w() =  ch*cp*cb + sh*sp*sb;
        x() =  ch*sp*cb + sh*cp*sb;
        y() = -ch*sp*sb + sh*cp*cb;
        z() = -sh*sp*cb + ch*cp*sb;
    }
    
    //--------------------------------------------------------------------------
    // Normalize
    void normalize ()
    {
        // Compute magnitude of the quaternion
        f32 mag = (f32)sqrt ( w()*w() + x()*x() + y()*y() + z()*z() );
        
        // Check for bogus length to protect against divide by zero
        if ( mag > 0.0f )
        {
            // Normalize it
            f32 oneOverMag = 1.0f / mag;
            w() *= oneOverMag;
            x() *= oneOverMag;
            y() *= oneOverMag;
            z() *= oneOverMag;
        }
        else
        {
            LOG_E ( "Quaternion", "Trying to normalize a quaternion of length 0" );
            *this = Quaternion ( 1.0f, Vector3(0.0f, 0.0f, 0.0f) );
        }
    }
    
    //--------------------------------------------------------------------------
    // Conjugate
    Quaternion conjugate ()
    {
        Quaternion result;
        
        // Use the same rotation amount, but opposite axis.
        result.w() = w();
        result.xyz() = -xyz();
        
        return result;
    }
    
    //--------------------------------------------------------------------------
    
    
    //--------------------------------------------------------------------------
    // Q * Q
    // Quaternion multiplication
    Quaternion operator* ( const Quaternion& Right ) const
    {
        Quaternion result;
        
        result.w() = w()*Right.w() - x()*Right.x() - y()*Right.y() - z()*Right.z();
        result.x() = w()*Right.x() + x()*Right.w() + y()*Right.z() - z()*Right.y();
        result.y() = w()*Right.y() - x()*Right.z() + y()*Right.w() + z()*Right.x();
        result.z() = w()*Right.z() + x()*Right.y() - y()*Right.x() + z()*Right.w();
        
        return result;
    }
    Quaternion& operator*= ( const Quaternion& Right )
    {
        *this = *this * Right;
        return *this;
    }
};

class IdentityQuaternion : public Quaternion
{
public:
    IdentityQuaternion ()
    : Quaternion ( 1.0f, ZeroVector3() )
    {}
};

//------------------------------------------------------------------------------
// Conjugate
static inline Quaternion conjugate ( const Quaternion& quat )
{
    Quaternion result ( quat );
    result.conjugate ();
    return result;
}

//------------------------------------------------------------------------------
// Normalize
static inline Quaternion normalize ( const Quaternion& quat )
{
    Quaternion result ( quat );
    result.normalize ();
    return result;
}
