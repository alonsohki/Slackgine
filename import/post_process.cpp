#include "post_process.h"
#include "l3m/Components/geometry.h"
#include "l3m/Components/scene.h"
#include "math/matrix.h"
#include "shared/util.h"

static bool ProcessScene ( l3m::Model* model, l3m::Scene* sce, const char** error )
{
    for ( l3m::Scene::nodesVector::iterator iter = sce->geometryNodes().begin();
          iter != sce->geometryNodes().end();
          ++iter )
    {
        l3m::Scene::Node& node = *iter;

        // Find the geometry associated to this url.
        for ( l3m::Model::componentVector::iterator miter = model->components().begin();
              miter != model->components().end();
              ++miter )
        {
            if ( (*miter)->type() == "geometry" && static_cast < l3m::Geometry* > ( *miter )->geometry().name() == node.url )
            {
                Renderer::Geometry& g = static_cast<l3m::Geometry *>(*miter)->geometry();
                Matrix Q, R;
                Matrix::QRDecompose ( node.transform, &Q, &R );
                if ( fabs(R.Determinant() - 1.0f) > 0.00001f )
                {
                    // Apply the scaling to every vertex.
                    Renderer::Vertex* v = g.vertices();
                    for ( u32 i = 0; i < g.numVertices(); ++i )
                        v[i].pos() = R * v[i].pos();

                    node.transform.Assign3x3(Q);
                }
            }
        }
    }

    return true;
}

static bool ProcessGeometry ( l3m::Geometry* g_, const char** error )
{
    // Calculate the bounding box and sphere
    Renderer::Geometry& g = g_->geometry();
    float minX = g.vertices()[0].pos().x();
    float maxX = g.vertices()[0].pos().x();
    float minY = g.vertices()[0].pos().y();
    float maxY = g.vertices()[0].pos().y();
    float minZ = g.vertices()[0].pos().z();
    float maxZ = g.vertices()[0].pos().z();
    
    
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
    }
    
    g.boundingBox() = BoundingBox ( minX, maxX, minY, maxY, minZ, maxZ );
    minX = fabs(minX); maxX = fabs(maxX);
    minY = fabs(minY); maxY = fabs(maxY);
    minZ = fabs(minZ); maxZ = fabs(maxZ);
    g.boundingSphere() = BoundingSphere( max(max(max(max(max(minX, maxX), minY), maxY), minZ), maxZ) );
    
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
    
    return true;
}
