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
// FILE:        model_manager.h
// PURPOSE:     Model resource management. Model requesting and releasing.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#include <deque>
#include <vector>
#include <list>
#include "shared/FastDelegate.h"
#include "l3m/l3m.h"
#if USE_THREADS
#       include "shared/thread.h"
#       include "shared/mutex.h"
#       include "shared/thread_condition.h"
#endif
#include "time.h"

namespace Core
{
    
class ModelManager
{
    friend class Slackgine;

public:
    enum Priority
    {
        PRIORITY_NONE   = 0,
        PRIORITY_LOW    = 1,
        PRIORITY_NORMAL = 2,
        PRIORITY_HIGH   = 3,
        PRIORITY_NOW    = 4
    };
    
private:
    enum
    {
        MAX_TIME_IN_QUEUE       = 5000, // In miliseconds
        PRIORITY_COUNT          = 4,
        SYNCHRONOUS_MAX_TIME    = 100,  // Max time in ms to use when not using threads.
        DEFAULT_MAX_MEMORY      = 100   // Max memory in bytes as threshold for the garbage collection
    };
    
public:
    typedef FastDelegate1<l3m::Model*, bool> RequestCallback;
    typedef std::vector<RequestCallback> CallbackVector;
private:
    // Structure to track the dependencies
    struct ModelNode;
    
    struct DependencyTracker
    {
        ModelNode*      parent;
        std::string     name;
        
        bool    OnLoad  ( l3m::Model* model );
    };
    typedef std::list < DependencyTracker > DependencyList;
    typedef std::vector < l3m::Model* > ModelVector;
    
    struct ModelNode
    {
        std::string     name;
        l3m::Model*     model;
        bool            loaded;
        u32             refCount;
        
        // Dependencies
        DependencyList  requestedDeps;
        ModelVector     loadedDeps;
        
        // Graveyard stuff
        ModelNode*      graveyard_next;
        ModelNode*      graveyard_prev;
        
        // Request stuff
        CallbackVector  requestCallbacks;
        time_t          requestTime;
        Priority        requestPriority;
    };
    typedef std::map<std::string, ModelNode> ModelMap;
    typedef std::map<const l3m::Model*, ModelNode*> ModelptrMap;
    typedef std::deque<ModelNode*> RequestQueue;
    typedef std::list<ModelNode*> RequestList;
    typedef std::vector<std::string> PathVector;
    typedef std::vector<l3m::Model*> GarbageVector;
    
    
private:
                        ModelManager                    ( const Time& time );
                        ~ModelManager                   ();
    
public:
    bool                AddLookupPath                   ( const std::string& path );
    void                SetMaxMemory                    ( u32 limit ) { m_maxMemory = limit; }
    u32                 GetMaxMemory                    () const { return m_maxMemory; }
    
    bool                Request                         ( const std::string& model, RequestCallback callback, Priority priority = PRIORITY_NORMAL );
    l3m::Model*         RequestBlocking                 ( const std::string& model );
    bool                CancelRequest                   ( const std::string& model, RequestCallback callback );
    bool                Release                         ( const l3m::Model* model );
    bool                ReleaseAllReferences            ( const l3m::Model* model );

    void                LoadAllRequestedModels          ();
    void                ClearUnlinkedModels             ();
    
    void                Tick                            ();
    
private:
    // Request management functions
    bool                InternalRequest                 ( const std::string& model, RequestCallback callback, Priority priority );
    l3m::Model*         InternalRequestBlocking         ( const std::string& model );
    bool                InternalCancelRequest           ( const std::string& model, RequestCallback callback );
    
    bool                ProcessOne                      ( ModelNode** nodeptr = 0 );
    ModelNode*          CreateModelNode                 ( const std::string& model );
    ModelNode*          FindModelNode                   ( const std::string& model );
    ModelNode*          FindModelNode                   ( const l3m::Model* model );
    bool                GetModelAccessPath              ( const std::string& model, std::string* path = 0 ) const;
    bool                CanModelBeLoaded                ( const std::string& model ) const;
    void                ProcessRequest                  ( ModelNode* req );
    void                DispatchRequest                 ( ModelNode* req );
#if USE_THREADS
    void*               RequestThread                   ( Thread*, void* );
    void                Lock                            ();
    void                Unlock                          ();
#endif
    void                Unlink                          ( ModelNode* node, bool toTheGraveyard = true );
    bool                InternalRelease                 ( ModelNode* node );
    void                MoveFromGraveyard               ( ModelNode* node );
    void                CollectGarbage                  ();
    bool                MakeDependencyTracker           ( ModelNode* node, const std::string& dep );
    
    
private:
    const Time&         m_time;
    ModelMap            m_models;
    ModelptrMap         m_modelPtrs;
    RequestQueue        m_queues [ 4 ];
    RequestList         m_dependencyWaiters;
    struct
    {
        ModelNode*      first;
        ModelNode*      last;
    } m_graveyard;
    Priority            m_nostarveQueue;
#if USE_THREADS
    Thread              m_thread;
    Mutex               m_mutex;
    ThreadCondition     m_threadCond;
#endif
    PathVector          m_paths;
    u32                 m_maxMemory;
    u32                 m_currentMemory;
    GarbageVector       m_garbage;
};
    
}