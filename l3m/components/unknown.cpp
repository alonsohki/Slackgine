/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2012
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

UnknownComponent::UnknownComponent(const std::string& type, float version, u32 componentLength, bool compressed, u32 originalLength)
: IComponent ( type.c_str(), version )
{
    mComponentLength = componentLength;
    mOriginalLength = originalLength;
    mCompressed = compressed;
    mData = sgNew char [ mComponentLength ];
}

UnknownComponent::~UnknownComponent()
{
    if ( mData != 0 )
        sgDelete [] mData;
    mData = 0;
}

bool UnknownComponent::load ( l3m::Model*, l3m::IStream& stream, float version )
{
    if ( stream.readData ( mData, mComponentLength, 1 ) != 1 )
        return setError ( "Error loading an unknown(%s) component data", IComponent::type().c_str() );
    return true;
}

bool UnknownComponent::save ( l3m::Model*, l3m::OStream& stream )
{
    if ( !stream.writeData ( mData, mComponentLength, 1 ) )
        return setError ( "Error saving an unknown(%s) component data", IComponent::type().c_str() );
    return true;
}
