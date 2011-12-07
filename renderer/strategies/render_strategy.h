/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        render_strategy.h
// PURPOSE:     Interface for rendering strategies.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#include <vector>
#include "core/slackgine.h"
#include "renderer/shader.h"

namespace Core { class Slackgine; }

namespace Renderer
{

class RenderStrategy
{
    //--------------------------------------------------------------------------
    // Pass management structures
    enum PassType
    {
        PASS_GEOMETRY = 0,
        PASS_COMPOSITION,
        PASS_POSTPROCESS,
        PASS_COUNT
    };
    
    class Pass
    {
    public:
        //----------------------------------------------------------------------
        // Constructor/Destructor
                        Pass    ( PassType type );
        virtual         ~Pass   ();
        
        //----------------------------------------------------------------------
        // Pass cloning, used in strategy blending.
        virtual Pass*   clone           () = 0;
        
        //----------------------------------------------------------------------
        // Virtual function used to let the passes set up. Specially useful when
        // executing passes that got blended from other strategies.
        virtual void    setup           () = 0;
        
        //----------------------------------------------------------------------
        // Pass type
        PassType        getType         () const { return m_type; }
        
        //----------------------------------------------------------------------
        // Interface functions to get the shaders associated with this pass
        virtual Renderer::IShader*      getVertexShader         () { return 0; }
        virtual Renderer::IShader*      getFragmentShader       () { return 0; }
        
    private:
        //----------------------------------------------------------------------
        // Private attributes
        PassType        m_type;
    };

    typedef std::vector<Pass*> PassVector;

public:
    //--------------------------------------------------------------------------
    // Constructor / Destructor
                RenderStrategy          ();
    virtual     ~RenderStrategy         ();
    
    //--------------------------------------------------------------------------
    // Functions to allow the child classes to define their passes
protected:
    void        addPass                 ( Pass* pass );
    
    //--------------------------------------------------------------------------
    // Also allow strategy blending.
    void        blendStrategy           ( RenderStrategy* strategy );
    
public:
    
    //--------------------------------------------------------------------------
    // Method to execute this strategy
    bool        execute                 ( Core::Slackgine* sg );
    
    //--------------------------------------------------------------------------
    // Error management
    void        getError                ( char* dest ) const;
    
private:
    //--------------------------------------------------------------------------
    // Private class attributes
    char        m_error [ 256 ];
    PassVector  m_passes [ PASS_COUNT ];
};

}
