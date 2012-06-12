/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2012
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        pose.cpp
// PURPOSE:     Pose definition.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include "pose.h"

using namespace Renderer;

void Pose::calculateTransforms ( Matrix* matrices )
{
    for ( u32 i = 0; i < numJoints(); ++i )
    {
        matrices[i] = QTransform2Matrix ( m_transforms[i] );
    }
}
