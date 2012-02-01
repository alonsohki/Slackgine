/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
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
, m_renderStrategy ( 0 )
{
    m_renderer = Renderer::Factory::CreateRenderer ();
}

Slackgine::~Slackgine ()
{
    delete m_renderer;
    if ( m_renderStrategy != 0 )
        delete m_renderStrategy;
}

bool Slackgine::initialize ()
{
    LOG_V ( "Slackgine", "Initializing the engine" );
    setContext ( this );
    if ( m_renderer->initialize () == false )
    {
        m_renderer->getError ( m_error );
        return false;
    }
    
    return true;
}

void Slackgine::tick ()
{
    // Set this process context to this
    setContext ( this );

    m_time.tick ();
    m_modelManager.tick ();
    m_world.tick ();
}

bool Slackgine::render (Camera* cam)
{
    if ( m_renderStrategy != 0 )
    {
        Matrix lookAt = IdentityMatrix ();
        Matrix projection = IdentityMatrix ();
        
        if ( !m_renderStrategy->setup( this ) )
            return false;
        
        if ( cam != 0 )
        {
            projection = cam->getProjection ();
            lookAt = LookatMatrix ( cam->transform().orientation(), cam->transform().translation() );
        }
        
        if ( getRenderer()->beginScene(projection, lookAt) == false )
        {
            getRenderer()->getError ( m_error );
            return false;
        }
        else
        {
            m_renderStrategy->execute ( this );
            getRenderer()->endScene();
        }
        
        m_renderStrategy->cleanup ( this );
    }
    else
    {
        strcpy ( m_error, "Trying to render without having defined a render strategy" );
        return false;
    }

    return true;
}

void Slackgine::getError ( char* dest ) const
{
    strcpy ( dest, m_error );
}

void Slackgine::setRenderStrategy(Renderer::RenderStrategy* strategy)
{
    if ( m_renderStrategy != 0 )
        delete m_renderStrategy;
    m_renderStrategy = strategy;
}
