/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        unknown.cpp
// PURPOSE:     Special component to store unknown component types.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include "unknown.h"

using namespace l3m;

UnknownComponent::UnknownComponent(const std::string& type, float version, u32 len)
: IComponent ( type.c_str(), version )
{
    m_len = len;
    m_data = new char [ len ];
}

UnknownComponent::~UnknownComponent()
{
    if ( m_data != 0 )
        delete [] m_data;
    m_data = 0;
}

bool UnknownComponent::Load ( l3m::Model*, l3m::IStream& stream, float version )
{
    if ( stream.ReadData ( m_data, m_len, 1 ) != 1 )
        return SetError ( "Error loading an unknown(%s) component data", IComponent::type().c_str() );
    return true;
}

bool UnknownComponent::Save ( l3m::Model*, l3m::OStream& stream )
{
    if ( !stream.WriteData ( m_data, m_len, 1 ) )
        return SetError ( "Error saving an unknown(%s) component data", IComponent::type().c_str() );
    return true;
}
