/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        transformable.h
// PURPOSE:     Base class for any transformable entity.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#include "math/vector.h"
#include "math/euler.h"
#include "math/quaternion.h"
#include "math/transform.h"

namespace Core
{
    
class Transformable
{
public:
    enum TransformAxis
    {
        WORLD,
        INERTIAL,
        OBJECT
    };
public:
                        Transformable           ();
                        Transformable           ( const Transform& transform );
    virtual             ~Transformable          () {}

    void                move                    ( const Vector3& vec, TransformAxis axis = OBJECT ) { return translate(vec, axis); }
    void                translate               ( const Vector3& vec, TransformAxis axis = OBJECT );
    void                setPosition             ( const Vector3& pos );

    void                rotate                  ( const EulerAngles& rot, TransformAxis axis = OBJECT );
    void                rotate                  ( const Matrix3& rot, TransformAxis axis = OBJECT );
    void                setRotation             ( const EulerAngles& rot );
    void                setRotation             ( const Matrix3& rot );
    
    void                tick                    () {}
    
    //--------------------------------------------------------------------------
    // Accessors
    Transform&          transform               () { return m_transform; }
    const Transform&    transform               () const { return m_transform; }
    
private:
    Transform   m_transform;
};
    
}
