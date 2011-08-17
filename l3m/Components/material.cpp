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
// FILE:        material.cpp
// PURPOSE:     Materials component.
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

bool Material::Load (l3m::IStream& fp, float version)
{
    // Load the name
    if ( fp.ReadStr (m_name) <= 0 )
        return SetError ( "Error reading the material name" );
    
    // Load the color attributes
    if ( fp.ReadColor (&m_material.ambient (), 1) != 1 )
        return SetError ( "Error reading the material ambient value" );
    if ( fp.ReadColor (&m_material.diffuse (), 1) != 1 )
        return SetError ( "Error reading the material diffuse value" );
    if ( fp.ReadColor (&m_material.specular (), 1) != 1 )
        return SetError ( "Error reading the material specular value" );
    if ( fp.ReadFloat (&m_material.shininess (), 1) != 1 )
        return SetError ( "Error reading the material shininess value" );
    
    return true;
}

bool Material::Save ( l3m::OStream& fp )
{
    // Write the name
    if ( ! fp.WriteStr (m_name) )
        return SetError ( "Error writing the material name" );
    
    // Wirte the color attributes
    if ( ! fp.WriteColor (&m_material.ambient (), 1) )
        return SetError ( "Error writing the material ambient value" );
    if ( ! fp.WriteColor (&m_material.diffuse (), 1) )
        return SetError ( "Error writing the material diffuse value" );
    if ( ! fp.WriteColor (&m_material.specular (), 1) )
        return SetError ( "Error writing the material specular value" );
    if ( ! fp.WriteFloat (&m_material.shininess (), 1) )
        return SetError ( "Error writing the material shininess value" );
    
    return true;
}
