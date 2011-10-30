/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        model_renderer.cpp
// PURPOSE:     Component controlling the models rendering.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include "model_renderer.h"
#include "core/entity.h"
#include "l3m/Components/scene.h"
#include "l3m/Components/camera.h"

using namespace Core::Entities;

ModelRenderer::ModelRenderer ( l3m::Model* model )
: IComponent ( 0, false, true )
{
    m_scene = 0;
    m_model = model;
    if ( m_model )
        initialize ();
}

ModelRenderer::~ModelRenderer ()
{
}

void ModelRenderer::render ( Renderer::IRenderer* renderer, const Transform& transform )
{
    if ( !m_model || !m_scene )
        return;
    
    for ( nodesVector::iterator iter = m_vecNodes.begin ();
          iter != m_vecNodes.end ();
          ++iter )
    {
        Node& node = *iter;
        renderer->render( node.geometry, transform * node.transform );
    }
}

static Renderer::Geometry* findGeometryByURL ( l3m::Model* model, const std::string& url )
{
    const l3m::Model::componentVector& components = model->components ();
    for ( l3m::Model::componentVector::const_iterator iter = components.begin();
          iter != components.end();
          ++iter )
    {
        if ( (*iter)->type() == "geometry" )
        {
            l3m::Geometry* g = static_cast < l3m::Geometry* > ( *iter );
            if ( g->geometry().name() == url )
                return &(g->geometry());
        }
    }
    
    return 0;
}

void ModelRenderer::initialize ()
{
    if ( !m_model )
        return;
    
    // Find the scene
    l3m::Scene* scene = 0;
    const l3m::Model::componentVector& components = m_model->components ();
    for ( l3m::Model::componentVector::const_iterator iter = components.begin();
          iter != components.end();
          ++iter )
    {
        if ( (*iter)->type() == "scene" )
        {
            scene = static_cast < l3m::Scene* > ( *iter );
            break;
        }
    }
    
    m_scene = scene;
    if ( m_scene )
    {
        // Find the actual memory references to every geometry in the scene.
        const l3m::Scene::nodesVector& geometries = m_scene->geometryNodes();
        for ( l3m::Scene::nodesVector::const_iterator iter = geometries.begin();
              iter != geometries.end ();
              ++iter )
        {
            const l3m::Scene::Node& node = *iter;
            const std::string& url = node.url;
            Renderer::Geometry* g = findGeometryByURL(m_model, url);
            if ( g != 0 )
            {
                Node newNode;
                newNode.geometry = g;
                newNode.transform = node.transform;
                m_vecNodes.push_back ( newNode );
            }
        }
    }
}
