/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        memory_leak_detector.h
// PURPOSE:     Class that logs all the allocations over a threshold and throws
//              a warning if they haven't been collected after a certain time.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#include <list>
#include <set>
#include <sys/types.h>
#include "memory.h"

class MemoryLeakDetector
{
public:
    //--------------------------------------------------------------------------
    // Configuration
    enum
    {
        ALLOCATION_THRESHOLD    = 1024 * 512,      // Log only allocations of more than 512KB
        MAX_TIME_ALLOCATED      = 60000            // Maximum time in miliseconds that a logged block should remain allocated
    };
    
    //--------------------------------------------------------------------------
    // Singleton stuff
private:
    static MemoryLeakDetector ms_instance;
public:
    static MemoryLeakDetector* getInstance ()
    {
        return &ms_instance;
    }
    
    //--------------------------------------------------------------------------
    // Nodes allocating the contained data
public:
    struct Node
    {
        u64         dateMs;
        const char* file;
        u32         line;
        void*       ptr;
        size_t      size;
        bool        warned;
    };
    typedef std::list<Node> NodeList;
private:
    NodeList mAllocations;
    
    //--------------------------------------------------------------------------
    // Also use a checks set to know if we have tracked a pointer
    typedef std::set<void*> CheckSet;
    CheckSet mChecks;
    
    //--------------------------------------------------------------------------
    // Do not log the allocations made inside this class (for example, when
    // saving one of the nodes data). Else, it would create an infinite loop
    // recursion.
    bool mAllocatingMyself;
    
    //--------------------------------------------------------------------------
    // Toggle to know if the warning messages are enabled
    bool mWarningsEnabled;


    //--------------------------------------------------------------------------
    // Construction/Destruction
private:
    MemoryLeakDetector  ();
    ~MemoryLeakDetector ();
    
public:
    //--------------------------------------------------------------------------
    // These functions allow enabling and disabling the warning messages
    void    setWarningsEnabled  ( bool state );
    bool    areWarningsEnabled  () { return mWarningsEnabled; }
    
    //--------------------------------------------------------------------------
    // Function that returns a list with the currently logged allocations
    const NodeList&     getAllocations      () const { return mAllocations; }
    
    //--------------------------------------------------------------------------
    // Functions used to register allocations.
private:
    friend class MemoryManager;
    void    logAllocation   ( void* ptr, u32 size, const char* file, u32 line );
    void    logFree         ( void* ptr );
    
    //--------------------------------------------------------------------------
    // Function that throws the memory leak warnings
private:
    void    checkMemoryLeaks    ();
};
