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
#include "shared/memory_leak_detector.h"

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
    m_renderer = Renderer::Factory::createRenderer ();
}

Slackgine::~Slackgine ()
{
    if ( m_renderer != 0 )
    {
        sgDelete m_renderer;
        m_renderer = 0;
    }
    if ( m_renderStrategy != 0 )
    {
        sgDelete m_renderStrategy;
        m_renderStrategy = 0;
    }

#ifdef PROFILING
    //--------------------------------------------------------------------------
    // Log memory leaks
    const MemoryLeakDetector::NodeList& allocations = MemoryLeakDetector::getInstance()->getAllocations();
    if ( allocations.size() > 0 )
    {
        fprintf ( stderr, "\n**** WARNING **** Not freed chunks found by the memory leak detector:\n" );
        for ( MemoryLeakDetector::NodeList::const_iterator iter = allocations.begin();
              iter != allocations.end();
              ++iter )
        {
            const MemoryLeakDetector::Node& node = *iter;
            u32 ms = node.dateMs % 1000;
            time_t secs = node.dateMs;
            struct tm* lt = localtime ( &secs );
            
            char dateBuffer [ 256 ];
            strftime ( &dateBuffer[0], sizeof(dateBuffer), "%D %T", lt );
            char sizeBuffer [ 64 ];
            fprintf ( stderr, "\t** [%s.%03u]\tptr=%p\tsize=%s\t%s:%u \n",
                      dateBuffer, ms, node.ptr,
                      niceDataSize(node.size, sizeBuffer, sizeof(sizeBuffer)),
                      node.file, node.line );
        }
        fprintf ( stderr, "\n" );
    }
#endif
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

bool Slackgine::beginScene (Camera* cam)
{
    if ( m_renderStrategy == 0 )
    {
        strcpy ( m_error, "No render strategy set" );
        return false;
    }

    Matrix lookAt = IdentityMatrix ();
    Matrix projection = IdentityMatrix ();

    if ( m_renderStrategy->beginScene ( this, cam ) == false )
        return false;

    if ( cam != 0 )
    {
        projection = cam->getProjection ();
        lookAt = LookatMatrix ( cam->transform().orientation(), cam->transform().translation() );
    }

    if ( getRenderer()->beginScene(projection, lookAt, MakeDelegate(&getTextureManager(), &TextureManager::find)) == false )
    {
        getRenderer()->getError ( m_error );
        return false;
    }
    
    return true;
}

bool Slackgine::render (Entity* startAt)
{
    if ( startAt == 0 )
        startAt = &getWorld ();
    return m_renderStrategy->execute ( this, startAt );
}

bool Slackgine::endScene()
{
    if ( m_renderStrategy->endScene ( this ) == false )
        return false;

    if ( getRenderer()->endScene() == false )
    {
        getRenderer()->getError(m_error);
        return false;
    }
    
    return true;
}

void Slackgine::getError ( char* dest ) const
{
    strcpy ( dest, m_error );
}

void Slackgine::forEachEntity ( ForEachEntityDelegate delegate, bool includeInvisible, Entity* startAt )
{
    std::deque < Entity* > entities;
    
    // Start at the given entity
    if ( startAt != 0 )
        entities.push_back ( startAt );
    else
        entities.push_back ( &getWorld() );

    // Repeat until no more entities
    while ( entities.size() > 0 )
    {
        // Get the current entity to render
        Entity* cur = entities.front ();
        entities.pop_front ();
        
        if ( includeInvisible || cur->isVisible() )
        {
            // Push its children to the front of the deque
            for ( Entity::EntityVector::iterator iter = cur->getChildren().begin(); iter != cur->getChildren().end(); ++iter )
                entities.push_front ( *iter );
            
            delegate ( this, cur );
        }
    }
}

void Slackgine::setRenderStrategy(Renderer::RenderStrategy* strategy)
{
    if ( m_renderStrategy != 0 )
        sgDelete m_renderStrategy;
    m_renderStrategy = strategy;
}

Renderer::RenderStrategy* Slackgine::getRenderStrategy()
{
    return m_renderStrategy;
}
