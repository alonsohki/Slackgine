#ifndef REFCOUNT_H
#define	REFCOUNT_H

#include "platform.h"

class RefCount
{
private:
    u32 m_count;
    
public:
    RefCount () : m_count(0) {}
    virtual ~RefCount () {}
    
    u32 AddRef () { return ++m_count; }
    u32 RemoveRef () { if ( m_count > 0 ) --m_count; return m_count; }
    
    u32 GetCount () const { return m_count; }
};

#endif	/* REFCOUNT_H */

