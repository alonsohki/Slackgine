#ifndef L3MWITHDESCRIPTION_H
#define	L3MWITHDESCRIPTION_H

#include <string>
#include "l3m/l3m.h"

class l3mWithDescription : public l3m
{
    std::string m_description;
    
public:
    l3mWithDescription ( const std::string& desc = "" )
    : l3m ( "descriptable" )
    , m_description ( desc )
    {
        DeclareMetadata("description");
        DeclareMetadata("other stuff");
    }
    
    ~l3mWithDescription () {}
    
    bool SaveMetadata( const std::string& name, std::ostream& fp )
    {
        if ( name == "description" )
            return WriteStr ( m_description, fp );
        else if ( name == "other stuff" )
        {
            unsigned int magicNumber = 0xFABADA;
            return Write32 ( &magicNumber, 1, fp );
        }
        
        return false;
    }
    
    bool LoadMetadata ( const std::string& name, std::istream& fp )
    {
        if ( name == "description" )
            return ReadStr ( m_description, fp );
        else if ( name == "other stuff" )
        {
            unsigned int magicNumber;
            if ( Read32 ( &magicNumber, 1, fp ) )
                if ( magicNumber != 0xFABADA )
                    fprintf(stderr, "Invalid magic number!\n" );
                else
                    return true;
        }
        
        return false;
    }
    
    const std::string& description() const { return m_description; }
    std::string& description() { return m_description; }
};

#endif	/* L3MWITHDESCRIPTION_H */

