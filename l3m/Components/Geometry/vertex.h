#pragma once

#include "math/vector.h"
#include "shared/color.h"

namespace l3m
{

class Vertex
{
public:
    enum
    {
        LOAD_POSITION   = 0x01,
        LOAD_NORMAL     = 0x02,
        LOAD_TEX2D      = 0x04,
        LOAD_ALL        = LOAD_POSITION|LOAD_NORMAL|LOAD_TEX2D
    };
    
private:
    // Cuidadín con tocar estos atributos.
    // Muchos algoritmos confían en el stride entre ellos e incluso en el orden.
    Vector3     m_pos;
    Vector3     m_norm;
    Vector2     m_tex2d;
    
public:
    Vertex () {}
    ~Vertex () {}
    
    Vertex ( const Vertex& Right )
    {
        operator= ( Right );
    }
    
    Vertex& operator= ( const Vertex& Right )
    {
        m_pos = Right.m_pos;
        m_norm = Right.m_norm;
        m_tex2d = Right.m_tex2d;
    }
    
    bool operator== ( const Vertex& Right ) const
    {
        return memcmp ( this, &Right, sizeof(Vertex) ) == 0;
    }
    bool operator!= ( const Vertex& Right ) const
    {
        return !operator==(Right);
    }
    
    // Accessors
public:
    const Vector3&      pos     () const { return m_pos; }
    const Vector3&      norm    () const { return m_norm; }
    const Vector2&      tex2d   () const { return m_tex2d; }
    const float*        base    () const { return reinterpret_cast<const float*>(&pos()); }
    
    Vector3&            pos     () { return m_pos; }
    Vector3&            norm    () { return m_norm; }
    Vector2&            tex2d   () { return m_tex2d; }
    float*              base    () { return reinterpret_cast<float *>(&pos()); }
    
public:
    void                Load            ( const float* source, unsigned int flags, unsigned int stride, unsigned int count = 1 );
    void                Load            ( const Vertex* source, unsigned int count ) { Load ( reinterpret_cast<const float *>(source), LOAD_ALL, 0, count); }
    
    static Vertex*      Allocate        ( unsigned int count );
    static Vertex*      LoadAllocating  ( const float* source, unsigned int flags, unsigned int stride, unsigned int count );
    static Vertex*      LoadAllocating  ( const Vertex* source, unsigned int count );
};

}
