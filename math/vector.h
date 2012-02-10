/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        vector.h
// PURPOSE:     Vectors.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#include <cmath>
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
    
    Vector2 ( const Vector2& vec )
    {
        x() = vec.x();
        y() = vec.y();
    }
    
    Vector2 ( float fX, float fY )
    {
        x() = fX;
        y() = fY;
    }
    
    Vector2 ( const float* f )
    {
        x() = f[0];
        y() = f[1];
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
    Vector3 () {}

    Vector3 ( f32 fX, f32 fY, f32 fZ )
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
    Vector3 ( const f32* vec )
    {
        operator=(vec);
    }
    Vector3& operator= ( const Vector3& v )
    {
        return operator=(v.v);
    }
    Vector3& operator= ( const f32* vec )
    {
        for ( u8 i = 0; i < 3; ++i )
            v[i] = vec[i];
        return *this;
    }
    
    bool operator== ( const Vector3& vec ) const
    {
        return v[0] == vec.v[0] && v[1] == vec.v[1] && v[2] == vec.v[2];
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
    
    //--------------------------------------------------------------------------
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
    
    //--------------------------------------------------------------------------
    // -V
    Vector3 operator- () const
    {
        Vector3 ret;
        for ( u8 i = 0; i < 3; ++i )
            ret.v[i] = -v[i];
        return ret;
    }
    
    //--------------------------------------------------------------------------
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

    //--------------------------------------------------------------------------
    // V + k
    Vector3 operator+ ( const f32& k ) const
    {
        return Vector3 ( x() + k, y() + k, z() + k );
    }
    Vector3& operator+= ( const f32& k )
    {
        *this = *this + k;
        return *this;
    }
    
    //--------------------------------------------------------------------------
    // V - k
    Vector3 operator- ( const f32& k ) const
    {
        return Vector3 ( x() - k, y() - k, z() - k );
    }
    Vector3& operator-= ( const f32& k )
    {
        *this = *this - k;
        return *this;
    }
    
    //--------------------------------------------------------------------------
    // V * k
    Vector3 operator* ( const f32& k ) const
    {
        return Vector3 ( x() * k, y() * k, z() * k );
    }
    Vector3& operator*= ( const f32& k )
    {
        *this = *this * k;
        return *this;
    }
    
    //--------------------------------------------------------------------------
    // V / k
    Vector3 operator/ ( const f32& k ) const
    {
        return Vector3 ( x() / k, y() / k, z() / k );
    }
    Vector3& operator/= ( const f32& k )
    {
        *this = *this / k;
        return *this;
    }
    
    //--------------------------------------------------------------------------
    // V * V (Cross product)
    Vector3 Cross ( const Vector3& vec ) const
    {
        float ret[3];
        ret[0] = y()*vec.z() - z()*vec.y();
        ret[1] = z()*vec.x() - x()*vec.z();
        ret[2] = x()*vec.y() - y()*vec.x();
        return Vector3(ret);
    }
    
    //--------------------------------------------------------------------------
    // V * V (Dot product)
    f32 Dot ( const Vector3& vec ) const
    {
        return x()*vec.x() + y()*vec.y() + z()*vec.z();
    }
    
    //--------------------------------------------------------------------------
    // Normalization and length
    f32 Length () const
    {
        return sqrt(x()*x() + y()*y() + z()*z());
    }
    static f32 Length ( const Vector3& thiz )
    {
        return thiz.Length();
    }
    
    void Normalize ()
    {
        f32 length = Length ();
        if ( fabs(length) > 0.000001f )
        {
            x() = x() / length;
            y() = y() / length;
            z() = z() / length;
        }
    }
    static Vector3 Normalize ( const Vector3& vec )
    {
        Vector3 v = vec;
        v.Normalize ();
        return v;
    }
};

class Vector4
{
private:
    typedef struct { f32 fX, fY, fZ, fW; } __s4f;
    
    union
    {
        f32 v [ 4 ];
        __s4f s;
    };
    
public:
    Vector4 () {}

    Vector4 ( f32 fX, f32 fY, f32 fZ, f32 fW )
    {
        x() = fX;
        y() = fY;
        z() = fZ;
        w() = fW;
    }
    
    // Copy, assignment, compare.
    Vector4 ( const Vector4& v )
    {
        operator=(v.v);
    }
    Vector4 ( const f32* vec )
    {
        operator=(vec);
    }
    Vector4& operator= ( const Vector4& v )
    {
        return operator=(v.v);
    }
    Vector4& operator= ( const f32* vec )
    {
        for ( u8 i = 0; i < 4; ++i )
            v[i] = vec[i];
        return *this;
    }
    
    bool operator== ( const Vector4& vec ) const
    {
        return v[0] == vec.v[0] && v[1] == vec.v[1] && v[2] == vec.v[2] && v[3] == vec.v[3];
    }
    bool operator!= ( const Vector4& vec ) const
    {
        return !operator==(vec);
    }
    
    
    // Accessors
    const f32& x() const { return s.fX; }
    const f32& y() const { return s.fY; }
    const f32& z() const { return s.fZ; }
    const f32& w() const { return s.fW; }
    const f32* vector() const { return v; }
    
    f32& x() { return s.fX; }
    f32& y() { return s.fY; }
    f32& z() { return s.fZ; }
    f32& w() { return s.fW; }
    f32* vector() { return v; }
    
    //--------------------------------------------------------------------------
    // V + V
    Vector4 operator+ ( const Vector4& vec ) const
    {
        Vector4 ret;
        for ( u8 i = 0; i < 4; ++i )
            ret.v[i] = v[i] + vec.v[i];
        return ret;
    }
    Vector4& operator+= ( const Vector4& vec )
    {
        for ( u8 i = 0; i < 4; ++i )
            v[i] += vec.v[i];
        return *this;
    }
    
    //--------------------------------------------------------------------------
    // -V
    Vector4 operator- () const
    {
        Vector4 ret;
        for ( u8 i = 0; i < 4; ++i )
            ret.v[i] = -v[i];
        return ret;
    }
    
    //--------------------------------------------------------------------------
    // V - V
    Vector4 operator- ( const Vector4& vec ) const
    {
        Vector4 ret;
        for ( u8 i = 0; i < 4; ++i )
            ret.v[i] = v[i] - vec.v[i];
        return ret;
    }
    Vector4& operator-= ( const Vector4& vec )
    {
        for ( u8 i = 0; i < 4; ++i )
            v[i] -= vec.v[i];
        return *this;
    }

    //--------------------------------------------------------------------------
    // V + k
    Vector4 operator+ ( const f32& k ) const
    {
        return Vector4 ( x() + k, y() + k, z() + k, w() + k );
    }
    Vector4& operator+= ( const f32& k )
    {
        *this = *this + k;
        return *this;
    }
    
    //--------------------------------------------------------------------------
    // V - k
    Vector4 operator- ( const f32& k ) const
    {
        return Vector4 ( x() - k, y() - k, z() - k, w() - k );
    }
    Vector4& operator-= ( const f32& k )
    {
        *this = *this - k;
        return *this;
    }
    
    //--------------------------------------------------------------------------
    // V * k
    Vector4 operator* ( const f32& k ) const
    {
        return Vector4 ( x() * k, y() * k, z() * k, w() * k );
    }
    Vector4& operator*= ( const f32& k )
    {
        *this = *this * k;
        return *this;
    }
    
    //--------------------------------------------------------------------------
    // V / k
    Vector4 operator/ ( const f32& k ) const
    {
        return Vector4 ( x() / k, y() / k, z() / k, w() / k );
    }
    Vector4& operator/= ( const f32& k )
    {
        *this = *this / k;
        return *this;
    }
    
    //--------------------------------------------------------------------------
    // V * V (Dot product)
    f32 Dot ( const Vector4& vec ) const
    {
        return x()*vec.x() + y()*vec.y() + z()*vec.z() + w()*vec.w();
    }
    
    //--------------------------------------------------------------------------
    // Normalization and length
    f32 Length () const
    {
        return sqrt(x()*x() + y()*y() + z()*z() + w()*w());
    }
    static f32 Length ( const Vector4& thiz )
    {
        return thiz.Length();
    }
    
    void Normalize ()
    {
        f32 length = Length ();
        if ( fabs(length) > 0.000001f )
        {
            f32 f = 1.0f / length;
            x() = x() * f;
            y() = y() * f;
            z() = z() * f;
            w() = w() * f;
        }
    }
    static Vector4 Normalize ( const Vector4& vec )
    {
        Vector4 v = vec;
        v.Normalize ();
        return v;
    }
};


//------------------------------------------------------------------------------
// Null vectors
class ZeroVector2 : public Vector2
{
public:
    ZeroVector2 ()
    : Vector2 ( 0.0f, 0.0f )
    {}
};

class ZeroVector3 : public Vector3
{
public:
    ZeroVector3 ()
    : Vector3 ( 0.0f, 0.0f, 0.0f )
    {}
};

class ZeroVector4 : public Vector4
{
public:
    ZeroVector4 ()
    : Vector4 ( 0.0f, 0.0f, 0.0f, 0.0f )
    {}
};

//------------------------------------------------------------------------------
// Dot product of vectors
static inline f32 Dot ( const Vector3& vec1, const Vector3& vec2 )
{
    return vec1.Dot ( vec2 );
}

//------------------------------------------------------------------------------
// Cross product of vectors
static inline Vector3 Cross ( const Vector3& vec1, const Vector3& vec2 )
{
    return vec1.Cross(vec2);
}

//------------------------------------------------------------------------------
// Vector transform by quaternion
// Actually equivalent to q^-1 * V * q
#include "quaternion.h"
static inline Vector3 operator* ( const Vector3& vec, const Quaternion& quat )
{
    Quaternion p ( 0, vec );
    Quaternion q = quat;
    q.normalize ();
    Quaternion qInverse = q.conjugate();
    
    return ( q * p * qInverse ).xyz ();
}
static inline Vector3& operator*= ( Vector3& vec, const Quaternion& quat )
{
    vec = vec * quat;
    return vec;
}
