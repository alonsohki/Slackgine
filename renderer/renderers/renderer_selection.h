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
