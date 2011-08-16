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
// FILE:        texture.h
// PURPOSE:     Texture component.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#include "shared/pixmap.h"
#include "../component.h"

namespace l3m
{

class Texture : public IComponent
{
public:
    static IComponent* Create ()
    {
        return new Texture ();
    }
    
public:
                Texture         ();
                ~Texture        ();
    
    bool        Load            ( l3m::IStream& stream, float version );
    bool        Save            ( l3m::OStream& stream );
    
    
    // Accessors
    std::string&        id      () { return m_id; }
    Pixmap&             pixmap  () { return m_image; }
    
    const std::string&  id      () const { return m_id; }
    const Pixmap&       pixmap  () const { return m_image; }
    
private:
    std::string         m_id;
    Pixmap              m_image;
};

}