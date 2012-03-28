/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
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
#include "shared/util.h"
#include "vector.h"
#include "quaternion.h"
#include "qtransform.h"

#undef FAST_MATRIX_INVERSION


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
    f32 determinant () const
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
        float m [ 16 ];

        Vector3 a1 = Vector3 ( &thiz.m[0][0] );
        Vector3 a2 = Vector3 ( &thiz.m[1][0] );
        Vector3 a3 = Vector3 ( &thiz.m[2][0] );
        
        Vector3 u1 = a1;
        Vector3 e1 = Vector3::normalize(u1);
        
        Vector3 u2 = a2 - e1*dot(e1,a2);
        Vector3 e2 = Vector3::normalize(u2);
        
        Vector3 u3 = a3 - e1*dot(a3,e1) - e2*dot(a3,e2);
        Vector3 e3 = Vector3::normalize(u3);
        
        if ( Q )
        {
            m[0] = e1.x();  m[1] = e1.y();  m[2] = e1.z();
            m[3] = e2.x();  m[4] = e2.y();  m[5] = e2.z();
            m[6] = e3.x();  m[7] = e3.y();  m[8] = e3.z();
            *Q = m;
        }

        if ( R )
        {
            memset ( m, 0, sizeof(m) );
            m[0] = e1.dot(a1);
            m[3] = e1.dot(a2); m[4] = e2.dot(a2);
            m[6] = e1.dot(a3); m[7] = e2.dot(a3); m[8] = e3.dot(a3);
            *R = m;
        }
    }
    
    
    //--------------------------------------------------------------------------
    // Transposition
    Matrix3& transpose ()
    {
        *this = Matrix3::transpose(*this);
        return *this;
    }
    static Matrix3 transpose ( const Matrix3& thiz )
    {
        Matrix3 ret;
        for ( u8 i = 0; i < 3; ++i )
            for ( u8 j = 0; j < 3; ++j )
                ret.m[i][j] = thiz.m[j][i];
        return ret;
    }
    
    //--------------------------------------------------------------------------
    // Inversion
    Matrix3& invert ()
    {
        *this = Matrix3::invert(*this);
        return *this;
    }
    static Matrix3 invert ( const Matrix3& thiz )
    {
        Matrix3 ret;
#ifdef FAST_MATRIX_INVERSION
        ret = transpose ( thiz );
#else
        // Use Gauss-Jordan
        f32 (&inverse)[3][3] = ret.m;
        f32 mat[3][3];
        
        // Load the identity in the inverse
        for ( u32 i = 0; i < 3; ++i )
            for ( u32 j = 0; j < 3; ++j )
                inverse[i][j] = 0.0f;
        for ( u32 i = 0; i < 3; ++i )
            inverse[i][i] = 1.0f;
        
        // Copy the original matrix
        for ( u32 i = 0; i < 3; ++i )
            for ( u32 j = 0; j < 3; ++j )
                mat[i][j] = thiz.m[i][j];
        
        // Do the elimination for each columns
        for ( u32 i = 0; i < 3; ++i )
        {
            // Find a non-zero pivot below the current pivot
            if ( mat[i][i] == 0.0f )
            {
                u32 pivotRow = i;
                for ( u32 j = i + 1; j < 3; ++i )
                {
                    if ( mat[j][i] != 0.0f )
                    {
                        pivotRow = j;
                        break;
                    }
                }
                
                // Check if non invertible
                if ( pivotRow == i )
                    return thiz;
                
                // Swap rows
                for ( u32 j = 0; j < 3; ++i )
                {
                    Swap ( mat[i][j], mat[pivotRow][j] );
                    Swap ( inverse[i][j], inverse[pivotRow][j] );
                }
            }
            
            // Divide the pivot row by the pivot
            f32 pivot = mat[i][i];
            for ( u32 j = 0; j < 3; ++j )
            {
                mat[i][j] /= pivot;
                inverse[i][j] /= pivot;
            }
            
            // Reduce all other rows
            for ( u32 j = 0; j < 3; ++j )
            {
                if ( i != j )
                {
                    f32 factor = mat[j][i];
                    for ( u32 k = 0; k < 3; ++k )
                    {
                        mat[j][k] = mat[j][k] - mat[i][k] * factor;
                        inverse[j][k] = inverse[j][k] - inverse[i][k] * factor;
                    }
                }
            }
        }
#endif
        return ret;
    }
    
    //--------------------------------------------------------------------------
    // Load identity
    void loadIdentity ()
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
            ret.loadIdentity ();
        else
        {
            if ( i < 0 )
            {
                ret.invert();
                i = -i;
            }
            while ( --i > 0 )
                ret *= ret;
        }
        
        return ret;
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
    void assign3x3 ( const Matrix& m )
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
    f32 determinant () const
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
        float m [ 16 ];
        
        Vector3 a1 = Vector3 ( &thiz.m[0][0] );
        Vector3 a2 = Vector3 ( &thiz.m[1][0] );
        Vector3 a3 = Vector3 ( &thiz.m[2][0] );
        
        Vector3 u1 = a1;
        Vector3 e1 = Vector3::normalize(u1);
        
        Vector3 u2 = a2 - e1*dot(e1,a2);
        Vector3 e2 = Vector3::normalize(u2);
        
        Vector3 u3 = a3 - e1*dot(a3,e1) - e2*dot(a3,e2);
        Vector3 e3 = Vector3::normalize(u3);
        
        if ( Q )
        {
            m[0] = e1.x();  m[1] = e1.y();  m[2] = e1.z();  m[3] = 0.0f;
            m[4] = e2.x();  m[5] = e2.y();  m[6] = e2.z();  m[7] = 0.0f;
            m[8] = e3.x();  m[9] = e3.y();  m[10] = e3.z(); m[11] = 0.0f;
            m[12] = 0.0f;   m[13] = 0.0f;   m[14] = 0.0f;   m[15] = 1.0f;
            *Q = m;
        }
        
        if ( R )
        {
            memset ( m, 0, sizeof(m) );
            m[0] = e1.dot(a1);
            m[4] = e1.dot(a2); m[5] = e2.dot(a2);
            m[8] = e1.dot(a3); m[9] = e2.dot(a3); m[10] = e3.dot(a3);
            m[15] = 1.0f;
            *R = m;
        }
    }
    
    
    //--------------------------------------------------------------------------
    // Transposition
    Matrix& transpose ()
    {
        *this = Matrix::transpose(*this);
        return *this;
    }
    static Matrix transpose ( const Matrix& thiz )
    {
        Matrix ret;
        for ( u8 i = 0; i < 4; ++i )
            for ( u8 j = 0; j < 4; ++j )
                ret.m[i][j] = thiz.m[j][i];
        return ret;
    }
    
    //--------------------------------------------------------------------------
    // Inversion
    Matrix& invert ()
    {
        *this = Matrix::invert(*this);
        return *this;
    }
    static Matrix invert ( const Matrix& thiz )
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
#else
        // Use Gauss-Jordan
        f32 (&inverse)[4][4] = ret.m;
        f32 mat[4][4];
        
        // Load the identity in the inverse
        for ( u32 i = 0; i < 4; ++i )
            for ( u32 j = 0; j < 4; ++j )
                inverse[i][j] = 0.0f;
        for ( u32 i = 0; i < 4; ++i )
            inverse[i][i] = 1.0f;
        
        // Copy the original matrix
        for ( u32 i = 0; i < 4; ++i )
            for ( u32 j = 0; j < 4; ++j )
                mat[i][j] = thiz.m[i][j];
        
        // Do the elimination for each columns
        for ( u32 i = 0; i < 4; ++i )
        {
            // Find a non-zero pivot below the current pivot
            if ( mat[i][i] == 0.0f )
            {
                u32 pivotRow = i;
                for ( u32 j = i + 1; j < 4; ++i )
                {
                    if ( mat[j][i] != 0.0f )
                    {
                        pivotRow = j;
                        break;
                    }
                }
                
                // Check if non invertible
                if ( pivotRow == i )
                    return thiz;
                
                // Swap rows
                for ( u32 j = 0; j < 4; ++j )
                {
                    Swap ( mat[i][j], mat[pivotRow][j] );
                    Swap ( inverse[i][j], inverse[pivotRow][j] );
                }
            }
            
            // Divide the pivot row by the pivot
            f32 pivot = mat[i][i];
            for ( u32 j = 0; j < 4; ++j )
            {
                mat[i][j] /= pivot;
                inverse[i][j] /= pivot;
            }
            
            // Reduce all other rows
            for ( u32 j = 0; j < 4; ++j )
            {
                if ( i != j )
                {
                    f32 factor = mat[j][i];
                    for ( u32 k = 0; k < 4; ++k )
                    {
                        mat[j][k] = mat[j][k] - mat[i][k] * factor;
                        inverse[j][k] = inverse[j][k] - inverse[i][k] * factor;
                    }
                }
            }
        }
