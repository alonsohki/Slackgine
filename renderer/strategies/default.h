/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        default.h
// PURPOSE:     Default strategy.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#include "render_strategy.h"

namespace Renderer { namespace Strategy
{
    
class Default : public RenderStrategy
{
public:
    //--------------------------------------------------------------------------
    // Constructor/Destructor
                Default         ();
                ~Default        ();
    
    //--------------------------------------------------------------------------
    // Abstract methods to handle initialization/finalization
    bool        beginScene          ( Core::Slackgine* sg, Core::Camera* cam );
    bool        endScene            ( Core::Slackgine* sg );
    
    //--------------------------------------------------------------------------
    // Execute
    bool        execute             ( Core::Slackgine* sg, Core::Entity* startAt );
    
    //--------------------------------------------------------------------------
    // For each entity
    bool        forEachEntity       ( Core::Slackgine* sg, Core::Entity* entity );
    
private:
    //--------------------------------------------------------------------------
    // Stuff related to the deferred transparency rendering
    struct DeferredMesh
    {
        Renderer::Geometry* geometry;
        Renderer::Mesh*     mesh;
        f32                 distance;
        Transform           transform;
        
        DeferredMesh ( Renderer::Geometry* g, Renderer::Mesh* m, f32 distance, const Transform& transform )
        {
            this->geometry = g;
            this->mesh = m;
            this->distance = distance;
            this->transform = transform;
        }

        bool operator< ( const DeferredMesh& other ) const
        {
            return this->distance > other.distance;
        }
    };
    typedef std::list<DeferredMesh> TransparentMeshList;
    TransparentMeshList             m_listTransparencies;
    
    //--------------------------------------------------------------------------
    // Default program
    Renderer::IProgram*     m_program;
    
    //--------------------------------------------------------------------------
    // The lookAt matrix of the camera being used to render right now
    Matrix                  m_matLookAt;
};
    
} }
