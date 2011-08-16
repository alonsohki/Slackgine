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
// FILE:        model_renderer_factory.h
// PURPOSE:     Factory for model renderers, to avoid re-creating these components for the same model.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#include <map>
#include "entity_components/model_renderer.h"
#include "l3m/l3m.h"

namespace Core
{

class ModelRendererFactory
{
public:
    struct Node
    {
        Entities::ModelRenderer*  component;
        u32                       refs;
    };
    typedef std::map < l3m::Model*, Node > nodeMap;
    
public:
    static Entities::ModelRenderer*     Create          ( l3m::Model* model );
    static void                         Release         ( Entities::ModelRenderer* component );
    
private:
    static nodeMap      ms_mapNodes;
};

}