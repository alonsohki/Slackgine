/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2012
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
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
    m_lastSnapshot = getSystemTimeMS();
}

Time::~Time ()
{
}

u32 Time::getSystemTime ()
{
    return getSystemTimeMS() / 1000;
}

u64 Time::getSystemTimeMS ()
{
    timespec ts;
    clock_gettime ( CLOCK_MONOTONIC, &ts );
    u64 ms = ts.tv_sec * 1000;
    ms += ts.tv_nsec / 1000000;
    return ms;
}

void Time::systemSleepMS ( u32 ms ) const
{
    usleep ( ms * 1000 );
}

void Time::sleepMS ( u32 ms ) const
{
    systemSleepMS ( ms / m_timestep );
}

void Time::tick ()
{
    u64 systemTime = getSystemTimeMS ();
    float timeSpent = ( systemTime - m_lastSnapshot ) * m_timestep + m_waste;
    m_lastSnapshot = systemTime;
    u64 timeSpentInt = floor(timeSpent);
    m_waste = timeSpent - timeSpentInt;
    m_timeMS += timeSpentInt;
}
