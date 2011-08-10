#include "post_process.h"
#include "l3m/Components/geometry.h"
#include "l3m/Components/scene.h"
#include "math/matrix.h"

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

bool PostProcess ( l3m::Model* model, const char** error )
{
    using namespace l3m;
    for ( Model::componentVector::iterator iter = model->components().begin();
          iter != model->components().end();
          ++iter )
    {
        IComponent* comp = *iter;
        if ( comp->type() == "scene" )
            if ( !ProcessScene ( model, static_cast < Scene* > ( comp ), error ) )
                return false;
    }
    
    return true;
}
