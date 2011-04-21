#ifndef PROGRAM_H
#define	PROGRAM_H

#include <iostream>

class IProgram
{
public:
                        IProgram        ();
                        IProgram        ( const char* file );
                        IProgram        ( std::istream& in );

    virtual             ~IProgram       () {}
    
    bool                Load            ( const char* file );
    virtual bool        Load            ( std::istream& in ) = 0;
};

#include "renderers/renderer_selection.h"

#endif	/* PROGRAM_H */

