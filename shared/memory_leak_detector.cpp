/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2012
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        memory_leak_detector.cpp
// PURPOSE:     Class that logs all the allocations over a threshold and throws
//              a warning if they haven't been collected after a certain time.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include <cstdio>
#include <ctime>
#include <core/time.h>
#include "memory_leak_detector.h"
#include "util.h"

MemoryLeakDetector MemoryLeakDetector::ms_instance;

MemoryLeakDetector::MemoryLeakDetector()
{
#ifdef DETECT_MEMORY_LEAKS
    mWarningsEnabled = true;
#else
    mWarningsEnabled = false;
#endif
    mAllocatingMyself = false;
}

MemoryLeakDetector::~MemoryLeakDetector()
{
}

void MemoryLeakDetector::setWarningsEnabled(bool state)
{
    mWarningsEnabled = state;
}

void MemoryLeakDetector::logAllocation(void* ptr, u32 size, const char* file, u32 line)
{
    //--------------------------------------------------------------------------
    // Do not log allocations below a given threshold.
    if ( size < ALLOCATION_THRESHOLD )
        return;
    
    if ( mAllocatingMyself == true )
        return;
    
    if ( mWarningsEnabled )
        checkMemoryLeaks();
    
    //--------------------------------------------------------------------------
    // Construct a node and add it to the allocation list.
    Node node;
    node.dateMs = Core::Time::getSystemTimeMS();
    node.file = file;
    node.line = line;
    node.ptr = ptr;
    node.size = size;
    node.warned = !mWarningsEnabled;
    mAllocations.push_front( node );
    
    //--------------------------------------------------------------------------
    // Mark this pointer as logged
    mChecks.insert(ptr);
    
    mAllocatingMyself = true;
    mAllocatingMyself = false;
}

void MemoryLeakDetector::logFree(void* ptr)
{
    if ( mAllocatingMyself == true )
        return;
    
    //--------------------------------------------------------------------------
    // First of all check if we are tracking this pointer
    CheckSet::iterator check = mChecks.find ( ptr );
    if ( check != mChecks.end() )
    {
        //----------------------------------------------------------------------
        // Find and remove the entry
        mChecks.erase ( check );
        for ( NodeList::iterator iter = mAllocations.begin();
            iter != mAllocations.end();
            ++iter )
        {
            Node& node = *iter;
            if ( node.ptr == ptr )
            {
                mAllocations.erase ( iter );
                break;
            }
        }
    }
    
    if ( mWarningsEnabled )
        checkMemoryLeaks();
    
    mAllocatingMyself = true;
    mAllocatingMyself = false;
}

void MemoryLeakDetector::checkMemoryLeaks()
{
    u64 now = Core::Time::getSystemTimeMS();
    for ( NodeList::iterator iter = mAllocations.begin();
          iter != mAllocations.end();
          ++iter )
    {
        Node& node = *iter;
        
        //----------------------------------------------------------------------
        // Nodes are inserted sorted by time, so it's safe to stop here.
        if ( node.warned == true )
            break;
        
        if ( ( node.dateMs + (MAX_TIME_ALLOCATED*1000) ) <= now )
        {
            char sizeBuffer [ 64 ];
            fprintf ( stderr,
                      "**** WARNING ****: Possible memory leak at %p (%s). Allocated from %s:%u\n",
                      node.ptr, niceDataSize(node.size, sizeBuffer, sizeof(sizeBuffer)),
                      node.file, node.line );
            node.warned = true;
        }
    }
}
