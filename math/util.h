/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        util.h
// PURPOSE:     Mathematical utilities.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#include <cmath>

static const f32 kPi = 3.141592f;
static const f32 k2Pi = kPi * 2.0f;
static const f32 kPiOver2 = kPi / 2.0f;
static const f32 kOverPi = 1.0f / kPi;
static const f32 k1Over2Pi = 1.0f / k2Pi;

//------------------------------------------------------------------------------
// Transform radians to degrees
static inline f32 rad2deg ( const f32 angle )
{
    return angle * 180.0f / kPi;
}

//------------------------------------------------------------------------------
// Transform degrees to radians
static inline f32 deg2rad ( const f32 angle )
{
    return angle * kPi / 180;
}

//------------------------------------------------------------------------------
// Calculate simultaneously the sin and cos of a given angle
static inline void sinCos ( f32* pSin, f32* pCos, f32 angle )
{
    *pSin = sin ( angle );
    *pCos = cos ( angle );
}

//------------------------------------------------------------------------------
// Wrap an angle in the range -pi...pi by adding the correct
// multiple of 2 pi
static inline f32 wrapPi ( f32 angle )
{
    angle += kPi;
    angle -= floor ( angle * k1Over2Pi ) * k2Pi;
    angle -= kPi;
    return angle;
}
