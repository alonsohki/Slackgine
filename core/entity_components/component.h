/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        component.h
// PURPOSE:     Interface for the entity components.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#include "renderer/renderer.h"

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
    
    virtual             ~IComponent () {}
    
    bool                isTickable      () const { return m_isTickable; }
    bool                isRenderable    () const { return m_isRenderable; }
    Entity*             parent          () const { return m_parent; }
    Entity*&            parent          () { return m_parent; }
    
    virtual void        tick            () {}
    virtual void        preRender       ( Renderer::IRenderer* renderer ) {}
    virtual void        render          ( Renderer::IRenderer* renderer ) {}
    virtual void        postRender      ( Renderer::IRenderer* renderer ) {}
    
private:
    Entity*     m_parent;
    bool        m_isTickable;
    bool        m_isRenderable;
};
    
} }
