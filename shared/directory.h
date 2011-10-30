/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        directory.h
// PURPOSE:     Platform-independent directory iteration.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#include <dirent.h>

class Directory
{
public:
    enum EntryType
    {
        ENTRY_FILE,
        ENTRY_DIRECTORY
    };
    
public:
    Directory ( const char* path )
    {
        m_dir = opendir ( path );
        next ();
    }
    
    ~Directory ()
    {
        if ( m_dir != 0 )
            closedir ( m_dir );
    }
    
    bool        hasNext         () const
    {
        return m_dir != 0;
    }
    
    void        next            ()
    {
        if ( m_dir && ( m_entry = readdir ( m_dir ) ) == 0 )
        {
            closedir ( m_dir );
            m_dir = 0;
        }
    }
    
    EntryType   type            () const
    {
        if ( m_entry->d_type == DT_DIR )
            return ENTRY_DIRECTORY;
        else
            return ENTRY_FILE;
    }
    
    const char* entry           () const
    {
        return m_entry->d_name;
    }
    
private:
    DIR*        m_dir;
    dirent*     m_entry;
};
