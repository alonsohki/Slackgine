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
// FILE:        boundingsphere.h
// PURPOSE:     Bounding spheres.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#include "vector.h"

class BoundingSphere
{
public:
    BoundingSphere ( float radius = 0.0f )
    : m_radius ( radius )
    {
        m_radiusSquared = radius*radius;
    }
    
    const float&        radius          () const { return m_radius; }
    
    bool        IsPointWithin           ( const Vector3& point ) const
    {
        return m_radiusSquared >= ( point.x()*point.x() + point.y()*point.y() + point.z()*point.z() );
    }
    
private:
    float       m_radius;
    float       m_radiusSquared;
};