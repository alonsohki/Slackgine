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
// FILE:        model_renderer.cpp
// PURPOSE:     Component controlling the models rendering.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include "model_renderer.h"
#include "core/entity.h"
#include "l3m/Components/scene.h"

using namespace Core::Entities;
using namespace l3m;

ModelRenderer::ModelRenderer ( l3m::Model* model )
: IComponent ( 0, false, true )
{
    m_scene = 0;
    m_model = model;
    if ( m_model )
        Initialize ();
}

ModelRenderer::~ModelRenderer ()
{
}

void ModelRenderer::Render ( Renderer::IRenderer* renderer, const Matrix& mat )
{
    if ( !m_model || !m_scene )
        return;
    
    for ( nodesVector::iterator iter = m_vecNodes.begin ();
          iter != m_vecNodes.end ();
          ++iter )
    {
        Node& node = *iter;
        renderer->Render( node.geometry, node.transform * mat );
    }
}

static Renderer::Geometry* FindGeometryByURL ( Model* model, const std::string& url )
{
    const Model::componentVector& components = model->components ();
    for ( Model::componentVector::const_iterator iter = components.begin();
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

void ModelRenderer::Initialize ()
{
    if ( !m_model )
        return;
    
    // Find the scene
    Scene* scene = 0;
    const Model::componentVector& components = m_model->components ();
    for ( Model::componentVector::const_iterator iter = components.begin();
          iter != components.end();
          ++iter )
    {
        if ( (*iter)->type() == "scene" )
        {
            scene = static_cast < Scene* > ( *iter );
            break;
        }
    }
    
    m_scene = scene;
    if ( m_scene )
    {
        // Find the actual memory references to every geometry in the scene.
        const Scene::nodesVector& geometries = m_scene->geometryNodes();
        for ( Scene::nodesVector::const_iterator iter = geometries.begin();
              iter != geometries.end ();
              ++iter )
        {
            const Scene::Node& node = *iter;
            const std::string& url = node.url;
            Renderer::Geometry* g = FindGeometryByURL(m_model, url);
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
