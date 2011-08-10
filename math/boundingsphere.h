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