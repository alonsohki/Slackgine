#include "face.h"
#include <cstring>
#include <cstdlib>

void Face::Load ( const unsigned int* indices, unsigned int faceCount )
{
    memcpy ( &m_index, indices, sizeof(unsigned int) * 3 * faceCount );
}

Face* Face::Allocate ( unsigned int count )
{
    return (Face *)malloc(sizeof(Face) * count);
}

Face* Face::LoadAllocating ( const unsigned int* indices, unsigned int faceCount )
{
    Face* faces = Allocate(faceCount);
    faces->Load ( indices, faceCount );
    return faces;
}
