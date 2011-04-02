#include <cstring>
#include <cstdio>
#include <cstdlib>
#include "l3m.h"

l3m::l3m()
: groups ( 0 )
{
}

l3m::l3m(const l3m& orig)
: groups ( 0 )
{
}

l3m::~l3m()
{
    while ( groups != 0 )
    {
        __VertexGroup* next = groups->next;
        free(groups->vertices);
        delete groups;
        groups = next;
    }
}

void l3m::LoadGroup(const std::string& name, const Vertex* pVertices, unsigned int count)
{
    __VertexGroup* group = new __VertexGroup ( name );
    group->vertices = Vertex::LoadAllocating(pVertices, count);
    group->next = groups;
    groups = group;
}

void l3m::LoadGroup(const std::string& name, const float* pVertices, unsigned int flags, unsigned int stride, unsigned int count)
{
    __VertexGroup* group = new __VertexGroup ( name );
    group->vertices = Vertex::LoadAllocating(pVertices, flags, stride, count);
    group->next = groups;
    groups = group;
}
