/* 
 * File:   l3mStream.h
 * Author: ryden
 *
 * Created on 18 de julio de 2011, 18:06
 */

#pragma once

#include <iostream>
#include "shared/platform.h"

namespace l3m
{

class IOStream
{
public:
    enum EFlags
    {
        NONE                    = 0x0000,
        COMPRESSION             = 0x0001,
        ENDIAN_SWAPPING         = 0x0002
    };
    
public:
    IOStream ( std::istream* istream, std::ostream* ostream, u32 flags = NONE )
    : m_istream ( istream ), m_ostream ( ostream ), m_flags ( flags )
    {
        SetupFlags ();
    }
    virtual     ~IOStream       () {}
    
    void        SetFlags        ( u32 flags )
    {
        m_flags = flags;
        SetupFlags ();
    }
    
    bool        Write16         ( const u16* v, u32 nmemb );
    bool        Write32         ( const u32* v, u32 nmemb );
    bool        Write64         ( const u64* v, u32 nmemb );
    bool        WriteFloat      ( const float* v, u32 nmemb );
    bool        WriteStr        ( const std::string& str );
    bool        WriteData       ( const char* data, u32 size, u32 nmemb );
    
    size_t      Read16          ( u16* v, u32 nmemb );
    size_t      Read32          ( u32* v, u32 nmemb );
    size_t      Read64          ( u64* v, u32 nmemb );
    size_t      ReadFloat       ( float* v, u32 nmemb );
    size_t      ReadStr         ( std::string& str );
    size_t      ReadData        ( char* data, u32 size, u32 nmemb );
    
    u32         flags           () const { return m_flags; }
    
private:
    void        SetupFlags    ();
    size_t (*m_endian16writer)(const u16*, u32, std::ostream&);
    size_t (*m_endian32writer)(const u32*, u32, std::ostream&);
    size_t (*m_endian64writer)(const u64*, u32, std::ostream&);
    size_t (*m_endian16reader)(u16*, u32, std::istream&);
    size_t (*m_endian32reader)(u32*, u32, std::istream&);
    size_t (*m_endian64reader)(u64*, u32, std::istream&);

private:
    std::istream*       m_istream;
    std::ostream*       m_ostream;
    u32                 m_flags;
};


class IStream : public IOStream
{
public:
    IStream ( std::istream* istream, u32 flags = NONE ) : IOStream ( istream, 0, flags ) {}
};

class OStream : public IOStream
{
public:
    OStream ( std::ostream* ostream, u32 flags = NONE ) : IOStream ( 0, ostream, flags ) {}
};

}
