#include <cstring>
#include <cstdlib>
#include "vertex.h"

void Vertex::Load ( const float* source, unsigned int flags, unsigned int stride, unsigned int count )
{
    // TODO: Optimizar esto, sacando los if fuera de los bucles y copiando por bloques.
    // Antes de nada, verificar si es necesario. En la mayor parte de los casos
    // esta información debería leerse desde un fichero L3M, y en estos ficheros
    // tendremos toda la información con stride 0, por lo que no requerirá optimización alguna.
    if ( flags != LOAD_ALL )
    {
        Vertex* dest = this;
        while ( count > 0 )
        {
            if ( flags & LOAD_POSITION )
            {
                memcpy ( &dest->m_pos, source, sizeof(float)*3 );
                source += 3;
            }
            if ( flags & LOAD_NORMAL )
            {
                memcpy ( &dest->m_norm, source, sizeof(float)*3 );
                source += 3;
            }
            if ( flags & LOAD_COLOR )
            {
                memcpy ( &dest->m_color, source, sizeof(float)*4 );
                source += 4;
            }
            source += stride;
            ++dest;
            --count;
        }
    }
    else if ( stride != 0 )
    {
        Vertex* dest = this;
        while ( count > 0 )
        {
            memcpy ( dest, source, sizeof(float)*10 );
            source += 10 + stride;
            ++dest;
            --count;
        }
    }
    else
    {
        memcpy ( this, source, sizeof(Vertex) * count );
    }
}

Vertex* Vertex::Allocate ( unsigned int count )
{
    return (Vertex *)malloc(sizeof(Vertex)*count);
}

Vertex* Vertex::LoadAllocating ( const float* source, unsigned int flags, unsigned int stride, unsigned int count )
{
    Vertex* vertices = Allocate(count);
    vertices->Load ( source, flags, stride, count );
    return vertices;
}

Vertex* Vertex::LoadAllocating ( const Vertex* source, unsigned int count )
{
    Vertex* vertices = Allocate(count);
    vertices->Load ( source, count );
    return vertices;
}
