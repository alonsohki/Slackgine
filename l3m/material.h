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
// FILE:        material.h
// PURPOSE:     Material definition.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#include "color.h"

class Material
{
public:
                        Material        () {}
                        ~Material       () {}
    
    // Accessors
    const Color&        ambient         () const { return m_ambient; }
    const Color&        diffuse         () const { return m_diffuse; }
    const Color&        specular        () const { return m_specular; }
    const Color&        emission        () const { return m_emission; }
    
    Color&              ambient         () { return m_ambient; }
    Color&              diffuse         () { return m_diffuse; }
    Color&              specular        () { return m_specular; }
    Color&              emission        () { return m_emission; }

private:
    Color       m_ambient;
    Color       m_diffuse;
    Color       m_specular;
    Color       m_emission;
};
