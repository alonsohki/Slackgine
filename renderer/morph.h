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
  
    Morph    () 
    : m_numShapes(0)
    , m_numActiveShapes(0)
    {}

    ~Morph   () {}

    void reset() {
      m_numActiveShapes = 0;
    }

    // disable an active shape
    void disableActiveShape( int activeShapeIndex );

    // set the weight of a shape if it already active, if its not it will
    // make the shape active or it will return false (if it couldn't be done)
    bool setShapeWeight( int shapeId, float weight );

    // add a shape to the list of active shapes
    bool addActiveShape( int shapeId, float weight );            

    // find the id of a shape name (-1 if could not be found)
    int findShapeId( std::string name ) const;

    // find the index of shapeId (-1 if could not be found)
    int findActiveShapeIndex( int shapeId ) const;

    // checks the internal state and fix inconsistencies
    bool checkAndFix();

    //--------------------------------------------------------------------------
    // Conveniency methods

    void setActiveShapeWeight ( int activeShapeIndex, float weight ) {
        m_activeWeights[activeShapeIndex] = weight;
    }

public:
    //--------------------------------------------------------------------------
    // Public accessors
    std::string&        name              () { return m_name; }
    std::string*        shapeNames        () { return &m_shapeNames[0]; }
    u32&                numShapes         () { return m_numShapes; }
    u32*                activeShapes      () { return &m_activeShapes[0]; }
    f32*                activeWeights     () { return &m_activeWeights[0]; }
    u32&                numActiveShapes   () { return m_numActiveShapes; }
    
    const std::string&  name              () const { return m_name; }
    const std::string*  shapeNames        () const { return &m_shapeNames[0]; }
    const u32&          numShapes         () const { return m_numShapes; }
    const u32*          activeShapes      () const { return &m_activeShapes[0]; }
    const f32*          activeWeights     () const { return &m_activeWeights[0]; }
    const u32&          numActiveShapes   () const { return m_numActiveShapes; }


private:
    //--------------------------------------------------------------------------
    // Name (unique identifier) for this pose
    std::string     m_name;

    //--------------------------------------------------------------------------
    // Names for each shape
    std::string     m_shapeNames [ MAX_SHAPES ];    
    
    //--------------------------------------------------------------------------
    // Number of shapes
    u32             m_numShapes;
    
    //--------------------------------------------------------------------------
    // Active shapes
    u32             m_activeShapes [ MAX_ACTIVE_SHAPES ];
    
    //--------------------------------------------------------------------------
    // Weight of each active shape
    f32             m_activeWeights [ MAX_ACTIVE_SHAPES ];
    
    //--------------------------------------------------------------------------
    // Number of active shapes
    u32             m_numActiveShapes;
};

}
