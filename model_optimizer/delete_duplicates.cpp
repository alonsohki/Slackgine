/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2012
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
    Vertex* vertices = g.vertices();
    
    // In a first step, detect the duplicate vertices and remove them.
    for ( u32 curVertex = 0; curVertex < numVertices; ++curVertex )
    {
        for ( u32 lookup = curVertex + 1; lookup < numVertices; ++lookup )
        {
            if ( vertices_match(g, curVertex, lookup) )
            {
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
                    }
                }
            }
        }
    }
    
    // Make a lookup table and mark the indices that have been found
    bool found [ numVertices ];
    memset ( &found[0], false, sizeof(bool)*numVertices );
    Renderer::Geometry::meshList& meshes = g.meshes();
    for ( Renderer::Geometry::meshList::iterator it = meshes.begin ();
            it != meshes.end ();
            ++it )
    {
        u32* indices = (*it)->indices();
        for ( u32 i = 0; i < (*it)->numIndices(); ++i )
        {
            found [ indices[i] ] = true;
        }
    }
    
    // Delete the indices that were not marked
    Vertex* newVertices = (Vertex *)sgMalloc(sizeof(Vertex)*numVertices);
    u32 n = 0;
    for ( u32 i = 0; i < numVertices; ++i )
    {
        if ( found[i] == false )
        {
            *numDuplicates = *numDuplicates + 1;
            
            // Update all the indices
            for ( Renderer::Geometry::meshList::iterator it = meshes.begin ();
                  it != meshes.end ();
                  ++it )
            {
                u32* indices = (*it)->indices();
                for ( u32 j = 0; j < (*it)->numIndices(); ++j )
                {
                    if ( indices[j] > i-*numDuplicates )
                        indices[j] = indices[j]-1;
                }
            }
        }
        else
        {
            newVertices [ n ] = vertices[i];
            ++n;
        }
    }
    
    u32 newVertexCount = n;
    
    // Update all the layers
    Renderer::Geometry::layerMap newMap;
    for ( Renderer::Geometry::layerMap::iterator iter = layers.begin();
          iter != layers.end();
          ++iter )
    {
        Renderer::Geometry::VertexLayer& oldLayer = iter->second;
        Renderer::Geometry::VertexLayer newLayer;
        newLayer.numLevels = oldLayer.numLevels;
        newLayer.elementSize = oldLayer.elementSize;
        newLayer.data = sgMalloc ( newLayer.numLevels * newLayer.elementSize * newVertexCount );
        
        for ( u32 l = 0; l < newLayer.numLevels; ++l )
        {
            u8* _oldData = (u8*)oldLayer.data;
            u8* _data = (u8*)newLayer.data;
            u8* oldData = &_oldData [ numVertices * oldLayer.elementSize * l ];
            u8* data = &_data [ newVertexCount * newLayer.elementSize * l ];
            u32 n = 0;
            
            for ( u32 i = 0; i < numVertices; ++i )
            {
                if ( found[i] == true )
                {
                    memcpy ( &data[n * newLayer.elementSize], &oldData[i * oldLayer.elementSize], newLayer.elementSize );
                    ++n;
                }
            }
        }
        newMap[iter->first] = newLayer;
    }
    g.vertexLayers() = newMap;
    g.set ( newVertices, newVertexCount );

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
