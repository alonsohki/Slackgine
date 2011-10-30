/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        program.h
// PURPOSE:     Interface for the renderer programs.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#ifndef PROGRAM_H
#define	PROGRAM_H

#include "shader.h"
#include "math/vector.h"
#include "math/matrix.h"

namespace Renderer
{

class IProgram
{
public:
    virtual             ~IProgram       () {}
    
    virtual bool        AttachShader    ( const IShader* shader ) = 0;
    virtual bool        DetachShader    ( const IShader* shader ) = 0;
    virtual bool        Link            ( ) = 0;
    virtual bool        Use             ( ) = 0;
    
    virtual bool        SetUniform      ( const std::string& name, f32 value ) = 0;
    virtual bool        SetUniform      ( const std::string& name, i32 value ) = 0;
    virtual bool        SetUniform      ( const std::string& name, const Vector2& vec ) = 0;
    virtual bool        SetUniform      ( const std::string& name, const Vector3& vec ) = 0;
    virtual bool        SetUniform      ( const std::string& name, const Matrix& mat ) = 0;
    
    virtual bool        Ok              ( ) const = 0;
    virtual void        GetError        ( char* dest ) const = 0;
};

}

#endif	/* PROGRAM_H */

