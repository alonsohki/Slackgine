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
       
    if ( ! stream.readStr(&m_morph.name()) )
      return setError ( "Error reading the morph name" );
    
    if ( ! stream.read32(&m_morph.numShapes()) )
      return setError ( "Error reading morph '%s' shape count", m_morph.name().c_str() );

    if ( ! stream.read32(&m_morph.numActiveShapes()) )
      return setError ( "Error reading morph '%s' active shape count", m_morph.name().c_str() );

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
    
    // load the array of active shapes
    u32 numActiveShapes = m_morph.numActiveShapes();
    m_morph.reset();
    for ( u32 i = 0; i <numActiveShapes; i++ )
    {
      u32 shape;
      f32 shapeWeight;
      
      if ( stream.read32(&shape) < 0 )
        return setError ( "Error reading morph '%s' active shapes", m_morph.name().c_str() );
        
      if ( stream.readFloat(&shapeWeight) < 0 )
        return setError ( "Error reading morph '%s' shape weights", m_morph.name().c_str() );
        
      // load only while under the limit
      if (i < Renderer::Morph::MAX_ACTIVE_SHAPES) {
        m_morph.addActiveShape(shape, shapeWeight);
      }
    }
    
    // fix inconsistencies
    m_morph.checkAndFix();
    
    return true;
}

bool Morph::save(l3m::Model*, l3m::OStream& stream)
{
    m_morph.checkAndFix();
     
    if ( ! stream.writeStr(m_morph.name()) )
      return setError ( "Error writing the morph '%s' name", m_morph.name().c_str() );
    
    if ( ! stream.write32(m_morph.numShapes()) )
      return setError ( "Error writing the morph '%s' shape count", m_morph.name().c_str() );

    if ( ! stream.write32(m_morph.numActiveShapes()) )
      return setError ( "Error writing the morph '%s' active shape count", m_morph.name().c_str() );
    
    for ( u32 i = 0; i < m_morph.numShapes(); i++ )
    {
      if ( ! stream.writeStr(m_morph.shapeNames()[i]) )
        return setError ( "Error writing the morph '%s' shape names", m_morph.name().c_str() );
    }
    
    for ( u32 i = 0; i < m_morph.numActiveShapes(); i++ )
    {
      if ( ! stream.write32(m_morph.activeShapes()[i]) )
        return setError ( "Error writing the morph '%s' active shapes", m_morph.name().c_str() );
      
      if ( ! stream.writeFloat(m_morph.activeWeights()[i]) )
        return setError ( "Error writing the morph '%s' shape weights", m_morph.name().c_str() );
    }
    
    return true;
}

