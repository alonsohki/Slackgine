#pragma once

#include "vector.h"

class BoundingBox
{
public:
    BoundingBox ( float minX = 0.0f, float maxX = 0.0f,
                  float minY = 0.0f, float maxY = 0.0f,
                  float minZ = 0.0f, float maxZ = 0.0f )
    : m_minX ( minX )
    , m_maxX ( maxX )
    , m_minY ( minY )
    , m_maxY ( maxY )
    , m_minZ ( minZ )
    , m_maxZ ( maxZ )
    {
    }
    
    const float&        minX    () const { return m_minX; }
    const float&        maxX    () const { return m_maxX; }
    const float&        minY    () const { return m_minY; }
    const float&        maxY    () const { return m_maxY; }
    const float&        minZ    () const { return m_minZ; }
    const float&        maxZ    () const { return m_maxZ; }
    
    bool        IsPointWithin           ( const Vector3& point ) const
    {
        return point.x() >= m_minX && point.x() <= m_maxX &&
               point.y() >= m_minY && point.y() <= m_maxY &&
               point.z() >= m_minZ && point.z() <= m_maxZ;
    }
            
private:
    float m_minX;
    float m_maxX;
    float m_minY;
    float m_maxY;
    float m_minZ;
    float m_maxZ;
};
