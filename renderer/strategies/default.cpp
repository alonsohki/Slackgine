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

bool Default::forEachEntity(Core::Slackgine* sg, Core::Entity* entity)
{
    // Render it!
    if ( entity->getModel() != 0 )
    {
        l3m::Scene* sce = l3m::Util::findScene ( entity->getModel() );
        if ( sce != 0 )
        {
            // For each geometry...
            for ( l3m::Scene::NodesVector::iterator iter = sce->geometryNodes().begin();
                iter != sce->geometryNodes().end();
                ++iter )
            {
                l3m::Scene::Node& node = *iter;
                sg->getRenderer()->render( node.geometry, entity->transform() * node.transform, MakeDelegate(this, &Default::forEachMesh) );
            }
        }
    }
    
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
    
    IRenderer* renderer = sg->getRenderer ();
    
    Core::Shader* sh = sg->getShaderManager ().load ( "default" );
    if ( !sh )
        return false;

    ProgramAutoDeleter defaultProgram;
    defaultProgram.ptr = Renderer::Factory::createProgram();
    if ( sh->vert() != 0 )
        defaultProgram.ptr->attachShader ( sh->vert() );
    if ( sh->frag() != 0 )
        defaultProgram.ptr->attachShader ( sh->frag() );
    if ( !defaultProgram.ptr->link() )
    {
        return false;
    }
    renderer->setProgram( defaultProgram.ptr );
    
    sg->forEachEntity ( MakeDelegate(this, &Default::forEachEntity) );
    return true;
}

bool Default::cleanup ( Core::Slackgine* sg )
{
    return true;
}
