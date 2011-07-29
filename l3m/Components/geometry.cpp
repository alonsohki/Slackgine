#include "geometry.h"

using namespace l3m;
using namespace Renderer;

Geometry::Geometry ( const std::string& name )
: IComponent ( "geometry", 1.0f )
, m_name ( name )
{
    m_mainVertexLayer.vertices = 0;
    m_mainVertexLayer.numVertices = 0;
}

Geometry::~Geometry ()
{
    for ( meshList::iterator iter = m_meshes.begin(); iter != m_meshes.end(); ++iter )
        delete *iter;
    FreeVertices ();
    
    // Delete all the vertex layers
    for ( layerMap::iterator iter = m_mapVertexLayers.begin ();
          iter != m_mapVertexLayers.end ();
          ++iter )
    {
        free ( iter->second.vertices );
    }
}

void Geometry::FreeVertices()
{
    if ( m_mainVertexLayer.vertices != 0 )
        free ( m_mainVertexLayer.vertices );
    m_mainVertexLayer.vertices = 0;
    m_mainVertexLayer.numVertices = 0;
}

void Geometry::Load(const float* pVertices, unsigned int flags, unsigned int stride, unsigned int vertexCount)
{
    FreeVertices ();
    m_mainVertexLayer.vertices = Vertex::LoadAllocating(pVertices, flags, stride, vertexCount);
    m_mainVertexLayer.numVertices = vertexCount;
}

void Geometry::Set(Vertex* pVertices, unsigned int vertexCount)
{
    if ( pVertices != m_mainVertexLayer.vertices )
    {
        FreeVertices ();
        m_mainVertexLayer.vertices = pVertices;
    }
    m_mainVertexLayer.numVertices = vertexCount;
}

void Geometry::LoadMesh ( Renderer::Mesh* mesh )
{
    m_meshes.push_back(mesh);
}


// Vertex layers
bool Geometry::CreateVertexLayer(const std::string& name, Renderer::Vertex* vertices, u32 vertexCount)
{
    layerMap::iterator iter = m_mapVertexLayers.find ( name );
    if ( iter != m_mapVertexLayers.end() )
        return false;

    VertexLayer layer;
    layer.numVertices = vertexCount;
    layer.vertices = vertices;
    m_mapVertexLayers.insert ( layerMap::value_type ( name, layer ) );
    
    return true;
}

bool Geometry::DeleteVertexLayer(const std::string& name)
{
    layerMap::iterator iter = m_mapVertexLayers.find ( name );
    if ( iter == m_mapVertexLayers.end() )
        return false;
    
    free ( iter->second.vertices );
    m_mapVertexLayers.erase ( iter );
}

Vertex* Geometry::vertices ( const std::string& layerName ) const
{
    Vertex* vertices = 0;
    layerMap::const_iterator iter = m_mapVertexLayers.find ( layerName );
    if ( iter != m_mapVertexLayers.end () )
        vertices = iter->second.vertices;
    return vertices;
}

u32 Geometry::numVertices ( const std::string& layerName ) const
{
    u32 num = 0;
    layerMap::const_iterator iter = m_mapVertexLayers.find ( layerName );
    if ( iter != m_mapVertexLayers.end () )
        num = iter->second.numVertices;
    return num;
}


