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
// FILE:        slackgine.cpp
// PURPOSE:     Slackgine main class.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include "slackgine.h"
#include "shared/log.h"

using namespace Core;

Slackgine* Slackgine::ms_context = 0;
void Slackgine::setContext ( Slackgine* sg )
{
    if ( sg != ms_context )
    {
        LOG_VV ( "Slackgine", "Setting context to %p", sg );
        ms_context = sg;
    }
}

Slackgine::Slackgine ()
: m_time ()
, m_textureManager ()
, m_modelManager ( m_textureManager, m_time )
{
    m_renderer = Renderer::Factory::CreateRenderer ();
}

Slackgine::~Slackgine ()
{
    delete m_renderer;
}

bool Slackgine::initialize ()
{
    LOG_V ( "Slackgine", "Initializing the engine" );
    return m_renderer->initialize ();
}

void Slackgine::tick ()
{
    // Set this process context to this
    setContext ( this );

    m_time.tick ();
    m_modelManager.tick ();
    m_world.tick ();
}

bool Slackgine::render ()
{
    if ( getRenderer()->beginScene() == false )
    {
        getRenderer()->getError ( m_error );
        return false;
    }
    else
    {
        getWorld().render ( getRenderer() );
        getRenderer()->endScene();
    }
    return true;
}

void Slackgine::getError ( char* dest ) const
{
    strcpy ( dest, m_error );
}
