/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        shader.cpp
// PURPOSE:     Class defining a handle to the shaders loaded by the shader
//              manager.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include "shader.h"

using namespace Core;

Shader::Shader ( const std::string& name )
: m_name ( name )
, m_vert ( 0 )
, m_frag ( 0 )
{
}

Shader::~Shader ()
{
    if ( m_vert != 0 )
        delete m_vert;
    if ( m_frag != 0 )
        delete m_frag;
}
