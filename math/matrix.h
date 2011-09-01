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
// FILE:        matrix.h
// PURPOSE:     Matrices.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#include <sstream>
#include <cmath>
#include <cstring>
#include "shared/platform.h"
#include "vector.h"

#define FAST_MATRIX_INVERSION


//------------------------------------------------------------------------------
//
// Special 3x3 matrix
//
class Matrix3
{
public:
    union
    {
        f32 v [ 9 ];
        f32 m [ 3 ][ 3 ];
    };
    
public:
    //--------------------------------------------------------------------------
    // Default constructor
    Matrix3 ( )
    {
    }
    
    //--------------------------------------------------------------------------
    // Constructor from a float vector
    Matrix3 ( const f32* vec )
    {
        operator= ( vec );
    }

    //--------------------------------------------------------------------------
    // Copy constructor
    Matrix3 ( const Matrix3& m )
    {
        operator=(m);
    }

    
    //--------------------------------------------------------------------------
    // Assignment operator
    Matrix3& operator= ( const Matrix3& m )
    {
        for ( u8 i = 0; i < 9; ++i )
            v[i] = m.v[i];
        return *this;
    }
    
    //--------------------------------------------------------------------------
    // Assignment operator, from a float vector
    Matrix3& operator= ( const float* vec )
    {
        for ( u8 i = 0; i < 9; ++i )
            v[i] = vec[i];
        return *this;
    }

    
    //--------------------------------------------------------------------------
    // Comparators
    bool operator== ( const Matrix3& m ) const
    {
        for ( u8 i = 0; i < 9; ++i )
            if ( v[i] != m.v[i] )
                return false;
        return true;
    }
    bool operator!= ( const Matrix3& m ) const
    {
        return !operator== ( m );
    }

    
    //--------------------------------------------------------------------------
    // Accessors
    f32*              vector  () { return v; }
    const f32*        vector  () const { return v; }
    
    //--------------------------------------------------------------------------
    // Convert this matrix to a string
    std::string toString () const
    {
        std::ostringstream os;
        for ( u8 i = 0; i < 3; ++i )
            os << m[0][i] << "\t" << m[1][i] << "\t" << m[2][i] << "\n";
        return os.str ();
    }

    
    //--------------------------------------------------------------------------
    // Operations

    //--------------------------------------------------------------------------
    // Determinant
    // | a b c |   
    // | d e f | = a*e*i + b*f*g + c*d*h - c*e*g - b*d*i - f*h*a
    // | g h i |   
    f32 Determinant () const
    {
        const f32 &a = m[0][0], &b = m[0][1], &c = m[0][2],
                  &d = m[1][0], &e = m[1][1], &f = m[1][2],
                  &g = m[2][0], &h = m[2][1], &i = m[2][2];
        return a*e*i + b*f*g + c*d*h - c*e*g - b*d*i - f*h*a;
    }
    
    //--------------------------------------------------------------------------
    // QR Decomposition, using Gram-Schmidt process, for the 3x3 submatrix.
    void QRDecompose ( Matrix3* Q, Matrix3* R ) const
    {
        return QRDecompose ( *this, Q, R );
    }
    
