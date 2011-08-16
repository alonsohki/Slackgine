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
// PURPOSE:     Mathematical utilities.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

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
