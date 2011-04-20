#ifndef MATRIX_H
#define	MATRIX_H


class Matrix
{
private:
    union
    {
        float v [ 16 ];
        float m [ 4 ][ 4 ];
    };
    
public:
    Matrix ( )
    {
        for ( unsigned int i = 0; i < 4; ++i )
            for ( unsigned int j = 0; j < 4; ++j )
                m[i][j] = ( i==j ? 1.0f : 0.0f );
    }
    
    float*              vector  () { return v; }
    const float*        vector  () const { return v; }
};

#endif	/* MATRIX_H */

