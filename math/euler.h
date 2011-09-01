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
// FILE:        euler.h
// PURPOSE:     Heading-pitch-back rotations
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#include "shared/platform.h"
#include "util.h"

class EulerAngles
{
private:
    union
    {
        struct
        {
            f32 m_heading;
            f32 m_pitch;
            f32 m_bank;
        };
        f32 v [ 3 ];
    };
    
public:
    //--------------------------------------------------------------------------
    // Default constructor: do nothing
    EulerAngles ()
    {}
    
    //--------------------------------------------------------------------------
    // Constructor to define a triple for the initialization
    EulerAngles ( const f32& heading, const f32& pitch, const f32& bank )
    : m_heading(heading), m_pitch(pitch), m_bank(bank)
    {
        Canonize ();
    }
    
    //--------------------------------------------------------------------------
    // Copy constructor
    EulerAngles ( const EulerAngles& other )
    {
        operator= ( other );
    }
    
    //--------------------------------------------------------------------------
    // Operators
    EulerAngles& operator= ( const EulerAngles& other )
    {
        m_heading = other.m_heading;
        m_pitch = other.m_pitch;
        m_bank = other.m_bank;
        Canonize ();
        
        return *this;
    }
    
    bool operator== ( const EulerAngles& other ) const
    {
        return other.m_heading == m_heading && other.m_pitch == m_pitch && other.m_bank == m_bank;
    }
    
    bool operator!= ( const EulerAngles& other ) const
    {
        return !operator== ( other );
    }
    
    //--------------------------------------------------------------------------
    // Accessors
    f32& heading () { return m_heading; }
    f32& pitch   () { return m_pitch; }
    f32& bank    () { return m_bank; }
    const f32& heading () const { return m_heading; }
    const f32& pitch   () const { return m_pitch; }
    const f32& bank    () const { return m_bank; }

    
    //--------------------------------------------------------------------------
    // Canonize
    // Determine the canonical Euler angle triple
    void Canonize ()
    {
        // First, wrap pitch in range -pi ... pi
        m_pitch = wrapPi(m_pitch);
        
        // Check for the back side of the matrix pitch outside
        // the canonical range of -pi/2 ... pi/2
        if ( m_pitch < -kPiOver2 )
        {
            m_pitch = -kPi - m_pitch;
            m_heading += kPi;
            m_bank += kPi;
        }
        else if ( m_pitch > kPiOver2 )
        {
            m_pitch = kPi - m_pitch;
            m_heading += kPi;
            m_bank += kPi;
        }
        
        // Now check for the gimbal lock case (within a slight tolerance)
        if ( fabs(m_pitch) > kPiOver2 - 0.0001f )
        {
            // We are in gimbal lock. Assign all rotation about the vertical
            // axis to heading.
            m_heading += m_bank;
            m_bank = 0.0f;
        }
        else
        {
            // Not in gimbal lock. Wrap the bank angle in canonical range.
            m_bank = wrapPi ( m_bank );
        }
        
        // Wrap heading in canonical range
        m_heading = wrapPi ( m_heading );
    }
};

//------------------------------------------------------------------------------
// IdentityEulerAngles
// Provide a construction for the EulerAngles class for identity orientations.
class IdentityEulerAngles : public EulerAngles
{
public:
    IdentityEulerAngles () : EulerAngles ( 0.0f, 0.0f, 0.0f ) {}
};
