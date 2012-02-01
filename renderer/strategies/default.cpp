/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        default.cpp
// PURPOSE:     CDefault strategy.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include "default.h"
#include "slackgine.h"
#include "core/shader_manager.h"

using namespace Renderer::Strategy;
using Core::Entity;

Default::Default ()
{
}

Default::~Default ()
{
}

bool Default::forEachMesh ( Renderer::Mesh* mesh )
{
    return true;
}

bool Default::setup (Core::Slackgine* sg)
{
    return true;
}

struct ProgramAutoDeleter
{
    Renderer::IProgram* ptr;

    ProgramAutoDeleter () : ptr ( 0 )
    {}
    ~ProgramAutoDeleter ()
    {
        if ( ptr != 0 )
            delete ptr;
    }
};

bool Default::execute (Core::Slackgine* sg)
{
    std::deque < Entity* > entities;
    IRenderer* renderer = sg->getRenderer ();
    
    Core::Shader* sh = sg->getShaderManager ().load ( "default" );
    if ( !sh )
        return false;

    ProgramAutoDeleter defaultProgram;
    defaultProgram.ptr = Renderer::Factory::createProgram();
    defaultProgram.ptr->attachShader ( sh->vert() );
    defaultProgram.ptr->attachShader ( sh->frag() );
    if ( !defaultProgram.ptr->link() )
    {
        return false;
    }
    
    // Begin the scene and push the world root to the deque
    entities.push_back( &sg->getWorld() );
    
    // Repeat until no more entities
    while ( entities.size() > 0 )
    {
        // Get the current entity to render
        Entity* cur = entities.front ();
        entities.pop_front ();
        
        // Push its children to the front of the deque
        for ( Entity::EntityVector::iterator iter = cur->getChildren().begin(); iter != cur->getChildren().end(); ++iter )
            entities.push_front ( *iter );

        // Render it!
        if ( cur->getModel() != 0 )
        {
            l3m::Scene* sce = l3m::Util::findScene ( cur->getModel() );
            if ( sce != 0 )
            {
                // For each geometry...
                for ( l3m::Scene::NodesVector::iterator iter = sce->geometryNodes().begin();
                      iter != sce->geometryNodes().end();
                      ++iter )
                {
                    l3m::Scene::Node& node = *iter;
                    renderer->setProgram( defaultProgram.ptr );
                    renderer->render( node.geometry, cur->transform() * node.transform, MakeDelegate(this, &Default::forEachMesh) );
                }
            }
        }
    }

    return true;
}

bool Default::cleanup ( Core::Slackgine* sg )
{
    return true;
}
