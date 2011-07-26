#include "geometry.h"

using namespace l3m;

bool Geometry::Load(l3m::IStream& fp, float version)
{
    // Geometry name
    fp.ReadStr( name() );
    
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
        
        // Read the vertex data
        u32 numVertices;
        if ( fp.Read32 ( &numVertices, 1 ) != 1 )
            return SetError ( "Error reading the vertex count" );
        Vertex* vertices = ( Vertex* )malloc ( sizeof(Vertex) * numVertices );
        if ( fp.ReadFloat(reinterpret_cast<float*>(vertices), numVertices*sizeof(Vertex)/sizeof(float)) != numVertices*sizeof(Vertex)/sizeof(float) )
            return SetError ( "Error reading the vertex data" );
        
        // Read the index data
        u32 numIndices;
        if ( fp.Read32 ( &numIndices, 1 ) != 1 )
            return SetError ( "Error reading the index count" );
        u32* indices = ( u32* )malloc ( sizeof(u32) * numIndices );
        if ( fp.Read32 ( indices, numIndices ) != numIndices )
            return SetError ( "Error reading the index data" );
        
        // Create the mesh
        Mesh* mesh = new Mesh ( name );
        mesh->Set ( vertices, numVertices, indices, numIndices, polyType );
        this->LoadMesh( mesh );
    }
    
    return true;
}

bool Geometry::Save(l3m::OStream& fp)
{
    // Geometry name
    if ( !fp.WriteStr( name() ) )
        return SetError ( "Error writing the geometry name" );

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
        
        // Write the vertex data
        u32 num = mesh->numVertices();
        if ( ! fp.Write32 ( &num, 1 ) )
            return SetError ( "Error writing the vertex count" );
        if ( ! fp.WriteFloat ( reinterpret_cast<float *>(mesh->vertices()), num*sizeof(Vertex) / sizeof(float) ) )
            return SetError ( "Error writing the vertex data" );

        // Write the index data
        num = mesh->numIndices();
        if ( ! fp.Write32( &num, 1 ) )
            return SetError ( "Error writing the index count" );
        if ( ! fp.Write32(mesh->indices(), num ) )
            return SetError ( "Error writing the index data" );
    }
    
    return true;
}
