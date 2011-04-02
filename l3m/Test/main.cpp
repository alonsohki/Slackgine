#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>
#include "l3m.h"

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
    
    l3m model;
    Mesh* mesh = Mesh::LoadAllocating("triangle", fVertices, Vertex::LOAD_POSITION|Vertex::LOAD_COLOR, 0, 3, faces, 1 );
    model.LoadMesh(mesh, "main");
    Mesh* mesh2 = Mesh::LoadAllocating("triangle", fVertices, Vertex::LOAD_POSITION|Vertex::LOAD_COLOR, 0, 3, faces, 1 );
    model.LoadMesh(mesh2, "blah");

    if ( ! model.SaveToFile ( "chromatic_tri.l3m" ) )
        fprintf ( stderr, "Error al guardar el fichero: %s\n", strerror(errno) );
    
    l3m model2;
    if ( ! model2.LoadFromFile ( "chromatic_tri.l3m" ) )
        fprintf ( stderr, "Error al cargar el fichero: %s\n", strerror(errno) );

    return 0;
}
