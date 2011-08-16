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
