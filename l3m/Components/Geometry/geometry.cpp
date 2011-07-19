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
    const meshList& meshes = meshes();
    u32 numMeshes = meshes.size ();
    if ( fp.Write32 ( &numMeshes, 1 ) != 1 )
        return SetError ( "Error writing the number of meshes" );

    for ( Geometry::meshList::const_iterator iter2 = meshes.begin(); iter2 != meshes.end(); ++iter2 )
    {
        Mesh* mesh = *iter2;

        // Write the mesh name
        FWRITE_STR ( mesh->name(), fp, ERROR_WRITING_MESH_NAME );

        // Write the polygon type
        FWRITE32 ( &(mesh->polyType()), 1, fp, ERROR_WRITING_POLYGON_TYPE );

        // Write the material data
        const Material& mat = mesh->material();
        FWRITE32 ( (u32*)&mat, 4, fp, ERROR_WRITING_MATERIAL_COLORS );

        // Write the vertex data
        unsigned int num = mesh->numVertices();
        FWRITE32 ( &num, 1, fp, ERROR_WRITING_VERTEX_COUNT );
        FWRITEF ( mesh->vertices(), (num * sizeof(Vertex)) / sizeof(float), fp, ERROR_WRITING_VERTEX_DATA );

        // Write the index data
        num = mesh->numIndices();
        FWRITE32 ( &num, 1, fp, ERROR_WRITING_INDEX_COUNT );
        FWRITE32 ( mesh->indices(), (num * sizeof(u32)) / sizeof(u32), fp, ERROR_WRITING_INDEX_DATA );
    }
    
    return true;
}
