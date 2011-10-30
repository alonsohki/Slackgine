/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
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
