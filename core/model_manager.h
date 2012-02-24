/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
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
#include "texture_manager.h"
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
        MAX_TIME_IN_QUEUE       = 5000,         // In miliseconds
        PRIORITY_COUNT          = 4,
        SYNCHRONOUS_MAX_TIME    = 100,          // Max time in ms to use when not using threads.
        DEFAULT_MAX_MEMORY      = 100*1024*1024 // Max memory in bytes as threshold for the garbage collection
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
        
        bool    onLoad  ( l3m::Model* model );
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
                        ModelManager                    ( TextureManager& textureManager, const Time& time );
                        ~ModelManager                   ();
    
public:
    bool                addLookupPath                   ( const std::string& path );
    void                setMaxMemory                    ( u32 limit ) { m_maxMemory = limit; }
    u32                 getMaxMemory                    () const { return m_maxMemory; }
    
    bool                loadFromStream                  ( std::istream& stream, l3m::Model* model );
    bool                request                         ( const std::string& model, RequestCallback callback, Priority priority = PRIORITY_NORMAL );
    l3m::Model*         requestBlocking                 ( const std::string& model );
    bool                cancelRequest                   ( const std::string& model, RequestCallback callback );
    bool                release                         ( const l3m::Model* model );
    bool                releaseAllReferences            ( const l3m::Model* model );

    //-------------------------------------------------------------------------
    // This method releases a model like he normal release, but prevents it
    // from going to the graveyard and forces its deletion.
    bool                releaseUnloading                ( const l3m::Model* model );
    bool                releaseAllReferencesUnloading   ( const l3m::Model* model );

    void                loadAllRequestedModels          ();
    void                clearUnlinkedModels             ();
    
    void                tick                            ();
    
private:
    // Request management functions
    bool                internalRequest                 ( const std::string& model, RequestCallback callback, Priority priority );
    l3m::Model*         internalRequestBlocking         ( const std::string& model );
    bool                internalCancelRequest           ( const std::string& model, RequestCallback callback );
    
    bool                processOne                      ( ModelNode** nodeptr = 0 );
    ModelNode*          createModelNode                 ( const std::string& model );
    ModelNode*          findModelNode                   ( const std::string& model );
    ModelNode*          findModelNode                   ( const l3m::Model* model );
    bool                getModelAccessPath              ( const std::string& model, std::string* path = 0 ) const;
    bool                canModelBeLoaded                ( const std::string& model ) const;
    void                processRequest                  ( ModelNode* req );
    void                dispatchRequest                 ( ModelNode* req );
#if USE_THREADS
    void*               requestThread                   ( Thread*, void* );
    void                lock                            ();
    void                unlock                          ();
#endif
    void                unlink                          ( ModelNode* node, bool toTheGraveyard = true );
    bool                internalRelease                 ( ModelNode* node, bool moveToGraveyard = true );
    void                moveFromGraveyard               ( ModelNode* node );
    void                collectGarbage                  ();
    bool                makeDependencyTracker           ( ModelNode* node, const std::string& dep );
    void                processLoadedModel              ( l3m::Model* model, ModelNode* node );
    
    
private:
    TextureManager&     m_textureManager;
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
