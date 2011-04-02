#include <arpa/inet.h>
#include <cerrno>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include "l3m.h"

l3m::l3m ( const std::string& type )
: m_groups ( 0 )
, m_type ( type )
{
}

l3m::l3m(const l3m& orig)
: m_groups ( 0 )
, m_type ( orig.m_type )
{
    // TODO
}

l3m::~l3m()
{
    while ( m_groups != 0 )
    {
        __VertexGroup* next = m_groups->next;
        free(m_groups->vertices);
        delete m_groups;
        m_groups = next;
    }
}

static bool WriteData ( const void* data, size_t size, unsigned int nmemb, FILE* fp )
{
    size_t s = fwrite ( data, size, nmemb, fp );
    if ( s < nmemb )
        return false;
    return true;
}

bool l3m::SaveToFile ( FILE* fp ) const
{
    
#define FWRITE(data, size, nmemb, fp) if ( ! WriteData(data, size, nmemb, fp) ) return false
#define FWRITE_STR(str, fp) { \
                unsigned int __size__ = htonl(str .length()); \
                FWRITE(&__size__, sizeof(unsigned int), 1, fp ); \
            } \
            FWRITE ( str .c_str(), sizeof(char), str .length(), fp )

    FWRITE ( L3M_BOM, sizeof(char), strlen(L3M_BOM), fp );
    float fVersion = L3M_VERSION;
    FWRITE ( &fVersion, sizeof(float), 1, fp );
    FWRITE_STR ( m_type, fp );

#undef FWRITE
#undef FWRITE_STR
    
    return true;
}

bool l3m::SaveToFile ( const char* path ) const
{
    FILE* fp = fopen ( path, "wb" );
    bool status;
    if ( !fp ) return false;
    
    status = SaveToFile(fp);
    fclose ( fp );
    return status;
}

l3m::VertexGroup* l3m::FindGroup ( const std::string& name ) const
{
    VertexGroup* group = m_groups;
    while ( group != 0 )
    {
        if ( group->name == name )
            return group;
        group = group->next;
    }
    return 0;
}


void l3m::LoadGroup(const std::string& name, const Vertex* pVertices, unsigned int count)
{
    __VertexGroup* group = new __VertexGroup ( name );
    group->vertices = Vertex::LoadAllocating(pVertices, count);
    group->next = m_groups;
    m_groups = group;
}

void l3m::LoadGroup(const std::string& name, const float* pVertices, unsigned int flags, unsigned int stride, unsigned int count)
{
    __VertexGroup* group = new __VertexGroup ( name );
    group->vertices = Vertex::LoadAllocating(pVertices, flags, stride, count);
    group->next = m_groups;
    m_groups = group;
}
