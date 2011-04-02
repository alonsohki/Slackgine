#ifndef FACE_H
#define	FACE_H

class Face
{
private:
    unsigned int m_index;
    
    // Accessors
public:
    const unsigned int& idx () const { return m_index; }
    unsigned int& idx () { return m_index; }
    
public:
                        Face            () {}

    void                Load            ( const unsigned int* indices, unsigned int count );
    
    static Face*        LoadAllocating  ( const unsigned int* indices, unsigned int count );
};

#endif	/* FACE_H */

