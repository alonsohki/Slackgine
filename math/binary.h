/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011 - Alberto Alonso
//
// Redistribution of this software, including any of its
// files, in either binary or source code form is totally
// forbidden without the permission of its owners.
//
// Owners reserve their right to legal action against any
// person or company that fails to do so.
//
// For more detailed information, see the LICENSE file in
// the top-level directory.
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
