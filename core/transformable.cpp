/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
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

void Transformable::setPosition (const Vector3& pos)
{
    m_transform.translation () = pos;
}

void Transformable::translate (const Vector3& vec, TransformAxis axis )
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

void Transformable::setRotation (const EulerAngles& rot)
{
    m_transform.orientation () = RotationMatrix( rot );
}

void Transformable::setRotation ( const Matrix3& m )
{
    m_transform.orientation () = m;
}

void Transformable::rotate ( const Matrix3& m, TransformAxis axis )
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

void Transformable::rotate (const EulerAngles& rot, TransformAxis axis )
{
    RotationMatrix m ( rot );
    return rotate ( m, axis );
}
