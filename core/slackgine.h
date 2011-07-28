#pragma once

#include "renderer/renderer.h"
#include "l3m/l3m.h"
#include "shared/platform.h"

namespace Core
{
    
class Slackgine
{
private:
    Renderer::IRenderer* m_renderer;
    
public:
                                Slackgine       ();
                                ~Slackgine      ();

    bool                        Initialize      ();
                
    Renderer::IRenderer*        renderer        () { return m_renderer; }
};

}