    static void QRDecompose ( const Matrix3& thiz, Matrix3* Q, Matrix3* R )
    {
        Vector3 a1 = Vector3 ( &thiz.m[0][0] );
        Vector3 a2 = Vector3 ( &thiz.m[1][0] );
        Vector3 a3 = Vector3 ( &thiz.m[2][0] );
        
        Vector3 u1 = a1;
        Vector3 e1 = Vector3::Normalize(u1);
        
        Vector3 u2 = a2 - e1*Dot(e1,a2);
        Vector3 e2 = Vector3::Normalize(u2);
        
        Vector3 u3 = a3 - e1*Dot(a3,e1) - e2*Dot(a3,e2);
        Vector3 e3 = Vector3::Normalize(u3);
        
        float m [ 16 ];
        m[0] = e1.x();  m[1] = e1.y();  m[2] = e1.z();
        m[3] = e2.x();  m[4] = e2.y();  m[5] = e2.z();
        m[6] = e3.x();  m[7] = e3.y();  m[8] = e3.z();
        *Q = m;
        
        memset ( m, 0, sizeof(m) );
        m[0] = e1.Dot(a1);
        m[3] = e1.Dot(a2); m[4] = e2.Dot(a2);
        m[6] = e1.Dot(a3); m[7] = e2.Dot(a3); m[8] = e3.Dot(a3);
        *R = m;
    }
    
    
    //--------------------------------------------------------------------------
    // Transposition
    Matrix3& Transpose ()
    {
        *this = Matrix3::Transpose(*this);
        return *this;
    }
    static Matrix3 Transpose ( const Matrix3& thiz )
    {
        Matrix3 ret;
        for ( u8 i = 0; i < 3; ++i )
            for ( u8 j = 0; j < 3; ++j )
                ret.m[i][j] = thiz.m[j][i];
        return ret;
    }
    
    //--------------------------------------------------------------------------
    // Inversion
    Matrix3& Invert ()
    {
        *this = Matrix3::Invert(*this);
        return *this;
    }
    static Matrix3 Invert ( const Matrix3& thiz )
    {
        Matrix3 ret;
#ifdef FAST_MATRIX_INVERSION
        ret = Transpose ( thiz );
#endif
        return ret;
    }
    
    //--------------------------------------------------------------------------
    // Load identity
    void LoadIdentity ()
    {
        for ( u8 i = 0; i < 3; ++i )
            for ( u8 j = 0; j < 3; ++j )
                m[i][j] = ( i==j ? 1.0f : 0.0f );
    }

    
    
    
    //--------------------------------------------------------------------------
    // Operations
    
    //--------------------------------------------------------------------------
    // M + M
    Matrix3 operator+ ( const Matrix3& m ) const
    {
        Matrix3 ret;
        for ( u8 i = 0; i < 9; ++i )
            ret.v[i] = v[i] + m.v[i];
        return ret;
    }
    Matrix3& operator+= ( const Matrix3& m )
    {
        for ( u8 i = 0; i < 9; ++i )
            v[i] += m.v[i];
        return *this;
    }
    
    //--------------------------------------------------------------------------
    // M - M
    Matrix3 operator-( const Matrix3& m ) const
    {
        Matrix3 ret;
        for ( u8 i = 0; i < 9; ++i )
            ret.v[i] = v[i] - m.v[i];
        return ret;
    }
    Matrix3& operator-= ( const Matrix3& m )
    {
        for ( u8 i = 0; i < 9; ++i )
            v[i] -= m.v[i];
        return *this;
    }
    
    //--------------------------------------------------------------------------
    // M*M
    Matrix3 operator* ( const Matrix3& Right ) const
    {
        Matrix3 ret;
        for ( u8 i = 0; i < 9; ++i )
            ret.v[i] = 0;
        
        f32 elem;
        for ( u8 i = 0; i < 3; ++i )
        {
            for ( u8 k = 0; k < 3; ++k )
            {
                elem = Right.m[i][k];
                for ( u8 j = 0; j < 3; ++j )
                {
                    ret.m[i][j] += elem * this->m[k][j];
                }
            }
        }
        
        return ret;
    }
    Matrix3& operator*= ( const Matrix3& Right )
    {
        *this = *this * Right;
        return *this;
    }
    
    //--------------------------------------------------------------------------
    // M + k
    Matrix3 operator+ ( float s ) const
    {
        Matrix3 ret;
        for ( u8 i = 0; i < 9; ++i )
            ret.v[i] = v[i] + s;
        return ret;
    }
    Matrix3& operator+= ( float s )
    {
        for ( u8 i = 0; i < 9; ++i )
            v[i] += s;
        return *this;
    }
    
