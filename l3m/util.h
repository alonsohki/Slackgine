/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        util.h
// PURPOSE:     Utilities for l3m models
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#include "model.h"
#include "component.h"
#include "Components/components.h"

namespace l3m
{
namespace Util
{

    //--------------------------------------------------------------------------
    // Function to find a given material in a model given its name.
    static inline l3m::Material* findMaterial ( Model* model, const std::string& name )
    {
        for ( Model::ComponentVector::iterator iter = model->components().begin();
              iter != model->components().end();
              ++iter )
        {
            if ( (*iter)->type() == "material" )
            {
                l3m::Material* mat = static_cast<l3m::Material*>(*iter);
                if ( mat->material().name() == name )
                    return mat;
            }
        }
        
        return 0;
    }
    
    //--------------------------------------------------------------------------
    // Function to find the scenes in a model
    static inline l3m::Scene* findScene ( Model* model, u32 idx = 0 )
    {
        for ( Model::ComponentVector::iterator iter = model->components().begin();
              iter != model->components().end();
              ++iter )
        {
            if ( (*iter)->type() == "scene" )
            {
                if ( idx == 0 )
                    return static_cast<l3m::Scene*>(*iter);
                else
                    --idx;
            }
        }

        return 0;
    }

    //--------------------------------------------------------------------------
    // Function to find a geometry in a model
    static inline l3m::Geometry* findGeometry ( Model* model, const std::string& name )
    {
        for ( Model::ComponentVector::iterator iter = model->components().begin();
              iter != model->components().end();
              ++iter )
        {
            if ( (*iter)->type() == "geometry" )
            {
                l3m::Geometry* g = static_cast<l3m::Geometry*>(*iter);
                
                if ( g->geometry().name() == name )
                    return g;
            }
        }
        
        return 0;
    }
}
}
