/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        stream.cpp
// PURPOSE:     l3m model streams to abstract IO special operations.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include <cassert>
#include "stream.h"

using namespace l3m;

IOStream::IOStream ( std::istream* istream, std::ostream* ostream, u32 flags )
: m_istream ( istream ), m_ostream ( ostream ), m_flags ( flags ), m_totalIn(0), m_totalOut(0)
{
    setupFlags ();
}

void IOStream::setFlags(u32 flags)
{
    m_flags = flags;
    setupFlags ();
}


//------------------------------------------------------------------------------
// Endianness functions
template < typename T >
static ssize_t identityWrite ( const T* v, u32 count, std::ostream& fp )
{
    fp.write(reinterpret_cast<const char*>(v), sizeof(T)*count);
    if ( !fp.fail() )
        return count;
    return -1;
}

template < typename T >
static ssize_t identityRead ( T* v, u32 count, std::istream& fp )
{
    fp.read ( reinterpret_cast<char *>(v), sizeof(T)*count );
    ssize_t s = fp.gcount () / sizeof(T);
    if ( ! fp.fail() )
        return s;
    return -1;
}

static ssize_t swap16Write ( const u16* v, u32 count, std::ostream& fp )
{
    u16 current;
    for ( u32 i = 0; i < count; ++i )
    {
        current = ( ( v[i] >> 8 ) & 0x00FF ) | ( v[i] << 8 & 0xFF00 );
        fp.write(reinterpret_cast<const char*>(&current), sizeof(u16));
        if ( fp.fail() )
            return i;
    }
    return count;
}

static ssize_t swap16Read ( u16* v, u32 count, std::istream& fp )
{
    u16 current;
    for ( u32 i = 0; i < count; ++i )
    {
        fp.read ( reinterpret_cast<char*>(&current), sizeof(u16) );
        if ( fp.gcount() < (ssize_t)sizeof(u16) )
            return i;
        v[i] = ( ( current >> 8 ) & 0x00FF ) | ( current << 8 & 0xFF00 );
    }
    return count;
}

static ssize_t swap32Write ( const u32* v, u32 count, std::ostream& fp )
{
    u32 current;
    for ( u32 i = 0; i < count; ++i )
    {
        current =  ( ( v[i] >> 24 ) & 0x000000FF ) |
                   ( ( v[i] >> 8  ) & 0x0000FF00 ) |
                   ( ( v[i] << 8  ) & 0x00FF0000 ) |
                   ( ( v[i] << 24 ) & 0xFF000000 );
        fp.write(reinterpret_cast<const char*>(&current), sizeof(u32));
        if ( fp.fail() )
            return i;
    }
    return count;
}

static ssize_t swap32Read ( u32* v, u32 count, std::istream& fp )
{
    u32 current;
    for ( u32 i = 0; i < count; ++i )
    {
        fp.read ( reinterpret_cast<char*>(&current), sizeof(u32) );
        if ( fp.gcount() < (ssize_t)sizeof(u32) )
            return i;
        v[i] =  ( ( current >> 24 ) & 0x000000FF ) |
                ( ( current >> 8  ) & 0x0000FF00 ) |
                ( ( current << 8  ) & 0x00FF0000 ) |
                ( ( current << 24 ) & 0xFF000000 );
    }
    return count;
}

static ssize_t swap64Write ( const u64* v, u32 count, std::ostream& fp )
{
    u64 current;
    for ( u32 i = 0; i < count; ++i )
    {
        current =  ( ( v[i] >> 56 ) & 0x00000000000000FFLL ) |
                   ( ( v[i] >> 40 ) & 0x000000000000FF00LL ) |
                   ( ( v[i] >> 24 ) & 0x0000000000FF0000LL ) |
                   ( ( v[i] >> 8  ) & 0x00000000FF000000LL ) |
                   ( ( v[i] << 8  ) & 0x000000FF00000000LL ) |
                   ( ( v[i] << 24 ) & 0x0000FF0000000000LL ) |
                   ( ( v[i] << 40 ) & 0x00FF000000000000LL ) |
                   ( ( v[i] << 56 ) & 0xFF00000000000000LL );
        fp.write(reinterpret_cast<const char*>(&current), sizeof(u64));
        if ( fp.fail() )
            return i;
    }
    return count;
}

static ssize_t swap64Read ( u64* v, u32 count, std::istream& fp )
{
    u64 current;
    for ( u32 i = 0; i < count; ++i )
    {
        fp.read ( reinterpret_cast<char*>(&current), sizeof(u64) );
        if ( fp.gcount() < (ssize_t)sizeof(u64) )
            return i;
        v[i] =  ( ( current >> 56 ) & 0x00000000000000FFLL ) |
                ( ( current >> 40 ) & 0x000000000000FF00LL ) |
                ( ( current >> 24 ) & 0x0000000000FF0000LL ) |
                ( ( current >> 8  ) & 0x00000000FF000000LL ) |
                ( ( current << 8  ) & 0x000000FF00000000LL ) |
                ( ( current << 24 ) & 0x0000FF0000000000LL ) |
                ( ( current << 40 ) & 0x00FF000000000000LL ) |
                ( ( current << 56 ) & 0xFF00000000000000LL );
    }
    return count;
}


