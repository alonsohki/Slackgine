#pragma once

#include <list>
#include <string>
#include "l3m/component.h"
#include "renderer/mesh.h"

namespace l3m
{

class Geometry : public IComponent
{
public:
    typedef std::list<Renderer::Mesh *> meshList;

private:
    std::string         m_name;
    meshList            m_meshes;
    
public:
    static IComponent* Create ()
    {
        return new Geometry ( "" );
    }
    
    Geometry ( const std::string& name )
    : IComponent ( "geometry", 1.0f )
    , m_name ( name )
    {
    }
    
    ~Geometry ()
    {
        for ( meshList::iterator iter = m_meshes.begin(); iter != m_meshes.end(); ++iter )
            delete *iter;
    }
    
    bool        Load            ( l3m::IStream& fp, float version );
    bool        Save            ( l3m::OStream& fp );
    
    void LoadMesh ( Renderer::Mesh* mesh )
    {
        m_meshes.push_back(mesh);
    }
    
    // Accessors
public:
    const std::string&          name    () const { return m_name; }
    const meshList&             meshes  () const { return m_meshes; }
    
    std::string&                name    () { return m_name; }
    meshList&                   meshes  () { return m_meshes; } 
};

}


