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
#include "core/entity_components/scene.h"

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
    glutInitWindowSize (1920/2, 1080/2);
    glutInitDisplayMode ( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
    glutCreateWindow ("Slackgine test");
    
    glutDisplayFunc ( display );
    atexit ( cleanup );
    glViewport(0, 0, 1920/2, 1080/2);

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
    {
        entity = new Entity ( model );
        entity->AddComponent( new Core::Entities::Scene() );
    }
    
    sg->Tick ();

    if ( sg->GetRenderer()->BeginScene() )
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
