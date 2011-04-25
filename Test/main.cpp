#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>
#include "slackgine.h"
#include "l3m/l3m.h"
#include "l3mWithDescription.h"

void display ( void );

static Entity* entity = 0;
static Slackgine* sg = 0;

int main(int argc, char** argv)
{
    float fVertices [] =
    {
        // Position  
        0.0f, 1.0f, 0.5f,
        -0.5f, 0.0f, 0.5f,
        0.5f, 0.0f, 0.5f,
    };
    unsigned int faces [] =
    {
        0, 1, 2
    };
    
    l3mWithDescription model ( "Triángulo cromático demostrativo de la interpolación de colores de OpenGL" );;
    Mesh* mesh = Mesh::LoadAllocating("triangle", fVertices, Vertex::LOAD_POSITION, 0, 3, faces, 3 );
    model.LoadMesh(mesh, "main");

    if ( model.Save ( "chromatic_tri.l3m" ) != l3m::OK )
        fprintf ( stderr, "Error al guardar el fichero: %s\n", model.error() );
    
    l3mWithDescription model2;
    if ( model2.Load ( "chromatic_tri.l3m" ) != l3m::OK )
        fprintf ( stderr, "Error al cargar el fichero: %s\n", model2.error() );
    
    printf ( "Descripción leída: %s\n", model2.description().c_str() );
    entity = new Entity ( &model2 );
    
    glutInit (&argc, argv);
    glutInitWindowSize (800, 600);
    glutInitDisplayMode ( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
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
    
    if ( sg->renderer()->BeginScene() )
    {
        entity->Update ( sg );
        sg->renderer()->EndScene ();
        
        glutSwapBuffers ();
    }
    else
        fprintf ( stderr, "Error al iniciar la escena.\n" );
}