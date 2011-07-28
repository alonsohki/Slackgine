#include "slackgine.h"

using namespace Core;

Slackgine::Slackgine ()
{
    m_renderer = Renderer::Factory::CreateRenderer ();
}

Slackgine::~Slackgine ()
{
    delete m_renderer;
}

bool Slackgine::Initialize ()
{
    return m_renderer->Initialize ();
}
