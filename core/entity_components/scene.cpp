/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        scene.cpp
// PURPOSE:     Component controlling the scene type entities.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include "scene.h"
#include "l3m/Components/camera.h"
#include "l3m/Components/scene.h"
#include "../entity.h"
#include "math/matrix.h"
#include "math/transform.h"

using namespace Core::Entities;

Scene::Scene ()
: IComponent ( 0, false, true )
, m_model ( 0 )
, m_camera ( 0 )
{
}

Scene::~Scene()
{
    destroyTheCamera ();
}

void Scene::destroyTheCamera ()
{
    if ( m_camera != 0 )
    {
        delete m_camera;
        m_camera = 0;
    }
}

void Scene::findTheModelCamera ( const l3m::Model* model )
{
    if ( m_model == model )
        return;
    
    m_model = model;
    if ( !m_model )
        return destroyTheCamera ();
    
    // Find the scene component
    l3m::Scene* scene = 0;
    const l3m::Model::ComponentVector& components = m_model->components();
    for ( l3m::Model::ComponentVector::const_iterator iter = components.begin();
          iter != components.end();
          ++iter )
    {
        if ( (*iter)->type() == "scene" )
        {
            scene = static_cast < l3m::Scene* > ( *iter );
            break;
        }
    }
    
    // Make sure that there is a scene
    if ( !scene )
        return destroyTheCamera ();

    // Find the scene camera
    if ( scene->camera() != "" )
    {
        // Find the referenced camera
        for ( l3m::Model::ComponentVector::const_iterator iter = components.begin();
              iter != components.end();
              ++iter )
        {
            if ( (*iter)->type() == "camera" )
            {
                l3m::Camera* cam = static_cast<l3m::Camera *>(*iter);
                if ( cam->name() == scene->camera() )
                {
                    switch ( cam->type() )
                    {
                        case l3m::Camera::CAMERA_ORTHOGRAPHIC:
                            m_camera = new Camera ();
                            m_camera->setOrthographic( cam->orthographicData().left,
                                                       cam->orthographicData().right,
                                                       cam->orthographicData().top,
                                                       cam->orthographicData().bottom,
                                                       cam->orthographicData().near,
                                                       cam->orthographicData().far );
                            break;
                        case l3m::Camera::CAMERA_PERSPECTIVE:
                            m_camera = new Camera ();
                            m_camera->setPerspective( cam->perspectiveData().fov,
                                                      cam->perspectiveData().aspect,
                                                      cam->perspectiveData().near,
                                                      cam->perspectiveData().far );
                            break;
                        default:
                            break;
                    }
                    
                    // If it was a valid camera, copy its transform
                    if ( m_camera )
                        m_camera->transform() = cam->transform();
                    
                    break;
                }
            }
        }
    }
}

void Scene::preRender ( Renderer::IRenderer* renderer )
{
    if ( !parent() )
        return;
    
    findTheModelCamera ( parent()->getModel() );
    if ( m_camera != 0 )
    {
        Matrix projection = m_camera->getProjection();
        LookatMatrix lookAt ( m_camera->transform().orientation(), m_camera->transform().translation() );
        renderer->setCamera ( projection, lookAt );
    }
}