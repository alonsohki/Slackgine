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
// FILE:        slackgine.h
// PURPOSE:     Slackgine main class.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#include "renderer/renderer.h"
#include "renderer/strategies/render_strategy.h"
#include "l3m/l3m.h"
#include "shared/platform.h"
#include "model_manager.h"
#include "time.h"
#include "world.h"

namespace Renderer { class RenderStrategy; }

namespace Core
{
    
class Slackgine
{
    //--------------------------------------------------------------------------
    // Context management
    // When a Slackgine instance gets ticket, the current thread context is set
    // to that Slackgine instance, doing the subsequent calls to the slackgine
    // use that context through getInstance().
    // TODO: Make it thread safe.
private:
    static Slackgine*           ms_context;
    static void                 setContext              ( Slackgine* );
public:
    static Slackgine*           getInstance             ()
    {
        return ms_context;
    }

public:
                                Slackgine               ();
                                ~Slackgine              ();

    bool                        initialize              ();
    
    void                        tick                    ();
    bool                        render                  ();
    bool                        renderGeometry          ();
                
    Renderer::IRenderer*        getRenderer             () { return m_renderer; }
    ModelManager&               getModelManager         () { return m_modelManager; }
    TextureManager&             getTextureManager       () { return m_textureManager; }
    Time&                       getTime                 () { return m_time; }
    World&                      getWorld                () { return m_world; }
    
    void                        getError                ( char* dest ) const;
    
    //--------------------------------------------------------------------------
    // Stuff related to the render strategy
    template < class T >
    void                        setRenderStrategy       ()
    {
        m_renderStrategy = new T ();
    }
    
private:
    Renderer::IRenderer*        m_renderer;
    Time                        m_time;
    TextureManager              m_textureManager;
    ModelManager                m_modelManager;
    World                       m_world;
    char                        m_error [ 256 ];
    Renderer::RenderStrategy*   m_renderStrategy;
};

}
