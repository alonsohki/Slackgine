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
private:
    Renderer::Geometry  m_geometry;

public:
    static IComponent* Create ()
    {
        return new Geometry ();
    }
    
                Geometry        ();
                ~Geometry       ();
    
    bool        Load            ( l3m::Model*, l3m::IStream& fp, f32 version );
    bool        Save            ( l3m::Model*, l3m::OStream& fp );
    
private:
    struct MaterialDeltaData
    {
        Renderer::Mesh* mesh;
        std::string     name;
    };
    
    //--------------------------------------------------------------------------
    // Delta resolver for the mesh materials
    static bool     ResolveMaterialDelta    ( IComponent* comp, l3m::Model* model, void* data );
    
    
    // Accessors
public:
    const Renderer::Geometry&           geometry        () const { return m_geometry; }
    Renderer::Geometry&                 geometry        () { return m_geometry; }
};

}


