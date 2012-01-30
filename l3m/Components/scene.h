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
        m_width = m_height = 0;
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
    u16&                        width                   () { return m_width; }
    u16&                        height                  () { return m_height; }
    
    //--------------------------------------------------------------------------
    // Constant accessors
public:
    const nodesVector&          geometryNodes           () const { return m_geometryNodes; }
    const std::string&          camera                  () const { return m_camera; }
    const u16&                  width                   () const { return m_width; }
    const u16&                  height                  () const { return m_height; }
    


private:
    nodesVector         m_geometryNodes;
    std::string         m_camera;
    u16                 m_width;
    u16                 m_height;
};

}

