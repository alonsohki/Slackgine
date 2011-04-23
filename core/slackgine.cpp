#include "slackgine.h"

Slackgine::Slackgine ()
{
    m_renderer = Renderer::CreateRenderer ();
}

Slackgine::~Slackgine ()
{
    delete m_renderer;
}

bool Slackgine::Initialize ()
{
    return m_renderer->Initialize ();
}
