#ifndef COLLADA_IMPORT_H
#define	COLLADA_IMPORT_H

#include "l3m/l3m.h"
#include "tinyxml/tinyxml.h"

class Collada
{
public:
    static bool Import ( TiXmlDocument&, l3m&, const char** err = 0 );
    
private:
    template < typename T >
    struct Source
    {
        Source() : count(0), array(0) {}
        Source(const Source& Right) { operator=(Right); }
        Source& operator=(const Source& Right)
        {
            id = Right.id;
            count = Right.count;
            array = new T[count];
            memcpy(array, Right.array, sizeof(T)*count);
            return *this;
        }
        ~Source () { delete [] array; }
        
        std::string id;
        u32 count;
        T* array;
    };
};

#endif	/* COLLADA_IMPORT_H */

