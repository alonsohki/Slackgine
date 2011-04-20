#ifndef OPENGL3_H
#define	OPENGL3_H

#include "../renderer.h"
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glut.h>

class OpenGL3_Renderer : public IRenderer
{
public:
    bool Initialize ()
    {
        return true;
    }
    
    bool BeginScene ()
    {
        glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        return true;
    }
    
    bool RenderEntity ( const Entity* pEntity )
    {
        return true;
    }
    
    bool EndScene ()
    {
        glutSwapBuffers();
        return true;
    }
};

#endif

