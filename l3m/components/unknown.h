/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        unknown.h
// PURPOSE:     Special component to store unknown component types.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#include "../component.h"

namespace l3m
{
    
class UnknownComponent : public IComponent
{
public:
                        UnknownComponent         ( const std::string& type, float version, u32 componentLength, bool compressed, u32 originalLength );
                        ~UnknownComponent        ();

    bool                load            ( l3m::Model*, l3m::IStream& stream, float version );
    bool                save            ( l3m::Model*, l3m::OStream& stream );
    
    bool                shouldCompress      () const { return mCompressed; }
    u32                 originalLength      () const { return mOriginalLength; }
    
private:
    u32     mComponentLength;
    char*   mData;
    bool    mCompressed;
    u32     mOriginalLength;
};

}
