#include "model_renderer.h"
#include "core/entity.h"

using namespace Core::Entities;

ModelRenderer::ModelRenderer ( Entity* parent )
: IComponent ( parent, false, true )
{
    m_model = parent->GetModel();
    if ( m_model )
        Initialize ();
}

ModelRenderer::~ModelRenderer ()
{
}

void ModelRenderer::Render ( Renderer::IRenderer* renderer )
{
    if ( !m_model )
        return;
}

void ModelRenderer::Initialize ()
{
    if ( !m_model )
        return;
    
    const l3m::Model::componentVector& components = m_model->components ();
    for ( l3m::Model::componentVector::const_iterator iter = components.begin();
          iter != components.end();
          ++iter )
    {
        
    }
}
