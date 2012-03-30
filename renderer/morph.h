/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        morph.h
// PURPOSE:     Morph definition.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//              Rayco Hernandez Garcia <raycohg@gmail.com>
//

#pragma once

#include <string>

namespace Renderer
{
class Morph
{
public:
    //--------------------------------------------------------------------------
    // Configuration stuff
    enum
    {
        MAX_SHAPES = 64,
        MAX_ACTIVE_SHAPES = 2,
    };

public:
            Morph    () {}
            ~Morph   () {}

public:
    //--------------------------------------------------------------------------
    // Public accessors
    std::string&        name            () { return m_name; }
    std::string*        shapeNames      () { return &m_shapeNames[0]; }
    f32*                shapeWeights    () { return &m_shapeWeights[0]; }
    u32&                numShapes       () { return m_numShapes; }
    u32*                activeShapes    () { return &m_activeShapes[0]; }
    u32&                numActiveShapes () { return m_numActiveShapes; }
    
    const std::string&  name            () const { return m_name; }
    const std::string*  shapeNames      () const { return &m_shapeNames[0]; }
    const f32*          shapeWeights    () const { return &m_shapeWeights[0]; }
    const u32&          numShapes       () const { return m_numShapes; }
    const u32*          activeShapes    () const { return &m_activeShapes[0]; }
    const u32&          numActiveShapes () const { return m_numActiveShapes; }


private:
    //--------------------------------------------------------------------------
    // Name (unique identifier) for this pose
    std::string     m_name;

    //--------------------------------------------------------------------------
    // Names for each shape
    std::string     m_shapeNames [ MAX_SHAPES ];
    
    //--------------------------------------------------------------------------
    // Weight of each shape
    f32             m_shapeWeights [ MAX_SHAPES ];
    
    //--------------------------------------------------------------------------
    // Number of shapes
    u32             m_numShapes;
    
    //--------------------------------------------------------------------------
    // Active shapes
    u32             m_activeShapes [ MAX_ACTIVE_SHAPES ];
    
    //--------------------------------------------------------------------------
    // Number of active shapes
    u32             m_numActiveShapes;
};

}
