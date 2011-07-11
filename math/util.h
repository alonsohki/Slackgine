#pragma once

static const float PI = 3.141592f;

static inline float rad2deg ( const float fAngle )
{
    return fAngle * 180.0f / PI;
}

static inline float deg2rad ( const float fAngle )
{
    return fAngle * PI / 180;
}
