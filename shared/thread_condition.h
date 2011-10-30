/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        thread_condition.h
// PURPOSE:     Thread condition encapsulation.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#if USE_THREADS

#pragma once

#include <pthread.h>
#include "mutex.h"

class ThreadCondition
{
public:
                ThreadCondition         ();
                ~ThreadCondition        ();
        
    void        Wait                    ( Mutex& mutex );
    void        Signal                  ();
    void        Broadcast               ();
        
private:
    pthread_cond_t      m_handler;
};

#endif
