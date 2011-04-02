#include <cstdio>
#include <cstdlib>
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
    
    l3m model;
    model.LoadGroup("main", fVertices, Vertex::LOAD_POSITION|Vertex::LOAD_COLOR, 0, 3 );
    
    printf ( "%f\n", model.group("main")[1].pos().x() );
    
    return 0;
}

