/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        camera.h
// PURPOSE:     Camera base class.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#include "shared/platform.h"
#include "math/matrix.h"
#include "transformable.h"
#include "l3m/components/camera.h"

namespace Core
{

class Camera : public Transformable
{
private:
    enum CameraType
    {
        CAMERA_PERSPECTIVE,
        CAMERA_ORTHOGRAPHIC
    };
    
public:
                        Camera          ( const l3m::Camera* cam = 0 );
    virtual             ~Camera         ();
    
    void                create          ( const l3m::Camera* cam );
    
    void                setOrthographic ( const f32& left, const f32& right,
                                          const f32& top,  const f32& bottom,
                                          const f32& near, const f32& far );
    void                setPerspective  ( const f32& fovy, const f32& aspect,
                                          const f32& near, const f32& far );
    
    void                setCorners      ( const f32& left, const f32& right,
                                          const f32& top,  const f32& bottom );
    void                setNearFar      ( const f32& near, const f32& far );
    void                setFovy         ( const f32& fovy );
    void                setAspect       ( const f32& aspect );
    
    const Matrix&       getProjection   () const { return m_projection; }
    
    virtual void        tick            () { Transformable::tick(); }
    
private:
    void                makeOrthographicMatrix  ();
    void                makePerspectiveMatrix   ();
    
private:
    Matrix      m_projection;
    CameraType  m_type;
    
    struct
    {
        f32 left;
        f32 right;
        f32 top;
        f32 bottom;
        f32 near;
        f32 far;
    } m_orthographic;
    
    struct
    {
        f32 fovy;
        f32 aspect;
        f32 near;
        f32 far;
    } m_perspective;
};
    
}
