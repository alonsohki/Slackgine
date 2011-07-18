#include <cstdio>
#include "model_optimizer.h"

#if 0
static void DeleteDuplicates ( l3m::Model* source )
{
    u32 numClones = 0;
    u32 numClonesLevel;
    
    for ( l3mComponent::geometryList::iterator i = source->geometries().begin(); i != source->geometries().end(); ++i )
    {
        for ( Geometry::meshList::iterator j = (*i)->meshes().begin(); j != (*i)->meshes().end(); ++j )
        {
            Mesh* mesh = *j;
            numClonesLevel = 0;

            for ( u32 k = 0; k < mesh->numVertices() - numClonesLevel; ++k )
            {
                const Vertex& cur = mesh->vertices()[k];

                for ( u32 check = k + 1; check < mesh->numVertices() - numClonesLevel; ++check )
                {
                    if ( mesh->vertices()[check] == cur )
                    {
                        ++numClonesLevel;
                        
                        // Replace all the indices pointing to the clone
                        for ( u32 idx = 0; idx < mesh->numIndices(); ++idx )
                            if ( mesh->indices()[idx] == check )
                                mesh->indices()[idx] = k;
                            else if ( mesh->indices()[idx] > check )
                                mesh->indices()[idx]--;
                    }
                    else if ( numClonesLevel > 0 )
                        mesh->vertices()[check-numClonesLevel] = mesh->vertices()[check];
                }
            }

            if ( numClonesLevel > 0 )
                mesh->Set ( mesh->vertices(), mesh->numVertices() - numClonesLevel, mesh->indices(), mesh->numIndices(), mesh->polyType() );
            numClones += numClonesLevel;
        }
    }
    
    fprintf ( stderr, "* Removed %u duplicates\n", numClones );
}
#endif

void ModelOptimizer::Optimize(l3m::Model* source)
{
    fprintf ( stderr, "Optimising model...\n" );
    //DeleteDuplicates ( source );
}
