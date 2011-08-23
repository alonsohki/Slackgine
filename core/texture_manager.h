/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011 - Alberto Alonso
//
// Redistribution of this software, including any of its
// files, in either binary or source code form is totally
// forbidden without the permission of its owners.
//
// Owners reserve their right to legal action against any
// person or company that fails to do so.
//
// For more detailed information, see the LICENSE file in
// the top-level directory.
//
// FILE:        texture_manager.h
// PURPOSE:     Basically, a texture dictionary.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#include <map>
#include <string>
#include "renderer/texture.h"
#include "l3m/Components/texture.h"

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
                                
    void                        Register                ( const l3m::Texture& texture );
    void                        Release                 ( const l3m::Texture& texture );
    
public:
    Renderer::ITexture*         Find                    ( const std::string& name ) const;
    
private:
    TextureNodeMap              m_textures;
};
    
}
