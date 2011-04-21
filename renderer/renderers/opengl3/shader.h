#ifndef OPENGL3_SHADER_H
#define	OPENGL3_SHADER_H

#include "../../shader.h"

class OpenGL3_Shader : public IShader
{
public:
    ~OpenGL3_Shader ()
    {
        
    }
    
    bool Load ( std::istream& fp )
    {
        
    }
    
    bool Ok () const
    {
        return true;
    }
};

#endif	/* OPENGL3_SHADER_H */