    //--------------------------------------------------------------------------
    // M - k
    Matrix3 operator- ( float s ) const
    {
        Matrix3 ret;
        for ( u8 i = 0; i < 9; ++i )
            ret.v[i] = v[i] - s;
        return ret;
    }
    Matrix3& operator-= ( float s )
    {
        for ( u8 i = 0; i < 9; ++i )
            v[i] -= s;
        return *this;
    }
    
    //--------------------------------------------------------------------------
    // M * k
    Matrix3 operator* ( float s ) const
    {
        Matrix3 ret;
        for ( u8 i = 0; i < 9; ++i )
            ret.v[i] = v[i] * s;
        return ret;
    }
    Matrix3& operator*= ( float s )
    {
        for ( u8 i = 0; i < 9; ++i )
            v[i] *= s;
        return *this;
    }
    
    //--------------------------------------------------------------------------
    // M / k
    Matrix3 operator/ ( float s ) const
    {
        Matrix3 ret;
        for ( u8 i = 0; i < 9; ++i )
            ret.v[i] = v[i] / s;
        return ret;
    }
    Matrix3& operator/= ( float s )
    {
        for ( u8 i = 0; i < 9; ++i )
            v[i] /= s;
        return *this;
    }
    
    //--------------------------------------------------------------------------
    // M^i
    Matrix3 operator^( i32 i ) const
    {
        Matrix3 ret ( *this );
        
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
    
    //--------------------------------------------------------------------------
    // M*V
    Vector3 operator* ( const Vector3& v ) const
    {
        float res[3] = { 0, 0, 0 };
        
        const f32* vector = v.vector();
        for ( u8 i = 0; i < 3; ++i )
        {
            const f32& elem = vector[i];
            for ( u8 k = 0; k < 3; ++k )
                res[k] += m[i][k] * elem;
        }
        
        return Vector3(res);
    }
};






//------------------------------------------------------------------------------
//
// Default 4x4 matrix
//
class Matrix
{
public:
    union
    {
        f32 v [ 16 ];
        f32 m [ 4 ][ 4 ];
    };
    
public:
    //--------------------------------------------------------------------------
    // Default constructor
    Matrix ( )
    {
    }
    
    //--------------------------------------------------------------------------
    // Constructor from a float vector
    Matrix ( const f32* vec )
    {
        operator= ( vec );
    }

    //--------------------------------------------------------------------------
    // Copy constructor
    Matrix ( const Matrix& m )
    {
        operator=(m);
    }
    
    //--------------------------------------------------------------------------
    // Copy from a 3x3 Matrix
    Matrix ( const Matrix3& m )
    {
        operator=(m);
    }

    
    //--------------------------------------------------------------------------
    // Assignment operator
    Matrix& operator= ( const Matrix& m )
    {
        for ( u8 i = 0; i < 16; ++i )
            v[i] = m.v[i];
        return *this;
    }
    
    //--------------------------------------------------------------------------
    // Assignment operator, from a float vector
    Matrix& operator= ( const float* vec )
    {
        for ( u8 i = 0; i < 16; ++i )
            v[i] = vec[i];
        return *this;
    }

    //--------------------------------------------------------------------------
    // Assignment operator, from a 3x3 matrix
    Matrix& operator= ( const Matrix3& m )
    {
        v[0]  = m.v[0];
        v[1]  = m.v[1];
        v[2]  = m.v[2];
        v[3]  = 0.0f;
        v[4]  = m.v[3];
        v[5]  = m.v[4];
        v[6]  = m.v[5];
        v[7]  = 0.0f;
        v[8]  = m.v[6];
        v[9]  = m.v[7];
        v[10] = m.v[8];
        v[11] = 0.0f;
        v[12] = 0.0f;
        v[13] = 0.0f;
        v[14] = 0.0f;
        v[15] = 1.0f;
        return *this;
    }
    
