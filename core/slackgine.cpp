#include "slackgine.h"

Slackgine::Slackgine ()
{
    m_renderer = Renderer::CreateRenderer ();
    m_renderer->Initialize ();
}

Slackgine::~Slackgine ()
{
    delete m_renderer;
}
