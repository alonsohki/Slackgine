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

bool Require::Load ( l3m::IStream& fp, float version )
{
    u16 reqType;
    if ( fp.Read16 ( &reqType, 1 ) != 1 )
        return SetError ( "Unable to load the requirement type" );
    m_reqType = static_cast < RequireType > ( reqType );
    
    if ( fp.ReadStr ( m_path ) < 1 )
        return SetError ( "Unable to load the requirement path" );
    return true;
}

bool Require::Save ( l3m::OStream& fp )
{
    u16 reqType = m_reqType;
    if ( fp.Write16 ( &reqType, 1 ) == false )
        return SetError ( "Unable to write the requirement type" );
    
    if ( fp.WriteStr ( m_path ) == false )
        return SetError ( "Unable to write the requirement path" );
    return true;
}
