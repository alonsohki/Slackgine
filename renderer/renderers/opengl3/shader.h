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
// FILE:        shader.h
// PURPOSE:     OpenGL3 shaders.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#include <cstring>
#include <string>
#include "opengl3.h"

namespace Renderer
{
    
class OpenGL3_Shader : public IShader
{
    bool        m_loaded;
    GLuint      m_handler;
    char        m_error [ 512 ];
    
public:
                        OpenGL3_Shader          ( IShader::Type type );
                        ~OpenGL3_Shader         ();
    
    bool                Load                    ( std::istream& fp );
    bool                Ok                      () const { return m_handler > 0 && m_loaded; }
    void                GetError                ( char* dest ) const { strcpy(dest, m_error); }
    
    GLuint&             handler                 () { return m_handler; }
    const GLuint&       handler                 () const { return m_handler; }
};

}
