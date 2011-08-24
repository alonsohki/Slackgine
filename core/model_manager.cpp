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
// FILE:        model_manager.cpp
// PURPOSE:     Model resource management. Model requesting and releasing.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>
#include "model_manager.h"
#include "shared/log.h"
#include "l3m/l3m.h"
#include "l3m/Components/require.h"
#include "l3m/Components/texture.h"

using namespace Core;

ModelManager::ModelManager ( TextureManager& textureManager, const Time& time )
: m_textureManager ( textureManager )
, m_time ( time )
{
    AddLookupPath ( "." );
    m_nostarveQueue = PRIORITY_NONE;
    m_graveyard.first = 0;
    m_graveyard.last = 0;
    SetMaxMemory ( DEFAULT_MAX_MEMORY );
    m_currentMemory = 0;
    
#if USE_THREADS
    m_thread.CreateStarted ( MakeDelegate(this, &ModelManager::RequestThread) );
#endif
}

ModelManager::~ModelManager ()
{
#if USE_THREADS
    // Stop the model loading thread
    m_thread.Stop ();
#endif

    // Delete all the allocated models
    while ( m_models.size() > 0 )
    {
        Unlink ( &(m_models.begin()->second), false );
    }
}

#if USE_THREADS
void* ModelManager::RequestThread ( Thread*, void* )
{
    LOG_V ( "ModelManager", "Starting the request thread" );
    for (;;)
    {
        Lock ();
        // Process one by one until the queues are empty.
        if ( !ProcessOne () )
        {
            // If there aren't any models pending to load, sleep until there is
            // a new request.
            m_threadCond.Wait ( m_mutex );
        }
        Unlock ();
    }
}
#endif

bool ModelManager::ProcessOne ( ModelNode** nodeptr )
{
    // First of all, check if any request is starving.
    for ( u32 i = (m_nostarveQueue + 1) % PRIORITY_COUNT;
          i != m_nostarveQueue;
          i = ( i + 1 ) % PRIORITY_COUNT )
    {
        if ( m_queues[i].size() > 0 )
        {
            for ( RequestQueue::iterator iter = m_queues[i].begin();
                  iter != m_queues[i].end();
                  ++iter )
            {
                ModelNode* node = *iter;
                if ( node->loaded == false &&
                    (m_time.GetSystemTimeMS () - node->requestTime) > MAX_TIME_IN_QUEUE )
                {
                    m_nostarveQueue = static_cast<Priority>(i);
                    ProcessRequest ( node );
                    if ( nodeptr != 0 )
                        *nodeptr = node;
                    return true;
                }
            }
        }
    }
    
    // Check for pending models in any queue, starting from the highest priority ones
    for ( u32 i = 0; i < PRIORITY_COUNT; ++i )
    {
        if ( m_queues[PRIORITY_COUNT-i-1].size () > 0 )
        {
            RequestQueue& queue = m_queues[PRIORITY_COUNT-i-1];
            for ( RequestQueue::iterator iter = queue.begin();
                  iter != queue.end();
                  ++iter )
            {
                ModelNode* node = *iter;
                if ( node->loaded == false )
                {
                    ProcessRequest ( node );
                    if ( nodeptr != 0 )
                        *nodeptr = node;
                    return true;
                }
            }
        }
    }
    
    return false;
}

bool ModelManager::AddLookupPath (const std::string& path)
{
    struct stat status;
    if ( stat(path.c_str(), &status) != 0 )
        return false;
    if ( !S_ISDIR(status.st_mode) )
        return false;
    m_paths.push_back ( path );
    LOG_V ( "ModelManager", "Added a new lookup path: %s", path.c_str() );
    
    return true;
}

bool ModelManager::CanModelBeLoaded (const std::string& model) const
{
    return GetModelAccessPath(model, 0) == true;
}

bool ModelManager::GetModelAccessPath (const std::string& model, std::string* path) const
{
    struct stat status;
    
    for ( PathVector::const_reverse_iterator iter = m_paths.rbegin ();
          iter != m_paths.rend();
          ++iter )
    {
        std::string lookupPath = *iter;
        lookupPath += "/";
        lookupPath += model;
        
        if ( stat(lookupPath.c_str(), &status) == 0 )
        {
            if ( !S_ISDIR(status.st_mode) )
            {
                if ( path != 0 )
                    *path = lookupPath;
                return true;
            }
        }
    }
    
    return false;
}

