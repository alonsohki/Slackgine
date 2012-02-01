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
                        UnknownComponent         ( const std::string& type, float version, u32 len );
                        ~UnknownComponent        ();

    bool                load            ( l3m::Model*, l3m::IStream& stream, float version );
    bool                save            ( l3m::Model*, l3m::OStream& stream );
    
private:
    u32     m_len;
    char*   m_data;
};

}
