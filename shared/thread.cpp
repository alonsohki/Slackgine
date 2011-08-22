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
// FILE:        thread.cpp
// PURPOSE:     Thread encapsulation.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#if USE_THREADS

#include <signal.h>
#include "thread.h"

Thread::Thread ( MainFn fn, u32 flags )
: m_flags ( 0 )
{
    Create ( fn, flags );
}

void Thread::Create ( MainFn fn, u32 flags )
{
    if ( !IsCreated() )
    {
        m_flags = flags;
        m_mainFn = fn;

        // Setup the attributes
        pthread_attr_init (&m_attributes);
        pthread_attr_setdetachstate (&m_attributes, ( m_flags & THREAD_JOINABLE ) == THREAD_JOINABLE );
        
        m_flags |= THREAD_CREATED;
        m_flags &= ~THREAD_STARTED;
    }
}

void Thread::CreateStarted (MainFn fn, void* data, u32 flags)
{
    Create ( fn, flags );
    Start ( data );
}

Thread::~Thread ()
{
    Stop ();
    if ( IsCreated() )
        pthread_attr_destroy (&m_attributes);
}

void Thread::Start ( void* data )
{
    if ( !IsStarted() )
    {
        // Create the init struct
        m_initStruct.data = data;

        // Create the thread
        pthread_create ( &m_handler, &m_attributes, __thread_init, this );
        m_flags |= THREAD_STARTED;
    }
}

void Thread::Stop ()
{
    if ( IsStarted() )
    {
        m_flags &= ~THREAD_STARTED;
        pthread_cancel ( m_handler );
    }
}

void* Thread::__thread_init (void* data)
{
    Thread* thiz = (Thread *)data;
    void* ret = thiz->m_mainFn ( thiz, thiz->m_initStruct.data );
    return ret;
}

void Thread::Join ( void** ret )
{
    if ( ( m_flags & THREAD_JOINABLE ) && IsStarted() )
    {
        pthread_join ( m_handler, ret );
    }
}

void Thread::Detach ()
{
    m_flags &= ~THREAD_JOINABLE;
    if ( IsStarted() )
        pthread_detach ( m_handler );
    else if ( IsCreated() )
        pthread_attr_setdetachstate ( &m_attributes, 1 );
}

#endif
