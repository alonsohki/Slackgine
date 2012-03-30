/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        morph.h
// PURPOSE:     Morph component.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//              Rayco Hernandez Garcia <raycohg@gmail.com>

#include "morph.h"

using namespace l3m;

Morph::Morph ()
: IComponent ( "morph", 1.0f )
{
}

Morph::~Morph ()
{
}

bool Morph::load(l3m::Model*, l3m::IStream& stream, float version)
{
    std::string ignoreStr;
    u32 ignore32;
    float ignoreFloat;
    
    
    if ( ! stream.readStr(&m_morph.name()) )
      return setError ( "Error reading the morph name" );
    
    if ( ! stream.read32(&m_morph.numShapes()) )
      return setError ( "Error reading morph '%s' shape count", m_morph.name().c_str() );

    if ( ! stream.read32(&m_morph.numActiveShapes()) )
      return setError ( "Error reading morph '%s' active shape count", m_morph.name().c_str() );

    // load the array of active shapes
    u32 numActiveShapes = 0;
    for ( u32 i = 0; i < m_morph.numActiveShapes(); i++ )
    {
      u32 shape;
      if (i < Renderer::Morph::MAX_ACTIVE_SHAPES) {
        if ( stream.read32(&shape) < 0 )
            return setError ( "Error reading morph '%s' active shapes", m_morph.name().c_str() );
        
        // ignore invalid active shape id
        if (shape < m_morph.numShapes() && shape < Renderer::Morph::MAX_SHAPES) {
          m_morph.activeShapes()[numActiveShapes++] = shape;
        }        
      }
      else // ignore the remaining active shapes (limit reached)
        stream.read32(&ignore32);
    }
        
    // initialize the usused active shapes to the first shape to prevent crash on bad usage
    for ( u32 i = m_morph.numActiveShapes(); i< Renderer::Morph::MAX_ACTIVE_SHAPES; i++ )
    {
      m_morph.activeShapes()[i] = 0;
    }
    // correct the active shape count
    m_morph.numActiveShapes() = numActiveShapes;
    
    
    // load the name of each shape
    for ( u32 i = 0; i < m_morph.numShapes(); i++ )
    {
      if (i < Renderer::Morph::MAX_SHAPES) {
        if ( stream.readStr(&m_morph.shapeNames()[i]) < 0 )
            return setError ( "Error reading morph '%s' shape names", m_morph.name().c_str() );
      }
      else // ignore the remaining shapes (limit reached)
        stream.readStr(&ignoreStr);
    }

    // load the weights of each shape
    for ( u32 i = 0; i < m_morph.numShapes(); i++ )
    {
      if (i < Renderer::Morph::MAX_SHAPES) {
        if ( stream.readFloat(&m_morph.shapeWeights()[i]) < 0 )
            return setError ( "Error reading morph '%s' shape weights", m_morph.name().c_str() );
      }
      else // ignore the remaining shapes (limit reached)
        stream.readFloat(&ignoreFloat);
    }
        
    // correct the shape count in case the model exceded the limit
    if (m_morph.numShapes() > Renderer::Morph::MAX_SHAPES) {
      m_morph.numShapes() = Renderer::Morph::MAX_SHAPES;
    }
    
    return true;
}

bool Morph::save(l3m::Model*, l3m::OStream& stream)
{
    if ( ! stream.writeStr(m_morph.name()) )
      return setError ( "Error writing the morph '%s' name", m_morph.name().c_str() );
    
    if ( ! stream.write32(m_morph.numShapes()) )
      return setError ( "Error writing the morph '%s' shape count", m_morph.name().c_str() );

    if ( ! stream.write32(m_morph.numActiveShapes()) )
      return setError ( "Error writing the morph '%s' active shape count", m_morph.name().c_str() );
    
    for ( u32 i = 0; i < m_morph.numActiveShapes(); i++ )
    {
      if ( ! stream.write32(m_morph.activeShapes()[i]) )
        return setError ( "Error writing the morph '%s' active shapes", m_morph.name().c_str() );
    }
    
    for ( u32 i = 0; i < m_morph.numShapes(); i++ )
    {
      if ( ! stream.writeStr(m_morph.shapeNames()[i]) )
        return setError ( "Error writing the morph '%s' shape names", m_morph.name().c_str() );
    }
    
    for ( u32 i = 0; i < m_morph.numShapes(); i++ )
    {
      if ( ! stream.writeFloat(m_morph.shapeWeights()[i]) )
        return setError ( "Error writing the morph '%s' shape weights", m_morph.name().c_str() );
    }
    
    return true;
}

