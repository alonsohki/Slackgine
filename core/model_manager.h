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