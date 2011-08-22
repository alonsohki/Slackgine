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
