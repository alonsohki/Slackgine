#pragma once

#include <map>
#include <string>
#include <vector>
#include "renderer/geometry.h"

namespace Renderer
{

class Geometry : public GeometryBase
{
public:
    typedef std::map<std::string, u32> layerOffsetMap;
    struct MeshNode
    {
        Mesh*   mesh;
        u32     offset;
    };
    typedef std::vector<MeshNode> meshNodeVector;

public:
                        Geometry        ();
                        ~Geometry       ();
    
    bool                initialized     () const { return m_initialized; }
    bool                Initialize      ();
    
    bool                m_initialized;
    GLuint              m_vertexBuffer;
    GLuint              m_elementBuffer;
    layerOffsetMap      m_offsets;
    meshNodeVector      m_meshNodes;
};

}