/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        vertex_weight.h
// PURPOSE:     Vertex weight for skinned geometries.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

namespace Renderer
{

struct VertexWeight
{
    //--------------------------------------------------------------------------
    // Configuration
    enum
    {
        MAX_ASSOCIATIONS = 2
    };
    
    //--------------------------------------------------------------------------
    // Data
    f32     weight;
    u32     joint;
};

struct VertexWeightSOA
{
    //--------------------------------------------------------------------------
    // Data
    f32     weight  [ VertexWeight::MAX_ASSOCIATIONS ];
    u32     joint   [ VertexWeight::MAX_ASSOCIATIONS ];
};

}
