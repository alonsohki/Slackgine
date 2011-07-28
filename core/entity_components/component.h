#pragma once

namespace Core { class Entity; }

namespace Core { namespace Entities {
   
class IComponent
{
public:
    IComponent ( Entity* parent, bool isTickable, bool isRenderable )
    : m_parent ( parent )
    , m_isTickable ( isTickable )
    , m_isRenderable ( isRenderable )
    {}
    
    virtual ~IComponent () {}
    
    bool                isTickable      () const { return m_isTickable; }
    bool                isRenderable    () const { return m_isRenderable; }
    Entity*             parent          () const { return m_parent; }
    
    virtual void        Tick            () {}
    virtual void        Render          () {}
    
private:
    Entity*     m_parent;
    bool        m_isTickable;
    bool        m_isRenderable;
};
    
} }
