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

    u32         getSystemTime           () const;
    u64         getSystemTimeMS         () const;
    u64         getTimeMS               () const { return m_timeMS; }
    void        setTimeStep             ( f32 step ) { m_timestep = step; }
    float       getTimeStep             () const { return m_timestep; }
    
    void        sleepMS                 ( u32 ms ) const;
    void        systemSleepMS           ( u32 ms ) const;

    void        tick                    ();
    
private:
    float       m_timestep;
    float       m_waste;
    u64         m_timeMS;
    u64         m_lastSnapshot;
};

}
