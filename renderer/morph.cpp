/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        morph.cpp
// PURPOSE:     Morph definition.
// AUTHORS:     Rayco Hernandez Garcia <raycohg@gmail.com>
//

#include "shared/platform.h"

#include "morph.h"

using namespace Renderer;

void Morph::disableActiveShape( int activeShapeIndex )
{
  int index = activeShapeIndex;
  if (index < 0 || index > (int)Morph::MAX_ACTIVE_SHAPES)
    return;
  
  // remove one active shape and shift the array starting at the given index
  m_numActiveShapes -= 1;  
  for (u32 i=(u32)index; i<m_numActiveShapes; i++) {
    m_activeShapes[i] = m_activeShapes[i+1];
    m_activeWeights[i] = m_activeWeights[i+1];
  }
  
}

bool Morph::setShapeWeight( int shapeId, float weight )
{
  int index = this->findActiveShapeIndex(shapeId);
  if (index < 0) {
    return this->addActiveShape(shapeId, weight);
  }
  else {
    m_activeWeights[index] = weight;
    return true;
  }
}

bool Morph::addActiveShape( int shapeId, float weight )
{
  int n = m_numActiveShapes;
  if ( (shapeId >= 0) && (shapeId < (int)Morph::MAX_SHAPES) && (n < (int)Morph::MAX_ACTIVE_SHAPES) )
  {
    m_activeShapes[n] = shapeId;
    m_activeWeights[n] = weight;
    m_numActiveShapes++;
    return true;
  }  
  return false;
}
            
int Morph::findShapeId( std::string name ) const
{
  for ( u32 i=0; i<m_numShapes; i++ ) {
    if ( m_shapeNames[i] == name )
      return i;
  }
  
  return -1;
}

int Morph::findActiveShapeIndex( int shapeId ) const
{
  for ( u32 i=0; i<m_numActiveShapes; i++ ) {
    if ( m_activeShapes[i] == (u32)shapeId )
      return i;
  }
  return -1;
}

bool Morph::checkAndFix()
{
  bool wasFixed = false;
  
  if ( m_numShapes > Morph::MAX_SHAPES ) {
    m_numShapes = Morph::MAX_SHAPES;
    wasFixed = true;
  }
  
  if ( m_numActiveShapes > Morph::MAX_ACTIVE_SHAPES ) {
    m_numActiveShapes = Morph::MAX_ACTIVE_SHAPES;
    wasFixed = true;
  }
  
  int numValidActiveShapes = 0;  
  for ( u32 i=0; i<m_numActiveShapes; i++ )
  {
    if ( m_activeShapes[i] < Morph::MAX_SHAPES ) {
      m_activeShapes[numValidActiveShapes]  = m_activeShapes[i];
      m_activeWeights[numValidActiveShapes] = m_activeWeights[i];
      numValidActiveShapes++;
    }
    else {
      wasFixed = true;
    }
  }
  m_numActiveShapes = numValidActiveShapes;
  
  for ( u32 i=m_numActiveShapes; i<Morph::MAX_ACTIVE_SHAPES; i++ )
  {
    m_activeShapes[i] = 0;
    m_activeWeights[i] = 0.0f;
  }
  
  return wasFixed;
}