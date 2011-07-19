#ifndef DEFAULT_H
#define	DEFAULT_H

#include "../strategy.h"

class GeometriesStrategy : public IStrategy
{
public:
    GeometriesStrategy ()
    {
    }
    
    ~GeometriesStrategy ()
    {
    }
    
    bool ParseData ( TiXmlElement& element, l3m::Model& model, const char** err );
    
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

#endif	/* DEFAULT_H */

