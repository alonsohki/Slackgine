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
// FILE:        time.h
// PURPOSE:     Time pass management and time helper functions
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#include "shared/platform.h"

namespace Core
{

class Time
{
public:
                Time                    ();
                ~Time                   ();

    u32         GetSystemTime           () const;
    u64         GetSystemTimeMS         () const;
    u64         GetTimeMS               () const { return m_timeMS; }
    void        SetTimeStep             ( float step ) { m_timestep = step; }
    float       GetTimeStep             () const { return m_timestep; }
    
    void        SleepMS                 ( u32 ms ) const;
    void        SystemSleepMS           ( u32 ms ) const;

    void        Tick                    ();
    
private:
    float       m_timestep;
    float       m_waste;
    u64         m_timeMS;
    u64         m_lastSnapshot;
};

}