ModelManager::ModelNode* ModelManager::FindModelNode ( const std::string& model )
{
    ModelMap::iterator iter = m_models.find ( model );
    if ( iter != m_models.end() )
        return &(iter->second);
    return 0;
}

ModelManager::ModelNode* ModelManager::FindModelNode ( const l3m::Model* model )
{
    ModelptrMap::iterator iter = m_modelPtrs.find ( model );
    if ( iter != m_modelPtrs.end() )
        return iter->second;
    return 0;
}

bool ModelManager::Request (const std::string& model, RequestCallback callback, Priority priority)
{
    if ( !CanModelBeLoaded(model) )
        return false;
    
#ifdef USE_THREADS
    Lock ();
#endif

    bool ret = InternalRequest ( model, callback, priority );
    
#ifdef USE_THREADS
    Unlock ();
#endif

    return ret;
}

bool ModelManager::InternalRequest ( const std::string& model, RequestCallback callback, Priority priority )
{
    bool shouldProcessQueues = false;
    LOG_V ( "ModelManager", "Requesting model '%s'...", model.c_str() );
    
    // Check if the model has already been loaded.
    ModelNode* node = FindModelNode ( model );
    if ( node != 0 )
    {
        // If the model has already been loaded, return it.
        if ( node->loaded == true )
        {
            // Check if this model was dead to give it life again.
            if ( node->refCount == 0 )
                MoveFromGraveyard ( node );
            if ( callback != 0 )
                callback ( node->model );
        }
        
        // If the model hasn't been loaded yet, but the priority doesn't change,
        // just add us to the requesters.
        else if ( node->requestPriority >= priority )
        {
            node->requestCallbacks.push_back ( callback );
        }
        
        // If it's not loaded and the new priority is higher, re-enqueue it.
        else
        {
            // Move it out of the current queue
            RequestQueue& queue = m_queues[node->requestPriority];
            for ( RequestQueue::iterator iter = queue.begin ();
                  iter != queue.end();
                  ++iter )
            {
                if ( *iter == node )
                {
                    queue.erase ( iter );
                    break;
                }
            }
            
            // Set the new attributes
            node->requestPriority = priority;
            node->requestCallbacks.push_back ( callback );
            
            // If the priority is not PRIORITY_NOW, add it to the new queue.
            // Else, process the request right now.
            if ( priority < PRIORITY_NOW )
            {
                m_queues[priority].push_back (node);
                shouldProcessQueues = true;
            }
            else
            {
                ProcessRequest ( node );
                DispatchRequest ( node );
            }
        }
    }
    else
    {
        // Create a new model node
        node = CreateModelNode ( model );
        node->requestPriority = priority;
        node->requestCallbacks.push_back ( callback );
        
        // Place it in the appropiate queue, if the priority is lower
        // than PRIORITY_NOW.
        if ( priority < PRIORITY_NOW )
        {
            m_queues[priority].push_back (node);
            shouldProcessQueues = true;
        }
        else
        {
            ProcessRequest ( node );
            DispatchRequest ( node );
        }
    }
    
    // Update the reference count
    node->refCount++;
    
#ifdef USE_THREADS
    // Tell the loading thread that there is a new model awaiting.
    if ( shouldProcessQueues )
        m_threadCond.Signal ();
#endif
    
    return true;
}

l3m::Model* ModelManager::RequestBlocking (const std::string& model)
{
    if ( !CanModelBeLoaded(model) )
        return 0;
    
#if USE_THREADS
    Lock ();
#endif
    
    l3m::Model* ret = InternalRequestBlocking ( model );
        
#if USE_THREADS
    Unlock ();
#endif
    
    return ret;
}

l3m::Model* ModelManager::InternalRequestBlocking (const std::string& model)
{
    // Check if the model has already been loaded.
    ModelNode* node = FindModelNode ( model );
    if ( node != 0 )
    {
        node->requestPriority = PRIORITY_NOW;
        if ( node->loaded == false )
            ProcessRequest ( node );
        if ( node->refCount == 0 )
            MoveFromGraveyard ( node );
    }
    else
    {
        node = CreateModelNode ( model );
        node->requestPriority = PRIORITY_NOW;
        node->refCount = 1;
        ProcessRequest ( node );
    }
    
    DispatchRequest ( node );
    return node->model;
}

