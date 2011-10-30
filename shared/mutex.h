/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        mutex.h
// PURPOSE:     Mutex encapsulation.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#if USE_THREADS

#include <pthread.h>

class Mutex
{
    friend class ThreadCondition;
    
public:
                Mutex           ();
                ~Mutex          ();
                
    void        Lock            ();
    bool        TryLock         ();
    void        Unlock          ();
    
private:
    pthread_mutex_t     m_handler;
};

#endif
