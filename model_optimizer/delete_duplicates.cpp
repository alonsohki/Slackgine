/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        delete_duplicates.cpp
// PURPOSE:     Strategy to delete duplicate vertices.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include "delete_duplicates.h"
#include "l3m/components/geometry.h"
#include "renderer/geometry.h"
#include "renderer/vertex.h"
#include "renderer/mesh.h"

using namespace l3m;
using Renderer::Vertex;
using Renderer::Mesh;

static bool vertices_match ( Renderer::Geometry& g, u32 left, u32 right )
{
    Vertex* vertices = g.vertices();
    if ( vertices[left] == vertices[right] )
    {
        // Check all the layers
        const Renderer::Geometry::layerMap& layers = g.vertexLayers();
        for ( Renderer::Geometry::layerMap::const_iterator iter = layers.begin();
              iter != layers.end();
              ++iter )
        {
            const Renderer::Geometry::VertexLayer& layer = iter->second;
            for ( u32 i = 0; i < layer.numLevels; ++i )
            {
                const char* data = (const char *)layer.data;
                data = &data[i * layer.elementSize * g.numVertices()];
                
                if ( memcmp ( &data[left*layer.elementSize], &data[right*layer.elementSize], layer.elementSize ) != 0 )
                    return false;
            }
        }
        
        return true;
    }
    
    return false;
}

static bool process_geometry ( Renderer::Geometry& g, u32* numDuplicates )
{
    *numDuplicates = 0;
    u32 numVertices = g.numVertices ();
    Renderer::Geometry::layerMap& layers = g.vertexLayers();
    
    // For each vertex...
    Vertex* vertices = g.vertices();
    for ( u32 curVertex = 0; curVertex < numVertices; ++curVertex )
    {
        for ( u32 lookup = curVertex + 1; lookup < numVertices; ++lookup )
        {
            if ( vertices_match(g, curVertex, lookup) )
            {
                *numDuplicates = *numDuplicates + 1;
                --numVertices;
                
                // Move all the vertices onwards, including all layers
                memcpy ( &vertices[lookup], &vertices[lookup+1], (numVertices - lookup) * sizeof(Vertex) );
                for ( Renderer::Geometry::layerMap::iterator iter = layers.begin ();
                      iter != layers.end();
                      ++iter )
                {
                    Renderer::Geometry::VertexLayer& layer = iter->second;
                    for ( u32 i = 0; i < layer.numLevels; ++i )
                    {
                        char* data = (char *)layer.data;
                        data = &data [ i * layer.elementSize * g.numVertices() ];
                        memcpy ( &data[lookup*layer.elementSize], &data[(lookup+1)*layer.elementSize], (numVertices - lookup) * layer.elementSize );
                    }
                }
                
                // Update all the indices that pointed to this vertex and the follwing
                Renderer::Geometry::meshList& meshes = g.meshes();
                for ( Renderer::Geometry::meshList::iterator it = meshes.begin ();
                      it != meshes.end ();
                      ++it )
                {
                    u32* indices = (*it)->indices();
                    for ( u32 i = 0; i < (*it)->numIndices(); ++i )
                    {
                        if ( indices[i] == lookup )
                            indices[i] = curVertex;
                        else if ( indices[i] > lookup )
                            indices[i] = indices[i] - 1;
                    }
                }
                
                --lookup;
            }
        }
    }
    
    g.Set ( vertices, numVertices );

    return true;
}

bool delete_duplicates ( Model* model )
{
    u32 totalDuplicates = 0;
    u32 numDuplicates;
    
    fprintf ( stderr, "Deleting duplicates...\n" );
    
    Model::ComponentVector& components = model->components();
    for ( Model::ComponentVector::iterator iter = components.begin ();
          iter != components.end();
          ++iter )
    {
        if ( (*iter)->type() == "geometry" )
        {
            l3m::Geometry* g = static_cast<l3m::Geometry*>(*iter);
            fprintf ( stderr, "- %s... ", g->geometry().name().c_str() );
            fflush ( stderr );
            if ( process_geometry ( g->geometry(), &numDuplicates ) == false )
            {
                fprintf ( stderr, "FAILED\n" );
                return false;
            }
            totalDuplicates += numDuplicates;
            fprintf ( stderr, "%u duplicates.\n", numDuplicates );
        }
    }
    
    fprintf ( stderr, "Deleted a total of %u duplicates\n", totalDuplicates );
    
    return true;
}
