#ifndef COLLADA_IMPORT_H
#define	COLLADA_IMPORT_H

#include "l3m/l3m.h"
#include "tinyxml/tinyxml.h"

class Collada
{
public:
    static bool Import ( TiXmlDocument&, l3m::Model&, const char** err = 0 );
};

#endif	/* COLLADA_IMPORT_H */

