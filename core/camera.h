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
// PURPOSE:     Camera base class.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#include "shared/platform.h"
#include "math/matrix.h"
#include "transformable.h"

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
                        Camera          ();
    virtual             ~Camera         ();
    
    void                SetOrthographic ( const f32& left, const f32& right,
                                          const f32& top,  const f32& bottom,
                                          const f32& near, const f32& far );
    void                SetPerspective  ( const f32& fovy, const f32& aspect,
                                          const f32& near, const f32& far );
    
    void                SetCorners      ( const f32& left, const f32& right,
                                          const f32& top,  const f32& bottom );
    void                SetNearFar      ( const f32& near, const f32& far );
    void                SetFovy         ( const f32& fovy );
    void                SetAspect       ( const f32& aspect );
    
    const Matrix&       GetProjection   () const { return m_projection; }
    
    virtual void        Tick            () { Transformable::Tick(); }
    
private:
    void                MakeOrthographicMatrix  ();
    void                MakePerspectiveMatrix   ();
    
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
