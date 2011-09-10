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
// FILE:        renderer.h
// PURPOSE:     OpenGL3 renderer.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#include <cstring>
#include "opengl3.h"
#include "math/matrix.h"
#include "math/transform.h"
#include "../../renderer.h"

namespace Renderer
{

class OpenGL3_Renderer : public IRenderer
{
private:
    bool        m_initialized;
    IShader*    m_vertexShader;
    IShader*    m_fragmentShader;
    IProgram*   m_program;
    char        m_error [ 512 ];
    Matrix      m_matProjection;
    Matrix      m_matLookat;

public:
                OpenGL3_Renderer        ();
                ~OpenGL3_Renderer       ();
    
    bool        Initialize              ();
    
    void        SetCamera               ( const Matrix& matProjection = IdentityMatrix(), const Matrix& matLookat = IdentityMatrix() );
    bool        BeginScene              ();
    void        PushState               ();
    bool        Render                  ( Geometry* geometry, const Transform& transform = IdentityTransform() );
    void        PopState                ();
    bool        EndScene                ();
    
    void        GetError                ( char* dest ) const { strcpy(dest, m_error); }
};

}
