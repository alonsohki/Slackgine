#ifndef VECTOR_H
#define VECTOR_H

class Vector2
{
private:
    typedef struct { float fX, fY; } __s2f;
    
    union
    {
        float v [ 2 ];
        __s2f s;
    };
    
public:
    Vector2 ()
    {
        x() = 0.0f;
        y() = 0.0f;
    }
    
    const float& x() const { return s.fX; }
    const float& y() const { return s.fY; }
    
    float& x() { return s.fX; }
    float& y() { return s.fY; }
};



class Vector3
{
private:
    typedef struct { float fX, fY, fZ; } __s3f;
    
    union
    {
        float v [ 3 ];
        __s3f s;
    };
    
public:
    Vector3 ()
    {
        x() = 0.0f;
        y() = 0.0f;
        z() = 0.0f;
    }
    
    const float& x() const { return s.fX; }
    const float& y() const { return s.fY; }
    const float& z() const { return s.fZ; }
    
    float& x() { return s.fX; }
    float& y() { return s.fY; }
    float& z() { return s.fZ; }
};

#endif
