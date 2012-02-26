/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        BufferWrapStream.h
// PURPOSE:     Class that wraps a buffer to make it possible to use it from
//              C++ streams.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#include <iostream>

namespace
{

template < typename T = char >
class WrapBuffer : public std::basic_streambuf<T, std::char_traits<T> >
{
    typedef typename std::char_traits<T> _Tr;
    typedef typename std::basic_streambuf<T, std::char_traits<T> > _T_parent;

    
public:
    WrapBuffer ( T* buffer, size_t len )
    {
        _T_parent::setg ( buffer, &buffer[0], &buffer[len] );
        _T_parent::setp ( buffer, &buffer[len] );
    }
    
    virtual ~WrapBuffer ()
    {
    }
    
protected:   
    virtual int uflow ()
    {
        return _Tr::eof();
    }

    virtual int underflow ()
    {
        return _Tr::eof();
    }
    
    virtual int overflow ( int c )
    {
        return _Tr::eof();
    }
};


template < typename T = char >
class BufferWrapIStream : public std::basic_istream<T, std::char_traits<T> >
{
    WrapBuffer<T> m_streambuffer;
    
public:
    BufferWrapIStream ( T* buffer, size_t length )
    : std::basic_istream<T, std::char_traits<T> > ( 0 )
    , m_streambuffer ( buffer, length )
    {
        this->init(&m_streambuffer);
    }
};

template < typename T = char >
class BufferWrapOStream : public std::basic_ostream<T, std::char_traits<T> >
{
    WrapBuffer<T> m_streambuffer;
    
public:
    BufferWrapOStream ( T* buffer, size_t length )
    : std::basic_ostream<T, std::char_traits<T> > ( 0 )
    , m_streambuffer ( buffer, length )
    {
        this->init(&m_streambuffer);
    }
};

template < typename T = char >
class BufferWrapStream : public std::basic_iostream<T, std::char_traits<T> >
{
    WrapBuffer<T> m_streambuffer;
    
public:
    BufferWrapStream ( T* buffer, size_t length )
    : std::basic_iostream<T, std::char_traits<T> > ( 0 )
    , m_streambuffer ( buffer, length )
    {
        this->init(&m_streambuffer);
    }

};

}
