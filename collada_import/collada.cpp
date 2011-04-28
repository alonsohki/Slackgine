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
            std::map<std::string, Source<float> > sources;
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
                    
                    sources[src.id] = src;
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
            sources[verticesElement->Attribute("id")] = sources[semantic->Attribute("source")+1];
            Source<float>& verticesSource = sources[semantic->Attribute("source")+1];
            verticesSource.id = semantic->Attribute("source")+1;
            
            // Get the polygon list
            TiXmlElement* polys = mesh->FirstChildElement("polylist");
            if ( !polys )
                return ERROR("Unable to find the polygon list");
            int polyCount = atoi(polys->Attribute("count"));
            
            // Find the offsets
            int offVertex = -1;
            int offNormal = -1;
            int offTex = -1;
            
            // Precompute the vertex and index count
            TiXmlElement* vcount = polys->FirstChildElement("vcount");
            if ( !vcount )
                return ERROR("Unable to find the vertex counts");
            const char* p = vcount->FirstChild()->Value();
            char* p2;
            u32 curCount = strtol(p, &p2, 10);
            u32 vertexCount = curCount;
            u32 indexCount = 3 + ( curCount - 3 ) * 3;
            for ( u32 i = 1; i < polyCount; ++i )
            {
                curCount = strtol(p2, &p2, 10);
                vertexCount += curCount;
                indexCount += 3 + ( curCount - 3 ) * 3;
            }
            
            // Allocate space for the vertices and indices
            Vertex* vertices = Vertex::Allocate(vertexCount);
            u32* indices = new u32 [ indexCount ];
            
            // Create the mesh
            Mesh* mesh = new Mesh ( "" );
            mesh->Set(vertices, vertexCount, indices, indexCount, Mesh::TRIANGLES );
            g->LoadMesh(mesh);
        }
    }
    
    return true;
#undef ERROR
}