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

