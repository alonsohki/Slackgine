#ifndef ENTITY_H
#define	ENTITY_H

#include "slackgine.h"
#include "math/matrix.h"
#include "l3m/l3m.h"

class Slackgine;

class Entity
{
private:
    Matrix              m_matrix;
    const l3m*          m_model;

public:
                        Entity                  ( const l3m* pModel = 0 );
    virtual             ~Entity                 ();
    
    virtual void        Update                  ( Slackgine* ctx );
    
    virtual const l3m*  GetModel                () const { return m_model; }
};

#endif	/* ENTITY_H */
