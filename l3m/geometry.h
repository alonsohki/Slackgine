#ifndef GEOMETRY_H
#define	GEOMETRY_H

#include <string>
#include "math/matrix.h"

class Geometry
{
public:
    typedef std::list<Mesh *> meshList;

private:
    std::string         m_name;
    Matrix              m_matrix;
    meshList            m_meshes;
    
public:
    Geometry ( const std::string& name )
    : m_name ( name )
    {
    }
    
    ~Geometry ()
    {
        for ( meshList::iterator iter = m_meshes.begin(); iter != m_meshes.end(); ++iter )
            delete *iter;
    }
    
    void LoadMesh ( Mesh* mesh )
    {
        m_meshes.push_back(mesh);
    }
    
    // Accessors
public:
    const std::string&          name    () const { return m_name; }
    const Matrix&               matrix  () const { return m_matrix; }
    const meshList&             meshes  () const { return m_meshes; }
    
    std::string&                name    () { return m_name; }
    Matrix&                     matrix  () { return m_matrix; }
    meshList&                   meshes  () { return m_meshes; }
};

#endif	/* GEOMETRY_H */