    //--------------------------------------------------------------------------
    // Comparators
    bool operator== ( const Matrix& m ) const
    {
        for ( u8 i = 0; i < 16; ++i )
            if ( v[i] != m.v[i] )
                return false;
        return true;
    }
    bool operator!= ( const Matrix& m ) const
    {
        return !operator== ( m );
    }

    
    //--------------------------------------------------------------------------
    // Accessors
    f32*              vector  () { return v; }
    const f32*        vector  () const { return v; }
    
    //--------------------------------------------------------------------------
    // Convert this matrix to a string
    std::string toString () const
    {
        std::ostringstream os;
        for ( u8 i = 0; i < 4; ++i )
            os << m[0][i] << "\t" << m[1][i] << "\t" << m[2][i] << "\t" << m[3][i] << "\n";
        return os.str ();
    }
    
    //--------------------------------------------------------------------------
    // Assign3x3
    void Assign3x3 ( const Matrix& m )
    {
        for ( u8 i = 0; i < 3; ++i )
            for ( u8 j = 0; j < 3; ++j )
                this->m[i][j] = m.m[i][j];
    }
        

    
    //--------------------------------------------------------------------------
    // Operations

    //--------------------------------------------------------------------------
    // Determinant
    // | a b c d |       | f g h |       | e g h |       | e f h |       | e f g |
    // | e f g h | = a * | j k l | - b * | i k l | + c * | i j l | - d * | i j k |
    // | i j k l |       | o p q |       | n p q |       | n o q |       | n o p |
    // | n o p q |
    f32 Determinant () const
    {
        const f32 &a = m[0][0], &b = m[0][1], &c = m[0][2], &d = m[0][3],
                  &e = m[1][0], &f = m[1][1], &g = m[1][2], &h = m[1][3],
                  &i = m[2][0], &j = m[2][1], &k = m[2][2], &l = m[2][3],
                  &n = m[3][0], &o = m[3][1], &p = m[3][2], &q = m[3][3];
        f32 det1 = f*k*q + g*l*o + h*j*p - h*k*o - g*j*q - f*l*p;
        f32 det2 = e*k*q + g*l*n + h*i*p - h*k*n - g*i*q - e*l*p;
        f32 det3 = e*j*q + f*l*n + h*i*o - h*j*n - f*i*q - e*l*o;
        f32 det4 = e*j*p + f*k*n + g*i*o - g*j*n - g*i*p - e*k*o;
        return a*det1 - b*det2 + c*det3 - d*det4;
    }
    
    //--------------------------------------------------------------------------
    // QR Decomposition, using Gram-Schmidt process, for the 3x3 submatrix.
    void QRDecompose ( Matrix* Q, Matrix* R ) const
    {
        return QRDecompose ( *this, Q, R );
    }
    
    static void QRDecompose ( const Matrix& thiz, Matrix* Q, Matrix* R )
    {
        Vector3 a1 = Vector3 ( &thiz.m[0][0] );
        Vector3 a2 = Vector3 ( &thiz.m[1][0] );
        Vector3 a3 = Vector3 ( &thiz.m[2][0] );
        
        Vector3 u1 = a1;
        Vector3 e1 = Vector3::Normalize(u1);
        
        Vector3 u2 = a2 - e1*Dot(e1,a2);
        Vector3 e2 = Vector3::Normalize(u2);
        
        Vector3 u3 = a3 - e1*Dot(a3,e1) - e2*Dot(a3,e2);
        Vector3 e3 = Vector3::Normalize(u3);
        
        float m [ 16 ];
        m[0] = e1.x();  m[1] = e1.y();  m[2] = e1.z();  m[3] = 0.0f;
        m[4] = e2.x();  m[5] = e2.y();  m[6] = e2.z();  m[7] = 0.0f;
        m[8] = e3.x();  m[9] = e3.y();  m[10] = e3.z(); m[11] = 0.0f;
        m[12] = 0.0f;   m[13] = 0.0f;   m[14] = 0.0f;   m[15] = 1.0f;
        *Q = m;
        
        memset ( m, 0, sizeof(m) );
        m[0] = e1.Dot(a1);
        m[4] = e1.Dot(a2); m[5] = e2.Dot(a2);
        m[8] = e1.Dot(a3); m[9] = e2.Dot(a3); m[10] = e3.Dot(a3);
        m[15] = 1.0f;
        *R = m;
    }
    
    
    //--------------------------------------------------------------------------
    // Transposition
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
    
