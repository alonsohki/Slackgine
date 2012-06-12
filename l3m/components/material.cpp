/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2012
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

bool Material::load(l3m::Model*, l3m::IStream& stream, float version)
{
    // Load the material name
    if ( stream.readStr(&m_material.name()) == 0 )
        return setError ( "Error reading the material name" );
    
    // Load the material data
    Color components[4];
    float shininess;
    b8 shadeless;
    b8 transparent;
    std::string texture;
    
    if ( stream.readBoolean(&transparent) == false )
        return setError ( "Error reading the material '%s' transparent attribute", m_material.name().c_str() );
    if ( stream.readBoolean(&shadeless) == false )
        return setError ( "Error reading the material '%s' shadeless attribute", m_material.name().c_str() );
    if ( stream.readColor(&components[0], 4) != 4 )
        return setError ( "Error reading the material '%s' color components", m_material.name().c_str() );
    if ( stream.readFloat(&shininess, 1) != 1 )
        return setError ( "Error reading the material '%s' shininess component", m_material.name().c_str() );
    if ( stream.readStr(&texture) < 0 )
        return setError ( "Error reading the material '%s' texture", m_material.name().c_str() );
    
    m_material.isTransparent() = transparent;
    m_material.isShadeless() = shadeless;
    m_material.ambient() = components[0];
    m_material.diffuse() = components[1];
    m_material.specular() = components[2];
    m_material.emission() = components[3];
    m_material.shininess() = shininess;
    m_material.texture() = texture;
    
    return true;
}

bool Material::save(l3m::Model*, l3m::OStream& stream)
{
    // Save the material name
    if ( stream.writeStr(m_material.name()) == false )
        return setError ( "Error writing the material name: %s", m_material.name().c_str() );
    
    // Save the material data
    if ( stream.writeBoolean(m_material.isTransparent()) == false )
        return setError ( "Error writing the material '%s' transparent attribute", m_material.name().c_str() );
    if ( stream.writeBoolean(m_material.isShadeless()) == false )
        return setError ( "Error writing the material '%s' shadeless attribute", m_material.name().c_str() );
    if ( stream.writeColor(&m_material.ambient(), 1) == false )
        return setError ( "Error writing the material '%s' ambient component", m_material.name().c_str() );
    if ( stream.writeColor(&m_material.diffuse(), 1) == false )
        return setError ( "Error writing the material '%s' diffuse component", m_material.name().c_str() );
    if ( stream.writeColor(&m_material.specular(), 1) == false )
        return setError ( "Error writing the material '%s' specular component", m_material.name().c_str() );
    if ( stream.writeColor(&m_material.emission(), 1) == false )
        return setError ( "Error writing the material '%s' emission component", m_material.name().c_str() );
    if ( stream.writeFloat(&m_material.shininess(), 1) == false )
        return setError ( "Error writing the material '%s' shininess component", m_material.name().c_str() );
    if ( stream.writeStr(m_material.texture()) == false )
        return setError ( "Error writing the material '%s' texture", m_material.name().c_str() );

    return true;
}
