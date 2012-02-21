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
bool Camera::load(l3m::Model*, l3m::IStream& fp, float version)
{
    // Read this camera name
    if ( fp.readStr(&m_name) < 0 )
        return setError ( "Error reading the camera name" );
    
    // Read the transform
    if ( fp.readTransform(&m_transform, 1) != 1 )
        return setError ( "Error reading the camera transform" );
    
    // Read the camera type
    if ( fp.read32 ( (u32 *)&m_type, 1 ) != 1 )
        return setError ( "Error reading the camera type" );
    
    // Using the just read camera type, read its parameters
    switch ( m_type )
    {
        case CAMERA_ORTHOGRAPHIC:
            if ( fp.readFloat ( (f32 *)&m_orthographicData, 6 ) != 6 )
                return setError ( "Error reading the orthographic camera data" );
            break;
        case CAMERA_PERSPECTIVE:
            if ( fp.readFloat ( (f32 *)&m_perspectiveData, 4 ) != 4 )
                return setError ( "Error reading the perspective camera data" );
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
bool Camera::save(l3m::Model*, l3m::OStream& fp)
{
    // Write this camera name
    if ( !fp.writeStr(m_name) )
        return setError ( "Error writing the camera name" );

    // Write the transform
    if ( !fp.writeTransform(&m_transform, 1) )
        return setError ( "Error writing the camera transform" );
    
    // Write the type
    if ( !fp.write32((u32 *)&m_type, 1) )
        return setError ( "Error writing the camera type" );
    
    // Write the camera parameters, depending on the camera type
    switch ( m_type )
    {
        case CAMERA_ORTHOGRAPHIC:
            if ( !fp.writeFloat ( (f32 *)&m_orthographicData, 6 ) )
                return setError ( "Error writing the orthographic camera data" );
            break;
        case CAMERA_PERSPECTIVE:
            if ( !fp.writeFloat ( (f32 *)&m_perspectiveData, 4 ) )
                return setError ( "Error writing the perspective camera data" );
            break;
        case CAMERA_UNKNOWN:
        default:
            break;
    }
    
    return true;
}
