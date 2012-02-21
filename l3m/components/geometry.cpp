/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        geometry.cpp
// PURPOSE:     Geometry component.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include "geometry.h"
#include "renderer/vertex.h"
#include "renderer/mesh.h"
#include "material.h"
#include "l3m/util.h"

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
bool Geometry::load(l3m::Model* model, l3m::IStream& fp, float version)
{
    // Geometry name
    if ( fp.readStr( &m_geometry.name() ) == -1 )
        return setError ( "Error reading the geometry name" );
    
    // Geometry boundaries
    float values[7];
    if ( fp.readFloat(values, 7) != 7 )
        return setError ( "Error reading the geometry boundary data" );
    m_geometry.boundingBox() = BoundingBox ( values[0], values[1], values[2], values[3], values[4], values[5] );
    m_geometry.boundingSphere() = BoundingSphere ( values[6] );
    
    // Geometry centroid
    if ( fp.readVector(&m_geometry.centroid()) != 1 )
        return setError ( "Error reading the geometry centroid" );
    
    // Geometry armature pose
    b8 hasPose;
    if ( !fp.readBoolean(&hasPose) )
        return setError ( "Error reading the geometry pose" );
    if ( hasPose )
    {
        if ( !fp.readStr(&m_poseUrl) )
            return setError ( "Error reading the geometry pose url" );
        model->registerDeltaResolver( this, Geometry::resolvePoseDelta, 0 );
    }
    
    // Read the vertex data
    u32 numVertices;
    if ( fp.read32 ( &numVertices, 1 ) != 1 )
        return setError ( "Error reading the vertex count" );
    Vertex* vertices = ( Vertex* )malloc ( sizeof(Vertex) * numVertices );
    if ( fp.readFloat(vertices->base(), numVertices*sizeof(Vertex)/sizeof(float)) != (ssize_t)(numVertices*sizeof(Vertex)/sizeof(float)) )
        return setError ( "Error reading the vertex data" );
    m_geometry.set ( vertices, numVertices );

    // Vertex layers
    u32 numLayers;
    if ( fp.read32 ( &numLayers, 1 ) != 1 )
        return setError ( "Error reading the vertex layer count" );
    for ( u32 i = 0; i < numLayers; ++i )
    {
        // Vertex layer name
        std::string name;
        if ( fp.readStr ( &name ) == -1 )
            return setError ( "Error reading the vertex layer name" );
        
        // Vertex layer level count
        u32 numLevels;
        if ( fp.read32 ( &numLevels, 1 ) != 1 )
            return setError ( "Error reading the vertex layer level count" );
        
        // Vertex layer element size
        u32 elementSize;
        if ( fp.read32 ( &elementSize, 1 ) != 1 )
            return setError ( "Error reading the vertex layer element size" );
        
        // Vertex layer data
        void* data = m_geometry.createVertexLayer ( name, numLevels, 0, elementSize );
        if ( fp.readData ( reinterpret_cast < char * > ( data ), elementSize, numLevels*numVertices ) != (ssize_t)(numLevels*numVertices) )
            return setError ( "Error reading the vertex layer data" );
    }
    
    // Read the meshes headers
    u32 numMeshes;
    if ( fp.read32 ( &numMeshes, 1 ) != 1 )
        return setError ( "Error reading the number of meshes" );
    
    // Read the meshes
    for ( u32 i = 0; i < numMeshes; ++i )
    {
        // Read the mesh name
        std::string name;
        fp.readStr(&name);
        
        // Read the polygon type
        u32 polyType_;
        if ( fp.read32 ( &polyType_, 1 ) != 1 )
            return setError ( "Error reading the polygon type" );
        Mesh::PolygonType polyType = static_cast < Mesh::PolygonType > ( polyType_ );
        
        // Read the material
        b8 thereIsMaterial;
        std::string materialName;
        
        if ( !fp.readBoolean(&thereIsMaterial) )
            return setError ( "Error reading the material" );
        if ( thereIsMaterial )
        {
            if ( fp.readStr ( &materialName ) == 0 )
                return setError ( "Error reading the material name" );
        }
        
        // Read the index data
        u32 numIndices;
        if ( fp.read32 ( &numIndices, 1 ) != 1 )
            return setError ( "Error reading the index count" );
        u32* indices = ( u32* )malloc ( sizeof(u32) * numIndices );
        if ( fp.read32 ( indices, numIndices ) != (ssize_t)numIndices )
            return setError ( "Error reading the index data" );
        
        // Create the mesh
        Mesh* mesh = new Mesh ();
        mesh->name () = name;
        mesh->set ( indices, numIndices, polyType );
        m_geometry.loadMesh( mesh );
        
        // Defer the material reference
        if ( thereIsMaterial )
        {
            MaterialDeltaData* matDelta = new MaterialDeltaData ();
            matDelta->name = materialName;
            matDelta->mesh = mesh;
            model->registerDeltaResolver ( this, Geometry::resolveMaterialDelta, matDelta );
        }
    }
    
    return true;
}

