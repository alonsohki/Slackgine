/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        shader_manager.cpp
// PURPOSE:     Shader manager, with the ability to load shaders from files
//              and generate the shader main function to link them.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>
#include "shader_manager.h"
#include "shared/log.h"

using namespace Core;

ShaderManager::ShaderManager ()
{
}

ShaderManager::~ShaderManager ()
{
    for ( ShaderMap::iterator iter = m_shaders.begin();
          iter != m_shaders.end();
          ++iter )
    {
        sgDelete iter->second;
    }
}

bool ShaderManager::addLookupPath ( const std::string& path )
{
    struct stat status;
    if ( stat(path.c_str(), &status) != 0 )
        return false;
    if ( !S_ISDIR(status.st_mode) )
        return false;
    LOG_I ( "ShaderManager", "Added new lookup path: %s", path.c_str() );
    m_paths.push_back ( path );
    
    return true;
}

Shader* ShaderManager::load ( const std::string& shader )
{
    // Try to locate an already loaded shader.
    ShaderMap::iterator iter = m_shaders.find ( shader );
    if ( iter != m_shaders.end() )
        return iter->second;
    
    LOG_V ( "ShaderManager", "Loading shader '%s'", shader.c_str() );
    
    // Try to find the source files for this shader.
    char filename [ 1024 ];
    std::string path;
    Renderer::IShader* vert = 0;
    Renderer::IShader* frag = 0;
    
    // Vertex shader.
    snprintf ( filename, sizeof(filename), "%s.vert.glsl", shader.c_str() );
    if ( getShaderAccessPath(filename, &path) == true )
    {
        LOG_VV ( "ShaderManager", "Loading vertex shader from '%s'", path.c_str() );
        vert = Renderer::Factory::createShader( Renderer::IShader::VERTEX_SHADER, path.c_str() );
        if ( vert && vert->ok() == false )
        {
            char error [ 1024 ];
            vert->getError ( error );
            LOG_E ( "ShaderManager", "Error loading the vertex shader: %s", error );
            sgDelete vert;
            vert = 0;
        }
    }
    
    // Fragment shader.
    snprintf ( filename, sizeof(filename), "%s.frag.glsl", shader.c_str() );
    if ( getShaderAccessPath(filename, &path) == true )
    {
        LOG_VV ( "ShaderManager", "Loading fragment shader from '%s'", path.c_str() );
        frag = Renderer::Factory::createShader( Renderer::IShader::FRAGMENT_SHADER, path.c_str() );
        if ( frag && frag->ok() == false )
        {
            char error [ 1024 ];
            frag->getError ( error );
            LOG_E ( "ShaderManager", "Error loading the fragment shader: %s", error );
            sgDelete frag;
            frag = 0;
        }
    }
    
    // Create the shader
    Shader* sh = 0;
    if ( vert != 0 || frag != 0 )
    {
        sh = sgNew Shader ( shader );
        sh->vert() = vert;
        sh->frag() = frag;
        
        // Register it in the loaded shaders map
        m_shaders.insert ( ShaderMap::value_type(sh->name(), sh) );
    }
    else
    {
        LOG_W ( "ShaderManager", "Shader '%s' didn't load because no source files were found for it", shader.c_str() );
    }
    
    return sh;
}

bool ShaderManager::getShaderAccessPath (const std::string& shader, std::string* path) const
{
    struct stat status;
    
    for ( PathVector::const_reverse_iterator iter = m_paths.rbegin ();
          iter != m_paths.rend();
          ++iter )
    {
        std::string lookupPath = *iter;
        lookupPath += "/";
        lookupPath += shader;
        
        if ( stat(lookupPath.c_str(), &status) == 0 )
        {
            if ( !S_ISDIR(status.st_mode) )
            {
                if ( path != 0 )
                    *path = lookupPath;
                return true;
            }
        }
    }
    
    return false;
}
