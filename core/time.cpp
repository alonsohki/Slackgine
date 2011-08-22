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
// FILE:        time.cpp
// PURPOSE:     Time pass management and time helper functions
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include <cmath>
#include <time.h>
#include <sys/time.h>
#include "time.h"
#include <unistd.h>

using namespace Core;

Time::Time ()
: m_timestep(1.0f)
, m_waste(0.0f)
, m_timeMS(0)
{
    m_lastSnapshot = GetSystemTimeMS();
}

Time::~Time ()
{
}

u32 Time::GetSystemTime () const
{
    return time(0);
}

u64 Time::GetSystemTimeMS () const
{
    timeval tv;
    gettimeofday ( &tv, 0 );
    u64 ms = tv.tv_sec * 1000;
    ms += tv.tv_usec / 1000;
    return ms;
}

void Time::SystemSleepMS ( u32 ms ) const
{
    usleep ( ms * 1000 );
}

void Time::SleepMS ( u32 ms ) const
{
    SystemSleepMS ( ms / m_timestep );
}

void Time::Tick ()
{
    u64 systemTime = GetSystemTimeMS ();
    float timeSpent = ( systemTime - m_lastSnapshot ) * m_timestep + m_waste;
    m_lastSnapshot = systemTime;
    u64 timeSpentInt = floor(timeSpent);
    m_waste = timeSpent - timeSpentInt;
    m_timeMS += timeSpentInt;
}
