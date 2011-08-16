/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011 - Alberto Alonso
//
// Redistribution of this software, including any of its
// files, in either binary or source code form is totally
// forbidden without the permission of its owners.
//
// Owners reserve their right to legal action against any
// person or company that fails to do so.
//
// For more detailed information, see the LICENSE file in
// the top-level directory.
//
// FILE:        scene.h
// PURPOSE:     Visual scene definition component.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

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

