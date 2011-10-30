/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        scene.h
// PURPOSE:     Component controlling the scene type entities.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#include "component.h"
#include "l3m/l3m.h"
#include "../camera.h"

namespace Core { namespace Entities {
    
class Scene : public IComponent
{
public:
    //--------------------------------------------------------------------------
    // Default constructor and destructor
    Scene       ();
    ~Scene      ();
    
    //--------------------------------------------------------------------------
    // Inherited methods from IComponent
    void        preRender               ( Renderer::IRenderer* renderer );
    
    //--------------------------------------------------------------------------
    // FindTheModelCamera
    // Finds the camera model in the l3m model.
private:
    void        findTheModelCamera      ( const l3m::Model* model );
    
    //--------------------------------------------------------------------------
    // DestroyTheCamera
    // Destroys the camera object.
private:
    void        destroyTheCamera        ();
    
    //--------------------------------------------------------------------------
    // Class private attributes
private:
    const l3m::Model*   m_model;
    Camera*             m_camera;
};

} }