bool ModelManager::CancelRequest ( const std::string& model, RequestCallback callback )
{
#ifdef USE_THREADS
    Lock ();
#endif
    
    bool ret = InternalCancelRequest ( model, callback );
    
#ifdef USE_THREADS
    Unlock ();
#endif
    
    return ret;
}

bool ModelManager::InternalCancelRequest ( const std::string& model, RequestCallback callback )
{
    bool ret = false;
    ModelNode* node = FindModelNode ( model );
    if ( node != 0 && node->loaded == false )
    {
        // Take out all the callbacks corresponding to the one given by parameter
        for ( CallbackVector::iterator iter = node->requestCallbacks.begin();
              iter != node->requestCallbacks.end();
            )
        {
            if ( *iter == callback )
            {
                ret = true;
                iter = node->requestCallbacks.erase ( iter );
                node->refCount--;
            }
            else
                ++iter;
        }

        // If there are no callbacks left, take it out of the queue and map
        if ( node->requestCallbacks.size() == 0 )
        {
            Unlink ( node, false );
        }
    }
    
    return ret;
}

bool ModelManager::Release (const l3m::Model* model)
{
    bool ret;
    
#ifdef USE_THREADS
    Lock ();
#endif
    
    ret = InternalRelease ( FindModelNode ( model ) );
    
#ifdef USE_THREADS
    Unlock ();
#endif
    
    return ret;
}

bool ModelManager::ReleaseAllReferences (const l3m::Model* model)
{
    bool ret = false;
    
#ifdef USE_THREADS
    Lock ();
#endif

    ModelNode* node = FindModelNode ( model );
    if ( node != 0 )
    {
        node->refCount = 1;
        ret = InternalRelease ( node );
    }

#ifdef USE_THREADS
    Unlock ();
#endif

    return ret;
}

bool ModelManager::InternalRelease (ModelNode* node)
{
    bool ret = false;
    
    if ( node != 0 )
    {
        if ( node->refCount > 0 )
        {
            --node->refCount;
            if ( node->refCount == 0 )
            {
                LOG_V ( "ModelManager", "Releasing model '%s' (%p)", node->name.c_str(), node->model );
                
                // Release it
                Unlink ( node, true );
                ret = true;
            }
        }
    }
    
    return ret;
}
    
#if USE_THREADS
void ModelManager::Lock ()
{
    m_mutex.Lock ();
}
void ModelManager::Unlock ()
{
    m_mutex.Unlock ();
}
#endif

ModelManager::ModelNode* ModelManager::CreateModelNode (const std::string& model)
{
    std::pair<ModelMap::iterator, bool> pair = m_models.insert ( ModelMap::value_type(model, ModelNode()) );
    ModelNode* node = &(pair.first->second);
    node->name = model;
    node->model = 0;
    node->refCount = 0;
    node->loaded = false;
    node->graveyard_next = 0;
    node->graveyard_prev = 0;
    node->requestPriority = PRIORITY_NONE;
    node->requestCallbacks.clear ();
    node->requestTime = m_time.GetSystemTimeMS ();
    return node;
}

