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
// FILE:        main.cpp
// PURPOSE:     Main test file.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>
#include "slackgine.h"
#include "l3m/l3m.h"

void display ( void );

using namespace Core;

static Entity* entity = 0;
static l3m::Model* model = 0;
static Slackgine* sg = 0;

static void cleanup ()
{
    if ( sg != 0 )
        delete sg;
}

int main(int argc, char** argv)
{
    glutInit (&argc, argv);
    glutInitWindowSize (800, 600);
    glutInitDisplayMode ( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
    glutCreateWindow ("Slackgine test");
    
    glutDisplayFunc ( display );
    atexit ( cleanup );
    
    glutMainLoop ();
    
    delete entity;
    
    return EXIT_SUCCESS;
}


void display ( void )
{
    if ( sg == 0 )
    {
        sg = new Slackgine ();
        sg->Initialize ();
        sg->GetModelManager().AddLookupPath ( ".." );
    }

    if ( model == 0 )
        model = sg->GetModelManager().RequestBlocking ("spherecube.l3m");
    if ( entity == 0 )
        entity = new Entity ( model );
    
    sg->Tick ();
    
    static float fRotX = -3.141592f/2;
    static float fRotY = 0;
    static float fTransX = 0;
    static float fTransDir = 1;
    fRotX += 3.141592f/1024;
    fRotY += 3.151592f/512;
    
    if ( fTransX > 2.5f )
        fTransDir = -1;
    else if ( fTransX < -2.5f )
        fTransDir = 1;
    fTransX += fTransDir * 0.002;

    float size = 10.0f;
    if ( sg->GetRenderer()->BeginScene( OrthographicMatrix(-size, size, size, -size, size, -size), /*TranslationMatrix(fTransX,0,0)*RotationMatrix(fRotX,1,0,0)*/RotationMatrix(fRotY,0,1,0)*RotationMatrix(3.141592f/2,-1,0,0) ) )
    {
        entity->Render ( sg->GetRenderer() );
        sg->GetRenderer()->EndScene ();
    }
    else
    {
        char error [ 1024 ];
        sg->GetRenderer()->GetError ( error );
        fprintf ( stderr, "Error al iniciar la escena: %s\n", error );
    }
    
    glutPostRedisplay ();
}
