/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        main.cpp
// PURPOSE:     Main test file.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>
#include "slackgine.h"
#include "l3m/l3m.h"
#include "renderer/strategies/default.h"

void display ( void );

using namespace Core;

static Entity* entity = 0;
static l3m::Model* model = 0;
static Slackgine* sg = 0;
static Camera* cam = 0;

static void cleanup ()
{
    if ( sg != 0 )
    {
        sgDelete sg;
        sg = 0;
    }
}

static void sighandler ( int signum )
{
    switch ( signum )
    {
        case SIGINT:
        {
            cleanup ();
            exit(0);
            break;
        }
    }
}

int main(int argc, char** argv)
{
    signal ( SIGINT, sighandler );

    glutInit (&argc, argv);
    glutInitWindowSize (1920/2, 1080/2);
    glutInitDisplayMode ( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_ALPHA );
    glutCreateWindow ("Slackgine test");
    
    glutDisplayFunc ( display );
    atexit ( cleanup );
    glViewport(0, 0, 1920/2, 1080/2);

    glutMainLoop ();
    
    sgDelete entity;
    
    return EXIT_SUCCESS;
}


void display ( void )
{
    if ( sg == 0 )
    {
        sg = sgNew Slackgine ();
        sg->initialize ();
        sg->getModelManager().addLookupPath ( ".." );
        sg->getShaderManager().addLookupPath( "../shaders" );
        sg->setRenderStrategy ( sgNew Renderer::Strategy::Default () );
    }

    if ( model == 0 )
    {
        model = sg->getModelManager().requestBlocking ("spherecube.l3m");
        if ( model->isOk() == true )
        {
            l3m::Scene* sce = l3m::Util::findScene(model);
            if ( sce != 0 )
                glutReshapeWindow( sce->width(), sce->height() );
            if ( entity != 0 )
                entity->setModel ( model );
        }
        else
        {
            fprintf ( stderr, "Error loading model: %s\n", model->error() );
        }
    }
    if ( entity == 0 )
    {
        entity = sgNew Entity ( model, &sg->getWorld() );
        l3m::Scene* sce = l3m::Util::findScene ( model );
        if ( sce != 0 )
        {
            cam = sgNew Camera ( sce->camera() );
        }
    }
    
    sg->tick ();
    glClearColor ( 0.0f, 0.0f, 0.0f, 1.0f );
    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    char error [ 1024 ];
    if ( !sg->beginScene(cam) )
    {
        sg->getError(error);
        fprintf ( stderr, "Error beginning the scene: %s\n", error );
    }
    else if ( !sg->render() )
    {
        sg->getError ( error );
        fprintf ( stderr, "Error rendering the scene: %s\n", error );
    }
    else if ( !sg->endScene() )
    {
        sg->getError(error);
        fprintf ( stderr, "Error ending the scene: %s\n", error );
    }
    
    glutSwapBuffers ();
    glutPostRedisplay ();
}
