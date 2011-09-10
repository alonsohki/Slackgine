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
    
    virtual void        Tick            () {}
    virtual void        PreRender       ( Renderer::IRenderer* renderer ) {}
    virtual void        Render          ( Renderer::IRenderer* renderer ) {}
    virtual void        PostRender      ( Renderer::IRenderer* renderer ) {}
    
private:
    Entity*     m_parent;
    bool        m_isTickable;
    bool        m_isRenderable;
};
    
} }
