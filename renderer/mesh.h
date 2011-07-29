#pragma once


#include <string>
#include <cstdlib>
#include "renderer/vertex.h"

namespace Renderer
{

class Mesh
{
public:
    enum PolygonType
    {
        TRIANGLES,
        TRIANGLE_STRIP,
        TRIANGLE_FAN,
        QUADS
    };

private:
    std::string         m_name;
    unsigned int*       m_indices;
    unsigned int        m_numIndices;
    PolygonType         m_polyType;
    
public:
                                Mesh ();
    virtual                     ~Mesh ();
    
private:
    void FreeIndices ();
    
    // Accessors
public:
    const std::string&          name            () const { return m_name; }
    const unsigned int*         indices         () const { return m_indices; }
    const unsigned int&         numIndices      () const { return m_numIndices; }
    const PolygonType&          polyType        () const { return m_polyType; }
    
    std::string&                name            () { return m_name; }
    unsigned int*&              indices         () { return m_indices; }
    PolygonType&                polyType        () { return m_polyType; }

    // Loads the face data, allocating space for it.
    void Load ( const unsigned int* pIndices, unsigned int indexCount, PolygonType polyType = TRIANGLES );
    
    // Loads the face data, storing the pointer to the previously allocated data.
    void Set ( unsigned int* pIndices, unsigned int indexCount,
               PolygonType polyType = TRIANGLES );
};

}

