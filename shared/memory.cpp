/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        memory.cpp
// PURPOSE:     Memory management
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include <cstdio>
#include <cstdlib>
#include "memory.h"
#include "memory_leak_detector.h"

void* MemoryManager::alloc ( size_t size, const char* file, u32 line )
{
    void* ptr = malloc ( size );
#ifdef DETECT_MEMORY_LEAKS
    MemoryLeakDetector::getInstance()->logAllocation ( ptr, size, file, line );
#endif
    return ptr;
}

void MemoryManager::free ( void* ptr )
{
#ifdef DETECT_MEMORY_LEAKS
    MemoryLeakDetector::getInstance()->logFree ( ptr );
#endif
    return ::free ( ptr );
}

#ifdef DEBUG
void* operator new ( size_t size ) throw(std::bad_alloc)
{
    return MemoryManager::alloc ( size, "<global new operator>", 0 );
}

void* operator new[] ( size_t size ) throw(std::bad_alloc)
{
    return MemoryManager::alloc ( size, "<global new[] operator>", 0 );
}

void* operator new ( size_t size, const char* file, u32 line ) throw(std::bad_alloc)
{
    return MemoryManager::alloc ( size, file, line );
}

void* operator new[] ( size_t size, const char* file, u32 line ) throw(std::bad_alloc)
{
    return MemoryManager::alloc ( size, file, line );
}

void operator delete ( void* ptr ) throw()
{
    return MemoryManager::free ( ptr );
}

void operator delete[] ( void* ptr ) throw()
{
    return MemoryManager::free ( ptr );
}
#endif