// Load/Save
bool Geometry::Load(l3m::IStream& fp, float version)
{
    // Geometry name
    if ( fp.ReadStr( name() ) == -1 )
        return SetError ( "Error reading the geometry name" );
    
    // Read the vertex data
    u32 numVertices;
    if ( fp.Read32 ( &numVertices, 1 ) != 1 )
        return SetError ( "Error reading the vertex count" );
    Vertex* vertices = ( Vertex* )malloc ( sizeof(Vertex) * numVertices );
    if ( fp.ReadFloat(vertices->base(), numVertices*sizeof(Vertex)/sizeof(float)) != numVertices*sizeof(Vertex)/sizeof(float) )
        return SetError ( "Error reading the vertex data" );

    // Vertex layers
    u32 numLayers;
    if ( fp.Read32 ( &numLayers, 1 ) != 1 )
        return SetError ( "Error reading the vertex layer count" );
    for ( u32 i = 0; i < numLayers; ++i )
    {
        // Vertex layer name
        std::string name;
        if ( fp.ReadStr ( name ) == -1 )
            return SetError ( "Error reading the vertex layer name" );
        
        // Vertex layer data
        if ( fp.Read32 ( &numVertices, 1 ) != 1 )
            return SetError ( "Error reading the vertex layer vertex count" );
        vertices = ( Vertex* )malloc ( sizeof(Vertex) * numVertices );
        if ( fp.ReadFloat(vertices->base(), numVertices*sizeof(Vertex)/sizeof(f32)) != numVertices*sizeof(Vertex)/sizeof(float) )
            return SetError ( "Error reading the vertex layer vertex data" );
        
        // Insert it.
        CreateVertexLayer ( name, vertices, numVertices );
    }
    
    // Read the meshes headers
    u32 numMeshes;
    if ( fp.Read32 ( &numMeshes, 1 ) != 1 )
        return SetError ( "Error reading the number of meshes" );
    
    // Read the meshes
    for ( u32 i = 0; i < numMeshes; ++i )
    {
        // Read the mesh name
        std::string name;
        fp.ReadStr(name);
        
        // Read the polygon type
        u32 polyType_;
        if ( fp.Read32 ( &polyType_, 1 ) != 1 )
            return SetError ( "Error reading the polygon type" );
        Mesh::PolygonType polyType = static_cast < Mesh::PolygonType > ( polyType_ );
        
        // Read the index data
        u32 numIndices;
        if ( fp.Read32 ( &numIndices, 1 ) != 1 )
            return SetError ( "Error reading the index count" );
        u32* indices = ( u32* )malloc ( sizeof(u32) * numIndices );
        if ( fp.Read32 ( indices, numIndices ) != numIndices )
            return SetError ( "Error reading the index data" );
        
        // Create the mesh
        Mesh* mesh = new Mesh ();
        mesh->name () = name;
//        mesh->Set ( vertices, numVertices, indices, numIndices, polyType );
        this->LoadMesh( mesh );
    }
    
    return true;
}

bool Geometry::Save(l3m::OStream& fp)
{
    // Geometry name
    if ( !fp.WriteStr( name() ) )
        return SetError ( "Error writing the geometry name" );
    
    // Geometry vertices
    u32 num = numVertices();
    if ( ! fp.Write32 ( &num, 1 ) )
        return SetError ( "Error writing the vertex count" );
    if ( ! fp.WriteFloat ( vertices()->base(), num * sizeof(Vertex) / sizeof(f32) ) )
        return SetError ( "Error writing the vertex data" );
    
    // Vertex layers
    num = m_mapVertexLayers.size();
    if ( ! fp.Write32 ( &num, 1 ) )
        return SetError ( "Error writing the vertex layer count" );
    for ( layerMap::const_iterator iter = m_mapVertexLayers.begin();
          iter != m_mapVertexLayers.end();
          ++iter )
    {
        const std::string& name = iter->first;
        const VertexLayer& layer = iter->second;
        
        // Write the vertex layer name
        if ( !fp.WriteStr ( name ) )
            return SetError ( "Error writing the vertex layer name" );
        
        // Write the vertex layer data
        num = layer.numVertices;
        if ( ! fp.Write32 ( &num, 1 ) )
            return SetError ( "Error writing the vertex layer vertex count" );
        if ( ! fp.WriteFloat ( layer.vertices->base(), num*sizeof(Vertex)/sizeof(f32) ) )
            return SetError ( "Error writing the vertex layer vertex data" );
    }

    // Write the meshes headers
    const meshList& meshes = this->meshes();
    u32 numMeshes = meshes.size ();
    if ( ! fp.Write32 ( &numMeshes, 1 ) )
        return SetError ( "Error writing the number of meshes" );

    for ( Geometry::meshList::const_iterator iter2 = meshes.begin(); iter2 != meshes.end(); ++iter2 )
    {
        Mesh* mesh = *iter2;

        // Write the mesh name
        if ( ! fp.WriteStr ( mesh->name() ) )
            return SetError ( "Error writing the mesh name" );

        // Write the polygon type
        u32 polyType = mesh->polyType();
        if ( ! fp.Write32 ( &polyType, 1 ) )
            return SetError ( "Error writing the mesh polygon type" );
        
        // Write the index data
        num = mesh->numIndices();
        if ( ! fp.Write32( &num, 1 ) )
            return SetError ( "Error writing the index count" );
        if ( ! fp.Write32(mesh->indices(), num ) )
            return SetError ( "Error writing the index data" );
    }
    
    return true;
}
