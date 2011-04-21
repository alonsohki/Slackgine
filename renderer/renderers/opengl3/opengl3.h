#ifndef OPENGL3_H
#define	OPENGL3_H

#include "renderer.h"
#include "shader.h"
#include "program.h"

class Renderer
{
public:
    static IRenderer* Instance ()
    {
        static IRenderer* instance = 0;
        if ( instance == 0 )
            instance = new OpenGL3_Renderer ();
        return instance;
    }
    
    static IProgram* CreateProgram ()
    {
        return new OpenGL3_Program ();
    }
    
    static IShader* CreateShader ( IShader::Type type, const char* file )
    {
        IShader* shader = new OpenGL3_Shader ( type );
        shader->Load ( file );
        return shader;
    }
    
    static IShader* CreateShader ( IShader::Type type, std::istream& fp )
    {
        IShader* shader = new OpenGL3_Shader ( type );
        shader->Load ( fp );
        return shader;
    }
};

#endif

