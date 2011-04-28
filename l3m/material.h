#ifndef MATERIAL_H
#define MATERIAL_H

#include "color.h"

class Material
{
public:
                        Material        () {}
                        ~Material       () {}
    
    // Accessors
    const Color&        ambient         () const { return m_ambient; }
    const Color&        diffuse         () const { return m_diffuse; }
    const Color&        specular        () const { return m_specular; }
    const Color&        emission        () const { return m_emission; }
    
    Color&              ambient         () { return m_ambient; }
    Color&              diffuse         () { return m_diffuse; }
    Color&              specular        () { return m_specular; }
    Color&              emission        () { return m_emission; }

private:
    Color       m_ambient;
    Color       m_diffuse;
    Color       m_specular;
    Color       m_emission;
};

#endif
