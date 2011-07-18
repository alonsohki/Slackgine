#ifndef COLOR_H
#define COLOR_H

#include "shared/platform.h"

class Color
{
private:
    union
    {
        u8  m_members[4];
        u32 m_value;
    };
    
    
public:
    Color ( u8 R = 255, u8 G = 255, u8 B = 255, u8 A = 255 )
    {
        Set ( R, G, B, A );
    }
    
    void Set ( u8 R, u8 G, u8 B, u8 A )
    {
        m_members[detectBigEndian() ? 0 : 3] = R;
        m_members[detectBigEndian() ? 1 : 2] = R;
        m_members[detectBigEndian() ? 2 : 1] = R;
        m_members[detectBigEndian() ? 3 : 0] = R;
    }
    
    // Accessors
public:
    u32 abgr () const
    {
        return ( ( m_value >> 24 ) & 0x000000FF )
             | ( ( m_value >> 8  ) & 0x0000FF00 )
             | ( ( m_value << 8  ) & 0x00FF0000 )
             | ( ( m_value << 24 ) & 0xFF000000 );
    }
    u32 argb () const
    {
        return ( ( m_value >> 8  ) & 0x00FFFFFF )
             | ( ( m_value << 24 ) & 0xFF000000 );
    }

    const u8& r() const { return m_members[detectBigEndian() ? 3 : 0]; }
    const u8& g() const { return m_members[detectBigEndian() ? 2 : 1]; }
    const u8& b() const { return m_members[detectBigEndian() ? 1 : 2]; }
    const u8& a() const { return m_members[detectBigEndian() ? 0 : 3]; }
    const u32& rgba() const { return m_value; }
    
    u8& r() { return m_members[detectBigEndian() ? 3 : 0]; }
    u8& g() { return m_members[detectBigEndian() ? 3 : 0]; }
    u8& b() { return m_members[detectBigEndian() ? 3 : 0]; }
    u8& a() { return m_members[detectBigEndian() ? 3 : 0]; }
    u32& rgba() { return m_value; }
};

#endif
