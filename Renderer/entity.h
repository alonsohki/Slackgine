#ifndef ENTITY_H
#define	ENTITY_H

class Entity
{
public:
                        Entity                  () {}
    virtual             ~Entity                 () {}
    
    virtual void        SetupForRendering       () {}
    virtual void        CleanupAfterRendering   () {}
};

#endif	/* ENTITY_H */

