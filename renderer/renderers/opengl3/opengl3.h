#ifndef OPENGL3_H
#define	OPENGL3_H

#include "renderer.h"
#include "shader.h"

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
    
    static IShader* CreateShader ( const char* file )
    {
        IShader* shader = new OpenGL3_Shader ();
        shader->Load ( file );
        return shader;
    }
    
    static IShader* CreateShader ( std::istream& fp )
    {
        IShader* shader = new OpenGL3_Shader ();
        shader->Load ( fp );
        return shader;
    }
};

#endif

