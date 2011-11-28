/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        log.h
// PURPOSE:     Slackgine logging.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include <cstdarg>

#define LOGGING
#define DEBUG_VERY_VERBOSE

#ifdef LOGGING
#       define LOG_I(tag, msg,...) Log("INFO", tag, msg, ## __VA_ARGS__ )
#       define LOG_W(tag, msg,...) Log("WARNING", tag, msg, ## __VA_ARGS__ )
#       define LOG_E(tag, msg,...) Log("ERROR", tag, msg, ## __VA_ARGS__ )

#       ifdef DEBUG
#               define LOG_V(tag, msg,...) Log("VERBOSE", tag, msg, ## __VA_ARGS__ )
#       else
#               define LOG_V(...)
#       endif

#       ifdef DEBUG_VERY_VERBOSE
#               define LOG_VV(tag, msg,...) Log("VVERBOSE", tag, msg, ## __VA_ARGS__ )
#       else
#               define LOG_VV(...)
#       endif
#else
#       define LOG_I(...)
#       define LOG_W(...)
#       define LOG_E(...)
#       define LOG_V(...)
#       define LOG_VV(...)
#endif

extern void vLog ( const char* level, const char* tag, const char* msg, va_list vl );
extern void Log ( const char* level, const char* tag, const char* msg, ... );
