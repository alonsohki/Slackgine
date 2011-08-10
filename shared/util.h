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
static inline T& min ( T& a, T& b )
{
    return ( a < b ) ? a : b;
}

template < typename T >
static inline T& max ( T& a, T& b )
{
    return ( a > b ) ? a : b;
}
