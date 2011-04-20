#include "renderer.h"
#include "renderers/opengl3.h"

IRenderer* IRenderer::GetRenderer ()
{
    static IRenderer* pRenderer = new OpenGL3_Renderer ();
    return pRenderer;
}