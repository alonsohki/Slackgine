/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        thread_condition.cpp
// PURPOSE:     Thread condition encapsulation.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#if USE_THREADS

#include "thread_condition.h"

ThreadCondition::ThreadCondition ()
{
    pthread_cond_init ( &m_handler, 0 );
}

ThreadCondition::~ThreadCondition ()
{
    pthread_cond_destroy ( &m_handler );
}

void ThreadCondition::Signal ()
{
    pthread_cond_signal ( &m_handler );
}

void ThreadCondition::Broadcast ()
{
    pthread_cond_broadcast ( &m_handler );
}

void ThreadCondition::Wait ( Mutex& mutex )
{
    pthread_cond_wait ( &m_handler, &mutex.m_handler );
}

#endif
