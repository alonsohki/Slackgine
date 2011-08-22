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
