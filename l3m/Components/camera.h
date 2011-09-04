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
// FILE:        camera.h
// PURPOSE:     Camera information, mainly for use in scene components.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#include "../component.h"
#include "math/transform.h"

namespace l3m
{
    
class Camera : public IComponent
{
public:
    //--------------------------------------------------------------------------
    // Define a way to identify this camera type and its attributes
    enum CameraType
    {
        CAMERA_UNKNOWN,
        CAMERA_ORTHOGRAPHIC,
        CAMERA_PERSPECTIVE
    };
    
    struct OrthographicData
    {
        f32     left;
        f32     right;
        f32     top;
        f32     bottom;
        f32     near;
        f32     far;
    };
    struct PerspectiveData
    {
        f32     aspect;
        f32     fov;
        f32     near;
        f32     far;
    };

public:
    //--------------------------------------------------------------------------
    // Static constructor of camera entities for the factory
    static IComponent*  Create          ()
    {
        return new Camera ();
    }
    
public:
    //--------------------------------------------------------------------------
    // Default constructor and destructor
    Camera      ();
    ~Camera     ();

    //--------------------------------------------------------------------------
    // Inherited methods from IComponent
    bool                Load            ( l3m::IStream& stream, float version );
    bool                Save            ( l3m::OStream& stream );

    //--------------------------------------------------------------------------
    // Accessors
    std::string&        name                    () { return m_name; }
    CameraType&         type                    () { return m_type; }
    Transform&          transform               () { return m_transform; }
    OrthographicData&   orthographicData        () { return m_orthographicData; }
    PerspectiveData&    perspectiveData         () { return m_perspectiveData; }
    
    //--------------------------------------------------------------------------
    // Constant accessors
    const std::string&          name                    () const { return m_name; }
    const CameraType&           type                    () const { return m_type; }
    const Transform&            transform               () const { return m_transform; }
    const OrthographicData&     orthographicData        () const { return m_orthographicData; }
    const PerspectiveData&      perspectiveData         () const { return m_perspectiveData; }

    //--------------------------------------------------------------------------
    // Class members
private:
    std::string         m_name;
    CameraType          m_type;
    Transform           m_transform;
    OrthographicData    m_orthographicData;
    PerspectiveData     m_perspectiveData;
};

}
