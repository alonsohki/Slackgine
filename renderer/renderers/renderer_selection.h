/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        renderer_selection.h
// PURPOSE:     Include the appropiate renderer depending on the architecture.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#if defined(ANDROID)
    #include "AndroidGLES20/gles20.h"
#else
    #include "opengl3/opengl3.h"
#endif
