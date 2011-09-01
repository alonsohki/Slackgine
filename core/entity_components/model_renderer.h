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
// FILE:        model_renderer.h
// PURPOSE:     Component controlling the models rendering.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#include <vector>
#include "l3m/l3m.h"
#include "l3m/Components/scene.h"
#include "component.h"

namespace Core { namespace Entities {
   
class ModelRenderer : public IComponent
{
public:
    struct Node
    {
        Renderer::Geometry*     geometry;
        Transform               transform;
    };
    typedef std::vector < Node > nodesVector;
    
public:
                        ModelRenderer           ( l3m::Model* model );
                        ~ModelRenderer          ();
                
    void                Render                  ( Renderer::IRenderer* renderer, const Transform& mat );
    l3m::Model*         model                   () const { return m_model; }
    
private:
    void                Initialize              ();
    
private:
    l3m::Model*         m_model;
    l3m::Scene*         m_scene;
    nodesVector         m_vecNodes;
};

} }