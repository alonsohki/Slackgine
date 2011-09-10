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
    void        PreRender               ( Renderer::IRenderer* renderer );
    
    //--------------------------------------------------------------------------
    // FindTheModelCamera
    // Finds the camera model in the l3m model.
private:
    void        FindTheModelCamera      ( const l3m::Model* model );
    
    //--------------------------------------------------------------------------
    // DestroyTheCamera
    // Destroys the camera object.
private:
    void        DestroyTheCamera        ();
    
    //--------------------------------------------------------------------------
    // Class private attributes
private:
    const l3m::Model*   m_model;
    Camera*             m_camera;
};

} }