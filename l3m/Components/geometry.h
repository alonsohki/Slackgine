#pragma once

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
    
    bool        Load            ( l3m::IStream& fp, f32 version );
    bool        Save            ( l3m::OStream& fp );
    
    
    // Accessors
public:
    const Renderer::Geometry&           geometry        () const { return m_geometry; }
    Renderer::Geometry&                 geometry        () { return m_geometry; }
};

}


