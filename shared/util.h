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

