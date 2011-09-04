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
// FILE:        transformable.cpp
// PURPOSE:     Base class for any transformable entity.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include "transformable.h"
#include "texture_manager.h"

using namespace Core;

Transformable::Transformable ()
: m_transform ( IdentityTransform() )
{
}

Transformable::Transformable ( const Transform& transform )
: m_transform ( transform )
{
}

void Transformable::SetPosition (const Vector3& pos)
{
    m_transform.translation () = pos;
}

void Transformable::Translate (const Vector3& vec, TransformAxis axis )
{
    switch ( axis )
    {
        case OBJECT:
            m_transform.translation () += vec * m_transform.orientation ();
            break;
        case WORLD:
        case INERTIAL:
            m_transform.translation () += vec;
            break;
    }
}

void Transformable::SetRotation (const EulerAngles& rot)
{
    m_transform.orientation () = RotationMatrix( rot );
}

void Transformable::SetRotation ( const Matrix3& m )
{
    m_transform.orientation () = m;
}

void Transformable::Rotate ( const Matrix3& m, TransformAxis axis )
{
    switch ( axis )
    {
        case WORLD:
            m_transform = Transform ( ZeroVector3(), m ) * m_transform;
            break;
        case INERTIAL:
            m_transform.orientation () = m * m_transform.orientation ();
            break;
        case OBJECT:
            m_transform.orientation () *= m;
            break;
    }
}

void Transformable::Rotate (const EulerAngles& rot, TransformAxis axis )
{
    RotationMatrix m ( rot );
    return Rotate ( m, axis );
}
