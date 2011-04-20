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
    
    bool RenderEntity ( const Entity* entity )
    {
        const l3m* model = entity->GetModel ();
        if ( model )
        {
            const l3m::groupMap& groups = model->GetGroups();
            for ( l3m::groupMap::const_iterator i = groups.begin(); i != groups.end(); ++i )
            {
                const l3m::meshList& meshes = i->second;
                for ( l3m::meshList::const_iterator j = meshes.begin(); j != meshes.end(); ++j )
                {
                    const Mesh* mesh = *j;
                }
            }
        }
        return true;
    }
    
    bool EndScene ()
    {
        glutSwapBuffers();
        return true;
    }
};

#endif

