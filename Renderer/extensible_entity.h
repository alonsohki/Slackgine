#ifndef EXTENSIBLE_ENTITY_H
#define	EXTENSIBLE_ENTITY_H

#include <vector>
#include "entity.h"

class ExtensibleEntity;

class IEntityExtension
{
public:
    virtual             ~IEntityExtension       () {}
    
    virtual const char* GetName                 () = 0;
    
    virtual void        Initialize              ( ExtensibleEntity* entity ) = 0;
    virtual void        Execute                 ( ExtensibleEntity* entity ) = 0;
    virtual void        Cleanup                 ( ExtensibleEntity* entity ) = 0;
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
                
    void                SetupForRendering       ();
    void                CleanupAfterRendering   ();
};

#endif	/* EXTENSIBLE_ENTITY_H */

