#ifndef COLOR_H
#define COLOR_H

class Color
{
private:
    typedef struct { float fR, fG, fB, fA; } __s4f;
    union
    {
        float v [ 4 ];
        __s4f s;
    };
    
public:
    Color ()
    {
        r() = 0.0f;
        g() = 0.0f;
        b() = 0.0f;
        a() = 0.0f;
    }
    
    // Accessors
public:
    const float& r() const { return s.fR; }
    const float& g() const { return s.fG; }
    const float& b() const { return s.fB; }
    const float& a() const { return s.fA; }
    
    float& r() { return s.fR; }
    float& g() { return s.fG; }
    float& b() { return s.fB; }
    float& a() { return s.fA; }
};

#endif
