#include "face.h"
#include <cstring>
#include <cstdlib>

void Face::Load ( const unsigned int* indices, unsigned int faceCount )
{
    memcpy ( &m_index, indices, sizeof(unsigned int) * 3 * faceCount );
}

Face* Face::LoadAllocating ( const unsigned int* indices, unsigned int faceCount )
{
    Face* faces = (Face *)malloc ( sizeof(Face) * faceCount );
    faces->Load ( indices, faceCount );
    return faces;
}