//------------------------------------------------------------------------------
// setupFlags
// Set the appropiate functions to handle the reading/writing according to the
// stream flags.
void IOStream::setupFlags ()
{
    if ( m_flags & ENDIAN_SWAPPING )
    {
        m_endian16writer = swap16Write;
        m_endian32writer = swap32Write;
        m_endian64writer = swap64Write;
        m_endian16reader = swap16Read;
        m_endian32reader = swap32Read;
        m_endian64reader = swap64Read;
    }
    else
    {
        m_endian16writer = identityWrite<u16>;
        m_endian32writer = identityWrite<u32>;
        m_endian64writer = identityWrite<u64>;
        m_endian16reader = identityRead<u16>;
        m_endian32reader = identityRead<u32>;
        m_endian64reader = identityRead<u64>;
    }
    
    // TODO: Compression. Do it making a new STL stream that compresses stuff,
    // creating an instance of it in the class stack and saving a reference to
    // it in m_istream and m_ostream. This special stream will just interface to
    // the given stream with data compression.
}



//------------------------------------------------------------------------------
// Read/Write functions
bool IOStream::writeBoolean ( const b8& v )
{
    return writeBoolean ( &v, 1 );
}

bool IOStream::writeBoolean ( const b8* v, u32 nmemb )
{
#ifdef DEBUG
    assert ( m_ostream != 0 );
#endif
    return writeData ( v, sizeof(b8), nmemb );
}

bool IOStream::readBoolean ( b8* v )
{
    return readBoolean ( v, 1 ) == 1;
}

ssize_t IOStream::readBoolean ( b8* v, u32 nmemb )
{
#ifdef DEBUG
    assert ( m_istream != 0 );
#endif
    return readData ( v, sizeof(b8), nmemb );
}

bool IOStream::write16 ( const u16* v, u32 nmemb )
{
#ifdef DEBUG
    assert ( m_ostream != 0 );
#endif
    ssize_t size = m_endian16writer ( v, nmemb, *m_ostream );
    if ( size >= 0 )
    {
        m_totalOut += size * sizeof(u16);
        return (u32)size >= nmemb;
    }
    else
        return false;
}

bool IOStream::write16(const u16& v)
{
    return write16 ( &v, 1 );
}

ssize_t IOStream::read16 ( u16* v, u32 nmemb )
{
#ifdef DEBUG
    assert ( m_istream != 0 );
#endif
    ssize_t size = m_endian16reader ( v, nmemb, *m_istream );
    m_totalIn += size * sizeof(u16);
    return size;
}

bool IOStream::write32 ( const u32* v, u32 nmemb )
{
#ifdef DEBUG
    assert ( m_ostream != 0 );
#endif
    ssize_t size = m_endian32writer ( v, nmemb, *m_ostream );
    if ( size >= 0 )
    {
        m_totalOut += size * sizeof(u32);
        return (u32)size >= nmemb;
    }
    else
        return false;
}

bool IOStream::write32(const u32& v)
{
    return write32 ( &v, 1 );
}

ssize_t IOStream::read32 ( u32* v, u32 nmemb )
{
#ifdef DEBUG
    assert ( m_istream != 0 );
#endif
    ssize_t size = m_endian32reader ( v, nmemb, *m_istream );
    m_totalIn += size * sizeof(u32);
    return size;
}

bool IOStream::write64 ( const u64* v, u32 nmemb )
{
#ifdef DEBUG
    assert ( m_ostream != 0 );
#endif
    ssize_t size = m_endian64writer ( v, nmemb, *m_ostream );
    if ( size >= 0 )
    {
        m_totalOut += size * sizeof(u64);
        return (u32)size >= nmemb;
    }
    else
        return false;
}

bool IOStream::write64(const u64& v)
{
    return write64 ( &v, 1 );
}

ssize_t IOStream::read64 ( u64* v, u32 nmemb )
{
#ifdef DEBUG
    assert ( m_istream != 0 );
#endif
    ssize_t size = m_endian64reader ( v, nmemb, *m_istream );
    m_totalIn += size * sizeof(u64);
    return size;
}

bool IOStream::writeFloat ( const f32* v, u32 nmemb )
{
#ifdef DEBUG
    assert ( m_ostream != 0 );
#endif
    ssize_t size = m_endian32writer ( reinterpret_cast<const u32*>(v), nmemb, *m_ostream );
    if ( size >= 0 )
    {
        m_totalOut += size * sizeof(f32);
        return (u32)size >= nmemb;
    }
    else
        return false;
}

