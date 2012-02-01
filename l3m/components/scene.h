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
#include "l3m/components/geometry.h"
#include "l3m/components/camera.h"

namespace l3m
{

class Geometry;
    
class Scene : public IComponent
{
public:
    struct Node
    {
        std::string                     url;
        Renderer::Geometry*             geometry;
        Transform                       transform;
        std::vector<std::string>        textures;
        Node () : transform (IdentityTransform()) {}
    };
    typedef std::vector < Node > NodesVector;
    
    //--------------------------------------------------------------------------
    // Delta resolver for the nodes geometry
private:
    static bool     resolveNodeData     ( IComponent* comp, l3m::Model* model, void* data );
    
    //--------------------------------------------------------------------------
    // Delta resolver for the scene camera
private:
    static bool     resolveCamera       ( IComponent* comp, l3m::Model* model, void* data );
    
public:
    static IComponent* create ()
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
    
    bool                load                    ( l3m::Model*, l3m::IStream& fp, float version );
    bool                save                    ( l3m::Model*, l3m::OStream& fp );
    
    Node&               createGeometryNode      ();
    
    
    //--------------------------------------------------------------------------
    // Accessors
public:
    NodesVector&                geometryNodes           () { return m_geometryNodes; }
    l3m::Camera*&               camera                  () { return m_camera; }
    std::string&                cameraUrl               () { return m_cameraUrl; }
    u16&                        width                   () { return m_width; }
    u16&                        height                  () { return m_height; }
    
    //--------------------------------------------------------------------------
    // Constant accessors
public:
    const NodesVector&          geometryNodes           () const { return m_geometryNodes; }
    const l3m::Camera*          camera                  () const { return m_camera; }
    const std::string&          cameraUrl               () const { return m_cameraUrl; }
    const u16&                  width                   () const { return m_width; }
    const u16&                  height                  () const { return m_height; }
    


private:
    NodesVector         m_geometryNodes;
    std::string         m_cameraUrl;
    l3m::Camera*        m_camera;
    u16                 m_width;
    u16                 m_height;
};

}

