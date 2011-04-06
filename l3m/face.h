#ifndef FACE_H
#define	FACE_H

class Face
{
public:
    static const unsigned int VERSION = 0;

private:
    unsigned int m_index;
    
    // Accessors
public:
    const unsigned int&         idx     () const { return m_index; }
    const unsigned int*         base    () const { return &idx(); }
    
    unsigned int&               idx     () { return m_index; }
    unsigned int*               base    () { return &idx(); }
    
public:
                        Face            () {}

    void                Load            ( const unsigned int* indices, unsigned int count );
    
    static Face*        Allocate        ( unsigned int count );
    static Face*        LoadAllocating  ( const unsigned int* indices, unsigned int count );
};

#endif	/* FACE_H */