bool IOStream::writeFloat(const float& v)
{
    return writeFloat ( &v, 1 );
}

ssize_t IOStream::readFloat ( f32* v, u32 nmemb )
{
#ifdef DEBUG
    assert ( m_istream != 0 );
#endif
    ssize_t size = m_endian32reader ( reinterpret_cast<u32*>(v), nmemb, *m_istream );
    m_totalIn += size * sizeof(f32);
    return size;
}

bool IOStream::writeStr ( const std::string& str )
{
#ifdef DEBUG
    assert ( m_ostream != 0 );
#endif
    u32 length = str.length ();
    if ( !write32 ( &length, 1 ) )
        return false;
    return writeData ( str.c_str(), sizeof(char), length );
}

ssize_t IOStream::readStr ( std::string& str )
{
#ifdef DEBUG
    assert ( m_istream != 0 );
#endif
    u32 length;
    if ( read32( &length, 1 ) != 1 )
        return -1;
    if ( length > 0 )
    {
        char* buffer = new char [ length ];
        ssize_t readLength = readData ( buffer, sizeof(char), length );
        if ( readLength <= 0 || (u32)readLength != length )
        {
            delete [] buffer;
            return -1;
        }
        str.assign ( buffer, length );
        delete [] buffer;
        return length;
    }
    else
        return 0;
}

bool IOStream::writeMatrix ( const Matrix* v, u32 nmemb )
{
#ifdef DEBUG
    assert ( m_ostream != 0 );
#endif
    return writeFloat ( v->vector(), 16*nmemb );
}

bool IOStream::writeMatrix ( const Matrix& mat )
{
#ifdef DEBUG
    assert ( m_ostream != 0 );
#endif
    return writeMatrix ( &mat, 1 );
}

ssize_t IOStream::readMatrix ( Matrix* v, u32 nmemb )
{
#ifdef DEBUG
    assert ( m_istream != 0 );
#endif
    return readFloat ( v->vector(), 16*nmemb ) / 16;
}

bool IOStream::readMatrix ( Matrix& mat )
{
#ifdef DEBUG
    assert ( m_istream != 0 );
#endif
    return readMatrix ( &mat, 1 ) == 1;
}

bool IOStream::writeVector ( const Vector3& vec )
{
#ifdef DEBUG
    assert ( m_ostream != 0 );
#endif
    return writeFloat ( vec.vector(), 3 );
}

ssize_t IOStream::readVector ( Vector3& vec )
{
#ifdef DEBUG
    assert ( m_istream != 0 );
#endif
    return readFloat ( vec.vector(), 3 ) / 3;
}

bool IOStream::writeColor ( const Color* col, u32 nmemb )
{
#ifdef DEBUG
    assert ( m_ostream != 0 );
#endif
    return write32 ( &col->value (), nmemb );
}

ssize_t IOStream::readColor ( Color* col, u32 nmemb )
{
#ifdef DEBUG
    assert ( m_istream != 0 );
#endif
    return read32 ( &col->value (), nmemb );
}

bool IOStream::writeQuaternion ( const Quaternion* v, u32 nmemb )
{
#ifdef DEBUG
    assert ( m_ostream != 0 );
#endif
    return writeFloat ( (f32 *)v, nmemb*4 );
}

ssize_t IOStream::readQuaternion ( Quaternion* v, u32 nmemb )
{
#ifdef DEBUG
    assert ( m_istream != 0 );
#endif
    return readFloat ( (f32 *)v, nmemb*4 ) / 4;
}

bool IOStream::writeTransform ( const Transform* v, u32 nmemb )
{
#ifdef DEBUG
    assert ( m_ostream != 0 );
#endif
    return writeFloat ( (f32 *)v, nmemb * sizeof(Transform) / sizeof(float) );
}

ssize_t IOStream::readTransform ( Transform* v, u32 nmemb )
{
#ifdef DEBUG
    assert ( m_istream != 0 );
#endif
    return readFloat ( (f32 *)v, nmemb * sizeof(Transform) / sizeof(float) ) / ( sizeof(Transform) / sizeof(float) );
}

bool IOStream::writeData ( const char* data, u32 size, u32 nmemb )
{
#ifdef DEBUG
    assert ( m_ostream != 0 );
#endif
    m_ostream->write ( reinterpret_cast<const char*>(data), size*nmemb );
    m_totalOut += size*nmemb;
    return true;
}

ssize_t IOStream::readData ( char* data, u32 size, u32 nmemb )
{
#ifdef DEBUG
    assert ( m_istream != 0 );
#endif
    m_istream->read ( data, size*nmemb );
    ssize_t numElems = m_istream->gcount () / size;
    m_totalIn += numElems * size;
    return numElems;
}
