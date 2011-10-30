/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        binary.h
// PURPOSE:     Binary operations.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

static inline u32 power_of_two_below ( u32 value )
{
    u32 mask = 1 << 31;
    for ( u32 i = 0; i < 32; ++i )
    {
        if ( value & mask )
            return mask;
        mask >>= 1;
    }
}

static inline u32 power_of_two_above ( u32 value )
{
    return power_of_two_below ( value ) << 1;
}
