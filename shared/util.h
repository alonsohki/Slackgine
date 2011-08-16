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
// FILE:        util.h
// PURPOSE:     Shader utilities that don't fit anywhere else.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#include "platform.h"

// Variable swap: Typical A = B and B = A
template < typename T >
static inline void Swap ( T& a, T& b )
{
    T temp = a;
    a = b;
    b = temp;
}

// Min and max
template < typename T >
static inline T min ( T a, T b )
{
    return ( a < b ) ? a : b;
}

template < typename T >
static inline T max ( T a, T b )
{
    return ( a > b ) ? a : b;
}

// Value management
template < typename T >
static inline T clamp ( T min, T value, T max )
{
    return ( value < min ) ? min : ((value > max) ? max : value);
}

