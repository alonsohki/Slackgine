#include "geometry.h"
#include "renderer/vertex.h"
#include "renderer/mesh.h"

using namespace l3m;
using Renderer::Vertex;
using Renderer::Mesh;

Geometry::Geometry()
: IComponent ( "geometry", 1.0f )
{
}

Geometry::~Geometry ()
{
}


// Load/Save
bool Geometry::Load(l3m::IStream& fp, float version)
{
    // Geometry name
    if ( fp.ReadStr( m_geometry.name() ) == -1 )
        return SetError ( "Error reading the geometry name" );
    
    // Geometry boundaries
    float values[7];
    if ( fp.ReadFloat(values, 7) != 7 )
        return SetError ( "Error reading the geometry boundary data" );
    m_geometry.boundingBox() = BoundingBox ( values[0], values[1], values[2], values[3], values[4], values[5] );
    m_geometry.boundingSphere() = BoundingSphere ( values[6] );
    
    // Read the vertex data
    u32 numVertices;
    if ( fp.Read32 ( &numVertices, 1 ) != 1 )
        return SetError ( "Error reading the vertex count" );
    Vertex* vertices = ( Vertex* )malloc ( sizeof(Vertex) * numVertices );
    if ( fp.ReadFloat(vertices->base(), numVertices*sizeof(Vertex)/sizeof(float)) != numVertices*sizeof(Vertex)/sizeof(float) )
        return SetError ( "Error reading the vertex data" );
    m_geometry.Set ( vertices, numVertices );

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
        u32 elementSize;
        if ( fp.Read32 ( &elementSize, 1 ) != 1 )
            return SetError ( "Error reading the vertex layer element size" );
        void* data = m_geometry.CreateVertexLayer ( name, 0, elementSize );
        if ( fp.ReadData ( reinterpret_cast < char * > ( data ), elementSize, numVertices ) != numVertices )
            return SetError ( "Error reading the vertex layer data" );
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
        mesh->Set ( indices, numIndices, polyType );
        m_geometry.LoadMesh( mesh );
    }
    
    return true;
}

bool Geometry::Save(l3m::OStream& fp)
{
    // Geometry name
    if ( !fp.WriteStr( m_geometry.name() ) )
        return SetError ( "Error writing the geometry name" );
    
    // Geometry boundaries
    float values[7];
    BoundingBox& bbox = m_geometry.boundingBox();
    values[0] = bbox.minX(); values[1] = bbox.maxX();
    values[2] = bbox.minY(); values[3] = bbox.maxY();
    values[4] = bbox.minZ(); values[5] = bbox.maxZ();
    values[6] = m_geometry.boundingSphere().radius();
    if ( ! fp.WriteFloat ( values, 7 ) )
        return SetError ( "Error writing the geometry boundary data" );
    
    // Geometry vertices
    u32 num = m_geometry.numVertices();
    if ( ! fp.Write32 ( &num, 1 ) )
        return SetError ( "Error writing the vertex count" );
    if ( ! fp.WriteFloat ( m_geometry.vertices()->base(), num * sizeof(Vertex) / sizeof(f32) ) )
        return SetError ( "Error writing the vertex data" );
    
    // Vertex layers
    const Renderer::Geometry::layerMap& vertexLayers = m_geometry.vertexLayers ();
    num = vertexLayers.size();
    if ( ! fp.Write32 ( &num, 1 ) )
        return SetError ( "Error writing the vertex layer count" );
    for ( Renderer::Geometry::layerMap::const_iterator iter = vertexLayers.begin();
          iter != vertexLayers.end();
          ++iter )
    {
        const std::string& name = iter->first;
        const Renderer::Geometry::VertexLayer& layer = iter->second;
        
        // Write the vertex layer name
        if ( !fp.WriteStr ( name ) )
            return SetError ( "Error writing the vertex layer name" );
        
        // Write the vertex layer data
        u32 size = layer.elementSize;
        if ( ! fp.Write32 ( &size, 1 ) )
            return SetError ( "Error writing the vertex layer element size" );
        if ( ! fp.WriteData ( reinterpret_cast<char*>(layer.data), size, m_geometry.numVertices() ) )
            return SetError ( "Error writing the vertex layer data" );
    }

    // Write the meshes headers
    const Renderer::Geometry::meshList& meshes = m_geometry.meshes();
    u32 numMeshes = meshes.size ();
    if ( ! fp.Write32 ( &numMeshes, 1 ) )
        return SetError ( "Error writing the number of meshes" );

    for ( Renderer::Geometry::meshList::const_iterator iter2 = meshes.begin(); iter2 != meshes.end(); ++iter2 )
    {
        Renderer::Mesh* mesh = *iter2;

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
