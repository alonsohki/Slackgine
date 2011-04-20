#ifndef COLOR_H
#define COLOR_H

class Color
{
private:
    typedef struct { unsigned char ucR, ucG, ucB, ucA; } __s4b;
    union
    {
        unsigned char v [ 4 ];
        __s4b s;
    };
    
public:
    Color ()
    {
        r() = 0;
        g() = 0;
        b() = 0;
        a() = 0;
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
