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
    if ( !l3mWithDescription::SignInFactory() )
        fprintf ( stderr, "No se ha podido registrar el l3m con descripción en la factoría\n" );
    
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
    
    l3m model2;
    if ( model2.Load ( "../collada_import/spherecube.l3m" ) != l3m::OK )
        fprintf ( stderr, "Error al cargar el fichero: %s\n", model2.error() );
    
    //printf ( "Descripción leída: %s\n", model2.description().c_str() );
    
    l3m::ErrorCode err;
    l3m* model3 = l3mFactory::CreateAndLoad( "chromatic_tri.l3m", &err );
    if ( !model3 || err != l3m::OK )
        fprintf ( stderr, "Error al cargar el fichero: %s\n", l3m::TranslateErrorCode(err) );
    else
    {
        printf("Tipo de modelo leído: %s\n", model3->type().c_str() );
    }

    entity = new Entity ( &model2 );
    
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
    
    if ( sg->renderer()->BeginScene( OrthographicMatrix(-50, 50, 50, -50, 50, -50), TranslationMatrix(fTransX,0,0)*RotationMatrix(fRotX,1,0,0)*RotationMatrix(fRotY,0,1,0) ) )
    {
        entity->Update ( sg );
        sg->renderer()->EndScene ();
    }
    else
        fprintf ( stderr, "Error al iniciar la escena.\n" );
    glutPostRedisplay ();
}