    //--------------------------------------------------------------------------
    // Inversion
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
        
        ret.m[0][3] = 0.0f;
        ret.m[1][3] = 0.0f;
        ret.m[2][3] = 0.0f;
        ret.m[3][3] = 1.0f;
#endif
        return ret;
    }
    
    //--------------------------------------------------------------------------
    // Load identity
    void LoadIdentity ()
    {
        for ( u8 i = 0; i < 4; ++i )
            for ( u8 j = 0; j < 4; ++j )
                m[i][j] = ( i==j ? 1.0f : 0.0f );
    }

    
    
    
    //--------------------------------------------------------------------------
    // Operations
    
    //--------------------------------------------------------------------------
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
    
    //--------------------------------------------------------------------------
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
    
    //--------------------------------------------------------------------------
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

    
    //--------------------------------------------------------------------------
    // M + k
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
    
    //--------------------------------------------------------------------------
    // M - k
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
    
    //--------------------------------------------------------------------------
    // M * k
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
    
    //--------------------------------------------------------------------------
    // M / k
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
    
    //--------------------------------------------------------------------------
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
    
    //--------------------------------------------------------------------------
    // M*V
    Vector3 operator* ( const Vector3& v ) const
    {
        float res[3] = { 0, 0, 0 };
        
        const f32* vector = v.vector();
        for ( u8 i = 0; i < 3; ++i )
        {
            const f32& elem = vector[i];
            for ( u8 k = 0; k < 3; ++k )
                res[k] += m[i][k] * elem;
            // Assume that the vector 4th component is 1
            res[i] += m[3][i];
        }
        
        return Vector3(res);
    }
};




//------------------------------------------------------------------------------
//
// Utility matrices
//

//------------------------------------------------------------------------------
// Identity matrix
class IdentityMatrix : public Matrix
{
public:
    IdentityMatrix ()
    {
        static f32 const s_identity[] = { 1.0f, 0.0f, 0.0f, 0.0f,
                                          0.0f, 1.0f, 0.0f, 0.0f,
                                          0.0f, 0.0f, 1.0f, 0.0f,
                                          0.0f, 0.0f, 0.0f, 1.0f };
        Matrix::operator= ( s_identity );
    }
};

//------------------------------------------------------------------------------
// 3x3 identity matrix
class IdentityMatrix3 : public Matrix3
{
public:
    IdentityMatrix3 ()
    {
        static f32 const s_identity[] = { 1.0f, 0.0f, 0.0f,
                                          0.0f, 1.0f, 0.0f,
                                          0.0f, 0.0f, 1.0f };
        Matrix3::operator= ( s_identity );
    }
};

//------------------------------------------------------------------------------
// Translation matrix
class TranslationMatrix : public Matrix
{
public:
    TranslationMatrix ( f32 x, f32 y, f32 z )
    {
        f32 v [ 16 ] = { 1.0f, 0.0f, 0.0f, 0.0f,
                         0.0f, 1.0f, 0.0f, 0.0f,
                         0.0f, 0.0f, 1.0f, 0.0f,
                         x, y, z, 1.0f };
        Matrix::operator= ( v );
    }
    
    TranslationMatrix ( f32* v )
    {
        *this = TranslationMatrix ( v[0], v[1], v[2] );
    }
};

//------------------------------------------------------------------------------
// Scaling matrix
class ScalingMatrix : public Matrix
{
public:
    ScalingMatrix ( f32 scale )
    {
        f32 v [ 16 ] = { scale, 0.0f, 0.0f, 0.0f,
                         0.0f, scale, 0.0f, 0.0f,
                         0.0f, 0.0f, scale, 0.0f,
                         0.0f, 0.0f, 0.0f, 1.0f };
        Matrix::operator= ( v );
    }
    
