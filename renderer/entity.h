#ifndef ENTITY_H
#define	ENTITY_H

#include "matrix.h"
#include "l3m/l3m.h"

class Entity
{
private:
    Matrix              m_matrix;
    const l3m*          m_model;

public:
                        Entity                  ( const l3m* pModel = 0 );
    virtual             ~Entity                 ();
    
    virtual void        SetupForRendering       () {}
    virtual void        CleanupAfterRendering   () {}
    
    const l3m*          GetModel                () const { return m_model; }
};

#endif	/* ENTITY_H */

