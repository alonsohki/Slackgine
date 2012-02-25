/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
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

//------------------------------------------------------------------------------
// Matrix2Transform
// Takes a matrix as parameter and yields a transform
class Matrix2Transform : public Transform
{
public:
    Matrix2Transform ( const Matrix& mat )
    {
        const f32* m = mat.vector();
        float fOrientation[9] = {
            m[0], m[1], m[2],
            m[4], m[5], m[6],
            m[8], m[9], m[10]
        };
        orientation() = Matrix3 ( fOrientation );
        translation() = Vector3 ( m[12], m[13], m[14] );
    }
};
