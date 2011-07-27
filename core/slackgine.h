/* 
 * File:   slackgine.h
 * Author: ryden
 *
 * Created on 23 de abril de 2011, 20:50
 */

#ifndef SLACKGINE_H
#define	SLACKGINE_H

#include "renderer/renderer.h"
#include "l3m/l3m.h"
#include "shared/platform.h"

namespace Renderer { class IRenderer; }

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

#endif	/* SLACKGINE_H */

