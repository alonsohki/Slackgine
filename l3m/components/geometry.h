/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        geometry.h
// PURPOSE:     Geometry component.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#include "l3m/model.h"
#include "l3m/component.h"
#include "renderer/geometry.h"

namespace l3m
{

class Geometry : public IComponent
{

public:
    static IComponent* create ()
    {
        return new Geometry ();
    }
    
public:
                Geometry        ();
                ~Geometry       ();
    
    bool        load            ( l3m::Model*, l3m::IStream& fp, f32 version );
    bool        save            ( l3m::Model*, l3m::OStream& fp );
    
    //--------------------------------------------------------------------------
    // Delta resolver for the mesh materials
private:
    struct MaterialDeltaData
    {
        Renderer::Mesh* mesh;
        std::string     name;
    };
    static bool     resolveMaterialDelta    ( IComponent* comp, l3m::Model* model, void* data );
    
    //--------------------------------------------------------------------------
    // Delta resolved for the geometry pose
private:
    static bool     resolvePoseDelta        ( IComponent* comp, l3m::Model* model, void* );
    
    
    //--------------------------------------------------------------------------
    // Accessors
public:
    const Renderer::Geometry&           geometry        () const { return m_geometry; }
    const std::string&                  poseUrl         () const { return m_poseUrl; }
    
    Renderer::Geometry&                 geometry        () { return m_geometry; }
    std::string&                        poseUrl         () { return m_poseUrl; }
    
private:
    //--------------------------------------------------------------------------
    // Geometry data
    Renderer::Geometry  m_geometry;
    
    //--------------------------------------------------------------------------
    // String containing the name of the skinning pose, if any.
    std::string     m_poseUrl;
};

}


