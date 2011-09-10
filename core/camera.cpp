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
// FILE:        camera.cpp
// PURPOSE:     Camera base class.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include "camera.h"

using namespace Core;

Camera::Camera ()
{
    SetOrthographic ( -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f );
}

Camera::~Camera ()
{
}

void Camera::SetOrthographic ( const float& left, const float& right,
                               const float& top, const float& bottom,
                               const float& near, const float& far )
{
    m_type = CAMERA_ORTHOGRAPHIC;
    m_orthographic.left = left;
    m_orthographic.right = right;
    m_orthographic.top = top;
    m_orthographic.bottom = bottom;
    m_orthographic.near = near;
    m_orthographic.far = far;
    MakeOrthographicMatrix ();
    
}

void Camera::SetPerspective ( const float& fovy, const float& aspect,
                              const float& near, const float& far )
{
    m_type = CAMERA_PERSPECTIVE;
    m_perspective.fovy = fovy;
    m_perspective.aspect = aspect;
    m_perspective.near = near;
    m_perspective.far = far;
    MakePerspectiveMatrix ();
}

void Camera::SetCorners (const f32& left, const f32& right, const f32& top, const f32& bottom)
{
    if ( m_type == CAMERA_ORTHOGRAPHIC )
    {
        m_orthographic.left = left;
        m_orthographic.right = right;
        m_orthographic.top = top;
        m_orthographic.bottom = bottom;
        MakeOrthographicMatrix ();
    }
}

void Camera::SetNearFar (const f32& near, const f32& far)
{
    if ( m_type == CAMERA_ORTHOGRAPHIC )
    {
        m_orthographic.near = near;
        m_orthographic.far = far;
        MakeOrthographicMatrix ();
    }
    else
    {
        m_perspective.near = near;
        m_perspective.far = far;
        MakePerspectiveMatrix ();
    }
}

void Camera::SetFovy (const f32& fovy)
{
    if ( m_type == CAMERA_PERSPECTIVE )
    {
        m_perspective.fovy = fovy;
        MakePerspectiveMatrix ();
    }
}

void Camera::SetAspect (const f32& aspect)
{
    if ( m_type == CAMERA_PERSPECTIVE )
    {
        m_perspective.aspect = aspect;
        MakePerspectiveMatrix ();
    }
}

void Camera::MakeOrthographicMatrix ()
{
    m_projection = OrthographicMatrix ( m_orthographic.left,
                                        m_orthographic.right,
                                        m_orthographic.top,
                                        m_orthographic.bottom,
                                        m_orthographic.near,
                                        m_orthographic.far );
}

void Camera::MakePerspectiveMatrix ()
{
    m_projection = PerspectiveMatrix ( m_perspective.aspect,
                                       m_perspective.fovy,
                                       m_perspective.near,
                                       m_perspective.far );
}
