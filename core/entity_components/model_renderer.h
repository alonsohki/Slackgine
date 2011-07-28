#pragma once

#include "component.h"

namespace Core { namespace Entities {
   
class ModelRenderer : public IComponent
{
public:
                ModelRenderer   ( Entity* parent );
                ~ModelRenderer  ();
};

} }