#endif
        return ret;
    }
    
    //--------------------------------------------------------------------------
    // Load identity
    void loadIdentity ()
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
            ret.loadIdentity ();
        else
        {
            if ( i < 0 )
            {
                ret.invert();
                i = -i;
            }
            while ( --i > 0 )
                ret *= ret;
        }
        
        return ret;
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
    
    ScalingMatrix ( const Vector3& vec )
    {
        Matrix::operator= ( ScalingMatrix ( vec.x(), vec.y(), vec.z() ) );
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
        Matrix3::operator= ( RotationMatrix ( euler.heading(), 0.0f, 0.0f, 1.0f ) *
                             RotationMatrix ( euler.pitch(),   1.0f, 0.0f, 0.0f ) *
                             RotationMatrix ( euler.bank(),    0.0f, 1.0f, 0.0f ) );
    }
};

//------------------------------------------------------------------------------
// Orthographic matrix
class OrthographicMatrix : public Matrix
{
public:
    OrthographicMatrix ( f32 left, f32 right, f32 top, f32 bottom, f32 pNear, f32 pFar )
    {
        const f32& l = left; const f32& r = right;
        const f32& t = top; const f32& b = bottom;
        const f32& n = pNear; const f32& f = pFar;
        f32 v [ 16 ] =
        {
            2.0f/(r-l),         0.0f,           0.0f,           0.0f,
            0.0f,               2.0f/(f-n),     0.0f,           0.0f,
            0.0f,               0.0f,           2.0f/(t-b),     0.0f,
            -(r+l)/(r-l),       -(f+n)/(f-n),   -(t+b)/(t-b),   1.0f
        };
        
        Matrix::operator= ( v );
    }
};

