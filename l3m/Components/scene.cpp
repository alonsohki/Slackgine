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
    // Load the camera
    if ( fp.ReadStr ( m_camera ) < 0 )
        return SetError ( "Error reading the scene camera" );

    // Load the geometry nodes.
    u32 numGeometryNodes;
    if ( fp.Read32 ( &numGeometryNodes, 1 ) != 1 )
        return SetError ( "Error reading the number of geometry nodes" );
    
    for ( u32 i = 0; i < numGeometryNodes; ++i )
    {
        Scene::Node& node = CreateGeometryNode();
        
        if ( fp.ReadStr(node.url) < 1 )
            return SetError ( "Error reading the geometry node url" );
        if ( fp.ReadTransform ( &node.transform, 1 ) != 1 )
            return SetError ( "Error reading the geometry node transform" );
        
        u32 numTextures;
        if ( fp.Read32 ( &numTextures, 1 ) != 1 )
            return SetError ( "Error reading the geometry node texture count" );
        for ( u32 t = 0; t < numTextures; ++t )
        {
            std::string texUrl;
            if ( fp.ReadStr(texUrl) < 1 )
                return SetError ( "Error reading the geometry node texture url" );
            node.textures.push_back ( texUrl );
        }
    }
    
    return true;
}

bool Scene::Save(l3m::OStream& fp)
{
    // Save the camera
    if ( !fp.WriteStr ( m_camera ) )
        return SetError ( "Error writing the scene camera" );

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
        if ( !fp.WriteTransform ( &node.transform, 1 ) )
            return SetError ( "Unable to write the geometry node transform" );

        u32 numTextures = node.textures.size();
        if ( !fp.Write32 (&numTextures, 1) )
            return SetError ( "Unable to write the geometry node texture count" );
        for ( u32 i = 0; i < numTextures; ++i )
        {
            if ( !fp.WriteStr(node.textures[i]) )
                return SetError ( "Unable to write the geometry node texture url" );
        }
    }
    
    return true;
}

Scene::Node& Scene::CreateGeometryNode ()
{
    m_geometryNodes.push_back ( Node() );
    return m_geometryNodes.back ();
}