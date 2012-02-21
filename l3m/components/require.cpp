/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        require.cpp
// PURPOSE:     Special component that tells the engine a model dependency.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include "require.h"

using namespace l3m;

Require::Require ()
: IComponent ( "require", 1.0f )
{
}

Require::~Require ()
{
}

bool Require::load ( l3m::Model*, l3m::IStream& fp, float version )
{
    u16 reqType;
    if ( fp.read16 ( &reqType, 1 ) != 1 )
        return setError ( "Unable to load the requirement type" );
    m_reqType = static_cast < RequireType > ( reqType );
    
    if ( fp.readStr ( &m_path ) < 1 )
        return setError ( "Unable to load the requirement path" );
    return true;
}

bool Require::save ( l3m::Model*, l3m::OStream& fp )
{
    u16 reqType = m_reqType;
    if ( fp.write16 ( &reqType, 1 ) == false )
        return setError ( "Unable to write the requirement type" );
    
    if ( fp.writeStr ( m_path ) == false )
        return setError ( "Unable to write the requirement path" );
    return true;
}
