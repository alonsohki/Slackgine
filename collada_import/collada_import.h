#ifndef COLLADA_IMPORT_H
#define	COLLADA_IMPORT_H

#include "l3m/l3m.h"
#include "tinyxml/tinyxml.h"

extern bool collada_import ( TiXmlDocument&, l3m&, const char** err = 0 );

#endif	/* COLLADA_IMPORT_H */

