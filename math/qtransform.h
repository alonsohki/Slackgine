/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        qtransform.h
// PURPOSE:     Transforms based on quaternions
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#include "vector.h"
#include "quaternion.h"

class QTransform
{
public:
    Quaternion  quat;
    Vector3     pos;
};
