#include "geometry.h"

using namespace l3m;

bool Geometry::Load(l3m::IStream& fp, float version)
{
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
    if ( fp.Write32 ( &numMeshes, 1 ) != 1 )
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
        unsigned int num = mesh->numVertices();
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
