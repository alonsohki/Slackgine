/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        scene.h
// PURPOSE:     Visual scene definition component.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#include <vector>
#include "math/transform.h"
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
        std::string                     url;
        Transform                       transform;
        std::vector<std::string>        textures;
        Node () : transform (IdentityTransform()) {}
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
    
    bool                Load                    ( l3m::Model*, l3m::IStream& fp, float version );
    bool                Save                    ( l3m::Model*, l3m::OStream& fp );
    
    Node&               CreateGeometryNode      ();
    
    
    //--------------------------------------------------------------------------
    // Accessors
public:
    nodesVector&                geometryNodes           () { return m_geometryNodes; }
    std::string&                camera                  () { return m_camera; }
    
    //--------------------------------------------------------------------------
    // Constant accessors
public:
    const nodesVector&          geometryNodes           () const { return m_geometryNodes; }
    const std::string&          camera                  () const { return m_camera; }
    


private:
    nodesVector         m_geometryNodes;
    std::string         m_camera;
};

}

