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
// FILE:        vertex.h
// PURPOSE:     Vertices.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#include "math/vector.h"
#include "shared/color.h"

namespace Renderer
{

class Vertex
{
public:
    enum
    {
        LOAD_POSITION   = 0x01,
        LOAD_NORMAL     = 0x02,
        LOAD_ALL        = LOAD_POSITION|LOAD_NORMAL
    };
    
private:
    // Cuidadín con tocar estos atributos.
    // Muchos algoritmos confían en el stride entre ellos e incluso en el orden.
    Vector3     m_pos;
    u32         m_padding1;
    Vector3     m_norm;
    u32         m_padding2;
    
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
    const float*        base    () const { return reinterpret_cast<const float*>(&pos()); }
    
    Vector3&            pos     () { return m_pos; }
    Vector3&            norm    () { return m_norm; }
    float*              base    () { return reinterpret_cast<float *>(&pos()); }
};

}
