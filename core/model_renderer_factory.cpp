/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011 - Alberto Alonso
//
// Redistribution of this software, including any of its
// files, in either binary or source code form is totally
// forbidden without the permission of its owners.
//
// Owners reserve their right to legal action against any
// person or company that fails to do so.
//
// For more detailed information, see the LICENSE file in
// the top-level directory.
//
// FILE:        model_renderer_factory.cpp
// PURPOSE:     Factory for model renderers, to avoid re-creating these components for the same model.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

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
