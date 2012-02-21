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
#include "components/components.h"

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
    
    //--------------------------------------------------------------------------
    // Function to find a geometry in a model
    static inline l3m::Camera* findCamera ( Model* model, const std::string& name )
    {
        for ( Model::ComponentVector::iterator iter = model->components().begin();
              iter != model->components().end();
              ++iter )
        {
            if ( (*iter)->type() == "camera" )
            {
                l3m::Camera* cam = static_cast<l3m::Camera*>(*iter);
                
                if ( cam->name() == name )
                    return cam;
            }
        }
        
        return 0;
    }
    
    //--------------------------------------------------------------------------
    // Function to find a given pose in a model given its name.
    static inline l3m::Pose* findPose ( Model* model, const std::string& name )
    {
        for ( Model::ComponentVector::iterator iter = model->components().begin();
              iter != model->components().end();
              ++iter )
        {
            if ( (*iter)->type() == "pose" )
            {
                l3m::Pose* pose = static_cast<l3m::Pose*>(*iter);
                if ( pose->pose().name() == name )
                    return pose;
            }
        }
        
        return 0;
    }
    
    
    //--------------------------------------------------------------------------
    // Function to remove a geometry and clear all the references to it
    static inline bool unlinkGeometry ( Model* model, const std::string& name )
    {
        for ( l3m::Model::ComponentVector::iterator iter = model->components().begin ();
            iter != model->components().end();
            ++iter )
        {
            if ( (*iter)->type() == "geometry" )
            {
                l3m::Geometry* g = static_cast < l3m::Geometry* > ( *iter );
                if ( g->geometry().name() == name )
                {
                    model->components().erase ( iter );

                    // Remove this geometry from all the scenes
                    l3m::Scene* sce = 0;
                    for ( u32 i = 0; ( sce = findScene(model, i)) != 0; ++i )
                    {
                        for ( l3m::Scene::NodesVector::iterator iter2 = sce->geometryNodes().begin();
                            iter2 != sce->geometryNodes().end(); )
                        {
                            if ( (*iter2).url == name )
                                iter2 = sce->geometryNodes().erase(iter2);
                            else
                                ++iter2;
                        }
                    }
                    return true;
                }
            }
        }
        
        // Not found.
        return false;
    }
}
}
