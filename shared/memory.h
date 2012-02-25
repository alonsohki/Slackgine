/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        memory.h
// PURPOSE:     Memory management
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#include "platform.h"
#include <sys/types.h>
#include <new>

class MemoryManager
{
public:
    static void* alloc  ( size_t size, const char* file, u32 line );
    static void  free   ( void* ptr );
};

#ifdef DEBUG
extern void* operator new ( size_t size ) throw(std::bad_alloc);
extern void* operator new[] ( size_t size ) throw(std::bad_alloc);
extern void* operator new ( size_t size, const char* file, u32 line ) throw(std::bad_alloc);
extern void* operator new[] ( size_t size, const char* file, u32 line ) throw(std::bad_alloc);
extern void operator delete ( void* ptr ) throw();
extern void operator delete[] ( void* ptr ) throw();
#define sgMalloc(size) MemoryManager::alloc ( size, __FILE__, __LINE__ )
#define sgFree(ptr) MemoryManager::free ( ptr )
#define sgNew new (__FILE__, __LINE__)
#define sgDelete delete
#else
#define sgMalloc malloc
#define sgFree free
#define sgNew new
#define sgDelete delete
#endif
