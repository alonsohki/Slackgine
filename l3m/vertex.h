#ifndef VERTEX_H
#define VERTEX_H

#include "vector.h"
#include "color.h"
#include "material.h"

class Vertex
{
public:
    enum
    {
        LOAD_POSITION   = 0x01,
        LOAD_NORMAL     = 0x02,
        LOAD_COLOR      = 0x04,
        LOAD_ALL        = LOAD_POSITION|LOAD_NORMAL|LOAD_COLOR
    };
    
private:
    // Cuidadín con tocar estos atributos.
    // Muchos algoritmos confían en el stride entre ellos e incluso en el orden.
    Vector3     m_pos;
    Vector3     m_norm;
    Color       m_color;
    
    // Accessors
public:
    const Vector3&      pos     () const { return m_pos; }
    const Vector3&      norm    () const { return m_norm; }
    const Color&        color   () const { return m_color; }
    
    Vector3&            pos     () { return m_pos; }
    Vector3&            norm    () { return m_norm; }
    Color&              color   () { return m_color; }
    
public:
                        Vertex          () {}
    void                Load            ( const float* source, unsigned int flags, unsigned int stride, unsigned int count = 1 );
    void                Load            ( const Vertex* source, unsigned int count ) { Load ( reinterpret_cast<const float *>(source), LOAD_ALL, 0, count); }
    
    static Vertex*      LoadAllocating  ( const float* source, unsigned int flags, unsigned int stride, unsigned int count );
    static Vertex*      LoadAllocating  ( const Vertex* source, unsigned int count );
};

#endif
