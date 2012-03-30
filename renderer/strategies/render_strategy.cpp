/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        render_strategy.cpp
// PURPOSE:     Interface for rendering strategies.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include "render_strategy.h"

using namespace Renderer;

RenderStrategy::RenderStrategy ()
{
    m_error[0] = '\0';
    resetMeshHandler ();
}

RenderStrategy::~RenderStrategy ()
{
}

bool RenderStrategy::execute ( Core::Slackgine* sg, Core::Entity* startAt )
{
   return true;
}

void RenderStrategy::getError(char* error) const
{
    strcpy ( error, m_error );
}

void RenderStrategy::setError(const char* error)
{
    snprintf ( m_error, sizeof(m_error), "%s", error );
}

void RenderStrategy::setMeshHandler(MeshRenderFn handler)
{
    m_meshDelegate = handler;
}

void RenderStrategy::resetMeshHandler ()
{
    setMeshHandler ( &RenderStrategy::defaultMeshHandler );
}

