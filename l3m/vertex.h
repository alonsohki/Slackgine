#ifndef VERTEX_H
#define VERTEX_H

#include "vector.h"
#include "color.h"
#include "material.h"

class Vertex
{
public:
    static const unsigned int VERSION = 0;
    
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
                        Vertex          () {}
    void                Load            ( const float* source, unsigned int flags, unsigned int stride, unsigned int count = 1 );
    void                Load            ( const Vertex* source, unsigned int count ) { Load ( reinterpret_cast<const float *>(source), LOAD_ALL, 0, count); }
    
    static Vertex*      Allocate        ( unsigned int count );
    static Vertex*      LoadAllocating  ( const float* source, unsigned int flags, unsigned int stride, unsigned int count );
    static Vertex*      LoadAllocating  ( const Vertex* source, unsigned int count );
};

#endif
