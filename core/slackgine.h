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
#include "l3m/l3m.h"
#include "shared/platform.h"
#include "model_manager.h"
#include "time.h"

namespace Core
{
    
class Slackgine
{
public:
                                Slackgine               ();
                                ~Slackgine              ();

    bool                        Initialize              ();
    
    void                        Tick                    ();
                
    Renderer::IRenderer*        GetRenderer             () { return m_renderer; }
    ModelManager&               GetModelManager         () { return m_modelManager; }
    TextureManager&             GetTextureManager       () { return m_textureManager; }
    Time&                       GetTime                 () { return m_time; }
    
private:
    Renderer::IRenderer*        m_renderer;
    Time                        m_time;
    TextureManager              m_textureManager;
    ModelManager                m_modelManager;
};

}
