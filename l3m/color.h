#ifndef COLOR_H
#define COLOR_H

#include "shared/platform.h"

class Color
{
private:
    typedef struct { u8 ucR, ucG, ucB, ucA; } __s4b;
    union
    {
        u8    v [ 4 ];
        __s4b s;
    };
    
public:
    Color ( u8 R = 255, u8 G = 255, u8 B = 255, u8 A = 255 )
    {
        r() = R;
        g() = G;
        b() = B;
        a() = A;
    }
    
    // Accessors
public:
    const unsigned char& r() const { return s.ucR; }
    const unsigned char& g() const { return s.ucG; }
    const unsigned char& b() const { return s.ucB; }
    const unsigned char& a() const { return s.ucA; }
    
    unsigned char& r() { return s.ucR; }
    unsigned char& g() { return s.ucG; }
    unsigned char& b() { return s.ucB; }
    unsigned char& a() { return s.ucA; }
};

#endif
