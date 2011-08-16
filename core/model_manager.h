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
#include "shared/FastDelegate.h"
#include "l3m/l3m.h"

namespace Core
{
    
class ModelManager
{
public:
    enum Priority
    {
        PRIORITY_NONE,
        PRIORITY_LOW,
        PRIORITY_NORMAL,
        PRIORITY_HIGH,
        PRIORITY_NOW
    };
    
public:
    typedef fastdelegate::FastDelegate1<bool, l3m::Model*> RequestCallback;
    typedef std::vector<RequestCallback> callbackVector;
private:
    struct RequestNode
    {
        callbackVector  callbacks;
        u32             requestDate;
    };
    typedef std::deque<RequestNode> requestQueue;
    
    struct ModelNode
    {
        l3m::Model*     model;
        bool            loaded;
        u32             refCount;
        ModelNode*      LRUnext;
        ModelNode*      LRUprev;
    };
    
    // Singleton stuff
private:
    static ModelManager*        ms_instance;
public:
    static ModelManager&        GetInstance     ()
    {
        if ( !ms_instance )
            ms_instance = new ModelManager ();
        return *ms_instance;
    }
    
private:
                        ModelManager                ();
                        ~ModelManager               ();
    
public:
    bool                Request                     ( const char* model, RequestCallback callback, Priority priority = PRIORITY_NORMAL );
    bool                Request                     ( std::istream& stream, RequestCallback callback, Priority priority = PRIORITY_NORMAL );
    l3m::Model*         RequestBlocking             ( const char* model );
    bool                Release                     ( const l3m::Model* model );

    void                ProcessQueue                ();
    void                LoadAllRequestedModels      ();
    
private:
    requestQueue        m_queues [ 4 ];
    Priority            m_nostarveQueue;
};
    
}