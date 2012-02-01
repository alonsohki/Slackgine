/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        texture_manager.h
// PURPOSE:     Basically, a texture dictionary.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#include <map>
#include <string>
#include "renderer/texture.h"
#include "l3m/components/texture.h"

namespace Core
{
    
class TextureManager
{
    friend class Slackgine;
    friend class ModelManager;
    
private:
    struct TextureNode
    {
        u32                     refs;
        Renderer::ITexture*     texture;
    };
    typedef std::map < std::string, TextureNode > TextureNodeMap;
    
private:
                                TextureManager          ();
                                ~TextureManager         ();
                                
    void                        registerTexture         ( const l3m::Texture& texture );
    void                        releaseTexture          ( const l3m::Texture& texture );
    
public:
    Renderer::ITexture*         find                    ( const std::string& name ) const;
    
private:
    TextureNodeMap              m_textures;
};
    
}
