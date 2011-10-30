/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        mutex.cpp
// PURPOSE:     Mutex encapsulation.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#if USE_THREADS

#include "mutex.h"

Mutex::Mutex ()
{
    pthread_mutex_init ( &m_handler, 0 );
}

Mutex::~Mutex ()
{
    pthread_mutex_destroy ( &m_handler );
}

void Mutex::Lock ()
{
    pthread_mutex_lock ( &m_handler );
}

bool Mutex::TryLock ()
{
    return pthread_mutex_trylock ( &m_handler ) == 0;
}

void Mutex::Unlock ()
{
    pthread_mutex_unlock ( &m_handler );
}

#endif
