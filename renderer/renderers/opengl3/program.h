/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        program.h
// PURPOSE:     OpenGL3 programs.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#include <cstring>
#include "opengl3.h"

namespace Renderer
{

class OpenGL3_Program : public IProgram
{
public:
    enum DefaultAttributeIndex
    {
        POSITION        = 0,
        NORMAL          = 1,
        TEX2D           = 2,
        COLOR           = 3
    };
    
private:
    GLuint      m_handler;
    bool        m_linked;
    char        m_error [ 512 ];
    
public:
                        OpenGL3_Program         ();
                        ~OpenGL3_Program        ();
    
    bool                AttachShader            ( const IShader* shader );
    bool                DetachShader            ( const IShader* shader );    
    bool                Link                    ();
    bool                Use                     ();
    
    bool                SetUniform              ( const std::string& name, f32 value );
    bool                SetUniform              ( const std::string& name, i32 value );
    bool                SetUniform              ( const std::string& name, const Vector2& vec );
    bool                SetUniform              ( const std::string& name, const Vector3& vec );
    bool                SetUniform              ( const std::string& name, const Matrix& mat );
    bool                SetUniform              ( const std::string& name, const Color& col, bool includeAlpha );
    
    bool                Ok                      () const { return m_linked && m_handler > 0; }
    void                GetError                ( char* dest ) const { strcpy(dest, m_error); }
    
    GLuint&             handler                 () { return m_handler; }
    const GLuint&       handler                 () const { return m_handler; }
};

}
