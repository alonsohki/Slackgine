/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        post_process.cpp
// PURPOSE:     Model post-processing to adjust to the needs of the slackgine.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include "post_process.h"
#include "l3m/Components/geometry.h"
#include "l3m/Components/scene.h"
#include "l3m/Components/texture.h"
#include "math/matrix.h"
#include "math/binary.h"
#include "shared/util.h"

static bool ProcessScene ( l3m::Model* model, l3m::Scene* sce, const char** error )
{
    for ( l3m::Scene::nodesVector::iterator iter = sce->geometryNodes().begin();
          iter != sce->geometryNodes().end();
          ++iter )
    {
        l3m::Scene::Node& node = *iter;
        
        // Orthogonalize the transforms
        for ( u32 i = 0; i < 4; ++i )
        {
            Transform& transform = node.transform;
            Matrix3 Q, R;
            Matrix3::QRDecompose ( transform.orientation(), &Q, &R );
            transform.orientation() = Q;
        }
    }

    return true;
}

static bool ProcessGeometry ( l3m::Geometry* g_, const char** error )
{
    // Calculate the bounding box, sphere and centroid
    Renderer::Geometry& g = g_->geometry();
    float minX = g.vertices()[0].pos().x();
    float maxX = g.vertices()[0].pos().x();
    float minY = g.vertices()[0].pos().y();
    float maxY = g.vertices()[0].pos().y();
    float minZ = g.vertices()[0].pos().z();
    float maxZ = g.vertices()[0].pos().z();
    Vector3 centroid = ZeroVector3 ();
    
    
    for ( u32 i = 0; i < g.numVertices(); ++i )
    {
        Vector3& pos = g.vertices()[i].pos();
#define CHECK(coord, op, dest) if ( pos. coord () op dest ) dest = pos. coord ();
        CHECK(x,<,minX);
        CHECK(x,>,maxX);
        CHECK(y,<,minY);
        CHECK(y,>,maxY);
        CHECK(z,<,minZ);
        CHECK(z,>,maxZ);
#undef  CHECK
        centroid += pos;
    }
    
    g.boundingBox() = BoundingBox ( minX, maxX, minY, maxY, minZ, maxZ );
    minX = fabs(minX); maxX = fabs(maxX);
    minY = fabs(minY); maxY = fabs(maxY);
    minZ = fabs(minZ); maxZ = fabs(maxZ);
    g.boundingSphere() = BoundingSphere( max(max(max(max(max(minX, maxX), minY), maxY), minZ), maxZ) );
    
    //--------------------------------------------------------------------------
    // Set the centroid
    g.centroid() = centroid / g.numVertices();
    
    // Only allow up to 2 texture levels.
    for ( Renderer::Geometry::layerMap::iterator iter = g.vertexLayers().begin();
          iter != g.vertexLayers().end();
          ++iter )
    {
        if ( iter->first == "uv" && iter->second.numLevels > 2 )
                iter->second.numLevels = 2;
    }
    return true;
}

static bool ProcessTexture ( l3m::Texture* tex, l3m::Model* model, const char** error )
{
    // Resample the pixmap to make its sizes power of 2
    Pixmap& pix = tex->pixmap();
    u32 power_of_2_width = power_of_two_above ( pix.width() );
    u32 power_of_2_height = power_of_two_above ( pix.height() );
    pix.Resample(power_of_2_width, power_of_2_height);
    
    return true;
}

bool PostProcess ( l3m::Model* model, const char** error )
{
    using namespace l3m;
    
    // Process the scenes
    for ( Model::componentVector::iterator iter = model->components().begin();
          iter != model->components().end();
          ++iter )
    {
        IComponent* comp = *iter;
        if ( comp->type() == "scene" )
        {
            if ( !ProcessScene ( model, static_cast < Scene* > ( comp ), error ) )
                return false;
        }
    }
    
    // Process the geometries
    for ( Model::componentVector::iterator iter = model->components().begin();
          iter != model->components().end();
          ++iter )
    {
        IComponent* comp = *iter;
        if ( comp->type() == "geometry" )
        {
            if ( !ProcessGeometry ( static_cast < l3m::Geometry* > ( comp ), error ) )
                return false;
        }
    }
    
    // Process the textures
    for ( Model::componentVector::iterator iter = model->components().begin();
          iter != model->components().end();
          ++iter )
    {
        IComponent* comp = *iter;
        if ( comp->type() == "texture" )
        {
            if ( !ProcessTexture ( static_cast < l3m::Texture* > ( comp ), model, error ) )
                return false;
        }
    }
    
    return true;
}
