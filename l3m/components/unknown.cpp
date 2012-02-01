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

bool UnknownComponent::load ( l3m::Model*, l3m::IStream& stream, float version )
{
    if ( stream.readData ( m_data, m_len, 1 ) != 1 )
        return setError ( "Error loading an unknown(%s) component data", IComponent::type().c_str() );
    return true;
}

bool UnknownComponent::save ( l3m::Model*, l3m::OStream& stream )
{
    if ( !stream.writeData ( m_data, m_len, 1 ) )
        return setError ( "Error saving an unknown(%s) component data", IComponent::type().c_str() );
    return true;
}
