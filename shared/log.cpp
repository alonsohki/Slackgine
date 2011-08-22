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
// FILE:        log.cpp
// PURPOSE:     Slackgine logging.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include <cstdio>
#include <sys/time.h>
#include "log.h"

void vLog ( const char* level, const char* tag, const char* msg, va_list vl )
{
    char buffer [ 1024 ];
    timeval tv;
    gettimeofday ( &tv, 0 );
    
    vsnprintf ( buffer, sizeof(buffer)/sizeof(buffer[0]), msg, vl );
    fprintf ( stdout, "[%s] [%lu.%lu] \"%s\": %s\n", level, tv.tv_sec, tv.tv_usec, tag, buffer );
}

void Log ( const char* level, const char* tag, const char* msg, ... )
{
    va_list vl;
    va_start ( vl, msg );
    vLog ( level, tag, msg, vl );
    va_end ( vl );
}
