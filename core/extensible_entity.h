#ifndef EXTENSIBLE_ENTITY_H
#define	EXTENSIBLE_ENTITY_H

#include <vector>
#include "slackgine.h"
#include "entity.h"

class ExtensibleEntity;

class IEntityExtension
{
public:
    virtual             ~IEntityExtension       () {}
    
    virtual const char* GetName                 () = 0;
    
    virtual void        Initialize              ( ExtensibleEntity* entity ) = 0;
    virtual void        Update                  ( ExtensibleEntity* entity, Slackgine* ctx ) = 0;
};

class ExtensibleEntity : public Entity
{
private:
    std::vector<IEntityExtension*> m_extensions;

public:
                        ExtensibleEntity        ();
                        ~ExtensibleEntity       ();
                
    bool                RegisterExtension       ( IEntityExtension* pExtension );
    IEntityExtension*   GetExtension            ( const char* name ) const;
                
    void                Update                  ( Slackgine* ctx );
};

#endif	/* EXTENSIBLE_ENTITY_H */

