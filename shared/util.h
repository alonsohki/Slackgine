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

template < typename T >
static inline T lerp ( T from, float alpha, T to )
{
    return from + to*alpha;
}
template < typename T >
static inline T lerpClamped ( T from, float alpha, T to )
{
    return clamp(from, lerp(from, alpha, to), to);
}
