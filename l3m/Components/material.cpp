/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        material.cpp
// PURPOSE:     Material component.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include "material.h"

using namespace l3m;

Material::Material ()
: IComponent ( "material", 1.0f )
{
}

Material::~Material ()
{
}

bool Material::Load(l3m::Model*, l3m::IStream& stream, float version)
{
    // Load the material name
    if ( stream.ReadStr(m_material.name()) == 0 )
        return SetError ( "Error reading the material name" );
    
    // Load the material data
    Color components[4];
    float shininess;
    b8 shadeless;
    
    if ( stream.ReadBoolean(&shadeless) == false )
        return SetError ( "Error reading the material '%s' shadeless atributte", m_material.name().c_str() );
    if ( stream.ReadColor(&components[0], 4) != 4 )
        return SetError ( "Error reading the material '%s' color components", m_material.name().c_str() );
    if ( stream.ReadFloat(&shininess, 1) != 1 )
        return SetError ( "Error reading the material '%s' shininess component", m_material.name().c_str() );
    
    m_material.isShadeless() = shadeless;
    m_material.ambient() = components[0];
    m_material.diffuse() = components[1];
    m_material.specular() = components[2];
    m_material.emission() = components[3];
    m_material.shininess() = shininess;
    
    return true;
}

bool Material::Save(l3m::Model*, l3m::OStream& stream)
{
    // Save the material name
    if ( stream.WriteStr(m_material.name()) == false )
        return SetError ( "Error writing the material name: %s", m_material.name().c_str() );
    
    // Save the material data
    if ( stream.WriteBoolean(m_material.isShadeless()) == false )
        return SetError ( "Error writing the material '%s' shadeless attribute", m_material.name().c_str() );
    if ( stream.WriteColor(&m_material.ambient(), 1) == false )
        return SetError ( "Error writing the material '%s' ambient component", m_material.name().c_str() );
    if ( stream.WriteColor(&m_material.diffuse(), 1) == false )
        return SetError ( "Error writing the material '%s' diffuse component", m_material.name().c_str() );
    if ( stream.WriteColor(&m_material.specular(), 1) == false )
        return SetError ( "Error writing the material '%s' specular component", m_material.name().c_str() );
    if ( stream.WriteColor(&m_material.emission(), 1) == false )
        return SetError ( "Error writing the material '%s' emission component", m_material.name().c_str() );
    if ( stream.WriteFloat(&m_material.shininess(), 1) == false )
        return SetError ( "Error writing the material '%s' shininess component", m_material.name().c_str() );

    return true;
}
