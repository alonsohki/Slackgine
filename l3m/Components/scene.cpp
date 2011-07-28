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