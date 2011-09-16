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
// FILE:        celshading.cpp
// PURPOSE:     Cel-shading strategy.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include "celshading.h"

using namespace Renderer::Strategy;

Celshading::Celshading ()
{
    m_error[0] = '\0';
}

Celshading::~Celshading ()
{
}

bool Celshading::render ( Core::Slackgine* sg )
{
    if ( !sg->render () )
    {
        sg->getError ( m_error );
        return false;
    }
    return true;
}

void Celshading::getError(char* error) const
{
    strcpy ( error, m_error );
}
