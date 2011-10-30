/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        log.cpp
// PURPOSE:     Slackgine logging.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include <cstdio>
#include <sys/time.h>
#include "log.h"
#if USE_THREADS
#include "shared/mutex.h"
#endif

void vLog ( const char* level, const char* tag, const char* msg, va_list vl )
{
#if USE_THREADS
    static Mutex* logMutex = 0;

    if ( logMutex == 0 )
        logMutex = new Mutex();
#endif

    char buffer [ 1024 ];
    timeval tv;
    gettimeofday ( &tv, 0 );
    
    vsnprintf ( buffer, sizeof(buffer)/sizeof(buffer[0]), msg, vl );
#if USE_THREADS
    logMutex->Lock();
#endif
    fprintf ( stdout, "[%s] [%lu.%lu] \"%s\": %s\n", level, tv.tv_sec, tv.tv_usec, tag, buffer );
#if USE_THREADS
    logMutex->Unlock();
#endif
}

void Log ( const char* level, const char* tag, const char* msg, ... )
{
    va_list vl;
    va_start ( vl, msg );
    vLog ( level, tag, msg, vl );
    va_end ( vl );
}
