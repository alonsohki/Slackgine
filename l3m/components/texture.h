/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
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
    static IComponent* create ()
    {
        return new Texture ();
    }
    
public:
                Texture         ();
                ~Texture        ();
    
    bool        load            ( l3m::Model*, l3m::IStream& stream, float version );
    bool        save            ( l3m::Model*, l3m::OStream& stream );
    
    
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