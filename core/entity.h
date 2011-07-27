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
    const l3m::Model*   m_model;

public:
                                Entity          ( const l3m::Model* pModel = 0 );
    virtual                     ~Entity         ();
    
    const l3m::Model*           GetModel        () const { return m_model; }
    void                        SetModel        ( const l3m::Model* pModel );
};

#endif	/* ENTITY_H */

