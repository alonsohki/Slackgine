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
    m_program = 0;
}

Default::~Default ()
{
    if ( m_program != 0 )
    {
        sgDelete m_program;
        m_program = 0;
    }
}

bool Default::beginScene (Core::Slackgine* sg, Core::Camera* cam)
{
    IRenderer* renderer = sg->getRenderer ();
    
    if ( m_program == 0 )
    {
        Core::Shader* sh = sg->getShaderManager ().load ( "default" );
        if ( !sh )
            return false;

        m_program = Renderer::Factory::createProgram();
        if ( sh->vert() != 0 )
            m_program->attachShader ( sh->vert() );
        if ( sh->frag() != 0 )
            m_program->attachShader ( sh->frag() );
        if ( !m_program->link() )
        {
            return false;
        }
    }
    
    renderer->setProgram( m_program );
    
    m_matLookAt = LookatMatrix ( cam->transform().orientation(), cam->transform().translation() );
    
    return true;
}

bool Default::forEachEntity(Core::Slackgine* sg, Core::Entity* entity)
{
    // Manage the entity stack
    if ( mEntityStack.size() > 0 && mEntityStack.back() != entity->getParent() )
        mEntityStack.pop_back();
    mEntityStack.push_back(entity);
    
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
                Transform transform = mEntityStack.getTransform ();
                sg->getRenderer()->render( node.geometry, transform, false, getMeshHandler() );

                // Enqueue transparent stuff for later
                for ( Geometry::meshNodeVector::iterator iter2 = node.geometry->m_meshNodes.begin();
                      iter2 != node.geometry->m_meshNodes.end();
                      ++iter2 )
                {
                    Mesh* mesh = (*iter2).mesh;
                    if ( mesh->material() != 0 && mesh->material()->isTransparent() == true )
                    {
                        Vector3 pos = m_matLookAt * transform.translation();
                        m_listTransparencies.push_back(DeferredMesh(node.geometry, mesh, pos.length(), transform));
                    }
                }
            }
        }
    }
    
    return true;
}

bool Default::execute (Core::Slackgine* sg, Core::Entity* startAt)
{
    mEntityStack.clear ();
    sg->forEachEntity ( MakeDelegate(this, &Default::forEachEntity), false, startAt );
    return true;
}

bool Default::endScene ( Core::Slackgine* sg )
{
    // Now, render the transparent stuff.
    m_listTransparencies.sort();
    for ( TransparentMeshList::iterator iter = m_listTransparencies.begin();
          iter != m_listTransparencies.end();
          ++iter )
    {
        DeferredMesh& node = *iter;
        sg->getRenderer()->renderGeometryMesh ( node.geometry, node.mesh, node.transform, getMeshHandler() );
    }
    m_listTransparencies.clear ();
    
    sg->getRenderer()->setProgram(0);
    if ( m_program != 0 )
    {
        sgDelete m_program;
        m_program = 0;
    }
    return true;
}