    ScalingMatrix ( f32 x, f32 y, f32 z )
    {
        f32 v [ 16 ] = { x, 0.0f, 0.0f, 0.0f,
                         0.0f, y, 0.0f, 0.0f,
                         0.0f, 0.0f, z, 0.0f,
                         0.0f, 0.0f, 0.0f, 1.0f };
        Matrix::operator= ( v );
    }
    
    ScalingMatrix ( f32* v )
    {
        Matrix::operator= ( ScalingMatrix ( v[0], v[1], v[2] ) );
    }
};

//------------------------------------------------------------------------------
// Rotation matrix, using 3x3 matrices
class RotationMatrix : public Matrix3
{
public:
    RotationMatrix ( f32 angle, f32 x, f32 y, f32 z )
    {
        f32 v [ 16 ];
        f32 c = cos(angle);
        f32 s = sin(angle);
        f32 norm = sqrt(x*x+y*y+z*z);
        x /= norm;
        y /= norm;
        z /= norm;
        
        v[0] = x*x*(1-c)+c;   v[1] = y*x*(1-c)+z*s; v[2] = x*z*(1-c)-y*s;
        v[3] = x*y*(1-c)-z*s; v[4] = y*y*(1-c)+c;   v[5] = y*z*(1-c)+x*s;
        v[6] = x*z*(1-c)+y*s; v[7] = y*z*(1-c)-x*s; v[8] = z*z*(1-c)+c;
        
        Matrix3::operator= ( v );
    }
    
    RotationMatrix ( const EulerAngles& euler )
    {
        Matrix3::operator= ( RotationMatrix ( euler.heading(), 0.0f, 1.0f, 0.0f ) *
                             RotationMatrix ( euler.pitch(),   1.0f, 0.0f, 0.0f ) *
                             RotationMatrix ( euler.bank(),    0.0f, 0.0f, 1.0f ) );
    }
};

//------------------------------------------------------------------------------
// Orthographic matrix
class OrthographicMatrix : public Matrix
{
public:
    OrthographicMatrix ( f32 left, f32 right, f32 top, f32 bottom, f32 near, f32 far )
    {
        const f32& l = left; const f32& r = right;
        const f32& t = top; const f32& b = bottom;
        const f32& n = near; const f32& f = far;
        f32 v [ 16 ] =
        {
            2.0f/(r-l), 0.0f, 0.0f, 0.0f,
            0.0f, 2.0f/(t-b), 0.0f, 0.0f,
            0.0f, 0.0f, 2.0f/(n-f), 0.0f,
            -(r+l)/(r-l), -(t+b)/(t-b), -(n+f)/(n-f), 1.0f
        };
        
        Matrix::operator= ( v );
    }
};

//------------------------------------------------------------------------------
// Perspective matrix
class PerspectiveMatrix : public Matrix
{
public:
    PerspectiveMatrix ( f32 fovy, f32 aspect, f32 near, f32 far )
    {
        f32 f = 1.0f / tan ( fovy*0.5f );
        f32 v [ 16 ] =
        {
            f/aspect,   0.0f,   0.0f,                   0.0f,
            0.0f,       f,      0.0f,                   0.0f,
            0.0f,       0.0f,   (far+near)/(near-far),  -1.0f,
            0.0f,       0.0f,   2*far*near/(near-far),  0.0f
        };
        
        Matrix::operator= ( v );
    }
};

//------------------------------------------------------------------------------
// Normals matrix
class MatrixForNormals : public Matrix
{
public:
    MatrixForNormals ( const Matrix& mat )
    {
        Matrix::operator= ( IdentityMatrix() );
        Matrix::Assign3x3( mat );
        Matrix::Invert();
        Matrix::Transpose();
    }
};