void ModelManager::ProcessRequest (ModelNode* node)
{
    using l3m::Model;
    using l3m::IComponent;
    using l3m::Require;
    using l3m::Texture;
    
    // Load the model and mark it as loaded
    std::string accessPath;
    if ( GetModelAccessPath(node->name, &accessPath) )
    {
        LOG_VV ( "ModelManager", "Processing request for '%s' at path '%s'", node->name.c_str(), accessPath.c_str() );
        Model* model = new Model ();
        model->Load ( accessPath );
        node->model = model;
        
        // Insert it into the model ptrs map
        m_modelPtrs.insert ( ModelptrMap::value_type ( model, node ) );
        
        // Account the memory used by this model
        m_currentMemory += model->size ();
        LOG_VV ( "ModelManager", "Model took %u bytes. GC status: %u/%u bytes (%.2f%%)", model->size(), m_currentMemory, GetMaxMemory (), 100.0f*m_currentMemory/(float)GetMaxMemory() );

        // Check every component for special actions.
        Model::componentVector& comps = model->components ();
        for ( Model::componentVector::iterator iter = comps.begin ();
              iter != comps.end();
              ++iter )
        {
            IComponent* comp = *iter;
            
            ////////////////////////////////////////////////////////////////////
            // Track its dependencies
            //
            if ( comp->type() == "require" )
            {
                Require* req = static_cast < Require* > ( comp );
                std::string reqPath = req->makepath ();
                if ( reqPath != "" )
                {
                    LOG_VV ( "ModelManager", "Processing request dependency '%s'", reqPath.c_str() );
                    if ( node->requestPriority == PRIORITY_NOW )
                    {
                        // Load the requirements blocking
                        l3m::Model* depModel = InternalRequestBlocking ( reqPath );
                        if ( depModel != 0 )
                            node->loadedDeps.push_back ( depModel );
                    }
                    else
                    {
                        MakeDependencyTracker ( node, reqPath );
                    }
                }
            }
            
            ////////////////////////////////////////////////////////////////////
            // Register the textures into the texture manager
            //
            else if ( comp->type() == "texture" )
            {
                Texture* tex = static_cast < Texture* > ( comp );
                if ( tex != 0 )
                    m_textureManager.Register ( *tex );
            }
        }
        
        bool hasDeps = node->requestedDeps.size() > 0;
        node->loaded = !hasDeps;
        if ( hasDeps )
        {
            // Take it out of its priority queue and move it to the dependency waiters
            RequestQueue& queue = m_queues [ node->requestPriority ];
            for ( RequestQueue::iterator iter = queue.begin();
                  iter != queue.end();
                  ++iter )
            {
                if ( *iter == node )
                {
                    queue.erase ( iter );
                    break;
                }
            }
            m_dependencyWaiters.push_back ( node );
            
            // Request all the dependencies
            DependencyList& deps = node->requestedDeps;
            for ( DependencyList::iterator iter = deps.begin();
                  iter != deps.end();
                  ++iter )
            {
                DependencyTracker& dep = *iter;
                InternalRequest ( dep.name, MakeDelegate ( &dep, &DependencyTracker::OnLoad ), node->requestPriority );
            }
        }
    }
}

void ModelManager::DispatchRequest ( ModelNode* req )
{
    LOG_VV ( "ModelManager", "Dispatching the request for the model '%s'", req->name.c_str() );
    
    // Call the callback chain
    for ( CallbackVector::iterator iter = req->requestCallbacks.begin();
          iter != req->requestCallbacks.end();
          ++iter )
    {
        RequestCallback& cbk = *iter;
        if ( cbk != 0 )
            cbk ( req->model );
    }
    req->requestCallbacks.clear ();
    
    // Take it out of the queue
    if ( req->requestPriority < PRIORITY_NOW )
    {
        RequestQueue& queue = m_queues[req->requestPriority];
        for ( RequestQueue::iterator iter = queue.begin();
              iter != queue.end();
              ++iter )
        {
            if ( *iter == req )
            {
                queue.erase ( iter );
                break;
            }
        }
    }
}

void ModelManager::Tick ()
{
#if USE_THREADS
    Lock ();
    
    // Dispatch all the models that have been loaded by the loading thread
    for ( u32 i = 0; i < PRIORITY_COUNT; ++i )
    {
        RequestQueue& queue = m_queues[PRIORITY_COUNT-i-1];
        while ( queue.size() > 0 && queue.front()->loaded == true )
        {
            DispatchRequest ( queue.front() );
        }
    }
#else
    // Process models using SYNCHRONOUS_MAX_TIME as time limit.
    u64 timeLimit = m_time.GetSystemTimeMS() + SYNCHRONOUS_MAX_TIME;
    std::vector<ModelNode*> processed_nodes;
    ModelNode* node;
    while ( timeLimit > m_time.GetSystemTimeMS() )
    {
        if ( ProcessOne ( &node ) == false )
            break;
        processed_nodes.push_back ( node );
    }
    
    for ( std::vector<ModelNode*>::iterator iter = processed_nodes.begin();
          iter != processed_nodes.end();
          ++iter )
    {
        DispatchRequest ( *iter );
    }
#endif

    // Check the dependency waiters
    for ( RequestList::iterator iter = m_dependencyWaiters.begin();
          iter != m_dependencyWaiters.end();
        )
    {
        ModelNode* node = *iter;
        if ( node->loaded == true )
        {
            DispatchRequest ( node );
            iter = m_dependencyWaiters.erase ( iter );
        }
        else
            ++iter;
    }

    CollectGarbage ();

#ifdef USE_THREADS
    Unlock ();
#endif
}

