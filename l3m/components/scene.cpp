/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        scene.cpp
// PURPOSE:     Visual scene definition component.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include "scene.h"
#include <l3m/util.h>

using namespace l3m;

bool Scene::load(l3m::Model* model, l3m::IStream& fp, float version)
{
    // Load the camera
    if ( fp.readStr ( m_cameraUrl ) < 0 )
        return setError ( "Error reading the scene camera" );
    model->registerDeltaResolver( this, resolveCamera, 0 );
    
    // Load the width and height
    if ( fp.read16(&m_width, 1) != 1 || fp.read16(&m_height, 1) != 1 )
        return setError ( "Error reading the scene width/height" );

    // Load the geometry nodes.
    u32 numGeometryNodes;
    if ( fp.read32 ( &numGeometryNodes, 1 ) != 1 )
        return setError ( "Error reading the number of geometry nodes" );
    
    geometryNodes().reserve(numGeometryNodes);
    for ( u32 i = 0; i < numGeometryNodes; ++i )
    {
        Scene::Node& node = CreateGeometryNode();
        
        if ( fp.readStr(node.url) < 1 )
            return setError ( "Error reading the geometry node url" );
        
        // Defer the geometry url resolution
        model->registerDeltaResolver(this, resolveNodeData, &node);
        
        if ( fp.readTransform ( &node.transform, 1 ) != 1 )
            return setError ( "Error reading the geometry node transform" );
        
        u32 numTextures;
        if ( fp.read32 ( &numTextures, 1 ) != 1 )
            return setError ( "Error reading the geometry node texture count" );
        for ( u32 t = 0; t < numTextures; ++t )
        {
            std::string texUrl;
            if ( fp.readStr(texUrl) < 1 )
                return setError ( "Error reading the geometry node texture url" );
            node.textures.push_back ( texUrl );
        }
    }
    
    return true;
}

bool Scene::save(l3m::Model*, l3m::OStream& fp)
{
    // Save the camera
    if ( !fp.writeStr ( m_cameraUrl ) )
        return setError ( "Error writing the scene camera" );
    
    // Save the width and height
    if ( !fp.write16(&m_width, 1) || !fp.write16(&m_height, 1) )
        return setError ( "Error writing the scene width and height" );

    // Save all the geometry nodes.
    u32 numGeometryNodes = m_geometryNodes.size();
    if ( !fp.write32 ( &numGeometryNodes, 1 ) )
        return setError ( "Unable to write the number of geometry nodes" );
    
    for ( NodesVector::const_iterator i = m_geometryNodes.begin();
          i != m_geometryNodes.end();
          ++i )
    {
        const Scene::Node& node = *i;
        
        if ( !fp.writeStr ( node.url ) )
            return setError ( "Unable to write the geometry node URL" );
        if ( !fp.writeTransform ( &node.transform, 1 ) )
            return setError ( "Unable to write the geometry node transform" );

        u32 numTextures = node.textures.size();
        if ( !fp.write32 (&numTextures, 1) )
            return setError ( "Unable to write the geometry node texture count" );
        for ( u32 i = 0; i < numTextures; ++i )
        {
            if ( !fp.writeStr(node.textures[i]) )
                return setError ( "Unable to write the geometry node texture url" );
        }
    }
    
    return true;
}

Scene::Node& Scene::CreateGeometryNode ()
{
    m_geometryNodes.push_back ( Node() );
    return m_geometryNodes.back ();
}

bool Scene::resolveNodeData(IComponent* comp, l3m::Model* model, void* data)
{
    Node* node = (Node*)data;

    l3m::Geometry* g = l3m::Util::findGeometry ( model, node->url );
    node->geometry = 0;
    if ( g != 0 )
        node->geometry = &g->geometry ();
    
    return true;
}

bool Scene::resolveCamera(IComponent* comp, l3m::Model* model, void* data)
{
    Scene* sce = (Scene *)comp;
    sce->camera() = l3m::Util::findCamera ( model, sce->m_cameraUrl );
    
    return true;
}
