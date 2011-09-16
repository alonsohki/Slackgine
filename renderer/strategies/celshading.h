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
// FILE:        celshading.h
// PURPOSE:     Cel-shading strategy.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#include "render_strategy.h"
#include "core/slackgine.h"

namespace Renderer { namespace Strategy
{
    
class Celshading : public IRenderStrategy
{
public:
    //--------------------------------------------------------------------------
    // Constructor/Destructor
    Celshading  ();
    ~Celshading ();
    
    //--------------------------------------------------------------------------
    // Method to render the scene
    bool        render          ( Core::Slackgine* sg );
    
    //--------------------------------------------------------------------------
    // Error management
    void        getError        ( char* error ) const;
    
private:
    //--------------------------------------------------------------------------
    // Private class attributes
    char        m_error [ 256 ];
};
    
}
}