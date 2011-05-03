#ifndef VECTOR_H
#define VECTOR_H

#include <cstring>
#include "shared/platform.h"

class Vector2
{
private:
    typedef struct { f32 fX, fY; } __s2f;
    
    union
    {
        f32 v [ 2 ];
        __s2f s;
    };
    
public:
    Vector2 ()
    {
        x() = 0.0f;
        y() = 0.0f;
    }
    
    const f32& x() const { return s.fX; }
    const f32& y() const { return s.fY; }
    const f32* vector() const { return v; }
    
    f32& x() { return s.fX; }
    f32& y() { return s.fY; }
    f32* vector() { return v; }
};



class Vector3
{
private:
    typedef struct { f32 fX, fY, fZ; } __s3f;
    
    union
    {
        f32 v [ 3 ];
        __s3f s;
    };
    
public:
    Vector3 ( f32 fX = 0.0f, f32 fY = 0.0f, f32 fZ = 0.0f )
    {
        x() = fX;
        y() = fY;
        z() = fZ;
    }
    
    // Copy, assignment, compare.
    Vector3 ( const Vector3& v )
    {
        operator=(v.v);
    }
    Vector3 ( f32* vec )
    {
        operator=(vec);
    }
    Vector3& operator= ( const Vector3& v )
    {
        return operator=(v.v);
    }
    Vector3& operator= ( const f32* vec )
    {
        memcpy ( v, vec, sizeof(v) );
        return *this;
    }
    
    bool operator== ( const Vector3& vec ) const
    {
        return memcmp ( v, vec.v, sizeof(v) ) == 0;
    }
    bool operator!= ( const Vector3& vec ) const
    {
        return !operator==(vec);
    }
    
    
    // Accessors
    const f32& x() const { return s.fX; }
    const f32& y() const { return s.fY; }
    const f32& z() const { return s.fZ; }
    const f32* vector() const { return v; }
    
    f32& x() { return s.fX; }
    f32& y() { return s.fY; }
    f32& z() { return s.fZ; }
    f32* vector() { return v; }
    
    // V + V
    Vector3 operator+ ( const Vector3& vec ) const
    {
        Vector3 ret;
        for ( u8 i = 0; i < 3; ++i )
            ret.v[i] = v[i] + vec.v[i];
        return ret;
    }
    Vector3& operator+= ( const Vector3& vec )
    {
        for ( u8 i = 0; i < 3; ++i )
            v[i] += vec.v[i];
        return *this;
    }
    
    // V - V
    Vector3 operator- ( const Vector3& vec ) const
    {
        Vector3 ret;
        for ( u8 i = 0; i < 3; ++i )
            ret.v[i] = v[i] - vec.v[i];
        return ret;
    }
    Vector3& operator-= ( const Vector3& vec )
    {
        for ( u8 i = 0; i < 3; ++i )
            v[i] -= vec.v[i];
        return *this;
    }
    
    // V * V (Cross product)
    Vector3 operator* ( const Vector3& vec ) const
    {
        float ret[3];
        ret[0] = y()*vec.z() - z()*vec.y();
        ret[1] = z()*vec.x() - x()*vec.z();
        ret[2] = x()*vec.y() - y()*vec.x();
        return Vector3(ret);
    }
    Vector3& operator*= ( const Vector3& vec )
    {
        *this = *this * vec;
        return *this;
    }
    
    // V * V (Dot product)
    f32 Dot ( const Vector3& vec ) const
    {
        return x()*vec.x() + y()*vec.y() + z()*vec.z();
    }
};

#endif