//------------------------------------------------------------------------------
// Perspective matrix
class PerspectiveMatrix : public Matrix
{
public:
    PerspectiveMatrix ( f32 aspect, f32 fovy, f32 pNear, f32 pFar )
    {
        f32 f = 1.0f / tan ( fovy*0.5f );
        f32 v [ 16 ] =
        {
            f/aspect,   0.0f,                           0.0f,           0.0f,
            0.0f,       (pFar+pNear)/(pFar-pNear),      0.0f,           1.0f,
            0.0f,       0.0f,                           f,              0.0f,
            0.0f,       2*pFar*pNear/(pFar-pNear),      0.0f,           0.0f
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
        Matrix::assign3x3( mat );
        Matrix::invert();
        Matrix::transpose();
    }
};

//------------------------------------------------------------------------------
// Lookat matrix
class LookatMatrix : public Matrix
{
public:
    LookatMatrix ( const Vector3& eye, const Vector3& target, const Vector3& up_ )
    {
        Vector3 forward;
        Vector3 side;
        
        Vector3 up = up_;
        up.normalize ();
        
	forward = target - eye;
	forward.normalize ();
        
        f32 match = forward.dot ( up );
        if ( fabs(fabs(match) - 1.0f) < 0.001f )
        {
            // Up and forward vector are parallel
            up = Vector3 ( 0.0f, -match, 0.0f );
            up.normalize ();
        }

        side = forward.cross(up);
        side.normalize();
                
	up = side.cross(forward);
        
        Vector3 t ( -eye.dot(side), -eye.dot(forward), -eye.dot(up) );

        f32 m [ 16 ] = {
            side.x(),           forward.x(),            up.x(),         0.0f,
            side.y(),           forward.y(),            up.y(),         0.0f,
            side.z(),           forward.z(),            up.z(),         0.0f,
            t.x(),              t.y(),                  t.z(),          1.0f
        };
        
        Matrix::operator= ( m );
    }
    
