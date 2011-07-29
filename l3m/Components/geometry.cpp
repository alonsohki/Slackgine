#include "geometry.h"

using namespace l3m;
using namespace Renderer;

Geometry::Geometry ( const std::string& name )
: IComponent ( "geometry", 1.0f )
, m_name ( name )
, m_vertices ( 0 )
, m_numVertices ( 0 )
{
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
        free ( iter->second.data );
    }
}

void Geometry::FreeVertices()
{
    if ( m_vertices != 0 )
        free ( m_vertices );
    m_vertices = 0;
    m_numVertices = 0;
}

void Geometry::Load(const float* pVertices, unsigned int flags, unsigned int stride, unsigned int vertexCount)
{
    FreeVertices ();
    m_vertices = Vertex::LoadAllocating(pVertices, flags, stride, vertexCount);
    m_numVertices = vertexCount;
}

void Geometry::Set(Vertex* pVertices, unsigned int vertexCount)
{
    if ( pVertices != m_vertices )
    {
        FreeVertices ();
        m_vertices = pVertices;
    }
    m_numVertices = vertexCount;
}

void Geometry::LoadMesh ( Renderer::Mesh* mesh )
{
    m_meshes.push_back(mesh);
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
    this->Set ( vertices, numVertices );

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
        void* data = CreateVertexLayer ( name, 0, elementSize );
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
        u32 size = layer.elementSize;
        if ( ! fp.Write32 ( &size, 1 ) )
            return SetError ( "Error writing the vertex layer element size" );
        if ( ! fp.WriteData ( reinterpret_cast<char*>(layer.data), size, numVertices() ) )
            return SetError ( "Error writing the vertex layer data" );
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
