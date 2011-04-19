#include <cstring>
#include "extensible_entity.h"

ExtensibleEntity::ExtensibleEntity ()
{
    
}

ExtensibleEntity::~ExtensibleEntity ()
{
    for ( std::vector<IEntityExtension*>::iterator i = m_extensions.begin(); i != m_extensions.end(); ++i )
        delete *i;
    m_extensions.clear ();
}

bool ExtensibleEntity::RegisterExtension(IEntityExtension* pExtension)
{
    if ( GetExtension(pExtension->GetName()) != 0 )
        return false;
    m_extensions.push_back ( pExtension );
    pExtension->Initialize(this);
    return true;
}

IEntityExtension* ExtensibleEntity::GetExtension ( const char* name ) const
{
    for ( std::vector<IEntityExtension*>::const_iterator i = m_extensions.begin(); i != m_extensions.end(); ++i )
        if ( !strcmp ( name, (*i)->GetName() ) == 0 )
            return *i;
    return 0;
}

void ExtensibleEntity::SetupForRendering()
{
    for ( std::vector<IEntityExtension*>::const_iterator i = m_extensions.begin(); i != m_extensions.end(); ++i )
        (*i)->Execute ( this );
}

void ExtensibleEntity::CleanupAfterRendering()
{
    for ( std::vector<IEntityExtension*>::const_iterator i = m_extensions.begin(); i != m_extensions.end(); ++i )
        (*i)->Cleanup ( this );
}
