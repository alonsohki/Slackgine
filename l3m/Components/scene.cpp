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
// FILE:        scene.cpp
// PURPOSE:     Visual scene definition component.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include "scene.h"

using namespace l3m;

bool Scene::Load(l3m::IStream& fp, float version)
{
    // Load the geometry nodes.
    u32 numGeometryNodes;
    if ( fp.Read32 ( &numGeometryNodes, 1 ) != 1 )
        return SetError ( "Error reading the number of geometry nodes" );
    
    for ( u32 i = 0; i < numGeometryNodes; ++i )
    {
        Scene::Node& node = CreateGeometryNode();
        
        fp.ReadStr(node.url);
        if ( fp.ReadMatrix ( node.transform ) != 1 )
            return SetError ( "Error reading the geometry node transform" );
    }
    
    return true;
}

bool Scene::Save(l3m::OStream& fp)
{
    // Save all the geometry nodes.
    u32 numGeometryNodes = m_geometryNodes.size();
    if ( !fp.Write32 ( &numGeometryNodes, 1 ) )
        return SetError ( "Unable to write the number of geometry nodes" );
    
    for ( nodesVector::const_iterator i = m_geometryNodes.begin();
          i != m_geometryNodes.end();
          ++i )
    {
        const Scene::Node& node = *i;
        
        if ( !fp.WriteStr ( node.url ) )
            return SetError ( "Unable to write the geometry node URL" );
        if ( !fp.WriteMatrix( node.transform ) )
            return SetError ( "Unable to write the geometry node transform" );
    }
    
    return true;
}

Scene::Node& Scene::CreateGeometryNode ()
{
    m_geometryNodes.push_back ( Node() );
    return m_geometryNodes.back ();
}