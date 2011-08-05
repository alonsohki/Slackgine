#pragma once

#include <vector>
#include "math/matrix.h"
#include "l3m/component.h"
#include "l3m/Components/geometry.h"

namespace l3m
{

class Geometry;
    
class Scene : public IComponent
{
public:
    struct Node
    {
        std::string             url;
        Matrix                  transform;
        Node () : transform (IdentityMatrix()) {}
    };
    typedef std::vector < Node > nodesVector;
    
public:
    static IComponent* Create ()
    {
        return new Scene ();
    }
    
    Scene ()
    : IComponent ( "scene", 1.0f )
    {
    }
    
    ~Scene ()
    {
    }
    
    bool                Load                    ( l3m::IStream& fp, float version );
    bool                Save                    ( l3m::OStream& fp );
    
    Node&               CreateGeometryNode      ();
    
    
    // Accessors
public:
    const nodesVector&          geometryNodes           () const { return m_geometryNodes; }
    nodesVector&                geometryNodes           () { return m_geometryNodes; }


private:
    nodesVector         m_geometryNodes;
};

}

