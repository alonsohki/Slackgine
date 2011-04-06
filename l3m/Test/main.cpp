#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>
#include "l3m.h"
#include "l3mWithDescription.h"

int main(int argc, char** argv)
{
    float fVertices [] =
    {
        // Position             Color
        0.0f, 1.0f, 0.0f,       1.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, 0.0f, 0.0f,      0.0f, 1.0f, 0.0f, 1.0f,
        0.5f, 0.0f, 0.0f,       0.0f, 0.0f, 1.0f, 1.0f
    };
    unsigned int faces [] =
    {
        0, 1, 2
    };
    
    l3mWithDescription model ( "Triángulo cromático demostrativo de la interpolación de colores de OpenGL" );;
    Mesh* mesh = Mesh::LoadAllocating("triangle", fVertices, Vertex::LOAD_POSITION|Vertex::LOAD_COLOR, 0, 3, faces, 1 );
    model.LoadMesh(mesh, "main");
    Mesh* mesh2 = Mesh::LoadAllocating("triangle", fVertices, Vertex::LOAD_POSITION|Vertex::LOAD_COLOR, 0, 3, faces, 1 );
    model.LoadMesh(mesh2, "blah");

    if ( model.SaveToFile ( "chromatic_tri.l3m" ) != l3m::OK )
        fprintf ( stderr, "Error al guardar el fichero: %s\n", model.error() );
    
    l3mWithDescription model2;
    if ( model2.LoadFromFile ( "chromatic_tri.l3m" ) != l3m::OK )
        fprintf ( stderr, "Error al cargar el fichero: %s\n", model2.error() );
    
    printf ( "Descripción leída: %s\n", model2.description().c_str() );

    return 0;
}
