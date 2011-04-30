#include "collada.h"

bool Collada::Import ( TiXmlDocument& xml, l3m& model, const char** err )
{
#define ERROR(msg) ( (err != 0) ? (*err=(msg)) == (const char*)0xfabada : false )
    
    TiXmlElement* collada = xml.FirstChildElement("COLLADA");
    if ( !collada )
        return ERROR("Couldn't find the root node");
    
    TiXmlElement* geometries = collada->FirstChildElement("library_geometries");
    if ( !geometries )
        return ERROR("Couldn't locate the model geometries");
    
    // Load all the geometries
    for ( TiXmlElement* geometry = geometries->FirstChildElement("geometry");
          geometry != 0;
          geometry = geometry->NextSiblingElement("geometry") )
    {
        const char* name = geometry->Attribute("id");
        if ( !name ) name = "";
        Geometry* g = model.CreateGeometry(name);

        // Load each mesh
        for ( TiXmlElement* mesh = geometry->FirstChildElement("mesh");
              mesh != 0;
              mesh = mesh->NextSiblingElement("mesh") )
        {
            // Load all the sources
            std::map<std::string, Source<float> > mapSources;
            for ( TiXmlElement* source = mesh->FirstChildElement("source");
                  source != 0;
                  source = source->NextSiblingElement("source") )
            {
                Source<float> src;
                src.id = source->Attribute("id");
                TiXmlElement* array = source->FirstChildElement("float_array");
                src.count = atoi(array->Attribute("count"));
                if ( src.count > 0 )
                {
                    const char* p = array->FirstChild()->Value();
                    char* p2;
                    src.array = new float[src.count];
                    src.array[0] = strtof(p, &p2);
                    for ( u32 i = 1; i < src.count; ++i )
                        src.array[i] = strtof ( p2, &p2 );
                    
                    mapSources[src.id] = src;
                }
            }
            
            // Get the vertex source id
            TiXmlElement* verticesElement = mesh->FirstChildElement("vertices");
            if ( !verticesElement )
                return ERROR("Unable to find the vertices declaration");
            
            TiXmlElement* semantic = verticesElement->FirstChildElement("input");
            if ( !semantic )
                return ERROR("Unable to find the vertices input semantic");
            // Make an alias of the source
            mapSources[verticesElement->Attribute("id")] = mapSources[semantic->Attribute("source")+1];
            Source<float>& verticesSource = mapSources[semantic->Attribute("source")+1];
            verticesSource.id = semantic->Attribute("source")+1;
            
            // Get the polygon list
            TiXmlElement* polys = mesh->FirstChildElement("polylist");
            if ( !polys )
                return ERROR("Unable to find the polygon list");
            int polyCount = atoi(polys->Attribute("count"));
            
            // Find the offsets and sources
            enum InputType { NONE, VERTEX, NORMAL, TEX };
            int offsets [ 10 ];
            Source<float>* sources [ 4 ];
            
            for ( u32 i = 0; i < sizeof(offsets)/sizeof(offsets[0]); ++i )
                offsets[i] = NONE;
            
            for ( TiXmlElement* input = polys->FirstChildElement("input");
                  input != 0;
                  input = input->NextSiblingElement("input") )
            {
                InputType type = NONE;
                const char* semantic = input->Attribute("semantic");
                if ( !strcmp(semantic, "VERTEX") ) type = VERTEX;
                else if ( !strcmp(semantic, "NORMAL") ) type = NORMAL;
                
                if ( type != NONE )
                {
                    offsets[atoi(input->Attribute("offset"))] = type;
                    sources[type] = &mapSources[input->Attribute("source")+1];
                }
            }
            
            // Precompute the vertex and index count
            std::vector<u32> vecCounts;
            TiXmlElement* vcount = polys->FirstChildElement("vcount");
            if ( !vcount )
                return ERROR("Unable to find the vertex counts");
            const char* p = vcount->FirstChild()->Value();
            char* p2;
            u32 curCount = strtol(p, &p2, 10);
            vecCounts.push_back(curCount);
            u32 vertexCount = curCount;
            u32 indexCount = 3 + ( curCount - 3 ) * 3;
            for ( u32 i = 1; i < polyCount; ++i )
            {
                curCount = strtol(p2, &p2, 10);
                vecCounts.push_back(curCount);
                vertexCount += curCount;
                indexCount += 3 + ( curCount - 3 ) * 3;
            }
            
            // Allocate space for the vertices and indices
            Vertex* vertices = Vertex::Allocate(vertexCount);
            u32* indices = new u32 [ indexCount ];
            
            // Load all the references
            std::vector<u32> vecIndices;
            TiXmlElement* indicesElement = polys->FirstChildElement("p");
            if ( !indicesElement )
                return ERROR("Unable to find the indices data");
            p = indicesElement->FirstChild()->Value();
            u32 curIndex = strtol(p, &p2, 10);
            vecIndices.push_back(curIndex);
            while ( *p2 != 0 )
            {
                curIndex = strtol ( p2, &p2, 10 );
                vecIndices.push_back(curIndex);
            }
            
            // Load the vertices
            Vertex* curVertex = vertices;
            curIndex = 0;
            for ( std::vector<u32>::const_iterator i = vecCounts.begin(); i != vecCounts.end(); ++i )
            {
                for ( u32 c = 0; c < *i; ++c )
                {
                    for ( u32 off = 0; off < sizeof(offsets)/sizeof(offsets[0]) && offsets[off] != NONE; ++off )
                    {
                        Source<float>* src = sources[offsets[off]];
                        u32 index = vecIndices[curIndex];
                        ++curIndex;
                        switch ( offsets[off] )
                        {
                            case VERTEX:
                                curVertex->pos() = Vector3 ( src->array[index*3], src->array[index*3+1], src->array[index*3+2] );
                                break;
                            case NORMAL:
                                curVertex->norm() = Vector3 ( src->array[index*3], src->array[index*3+1], src->array[index*3+2] );
                                break;
                        }
                    }
                    ++curVertex;
                }
            }
            
            // Create the indices, making triangles
            u32 totalCount = 0;
            curIndex = 0;
            for ( u32 i = 0; i < polyCount; ++i )
            {
                curCount = vecCounts[i];
                indices[curIndex] = totalCount;
                indices[curIndex + 1] = totalCount + 1;
                indices[curIndex + 2] = totalCount + 2;
                curIndex += 3;
                
                for ( u32 j = 0; j < curCount-3; ++j )
                {
                    indices[curIndex] = totalCount;
                    indices[curIndex + 1] = totalCount + j + 2;
                    indices[curIndex + 2] = totalCount + j + 3;
                    curIndex += 3;
                }
                
                totalCount += curCount;
            }
                
            // Create the mesh
            Mesh* mesh = new Mesh ( "" );
            mesh->Set(vertices, vertexCount, indices, indexCount, Mesh::TRIANGLES );
            g->LoadMesh(mesh);
        }
    }
    
    return true;
#undef ERROR
}