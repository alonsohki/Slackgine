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
// FILE:        render_strategy.cpp
// PURPOSE:     Interface for rendering strategies.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include "render_strategy.h"

using namespace Renderer;

RenderStrategy::Pass::Pass ( PassType type )
: m_type ( type )
{}

RenderStrategy::Pass::~Pass ()
{
}


RenderStrategy::RenderStrategy ()
{
    m_error[0] = '\0';
}

RenderStrategy::~RenderStrategy ()
{
    for ( u32 i = 0; i < PASS_MAX; ++i )
    {
        for ( PassVector::iterator iter = m_passes[i].begin();
              iter != m_passes[i].end();
              ++iter )
        {
            delete *iter;
        }
    }
}

bool RenderStrategy::execute ( Core::Slackgine* sg )
{
    // Perform the geometry passes
    for ( PassVector::iterator iter = m_passes[PASS_GEOMETRY].begin();
          iter != m_passes[PASS_GEOMETRY].end();
          ++iter )
    {
        (*iter)->setup ();
        if ( !sg->renderGeometry () )
        {
            sg->getError( m_error );
            return false;
        }
    }
    
    // Perform the composition passes
    for ( PassVector::iterator iter = m_passes[PASS_COMPOSITION].begin();
          iter != m_passes[PASS_COMPOSITION].end();
          ++iter )
    {
        (*iter)->setup ();
    }
    
    // Perform the post-process passes
    for ( PassVector::iterator iter = m_passes[PASS_POSTPROCESS].begin();
          iter != m_passes[PASS_POSTPROCESS].end();
          ++iter )
    {
        (*iter)->setup ();
    }

    return true;
}

void RenderStrategy::getError(char* error) const
{
    strcpy ( error, m_error );
}

void RenderStrategy::addPass ( Pass* pass )
{
    if ( pass->getType() >= PASS_MAX )
        return;
    m_passes[pass->getType()].push_back ( pass );
}

void RenderStrategy::blendStrategy ( RenderStrategy* strategy )
{
    for ( u32 i = 0; i < PASS_MAX; ++i )
    {
        for ( PassVector::iterator iter = strategy->m_passes[i].begin();
              iter != strategy->m_passes[i].end();
              ++iter )
        {
            m_passes[i].push_back ( (*iter)->clone() );
        }
    }
}
