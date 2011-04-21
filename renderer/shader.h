#ifndef SHADER_H
#define	SHADER_H

#include <iostream>
#include <fstream>

class IShader
{
public:
    virtual             ~IShader        () {}
    
    bool                Load            ( const char* file )
    {
        std::ifstream fp ( file, std::ios::in | std::ios::binary );
        if ( fp.fail() )
            return false;
        return Load ( fp );
    }
    virtual bool        Load            ( std::istream& fp ) = 0;
    virtual bool        Ok              () const = 0;
};

#include "renderers/renderer_selection.h"

#endif	/* SHADER_H */

