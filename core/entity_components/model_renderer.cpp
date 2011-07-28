#include "model_renderer.h"

using namespace Core::Entities;

ModelRenderer::ModelRenderer ( Entity* parent )
: IComponent ( parent, false, true )
{
}

ModelRenderer::~ModelRenderer ()
{
}