    LookatMatrix ( const Matrix3& orientation, const Vector3& eye )
    {
        const f32* col0 = &orientation.m[0][0];
        const f32* col1 = &orientation.m[1][0];
        const f32* col2 = &orientation.m[2][0];
        
        Vector3 t ( -eye.dot(col0), -eye.dot(col1), -eye.dot(col2) );
        
        f32 m [ 16 ] = {
            col0[0], col1[0], col2[0], 0.0f,
            col0[1], col1[1], col2[1], 0.0f,
            col0[2], col1[2], col2[2], 0.0f,
            t.x(),   t.y(),   t.z(),   1.0f
        };
        
        Matrix::operator= ( m );
    }
};

//------------------------------------------------------------------------------
// Matrix3 * Matrix4
static inline Matrix operator* ( const Matrix3& Left, const Matrix& Right )
{
    IdentityMatrix ret;
    ret.assign3x3( Left );
    return ret * Right;
}

//------------------------------------------------------------------------------
// Vector3 * Matrix
static inline Vector3 operator* ( const Vector3& v, const Matrix3& mat )
{
    float res[3] = { 0, 0, 0 };

    const f32* vector = v.vector();
    for ( u8 i = 0; i < 3; ++i )
    {
        const f32& elem = vector[i];
        for ( u8 k = 0; k < 3; ++k )
            res[k] += mat.m[i][k] * elem;
    }

    return Vector3(res);
}
static inline Vector3& operator*= ( Vector3& v, const Matrix3& mat )
{
    v = v * mat;
    return v;
}
static inline Vector3 operator* ( const Matrix3& mat, const Vector3& v )
{
    float res[3] = { 0, 0, 0 };

    const f32* vector = v.vector();
    for ( u8 i = 0; i < 3; ++i )
    {
        const f32& elem = vector[i];
        for ( u8 k = 0; k < 3; ++k )
            res[k] += mat.m[k][i] * elem;
    }

    return Vector3(res);
}


static inline Vector3 operator* ( const Vector3& v, const Matrix& mat )
{
    float res[3] = { 0, 0, 0 };

    const f32* vector = v.vector();
    for ( u8 i = 0; i < 3; ++i )
    {
        const f32& elem = vector[i];
        for ( u8 k = 0; k < 3; ++k )
            res[k] += mat.m[i][k] * elem;
        // Assume that the vector 4th component is 1
        res[i] += mat.m[3][i];
    }

    return Vector3(res);
}
static inline Vector3& operator*= ( Vector3& v, const Matrix& mat )
{
    v = v * mat;
    return v;
}
static inline Vector3 operator* ( const Matrix& mat, const Vector3& v )
{
    float res[3] = { 0, 0, 0 };

    const f32* vector = v.vector();
    for ( u8 i = 0; i < 3; ++i )
    {
        const f32& elem = vector[i];
        for ( u8 k = 0; k < 3; ++k )
            res[k] += mat.m[i][k] * elem;
        // Assume that the vector 4th component is 1
        res[i] += mat.m[3][i];
    }

    return Vector3(res);
}


//------------------------------------------------------------------------------
// Vector4 * Matrix
static inline Vector4 operator* ( const Vector4& v, const Matrix& mat )
{
    float res[4] = { 0, 0, 0, 0 };

    const f32* vector = v.vector();
    for ( u8 i = 0; i < 4; ++i )
    {
        const f32& elem = vector[i];
        for ( u8 k = 0; k < 4; ++k )
            res[k] += mat.m[i][k] * elem;
    }

    return Vector4(res);
}
static inline Vector4& operator*= ( Vector4& v, const Matrix& mat )
{
    v = v * mat;
    return v;
}
static inline Vector4 operator* ( const Matrix& mat, const Vector4& v )
{
    float res[4] = { 0, 0, 0, 0 };

    const f32* vector = v.vector();
    for ( u8 i = 0; i < 4; ++i )
    {
        const f32& elem = vector[i];
        for ( u8 k = 0; k < 4; ++k )
            res[k] += mat.m[i][k] * elem;
    }

    return Vector4(res);
}

//------------------------------------------------------------------------------
// Matrix4 -> Matrix3
static inline Matrix3 Matrix2Matrix3(const Matrix& mat )
{
    Matrix3 ret;
    for ( u8 i = 0; i < 3; ++i )
        for ( u8 j = 0; j < 3; ++j )
            ret.m[i][j] = mat.m[i][j];
    return ret;
}

//------------------------------------------------------------------------------
// Quaternion -> Matrix3
class Quaternion2Matrix : public Matrix3
{
public:
    Quaternion2Matrix ( const Quaternion& quat )
    {
        f32 qx = quat.x();
        f32 qy = quat.y();
        f32 qz = quat.z();
        f32 qw = quat.w();
        f32 qx2 = qx*qx;
        f32 qy2 = qy*qy;
        f32 qz2 = qz*qz;
        
        float m [ 16 ] = {
            1 - 2*qy2 - 2*qz2,      2*qx*qy + 2*qz*qw,      2*qx*qz - 2*qy*qw,
            2*qx*qy - 2*qz*qw,      1 - 2*qx2 - 2*qz2,      2*qy*qz + 2*qx*qw,
            2*qx*qz + 2*qy*qw,      2*qy*qz - 2*qx*qw,      1 - 2*qx2 - 2*qy2
        };
        
        Matrix3::operator= ( m );
    }
};

//------------------------------------------------------------------------------
// QTransform -> Matrix4
class QTransform2Matrix : public Matrix
{
public:
    QTransform2Matrix ( const QTransform& qt )
    {
        f32 qx = qt.quat.x();
        f32 qy = qt.quat.y();
        f32 qz = qt.quat.z();
        f32 qw = qt.quat.w();
        f32 qx2 = qx*qx;
        f32 qy2 = qy*qy;
        f32 qz2 = qz*qz;
        
        float m [ 16 ] = {
            1 - 2*qy2 - 2*qz2,      2*qx*qy + 2*qz*qw,      2*qx*qz - 2*qy*qw,  0.0f,
            2*qx*qy - 2*qz*qw,      1 - 2*qx2 - 2*qz2,      2*qy*qz + 2*qx*qw,  0.0f,
            2*qx*qz + 2*qy*qw,      2*qy*qz - 2*qx*qw,      1 - 2*qx2 - 2*qy2,  0.0f,
            qt.pos.x(),             qt.pos.y(),             qt.pos.z(),         1.0f
        };
        
        Matrix::operator= ( m );
    }
};

//------------------------------------------------------------------------------
// Matrix -> QTransform
class Matrix2QTransform : public QTransform
{
public:
    Matrix2QTransform ( const Matrix& mat )
    {
        quat.w() = sqrtf ( 1.0f + mat.m[0][0] + mat.m[1][1] + mat.m[2][2] ) / 2.0f;
        f32 w4 = 4.0f * quat.w();
        quat.x() = ( mat.m[1][2] - mat.m[2][1] ) / w4;
        quat.y() = ( mat.m[2][0] - mat.m[0][2] ) / w4;
        quat.z() = ( mat.m[0][1] - mat.m[1][0] ) / w4;
        pos.x() = mat.m[3][0];
        pos.y() = mat.m[3][1];
        pos.z() = mat.m[3][2];
    }
};