bool Geometry::save(l3m::Model*, l3m::OStream& fp)
{
    // Geometry name
    if ( !fp.writeStr( m_geometry.name() ) )
        return setError ( "Error writing the geometry name" );
    
    // Geometry boundaries
    float values[7];
    BoundingBox& bbox = m_geometry.boundingBox();
    values[0] = bbox.minX(); values[1] = bbox.maxX();
    values[2] = bbox.minY(); values[3] = bbox.maxY();
    values[4] = bbox.minZ(); values[5] = bbox.maxZ();
    values[6] = m_geometry.boundingSphere().radius();
    if ( ! fp.writeFloat ( values, 7 ) )
        return setError ( "Error writing the geometry boundary data" );
    
    // Geometry centroid
    if ( ! fp.writeVector( m_geometry.centroid() ) )
        return setError ( "Error writing the geometry centroid" );
    
    // Geometry pose
    if ( m_poseUrl != "" )
    {
        fp.writeBoolean( true );
        if ( ! fp.writeStr ( m_poseUrl ) )
            return setError ( "Error writing the geometry pose url" );
    }
    else
        fp.writeBoolean( false );
    
    // Geometry vertices
    u32 num = m_geometry.numVertices();
    if ( ! fp.write32 ( &num, 1 ) )
        return setError ( "Error writing the vertex count" );
    if ( ! fp.writeFloat ( m_geometry.vertices()->base(), num * sizeof(Vertex) / sizeof(f32) ) )
        return setError ( "Error writing the vertex data" );
    
    // Vertex layers
    const Renderer::Geometry::layerMap& vertexLayers = m_geometry.vertexLayers ();
    num = vertexLayers.size();
    if ( ! fp.write32 ( &num, 1 ) )
        return setError ( "Error writing the vertex layer count" );
    for ( Renderer::Geometry::layerMap::const_iterator iter = vertexLayers.begin();
          iter != vertexLayers.end();
          ++iter )
    {
        const std::string& name = iter->first;
        const Renderer::Geometry::VertexLayer& layer = iter->second;
        
        // Write the vertex layer name
        if ( !fp.writeStr ( name ) )
            return setError ( "Error writing the vertex layer name" );
        
        // Write the vertex layer level count
        if ( !fp.write32 ( &layer.numLevels, 1 ) )
            return setError ( "Error writing the vertex layer level count" );
        
        // Write the vertex layer element size
        u32 size = layer.elementSize;
        if ( ! fp.write32 ( &size, 1 ) )
            return setError ( "Error writing the vertex layer element size" );
        
        // Write the vertex layer data
        if ( ! fp.writeData ( reinterpret_cast<char*>(layer.data), size, layer.numLevels*m_geometry.numVertices() ) )
            return setError ( "Error writing the vertex layer data" );
    }

    // Write the meshes headers
    const Renderer::Geometry::meshList& meshes = m_geometry.meshes();
    u32 numMeshes = meshes.size ();
    if ( ! fp.write32 ( &numMeshes, 1 ) )
        return setError ( "Error writing the number of meshes" );

    for ( Renderer::Geometry::meshList::const_iterator iter2 = meshes.begin(); iter2 != meshes.end(); ++iter2 )
    {
        Renderer::Mesh* mesh = *iter2;

        // Write the mesh name
        if ( ! fp.writeStr ( mesh->name() ) )
            return setError ( "Error writing the mesh name" );

        // Write the polygon type
        u32 polyType = mesh->polyType();
        if ( ! fp.write32 ( &polyType, 1 ) )
            return setError ( "Error writing the mesh polygon type" );
        
        // Write the material id
        if ( mesh->material() != 0 )
        {
            fp.writeBoolean ( true );
            if ( ! fp.writeStr ( mesh->material()->name() ) )
                return setError ( "Error writing the material name" );
        }
        else
            fp.writeBoolean ( false );
            
        
        // Write the index data
        num = mesh->numIndices();
        if ( ! fp.write32( &num, 1 ) )
            return setError ( "Error writing the index count" );
        if ( ! fp.write32(mesh->indices(), num ) )
            return setError ( "Error writing the index data" );
    }
    
    return true;
}

bool Geometry::resolveMaterialDelta ( IComponent* comp, Model* model, void* data )
{
    MaterialDeltaData* dd = (MaterialDeltaData *)data;
    
    // Find the material with the given id
    l3m::Material* mat = l3m::Util::findMaterial ( model, dd->name );
    if ( mat != 0 )
        dd->mesh->material() = &mat->material();
    delete dd;
    return true; 
}

bool Geometry::resolvePoseDelta(IComponent* comp, l3m::Model* model, void*)
{
    Geometry* g = (Geometry *)comp;
    
    // Find the pose with the given url
    l3m::Pose* pose = l3m::Util::findPose ( model, g->poseUrl() );
    if ( pose != 0 )
        g->geometry().pose() = &pose->pose();
    
    return true;
}