/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        geometry.h
// PURPOSE:     OpenGL3 geometries.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

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
    bool                initialize      ();
    
    bool                bindVertexLayer ( IProgram* program,
                                          const std::string& attributeName,
                                          const std::string& layerName,
                                          u32 layerLevel,
                                          DataType type,
                                          bool normalize,
                                          u32 count,
                                          u32 offset
                                          );
    bool                unbindAttribute ( IProgram* program, const std::string& attributeName );
    
    bool                m_initialized;
    GLuint              m_vertexBuffer;
    GLuint              m_elementBuffer;
    layerOffsetMap      m_offsets;
    meshNodeVector      m_meshNodes;
};

}
