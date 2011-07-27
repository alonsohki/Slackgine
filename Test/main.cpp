#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>
#include "slackgine.h"
#include "l3m/l3m.h"

void display ( void );

static Entity* entity = 0;
static Slackgine* sg = 0;

int main(int argc, char** argv)
{
    l3m::Model model;
    if ( model.Load ( "../import/spherecube.l3m" ) == false )
    {
        fprintf ( stderr, "Error al cargar el modelo: %s\n", model.error() );
        return EXIT_FAILURE;
    }

    entity = new Entity ( &model );
    
    glutInit (&argc, argv);
    glutInitWindowSize (800, 600);
    glutInitDisplayMode ( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
    glutCreateWindow ("Slackgine test");
    
    glutDisplayFunc ( display );
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
    }
    
    static float fRotX = -3.141592f/2;
    static float fRotY = 0;
    static float fTransX = 0;
    static float fTransDir = 1;
    fRotX += 3.141592f/1024;
    fRotY += 3.151592f/8192;
    
    if ( fTransX > 2.5f )
        fTransDir = -1;
    else if ( fTransX < -2.5f )
        fTransDir = 1;
    fTransX += fTransDir * 0.002;
    
    if ( sg->renderer()->BeginScene( OrthographicMatrix(-5, 5, 5, -5, 5, -5), TranslationMatrix(fTransX,0,0)*RotationMatrix(fRotX,1,0,0)*RotationMatrix(fRotY,0,1,0) ) )
    {
        sg->renderer()->EndScene ();
    }
    else
        fprintf ( stderr, "Error al iniciar la escena.\n" );
    glutPostRedisplay ();
}