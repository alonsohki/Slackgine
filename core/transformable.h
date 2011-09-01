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

    void                Move                    ( const Vector3& vec, TransformAxis axis = OBJECT ) { return Translate(vec, axis); }
    void                Translate               ( const Vector3& vec, TransformAxis axis = OBJECT );
    void                SetPosition             ( const Vector3& pos );

    void                Rotate                  ( const EulerAngles& rot, TransformAxis axis = OBJECT );
    void                Rotate                  ( const Matrix3& rot, TransformAxis axis = OBJECT );
    void                SetRotation             ( const EulerAngles& rot );
    void                SetRotation             ( const Matrix3& rot );
    
    void                Tick                    () {}
    
    //--------------------------------------------------------------------------
    // Accessors
    Transform&          transform               () { return m_transform; }
    const Transform&    transform               () const { return m_transform; }
    
private:
    Transform   m_transform;
};
    
}