void ModelManager::LoadAllRequestedModels ()
{
#ifdef USE_THREADS
    Lock ();
#endif
    
    // Flush all queues
    while ( ProcessOne() );
    
    for ( u32 i = 0; i < PRIORITY_COUNT; ++i )
    {
        RequestQueue& queue = m_queues[PRIORITY_COUNT-i-1];
        while ( queue.size() >  0 )
            DispatchRequest ( queue.front () );
    }
    
#ifdef USE_THREADS
    Unlock ();
#endif
}

void ModelManager::Unlink ( ModelNode* node, bool toTheGraveyard )
{
    // Unlink it form the queues.
    if ( node->loaded == false && node->requestPriority <= PRIORITY_HIGH )
    {
        RequestQueue& queue = m_queues [ node->requestPriority ];
        for ( RequestQueue::iterator iter = queue.begin();
              iter != queue.end();
              ++iter )
        {
            if ( *iter == node )
            {
                queue.erase ( iter );
                break;
            }
        }
    }
    
    // Manage the dependencies
    if ( node->requestedDeps.size() > 0 )
    {
        // Unlink it from the dependency waiters
        for ( RequestList::iterator iter = m_dependencyWaiters.begin();
              iter != m_dependencyWaiters.end();
              ++iter )
        {
            if ( *iter == node )
            {
                m_dependencyWaiters.erase ( iter );
                break;
            }
        }
    }

    if ( !toTheGraveyard )
    {
        LOG_V ( "ModelManager", "Deleting model '%s' (%p)", node->name.c_str(), node->model );
        
        // Completely delete it.
        if ( node->model != 0 )
        {
            // Discount the memory used by this model.
            m_currentMemory -= node->model->size ();
            LOG_VV ( "ModelManager", "GC status after freeing model: %u/%u bytes (%.2f%%)", m_currentMemory, GetMaxMemory (), 100.0f*m_currentMemory/(float)GetMaxMemory() );
            
            // Unregister the textures from the texture manager.
            l3m::Model::componentVector& components = node->model->components ();
            for ( l3m::Model::componentVector::iterator iter = components.begin();
                  iter != components.end();
                  ++iter )
            {
                l3m::IComponent* comp = *iter;
                if ( comp->type() == "texture" )
                {
                    l3m::Texture* tex = static_cast < l3m::Texture* > ( comp );
                    if ( tex != 0 )
                        m_textureManager.Release ( *tex );
                }
            }
            
            // Cancel the dependency requests
            for ( DependencyList::iterator iter = node->requestedDeps.begin();
                  iter != node->requestedDeps.end();
                  ++iter )
            {
                DependencyTracker& tracker = *iter;
                InternalCancelRequest ( tracker.name, MakeDelegate ( &tracker, &DependencyTracker::OnLoad ) );
            }

            // Release the loaded dependencies
            for ( ModelVector::iterator iter = node->loadedDeps.begin();
                  iter != node->loadedDeps.end();
                  ++iter )
            {
                InternalRelease ( FindModelNode ( *iter ) );
            }
            
            // Unlink it from the graveyard
            if ( node->refCount == 0 )
            {
                if ( m_graveyard.first == node )
                    m_graveyard.first = node->graveyard_next;
                if ( m_graveyard.last == node )
                    m_graveyard.last = node->graveyard_prev;
                if ( node->graveyard_next )
                    node->graveyard_next->graveyard_prev = node->graveyard_prev;
                if ( node->graveyard_prev )
                    node->graveyard_prev->graveyard_next = node->graveyard_next;
            }
            
            m_garbage.push_back ( node->model );
            ModelptrMap::iterator iter = m_modelPtrs.find ( node->model );
            if ( iter != m_modelPtrs.end() )
                m_modelPtrs.erase ( iter );
        }
        ModelMap::iterator iter = m_models.find ( node->name );
        if ( iter != m_models.end() )
            m_models.erase ( iter );
    }
    else
    {
        LOG_V ( "ModelManager", "Sending model '%s' to the graveyard (%p)", node->name.c_str(), node->model );
        
        // Make sure that it loses its identity.
        node->refCount = 0;
        node->requestCallbacks.clear ();
        node->requestPriority = PRIORITY_NONE;
        node->requestTime = 0;
        
        // Link it to the graveyard.
        if ( m_graveyard.first == 0 )
        {
            m_graveyard.first = node;
            m_graveyard.last = node;
            node->graveyard_next = 0;
            node->graveyard_prev = 0;
        }
        else
        {
            node->graveyard_next = 0;
            node->graveyard_prev = m_graveyard.last;
            m_graveyard.last->graveyard_next = node;
            m_graveyard.last = node;
        }
    }
}

