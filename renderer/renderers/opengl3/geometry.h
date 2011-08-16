/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011 - Alberto Alonso
//
// Redistribution of this software, including any of its
// files, in either binary or source code form is totally
// forbidden without the permission of its owners.
//
// Owners reserve their right to legal action against any
// person or company that fails to do so.
//
// For more detailed information, see the LICENSE file in
// the top-level directory.
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
    bool                Initialize      ();
    
    bool                m_initialized;
    GLuint              m_vertexBuffer;
    GLuint              m_elementBuffer;
    layerOffsetMap      m_offsets;
    meshNodeVector      m_meshNodes;
};

}