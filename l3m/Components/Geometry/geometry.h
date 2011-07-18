#pragma once

#include <list>
#include <string>
#include "math/matrix.h"
#include "mesh.h"
#include "vertex.h"
#include "l3m/l3mComponent.h"

namespace l3m
{

class Geometry : public IComponent
{
public:
    typedef std::list<Mesh *> meshList;

private:
    std::string         m_name;
    Matrix              m_matrix;
    meshList            m_meshes;
    
public:
    static IComponent* Create ()
    {
        return new Geometry ( "" );
    }
    
    Geometry ( const std::string& name )
    : IComponent ( "geometry", 1.0f )
    , m_name ( name )
    , m_matrix ( IdentityMatrix() )
    {
    }
    
    ~Geometry ()
    {
        for ( meshList::iterator iter = m_meshes.begin(); iter != m_meshes.end(); ++iter )
            delete *iter;
    }
    
    bool        Load            ( l3m::IStream& fp, float version );
    bool        Save            ( l3m::OStream& fp );
    
    void LoadMesh ( Mesh* mesh )
    {
        m_meshes.push_back(mesh);
    }
    
    // Accessors
public:
    const std::string&          name    () const { return m_name; }
    const Matrix&               matrix  () const { return m_matrix; }
    const meshList&             meshes  () const { return m_meshes; }
    
    std::string&                name    () { return m_name; }
    Matrix&                     matrix  () { return m_matrix; }
    meshList&                   meshes  () { return m_meshes; } 
};

}


