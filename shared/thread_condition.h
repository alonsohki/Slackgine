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
