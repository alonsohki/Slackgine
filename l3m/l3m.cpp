#include "l3m.h"
#include "Components/factory.h"

using namespace l3m;

static const char* const L3M_BOM = "L3M\x01";
static const float L3M_VERSION = 2.0f;
static const enum
{
    L3M_MACHINE_ENDIAN,
    L3M_LOW_ENDIAN,
    L3M_BIG_ENDIAN
} L3M_SAVE_ENDIANNESS = L3M_LOW_ENDIAN;

bool Model::Load(std::istream& fp)
{
    // Check BOM
    char BOM [ 8 ];
    fp.read ( BOM, sizeof(char)*strlen(L3M_BOM) );
    if ( memcmp ( BOM, L3M_BOM, strlen(L3M_BOM) ) != 0 )
        return false;

    // Check for endianness swapping
    u8 targetIsBigEndian;
    fp.read ( reinterpret_cast<char*>(&targetIsBigEndian), 1 );
    u8 thisMachineIsBigEndian = detectBigEndian();
    bool doEndianSwapping = ( thisMachineIsBigEndian != targetIsBigEndian );
    
    u32 flags = IOStream::NONE;
    if ( doEndianSwapping )
        flags |= IOStream::ENDIAN_SWAPPING;
    
    // Generate the input stream.
    IStream is ( &fp, flags );
    u32 readFlags = IOStream::NONE;
    if ( is.Read32 ( &readFlags, 1 ) != 1 )
        return false;
    readFlags &= ~IOStream::ENDIAN_SWAPPING;
    readFlags |= flags & IOStream::ENDIAN_SWAPPING;
    is.SetFlags ( readFlags );
    
    // Read the number of components
    u32 numComponents;
    if ( is.Read32(&numComponents, 1) != 1 )
        return false;
    
    for ( u32 i = 0; i < numComponents; ++i )
    {
        // Read the component type
        std::string type;
        is.ReadStr ( type );
        
        // Read the component version
        float version;
        if ( is.ReadFloat ( &version, 1 ) != 1 )
            return false;
        
        IComponent* component = ComponentFactory::Create( type );
        if ( component == 0 )
            return false;
        if ( component->Load ( is, version ) == false )
            return false;
        
        m_vecComponents.push_back ( component );
    }
    
    return true;
}

bool Model::Save(std::ostream& fp)
{
    OStream os ( &fp, IOStream::NONE );
    
    // BOM
    os.WriteData ( L3M_BOM, sizeof(char), strlen(L3M_BOM) );
    
    // Write the BOM endianness identifier, based on the machine endianness and desired configuration
    u8 thisMachineIsBigEndian = detectBigEndian ();
    u8 targetIsBigEndian;
    
    if ( L3M_SAVE_ENDIANNESS == L3M_MACHINE_ENDIAN )
        targetIsBigEndian = thisMachineIsBigEndian;
    else
        targetIsBigEndian = ( L3M_SAVE_ENDIANNESS == L3M_BIG_ENDIAN );
    os.WriteData ( reinterpret_cast<char*>(&targetIsBigEndian), sizeof(u8), 1 );
    
    // Flags
    u32 flags = os.flags() & ~IOStream::ENDIAN_SWAPPING;
    if ( os.Write32 ( &flags, 1 ) != 1 )
        return false;
    
    // Versión
    float fVersion = L3M_VERSION;
    if ( !os.WriteFloat ( &fVersion, 1 ) != 1 )
        return false;
    
    // Number of components
    u32 numComponents = m_vecComponents.size ();
    if ( !os.Write32 ( &numComponents, 1 ) != 1 )
        return false;
    
    // Write each component
    for ( u32 i = 0; i < numComponents; ++i )
    {
        // Write the component type
        os.WriteStr ( m_vecComponents[i]->type() );
        
        // Write the component version
        float version = m_vecComponents[i]->version();
        if ( os.WriteFloat ( &version, 1 ) != 1 )
            return false;
        
        // Write the component
        if ( !m_vecComponents[i]->Save ( os ) )
            return false;
    }
    
    return true;
}
