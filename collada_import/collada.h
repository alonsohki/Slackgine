#ifndef COLLADA_IMPORT_H
#define	COLLADA_IMPORT_H

#include "l3m/l3m.h"
#include "tinyxml/tinyxml.h"

class Collada
{
public:
    static bool Import ( TiXmlDocument&, l3m&, const char** err = 0 );
    
private:
#if 0
    struct Source
    {
        struct IArray
        {
            virtual ~IArray () {}
            virtual void* GetData () = 0;
        };
        template<typename T> struct Array : public IArray
        {
            Array () : count(0), data(0) {}
            Array ( const Array& Right )
            {
                count = Right.count;
                data = new T [ count ];
                memcpy ( data, Right.data, count*sizeof(T) );
            }
            ~Array () { delete [] data; }
            void* GetData () { return data; }
            u32 count;
            T* data;
        };
        
        std::string id;
        std::map<std::string, IArray* > arrays;
    };
#endif
};

#endif	/* COLLADA_IMPORT_H */

