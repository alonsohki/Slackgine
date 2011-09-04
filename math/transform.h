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
#include "vector.h"

//------------------------------------------------------------------------------
// Transform class
// Encapsulates the rigid body transformations
//
class Transform
{
private:
    Matrix3     m_orientation;
    Vector3     m_translation;
    
public:
    //--------------------------------------------------------------------------
    // Default constructor: Do nothing
    Transform ()
    {}
    
    //--------------------------------------------------------------------------
    // Constructor taking a translation and an orientation
    Transform ( const Vector3& translation, const Matrix3& orientation )
    : m_orientation ( orientation )
    , m_translation ( translation )
    {}
    
    //--------------------------------------------------------------------------
    // Accessors
    Matrix3&            orientation () { return m_orientation; }
    const Matrix3&      orientation () const { return m_orientation; }
    Vector3&            translation () { return m_translation; }
    const Vector3&      translation () const { return m_translation; }
    
    //--------------------------------------------------------------------------
    // Transform*Transform
    // Transform concatenation
    Transform operator* ( const Transform& Right ) const
    {
        Transform ret;
        ret.orientation() = orientation() * Right.orientation();
        ret.translation() = Right.translation() * orientation() + translation();
        return ret;
    }
    Transform& operator*= ( const Transform& Right )
    {
        *this = *this * Right;
        return *this;
    }
};



//--------------------------------------------------------------------------
// Utility transforms

//--------------------------------------------------------------------------
// IdentityTransform
class IdentityTransform : public Transform
{
public:
    IdentityTransform ()
    : Transform ( ZeroVector3(), IdentityMatrix3() )
    {}
};


//------------------------------------------------------------------------------
// Transform2Matrix
// Takes a transform as parameter and yields a transform matrix
class Transform2Matrix : public Matrix
{
public:
    Transform2Matrix ( const Transform& transform )
    {
        Matrix::operator= ( transform.orientation() );
        Matrix::v[12] = transform.translation().x();
        Matrix::v[13] = transform.translation().y();
        Matrix::v[14] = transform.translation().z();
    }
};
