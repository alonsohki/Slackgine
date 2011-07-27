#pragma once

#include <vector>
#include "math/matrix.h"
#include "l3m/l3mComponent.h"
#include "l3m/Components/Geometry/geometry.h"

namespace l3m
{

class Geometry;
    
class Scene : public IComponent
{
private:
    template < typename T >
    struct Node
    {
        T*              ref;
        std::string     name;
        Matrix          transform;
        
        Node () : ref ( 0 ), transform (IdentityMatrix()) {}
    };
    
    std::vector < Node<Geometry> >      m_geometryNodes;
    
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
    
    bool        Load            ( l3m::IStream& fp, float version );
    bool        Save            ( l3m::OStream& fp );
};

}

