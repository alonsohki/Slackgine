/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        texture_manager.cpp
// PURPOSE:     Basically, a texture dictionary.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include "texture_manager.h"
#include "renderer/texture.h"
#include "renderer/renderer.h"
#include "shared/log.h"

using namespace Core;

TextureManager::TextureManager ()
{
}

TextureManager::~TextureManager ()
{
    // Free all the textures
    for ( TextureNodeMap::iterator iter = m_textures.begin();
          iter != m_textures.end();
          ++iter )
    {
        LOG_V ( "TextureManager", "Releasing the texture with id '%s'", iter->first.c_str() );
        delete iter->second.texture;
    }
}

void TextureManager::Register (const l3m::Texture& texture)
{
    const std::string& name = texture.id ();
    TextureNodeMap::iterator iter = m_textures.find ( name );
    if ( iter != m_textures.end() )
    {
        LOG_VV ( "TextureManager", "Registering a new reference to the texture id '%s'", name.c_str() );
        iter->second.refs++;
    }
    else
    {
        LOG_V ( "TextureManager", "Registering a texture with id '%s'", name.c_str() );
        const Pixmap& pixmap = texture.pixmap ();
        TextureNode node;
        node.refs = 1;
        node.texture = Renderer::Factory::createTexture ( pixmap.width (), pixmap.height (), Renderer::ITexture::RGBA8 );
        if ( node.texture != 0 )
            node.texture->loadPixmap ( pixmap );
        m_textures.insert ( TextureNodeMap::value_type ( name, node ) );
    }
}

void TextureManager::Release (const l3m::Texture& texture)
{
    const std::string& name = texture.id ();
    TextureNodeMap::iterator iter = m_textures.find ( name );
    if ( iter != m_textures.end() )
    {
        TextureNode& node = iter->second;
        node.refs--;
        if ( node.refs == 0 )
        {
            LOG_V ( "TextureManager", "Releasing the texture with id '%s'", name.c_str() );
                    
            // Free it
            delete node.texture;
            m_textures.erase ( iter );
        }
        else
        {
            LOG_VV ( "TextureManager", "Releasing a reference for the texture with id '%s'", name.c_str() );
        }
    }
}

Renderer::ITexture* TextureManager::Find (const std::string& name) const
{
    TextureNodeMap::const_iterator iter = m_textures.find ( name );
    Renderer::ITexture* tex = 0;
    if ( iter != m_textures.end() )
        tex = iter->second.texture;
    return tex;
}
