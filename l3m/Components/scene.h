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
    template < typename T >
    struct Node
    {
        T*              ref;
        std::string     url;
        Matrix          transform;
        Node () : ref ( 0 ), transform (IdentityMatrix()) {}
    };
    typedef std::vector < Node<Geometry> > geometryNodesVector;
    
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
    
    Node<Geometry>&     CreateGeometryNode      ();


private:
    geometryNodesVector         m_geometryNodes;
};

}

