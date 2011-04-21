#ifndef OPENGL3_H
#define	OPENGL3_H

#include "renderer.h"
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
};

#endif

