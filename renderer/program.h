#ifndef PROGRAM_H
#define	PROGRAM_H

#include "shader.h"

class IProgram
{
public:
    virtual             ~IProgram       () {}
    
    virtual bool        AttachShader    ( const IShader* shader ) = 0;
    virtual bool        DetachShader    ( const IShader* shader ) = 0;
    virtual bool        Link            ( ) = 0;
    virtual bool        Use             ( ) = 0;
    
    virtual bool        Ok              ( ) const = 0;
};

#endif	/* PROGRAM_H */

