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
// FILE:        interpolation.h
// PURPOSE:     Interpolation functions.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

// Linear interpolation
template < typename T >
static inline T lerp ( T from, float alpha, T to )
{
    return from + (to-from)*alpha;
}
template < typename T >
static inline T lerpClamped ( T from, float alpha, T to )
{
    return clamp(from, lerp(from, alpha, to), to);
}

// Cubic interpolation
template < typename T >
static inline T cubic_interpolate ( T from, T tangentFrom, float alpha, T to, T tangentTo )
{
    float alpha2 = alpha*alpha;
    float alpha3 = alpha2*alpha;
    return (2*alpha3 - 3*alpha2 + 1.0f)*from + (alpha3 - 2*alpha2 + alpha)*tangentFrom
         + (-2*alpha3 + 3*alpha2)*to + (alpha3 - alpha2)*tangentTo;
}