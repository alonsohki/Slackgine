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

Slackgine::Slackgine ()
: m_modelManager ( m_time )
{
    m_renderer = Renderer::Factory::CreateRenderer ();
}

Slackgine::~Slackgine ()
{
    delete m_renderer;
}

bool Slackgine::Initialize ()
{
    LOG_V ( "Slackgine", "Initializing the engine" );
    return m_renderer->Initialize ();
}

void Slackgine::Tick ()
{
    m_time.Tick ();
    m_modelManager.Tick ();
}
