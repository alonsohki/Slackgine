/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        celshading.cpp
// PURPOSE:     Cel-shading strategy.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include "celshading.h"
#include "slackgine.h"

using namespace Renderer::Strategy;

Celshading::Celshading ()
{
    Core::Slackgine::getInstance()->getShaderManager().load ( "celshading" );
}

Celshading::~Celshading ()
{
}
