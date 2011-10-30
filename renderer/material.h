/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        material.h
// PURPOSE:     Material definition.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#include "shared/color.h"

namespace Renderer
{

class Material
{
public:
                        Material        ( const Color& ambient = Color(255, 255, 255, 255),
                                          const Color& diffuse = Color(255, 255, 255, 255),
                                          const Color& specular = Color(0, 0, 0, 0),
                                          const float& shininess = 0.0f,
                                          const Color& emission = Color(0, 0, 0, 0) )
                        : m_ambient ( ambient )
                        , m_diffuse ( diffuse )
                        , m_specular ( specular )
                        , m_shininess ( shininess )
                        , m_emission ( emission )
                        {}
                        ~Material       () {}
    
    // Accessors
    const Color&        ambient         () const { return m_ambient; }
    const Color&        diffuse         () const { return m_diffuse; }
    const Color&        specular        () const { return m_specular; }
    const float&        shininess       () const { return m_shininess; }
    const Color&        emission        () const { return m_emission; }
    
    Color&              ambient         () { return m_ambient; }
    Color&              diffuse         () { return m_diffuse; }
    Color&              specular        () { return m_specular; }
    float&              shininess       () { return m_shininess; }
    Color&              emission        () { return m_emission; }

private:
    Color               m_ambient;
    Color               m_diffuse;
    Color               m_specular;
    float               m_shininess;
    Color               m_emission;
};

}
