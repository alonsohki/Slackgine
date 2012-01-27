/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        stream.h
// PURPOSE:     l3m model streams to abstract IO special operations.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#include <iostream>
#include "shared/platform.h"
#include "shared/color.h"
#include "math/vector.h"
#include "math/matrix.h"
#include "math/quaternion.h"
#include "math/transform.h"

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
    : m_istream ( istream ), m_ostream ( ostream ), m_flags ( flags ), m_totalIn(0), m_totalOut(0)
    {
        SetupFlags ();
    }
    virtual     ~IOStream       () {}
    
    void        SetFlags        ( u32 flags )
    {
        m_flags = flags;
        SetupFlags ();
    }
    
    bool        WriteBoolean    ( const b8* v, u32 nmemb );
    bool        WriteBoolean    ( const b8& v );
    bool        Write16         ( const u16* v, u32 nmemb );
    bool        Write32         ( const u32* v, u32 nmemb );
    bool        Write64         ( const u64* v, u32 nmemb );
    bool        WriteFloat      ( const float* v, u32 nmemb );
    bool        WriteStr        ( const std::string& str );
    bool        WriteMatrix     ( const Matrix& mat );
    bool        WriteVector     ( const Vector3& vec );
    bool        WriteColor      ( const Color* col, u32 nmemb );
    bool        WriteQuaternion ( const Quaternion* v, u32 nmemb );
    bool        WriteTransform  ( const Transform* v, u32 nmemb );
    bool        WriteData       ( const char* data, u32 size, u32 nmemb );
    
    bool        ReadBoolean     ( b8* v );
    ssize_t     ReadBoolean     ( b8* v, u32 nmemb );
    ssize_t     Read16          ( u16* v, u32 nmemb );
    ssize_t     Read32          ( u32* v, u32 nmemb );
    ssize_t     Read64          ( u64* v, u32 nmemb );
    ssize_t     ReadFloat       ( float* v, u32 nmemb );
    ssize_t     ReadStr         ( std::string& str );
    ssize_t     ReadMatrix      ( Matrix& mat );
    ssize_t     ReadVector      ( Vector3& vec );
    ssize_t     ReadColor       ( Color* col, u32 nmemb );
    ssize_t     ReadQuaternion  ( Quaternion* v, u32 nmemb );
    ssize_t     ReadTransform   ( Transform* v, u32 nmemb );
    ssize_t     ReadData        ( char* data, u32 size, u32 nmemb );
    
    u32         flags           () const { return m_flags; }
    const u32&  totalIn         () const { return m_totalIn; }
    const u32&  totalOut        () const { return m_totalOut; }
    
private:
    void        SetupFlags    ();
    ssize_t (*m_endian16writer)(const u16*, u32, std::ostream&);
    ssize_t (*m_endian32writer)(const u32*, u32, std::ostream&);
    ssize_t (*m_endian64writer)(const u64*, u32, std::ostream&);
    ssize_t (*m_endian16reader)(u16*, u32, std::istream&);
    ssize_t (*m_endian32reader)(u32*, u32, std::istream&);
    ssize_t (*m_endian64reader)(u64*, u32, std::istream&);

private:
    std::istream*       m_istream;
    std::ostream*       m_ostream;
    u32                 m_flags;
    u32                 m_totalIn;
    u32                 m_totalOut;
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
