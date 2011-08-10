#include "model_renderer_factory.h"

using namespace Core;
using Core::Entities::ModelRenderer;

ModelRendererFactory::nodeMap ModelRendererFactory::ms_mapNodes;

ModelRenderer* ModelRendererFactory::Create ( l3m::Model* model )
{
    nodeMap::iterator iter = ms_mapNodes.find ( model );
    ModelRenderer* component;

    if ( iter == ms_mapNodes.end() )
    {
        Node newNode;
        newNode.refs = 0;
        newNode.component = new ModelRenderer ( model );
        ms_mapNodes.insert ( nodeMap::value_type(model, newNode) );
        component = newNode.component;
    }
    else
    {
        Node& node = iter->second;
        node.refs++;
        component = node.component;
    }

    return component;
}

void ModelRendererFactory::Release ( ModelRenderer* component )
{
    nodeMap::iterator iter = ms_mapNodes.find ( component->model() );
    if ( iter != ms_mapNodes.end() )
    {
        Node& node = iter->second;
        node.refs--;
        if ( node.refs == 0 )
        {
            delete node.component;
            ms_mapNodes.erase ( iter );
        }
    }
}
