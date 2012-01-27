/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        camera.cpp
// PURPOSE:     Camera information, mainly for use in scene components.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include "camera.h"

using namespace l3m;

//------------------------------------------------------------------------------
// Default constructor
// Initialize to an unknown camera type, and with an identity transform.
Camera::Camera ()
: IComponent ( "camera", 1.0f )
, m_transform ( IdentityTransform() )
{
    m_type = CAMERA_UNKNOWN;
}

//------------------------------------------------------------------------------
// Destructor
// Nothing to do here.
Camera::~Camera ()
{
}

//------------------------------------------------------------------------------
// Camera::Load
// Load the camera data depending on the camera type
bool Camera::Load(l3m::Model*, l3m::IStream& fp, float version)
{
    // Read this camera name
    if ( fp.ReadStr(m_name) < 0 )
        return SetError ( "Error reading the camera name" );
    
    // Read the transform
    if ( fp.ReadTransform(&m_transform, 1) != 1 )
        return SetError ( "Error reading the camera transform" );
    
    // Read the camera type
    if ( fp.Read32 ( (u32 *)&m_type, 1 ) != 1 )
        return SetError ( "Error reading the camera type" );
    
    // Using the just read camera type, read its parameters
    switch ( m_type )
    {
        case CAMERA_ORTHOGRAPHIC:
            if ( fp.ReadFloat ( (f32 *)&m_orthographicData, 6 ) != 6 )
                return SetError ( "Error reading the orthographic camera data" );
            break;
        case CAMERA_PERSPECTIVE:
            if ( fp.ReadFloat ( (f32 *)&m_perspectiveData, 4 ) != 4 )
                return SetError ( "Error reading the perspective camera data" );
            break;
        case CAMERA_UNKNOWN:
        default:
            break;
    }

    return true;
}

//------------------------------------------------------------------------------
// Camera::Save
// Save the camera data depending on the camera type
bool Camera::Save(l3m::Model*, l3m::OStream& fp)
{
    // Write this camera name
    if ( !fp.WriteStr(m_name) )
        return SetError ( "Error writing the camera name" );

    // Write the transform
    if ( !fp.WriteTransform(&m_transform, 1) )
        return SetError ( "Error writing the camera transform" );
    
    // Write the type
    if ( !fp.Write32((u32 *)&m_type, 1) )
        return SetError ( "Error writing the camera type" );
    
    // Write the camera parameters, depending on the camera type
    switch ( m_type )
    {
        case CAMERA_ORTHOGRAPHIC:
            if ( !fp.WriteFloat ( (f32 *)&m_orthographicData, 6 ) )
                return SetError ( "Error writing the orthographic camera data" );
            break;
        case CAMERA_PERSPECTIVE:
            if ( !fp.WriteFloat ( (f32 *)&m_perspectiveData, 4 ) )
                return SetError ( "Error writing the perspective camera data" );
            break;
        case CAMERA_UNKNOWN:
        default:
            break;
    }
    
    return true;
}
