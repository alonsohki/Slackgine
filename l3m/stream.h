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
    //--------------------------------------------------------------------------
    // Constructor/Destructor
                IOStream        ( std::istream* istream, std::ostream* ostream, u32 flags = NONE );
    virtual     ~IOStream       () {}
    
    
    //--------------------------------------------------------------------------
    // Function to set the flags mentioned in the enumeration above
    void        setFlags        ( u32 flags );

    
    //--------------------------------------------------------------------------
    // Functions to write data
    bool        writeBoolean    ( const b8* v, u32 nmemb );
    bool        writeBoolean    ( const b8& v );
    bool        write16         ( const u16* v, u32 nmemb );
    bool        write16         ( const u16& v );
    bool        write32         ( const u32* v, u32 nmemb );
    bool        write32         ( const u32& v );
    bool        write64         ( const u64* v, u32 nmemb );
    bool        write64         ( const u64& v );
    bool        writeFloat      ( const float* v, u32 nmemb );
    bool        writeFloat      ( const float& v );
    bool        writeStr        ( const std::string& str );
    bool        writeMatrix     ( const Matrix* v, u32 nmemb );
    bool        writeMatrix     ( const Matrix& mat );
    bool        writeVector     ( const Vector3& vec );
    bool        writeColor      ( const Color* col, u32 nmemb );
    bool        writeQuaternion ( const Quaternion* v, u32 nmemb );
    bool        writeTransform  ( const Transform* v, u32 nmemb );
    bool        writeQTransform ( const QTransform* v, u32 nmemb );
    bool        writeQTransform ( const QTransform& trans );
    bool        writeData       ( const char* data, u32 size, u32 nmemb );
    
    //--------------------------------------------------------------------------
    // Functions to read data
    bool        readBoolean     ( b8* v );
    ssize_t     readBoolean     ( b8* v, u32 nmemb );
    ssize_t     read16          ( u16* v, u32 nmemb );
    ssize_t     read32          ( u32* v, u32 nmemb );
    ssize_t     read64          ( u64* v, u32 nmemb );
    ssize_t     readFloat       ( float* v, u32 nmemb );
    ssize_t     readStr         ( std::string& str );
    ssize_t     readMatrix      ( Matrix* v, u32 nmemb );
    bool        readMatrix      ( Matrix& mat );
    ssize_t     readVector      ( Vector3& vec );
    ssize_t     readColor       ( Color* col, u32 nmemb );
    ssize_t     readQuaternion  ( Quaternion* v, u32 nmemb );
    ssize_t     readTransform   ( Transform* v, u32 nmemb );
    ssize_t     readQTransform  ( QTransform* v, u32 nmemb );
    bool        readQTransform  ( QTransform& trans );
    ssize_t     readData        ( char* data, u32 size, u32 nmemb );
    
    //--------------------------------------------------------------------------
    // Public accessors
    u32         flags           () const { return m_flags; }
    const u32&  totalIn         () const { return m_totalIn; }
    const u32&  totalOut        () const { return m_totalOut; }
    
private:
    void        setupFlags    ();
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


//------------------------------------------------------------------------------
// Define two classes for input and output streams that use the above IOStream
// class.
class IStream : public IOStream
{
public:
    IStream ( std::istream* istream, u32 flags = NONE )
    : IOStream ( istream, 0, flags ) {}
};

class OStream : public IOStream
{
public:
    OStream ( std::ostream* ostream, u32 flags = NONE )
    : IOStream ( 0, ostream, flags ) {}
};

}
