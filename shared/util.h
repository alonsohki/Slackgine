/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        util.h
// PURPOSE:     Shader utilities that don't fit anywhere else.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#include <cstdio>
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

// Date size
static inline char* niceDataSize ( u64 size, char* buffer, size_t buffLen )
{
    if ( size < 1024 )
        snprintf ( buffer, buffLen, "%lluB", size );
    else if ( size < 1024*1024 )
        snprintf ( buffer, buffLen, "%llu.%02lluKB", size/1024, (size % 1024) / 10 );
    else if ( size < 1024*1024*1024 )
        snprintf ( buffer, buffLen, "%llu.%02lluMB", size/1024/1024, ( size % (1024*1024) ) / 1024 / 10 );
    else if ( size < 1024ULL*1024*1024*1024 )
        snprintf ( buffer, buffLen, "%llu.%02lluGB", size/1024/1024/1024, ( size % (1024ULL*1024*1024) ) / 1024 / 1024 / 10 );
    else
        snprintf ( buffer, buffLen, "%llu.%02lluTB", size/1024/1024/1024/1024, ( size % (1024ULL*1024*1024*1024) ) / 1024 / 1024 / 1024 / 10 );
    
    return buffer;
}