void ModelManager::MoveFromGraveyard (ModelNode* node)
{
    LOG_V ( "ModelManager", "Rescuing model '%s' from the graveyard (%p)", node->name.c_str(), node->model );

    // Dig deep in the tomb and take the body out.
    if ( node == m_graveyard.first )
        m_graveyard.first = node->graveyard_next;
    if ( node == m_graveyard.last )
        m_graveyard.last = node->graveyard_prev;
    if ( node->graveyard_next )
        node->graveyard_next->graveyard_prev = node->graveyard_prev;
    if ( node->graveyard_prev )
        node->graveyard_prev->graveyard_next = node->graveyard_next;
    
    node->refCount++;
}

void ModelManager::ClearUnlinkedModels ()
{
#ifdef USE_THREADS
    Lock ();
#endif
    
    u32 maxMemory = GetMaxMemory ();
    SetMaxMemory ( 0 );
    CollectGarbage ();
    SetMaxMemory ( maxMemory );
    
#ifdef USE_THREADS
    Unlock ();
#endif
}

void ModelManager::CollectGarbage ()
{
    for ( GarbageVector::iterator iter = m_garbage.begin();
          iter != m_garbage.end();
          ++iter )
    {
        LOG_VV ( "ModelManager", "Freeing model (%p) from memory", *iter );
        delete *iter;
    }
    m_garbage.clear ();
    
    if ( m_currentMemory > m_maxMemory )
    {
        // Check the graveyard to kill some zombies to make space.
        u32 numCollected = 0;
        ModelNode* ptrNext;
        for ( ModelNode* ptr = m_graveyard.first;
              ptr != 0 && m_currentMemory > m_maxMemory;
              ptr = ptrNext)
        {
            ptrNext = ptr->graveyard_next;
            Unlink ( ptr, false );
            ++numCollected;
        }
        
        if ( numCollected > 0 )
            LOG_V ( "ModelManager", "Collected %u models from garbage", numCollected );
    }
}

bool ModelManager::MakeDependencyTracker (ModelNode* node, const std::string& dep)
{
    if ( !CanModelBeLoaded ( dep ) )
        return false;
    
    DependencyTracker tracker;
    tracker.parent = node;
    tracker.name = dep;
    node->requestedDeps.push_back ( tracker );
    return true;
}

bool ModelManager::DependencyTracker::OnLoad (l3m::Model* model)
{
    for ( DependencyList::iterator iter = parent->requestedDeps.begin();
          iter != parent->requestedDeps.end();
          ++iter )
    {
        if ( &(*iter) == this )
        {
            LOG_VV ( "ModelManager", "Dependency name '%s' for the model '%s' satisfied successfuly", (*iter).name.c_str(), parent->name.c_str() );
            parent->requestedDeps.erase ( iter );
            parent->loadedDeps.push_back ( model );
            break;
        }
    }
    
    if ( parent->requestedDeps.size () == 0 )
    {
        LOG_VV ( "ModelManager", "All the model '%s' dependencies have been satisfied", parent->name.c_str() );
        parent->loaded = true;
    }
    
    return true;
}
