#ifndef MATRIX_H
#define	MATRIX_H

#include <cstring>
#include "shared/platform.h"
#include "vector.h"

#define FAST_MATRIX_INVERSION

class Matrix
{
private:
    union
    {
        f32 v [ 16 ];
        f32 m [ 4 ][ 4 ];
    };
    
public:
    Matrix ( )
    {
        LoadIdentity ();
    }
    
    f32*              vector  () { return v; }
    const f32*        vector  () const { return v; }
    
    // Copy, assignment, compare.
    Matrix ( const Matrix& m )
    {
        operator=(m);
    }
    Matrix& operator= ( const Matrix& m )
    {
        memcpy ( v, m.v, sizeof(m.v) );
        return *this;
    }
    
    bool operator== ( const Matrix& m ) const
    {
        return memcmp ( v, m.v, sizeof(m.v) ) == 0;
    }
    bool operator!= ( const Matrix& m ) const
    {
        return memcmp ( v, m.v, sizeof(m.v) ) != 0;
    }
    
    // Operations
    Matrix& Transpose ()
    {
        *this = Matrix::Transpose(*this);
        return *this;
    }
    static Matrix Transpose ( const Matrix& thiz )
    {
        Matrix ret;
        for ( u8 i = 0; i < 4; ++i )
            for ( u8 j = 0; j < 4; ++j )
                ret.m[i][j] = thiz.m[j][i];
        return ret;
    }
    
    Matrix& Invert ()
    {
        *this = Matrix::Invert(*this);
        return *this;
    }
    static Matrix Invert ( const Matrix& thiz )
    {
        Matrix ret;
#ifdef FAST_MATRIX_INVERSION
        for ( u8 i = 0; i < 3; ++i )
            for ( u8 j = 0; j < 3; ++j )
                ret.m[i][j] = thiz.m[j][i];
        for ( u8 i = 0; i < 3; ++i )
            ret.m[3][i] = -thiz.m[3][i];
#endif
        return ret;
    }
    
    void LoadIdentity ()
    {
        for ( u8 i = 0; i < 4; ++i )
            for ( u8 j = 0; j < 4; ++j )
                m[i][j] = ( i==j ? 1.0f : 0.0f );
    }
    
    // M + M
    Matrix operator+ ( const Matrix& m ) const
    {
        Matrix ret;
        for ( u8 i = 0; i < 16; ++i )
            ret.v[i] = v[i] + m.v[i];
        return ret;
    }
    Matrix& operator+= ( const Matrix& m )
    {
        for ( u8 i = 0; i < 16; ++i )
            v[i] += m.v[i];
        return *this;
    }
    
    // M - M
    Matrix operator-( const Matrix& m ) const
    {
        Matrix ret;
        for ( u8 i = 0; i < 16; ++i )
            ret.v[i] = v[i] - m.v[i];
        return ret;
    }
    Matrix& operator-= ( const Matrix& m )
    {
        for ( u8 i = 0; i < 16; ++i )
            v[i] -= m.v[i];
        return *this;
    }
    
    // M*M
    Matrix operator* ( const Matrix& Right ) const
    {
        Matrix ret;
        for ( u8 i = 0; i < 16; ++i )
            ret.v[i] = 0;
        
        f32 elem;
        for ( u8 i = 0; i < 4; ++i )
        {
            for ( u8 k = 0; k < 4; ++k )
            {
                elem = Right.m[i][k];
                for ( u8 j = 0; j < 4; ++j )
                {
                    ret.m[i][j] += elem * this->m[k][j];
                }
            }
        }
        
        return ret;
    }
    Matrix& operator*= ( const Matrix& m )
    {
        Matrix ret = operator*(m);
        *this = ret;
        return *this;
    }
    
    // M + s
    Matrix operator+ ( float s ) const
    {
        Matrix ret;
        for ( u8 i = 0; i < 16; ++i )
            ret.v[i] = v[i] + s;
        return ret;
    }
    Matrix& operator+= ( float s )
    {
        for ( u8 i = 0; i < 16; ++i )
            v[i] += s;
        return *this;
    }
    
    // M - s
    Matrix operator- ( float s ) const
    {
        Matrix ret;
        for ( u8 i = 0; i < 16; ++i )
            ret.v[i] = v[i] - s;
        return ret;
    }
    Matrix& operator-= ( float s )
    {
        for ( u8 i = 0; i < 16; ++i )
            v[i] -= s;
        return *this;
    }
    
    // M * s
    Matrix operator* ( float s ) const
    {
        Matrix ret;
        for ( u8 i = 0; i < 16; ++i )
            ret.v[i] = v[i] * s;
        return ret;
    }
    Matrix& operator*= ( float s )
    {
        for ( u8 i = 0; i < 16; ++i )
            v[i] *= s;
        return *this;
    }
    
    // M / s
    Matrix operator/ ( float s ) const
    {
        Matrix ret;
        for ( u8 i = 0; i < 16; ++i )
            ret.v[i] = v[i] / s;
        return ret;
    }
    Matrix& operator/= ( float s )
    {
        for ( u8 i = 0; i < 16; ++i )
            v[i] /= s;
        return *this;
    }
    
    // M^i
    Matrix operator^( i32 i ) const
    {
        Matrix ret ( *this );
        
        if ( i == 0 )
            ret.LoadIdentity ();
        else
        {
            if ( i < 0 )
            {
                ret.Invert();
                i = -i;
            }
            while ( --i > 0 )
                ret *= ret;
        }
        
        return ret;
    }
    
    // M*V
    Vector3 operator* ( const Vector3& v ) const
    {
        float res[3] = { 0, 0, 0 };
        
        f32 elem;
        for ( u8 i = 0; i < 3; ++i )
        {
            elem = v.vector()[i];
            for ( u8 k = 0; k < 3; ++k )
                res[k] += m[i][k] * elem;
        }
        
        return Vector3(res);
    }
};

#endif	/* MATRIX_H */

