#include <set>
#include <string>
#include <sstream>
#include "blender_stuff.h"
#include "BLO_readfile.h"
#include "DNA_armature_types.h"
#include "DNA_object_types.h"
#include "DNA_mesh_types.h"
#include "DNA_meshdata_types.h"
#include "DNA_scene_types.h"
#include "BKE_customdata.h"
#include "BKE_object.h"
#include "BLI_math.h"
#include "shared/platform.h"
#include "math/vector.h"
#include "l3m/l3m.h"
#include "l3m/Components/components.h"
#include "renderer/mesh.h"

/**
Translation map.
Used to translate every COLLADA id to a valid id, no matter what "wrong" letters may be
included. Look at the IDREF XSD declaration for more.
Follows strictly the COLLADA XSD declaration which explicitly allows non-english chars,
like special chars (e.g. micro sign), umlauts and so on.
The COLLADA spec also allows additional chars for member access ('.'), these
must obviously be removed too, otherwise they would be heavily misinterpreted.
*/
const unsigned char translate_start_name_map[256] = {
95,  95,  95,  95,  95,  95,  95,  95,  95,
95,  95,  95,  95,  95,  95,  95,  95,
95,  95,  95,  95,  95,  95,  95,  95,
95,  95,  95,  95,  95,  95,  95,  95,
95,  95,  95,  95,  95,  95,  95,  95,
95,  95,  95,  95,  95,  95,  95,  95,
95,  95,  95,  95,  95,  95,  95,  95,
95,  95,  95,  95,  95,  95,  95,  95,
65,  66,  67,  68,  69,  70,  71,  72,
73,  74,  75,  76,  77,  78,  79,  80,
81,  82,  83,  84,  85,  86,  87,  88,
89,  90,  95,  95,  95,  95,  95,  95,
97,  98,  99,  100,  101,  102,  103,  104,
105,  106,  107,  108,  109,  110,  111,  112,
113,  114,  115,  116,  117,  118,  119,  120,
121,  122,  95,  95,  95,  95,  95,  95,
95,  95,  95,  95,  95,  95,  95,  95,
95,  95,  95,  95,  95,  95,  95,  95,
95,  95,  95,  95,  95,  95,  95,  95,
95,  95,  95,  95,  95,  95,  95,  95,
95,  95,  95,  95,  95,  95,  95,  95,
95,  95,  95,  95,  95,  95,  95,  95,
95,  95,  95,  95,  95,  95,  95,  95,
95,  95,  95,  95,  95,  95,  95,  192,
193,  194,  195,  196,  197,  198,  199,  200,
201,  202,  203,  204,  205,  206,  207,  208,
209,  210,  211,  212,  213,  214,  95,  216,
217,  218,  219,  220,  221,  222,  223,  224,
225,  226,  227,  228,  229,  230,  231,  232,
233,  234,  235,  236,  237,  238,  239,  240,
241,  242,  243,  244,  245,  246,  95,  248,
249,  250,  251,  252,  253,  254,  255};

const unsigned char translate_name_map[256] = {
95,  95,  95,  95,  95,  95,  95,  95,  95,
95,  95,  95,  95,  95,  95,  95,  95,
95,  95,  95,  95,  95,  95,  95,  95,
95,  95,  95,  95,  95,  95,  95,  95,
95,  95,  95,  95,  95,  95,  95,  95,
95,  95,  95,  95,  45,  95,  95,  48,
49,  50,  51,  52,  53,  54,  55,  56,
57,  95,  95,  95,  95,  95,  95,  95,
65,  66,  67,  68,  69,  70,  71,  72,
73,  74,  75,  76,  77,  78,  79,  80,
81,  82,  83,  84,  85,  86,  87,  88,
89,  90,  95,  95,  95,  95,  95,  95,
97,  98,  99,  100,  101,  102,  103,  104,
105,  106,  107,  108,  109,  110,  111,  112,
113,  114,  115,  116,  117,  118,  119,  120,
121,  122,  95,  95,  95,  95,  95,  95,
95,  95,  95,  95,  95,  95,  95,  95,
95,  95,  95,  95,  95,  95,  95,  95,
95,  95,  95,  95,  95,  95,  95,  95,
95,  95,  95,  95,  95,  95,  95,  95,
95,  95,  95,  95,  95,  95,  95,  95,
95,  95,  95,  95,  95,  95,  95,  95,
95,  95,  95,  95,  95,  95,  183,  95,
95,  95,  95,  95,  95,  95,  95,  192,
193,  194,  195,  196,  197,  198,  199,  200,
201,  202,  203,  204,  205,  206,  207,  208,
209,  210,  211,  212,  213,  214,  95,  216,
217,  218,  219,  220,  221,  222,  223,  224,
225,  226,  227,  228,  229,  230,  231,  232,
233,  234,  235,  236,  237,  238,  239,  240,
241,  242,  243,  244,  245,  246,  95,  248,
249,  250,  251,  252,  253,  254,  255};

typedef std::map< std::string, std::vector<std::string> > map_string_list;
map_string_list global_id_map;

void clear_global_id_map()
{
	global_id_map.clear();
}

/** Look at documentation of translate_map */
std::string translate_id(const std::string &id)
{
	if (id.size() == 0)
	{ return id; }
	std::string id_translated = id;
	id_translated[0] = translate_start_name_map[(unsigned int)id_translated[0]];
	for (unsigned int i=1; i < id_translated.size(); i++)
	{
		id_translated[i] = translate_name_map[(unsigned int)id_translated[i]];
	}
	// It's so much workload now, the if() should speed up things.
	if (id_translated != id)
	{
		// Search duplicates
		map_string_list::iterator iter = global_id_map.find(id_translated);
		if (iter != global_id_map.end())
		{
			unsigned int i = 0;
			bool found = false;
			for (i=0; i < iter->second.size(); i++)
			{
				if (id == iter->second[i])
				{ 
					found = true;
					break;
				}
			}
			bool convert = false;
			if (found)
			{
			  if (i > 0)
			  { convert = true; }
			}
			else
			{ 
				convert = true;
				global_id_map[id_translated].push_back(id);
			}
			if (convert)
			{
				std::stringstream out;
				out << ++i;
				id_translated += out.str();
			}
		}
		else { global_id_map[id_translated].push_back(id); }
	}
	return id_translated;
}

std::string id_name(void *id)
{
	return ((ID*)id)->name + 2;
}

std::string get_geometry_id(Object *ob)
{
	return translate_id(id_name(ob->data)) + "-mesh";
}

std::string get_light_id(Object *ob)
{
	return translate_id(id_name(ob)) + "-light";
}

std::string get_joint_id(Bone *bone, Object *ob_arm)
{
	return translate_id(/*id_name(ob_arm) + "_" +*/ bone->name);
}

std::string get_camera_id(Object *ob)
{
	return translate_id(id_name(ob)) + "-camera";
}

std::string get_material_id(Material *mat)
{
	return translate_id(id_name(mat)) + "-material";
}

static Matrix get_node_transform_ob(Object *ob)
{
	float rot[3], loc[3], scale[3];

	if (ob->parent) {
		float C[4][4], tmat[4][4], imat[4][4], mat[4][4];

		// factor out scale from obmat

		copy_v3_v3(scale, ob->size);

		ob->size[0] = ob->size[1] = ob->size[2] = 1.0f;
		object_to_mat4(ob, C);
		copy_v3_v3(ob->size, scale);

		mul_serie_m4(tmat, ob->parent->obmat, ob->parentinv, C, NULL, NULL, NULL, NULL, NULL);

		// calculate local mat

		invert_m4_m4(imat, ob->parent->obmat);
		mul_m4_m4m4(mat, tmat, imat);

		// done

		mat4_to_eul(rot, mat);
		copy_v3_v3(loc, mat[3]);
	}
	else {
		copy_v3_v3(loc, ob->loc);
		copy_v3_v3(rot, ob->rot);
		copy_v3_v3(scale, ob->size);
	}

        return TranslationMatrix ( loc ) * RotationMatrix ( rot ) * ScalingMatrix ( scale );
}






static void ImportVertex ( Renderer::Vertex* to, MVert* from, float* uv )
{
    to->pos() = from->co;
    to->norm() = Vector3 ( from->no[0] / 32767.0f, from->no[1] / 32767.0f, from->no[2] / 32767.0f );
    if ( uv != 0 )
        to->tex2d() = Vector2 ( uv[0], uv[1] );
    else
        to->tex2d() = Vector2 ( 0, 0 );
}

static bool ImportMesh ( Renderer::Geometry* g, const std::string& name, u32 mat_index, Object* ob, l3m::Model* model )
{
    // Get the actual face count
    Mesh* me = (Mesh *)ob->data;
    u32 totface = me->totface;
    MFace *faces = me->mface;
    u32 actualFaceCount = 0;
    
    for ( u32 i = 0; i < totface; ++i )
    {
        MFace* face = &faces[i];
        if ( face->mat_nr == mat_index )
        {
            if ( face->v4 == 0 )
                ++actualFaceCount;
            else
                actualFaceCount += 2;
        }
    }
    
    // Make all the indices
    u32* indices = ( u32* )malloc ( sizeof(u32) * actualFaceCount * 3 );
    u32 indexIdx = 0;
    u32 curIndex = 0;
    for ( u32 i = 0; i < totface; ++i )
    {
        MFace* face = &faces[i];
        if ( face->mat_nr == mat_index )
        {
            indices[indexIdx++] = curIndex;
            indices[indexIdx++] = curIndex+1;
            indices[indexIdx++] = curIndex+2;
        }
        curIndex += 3;

        if ( face->v4 != 0 )
        {
            if ( face->mat_nr == mat_index )
            {
                indices[indexIdx++] = curIndex;
                indices[indexIdx++] = curIndex+1;
                indices[indexIdx++] = curIndex+2;
            }
            curIndex += 3;
        }
    }
    
    Renderer::Mesh* mesh = new Renderer::Mesh ();
    mesh->Set ( indices, actualFaceCount * 3, Renderer::Mesh::TRIANGLES );
    mesh->name() = name;
    g->LoadMesh( mesh );
    
    return true;
}

static bool ImportGeometry ( Renderer::Geometry* g, Object* ob, l3m::Model* model )
{
    Mesh* me = (Mesh *)ob->data;
    MFace *faces = me->mface;
    MVert* verts = me->mvert;
    u32 totvert = me->totvert;
    u32 totcol = me->totcol;
    u32 totface = me->totface;
    
    // Count the total number of vertices
    u32 actualVertexCount = 0;
    for ( u32 i = 0; i < totface; ++i )
    {
        MFace* face = &faces[i];
        if ( face->v4 != 0 )
            actualVertexCount += 6;
        else
            actualVertexCount += 3;
    }
    
    // Import the geometry vertices
    bool has_uvs = (bool)CustomData_has_layer(&me->fdata, CD_MTFACE);
    Renderer::Vertex* vertexArray = (Renderer::Vertex *)malloc ( sizeof(Renderer::Vertex) * actualVertexCount );
    u32 curVertex = 0;
    if ( has_uvs )
    {
        MTFace *tface = (MTFace*)CustomData_get_layer_n(&me->fdata, CD_MTFACE, 0);
        
        for ( u32 i = 0; i < totface; ++i )
        {
            MFace* face = &faces[i];
            ImportVertex ( &vertexArray [ curVertex++ ], &verts[ face->v1 ], tface[i].uv[0] );
            ImportVertex ( &vertexArray [ curVertex++ ], &verts[ face->v2 ], tface[i].uv[1] );
            ImportVertex ( &vertexArray [ curVertex++ ], &verts[ face->v3 ], tface[i].uv[2] );

            if ( face->v4 != 0 )
            {
                ImportVertex ( &vertexArray [ curVertex++ ], &verts[ face->v1 ], tface[i].uv[0] );
                ImportVertex ( &vertexArray [ curVertex++ ], &verts[ face->v3 ], tface[i].uv[2] );
                ImportVertex ( &vertexArray [ curVertex++ ], &verts[ face->v4 ], tface[i].uv[3] );
            }
        }
    }
    else
    {
        for ( u32 i = 0; i < totface; ++i )
        {
            MFace* face = &faces[i];
            ImportVertex ( &vertexArray [ curVertex++ ], &verts[ face->v1 ], 0 );
            ImportVertex ( &vertexArray [ curVertex++ ], &verts[ face->v2 ], 0 );
            ImportVertex ( &vertexArray [ curVertex++ ], &verts[ face->v3 ], 0 );

            if ( face->v4 != 0 )
            {
                ImportVertex ( &vertexArray [ curVertex++ ], &verts[ face->v1 ], 0 );
                ImportVertex ( &vertexArray [ curVertex++ ], &verts[ face->v3 ], 0 );
                ImportVertex ( &vertexArray [ curVertex++ ], &verts[ face->v4 ], 0 );
            }
        }
    }
    g->Set( vertexArray, actualVertexCount );
    
    // Load every mesh in this geometry
    if ( !totcol )
        return ImportMesh ( g, g->name(), 0, ob, model );
    else
    {
        for ( u32 i = 0; i < me->totcol; ++i )
        {
            char name [ 512 ];
            snprintf ( name, sizeof(name), "%s-%u", g->name().c_str(), i );
            if ( !ImportMesh ( g, name, i, ob, model ) )
                return false;
        }
        return true;
    }
}

static bool ImportSceneObject ( Object* ob, ::Scene* sce, l3m::Scene* modelScene )
{
    switch ( ob->type )
    {
        case OB_MESH:
        {
            l3m::Scene::Node& node = modelScene->CreateGeometryNode();
            node.url = get_geometry_id(ob);
            node.transform = get_node_transform_ob(ob);
            
            break;
        }
    }
    
    return true;
}

static bool ImportScene ( ::Scene* sce, l3m::Scene* modelScene )
{
    Base *base= (Base*) sce->base.first;
    while(base) {
            Object *ob = base->object;

            switch(ob->type) {
                    case OB_MESH:
                    case OB_CAMERA:
                    case OB_LAMP:
                    case OB_ARMATURE:
                    case OB_EMPTY:
                            if ( ImportSceneObject(ob, sce, modelScene) == false )
                                return false;
                            break;
            }

            base= base->next;
    }
    
    return true;
}

static bool import_blender ( ::Scene* sce, l3m::Model* model )
{
    // Import all geometries
    std::set<std::string> exportedGeometry;
    for ( Base* base = (Base *)sce->base.first; base != 0; base = base->next )
    {
        Object* ob = base->object;
        if ( ob->type == OB_MESH && ob->data )
        {
            std::string geom_id = get_geometry_id(ob);
            if (exportedGeometry.find(geom_id) == exportedGeometry.end())
            {
                exportedGeometry.insert(geom_id);
                l3m::Geometry* g = model->CreateComponent<l3m::Geometry>("geometry");
                g->geometry().name() = geom_id;

                if ( ! ImportGeometry ( &(g->geometry()), ob, model ) )
                {
                    fprintf ( stderr, "Error exporting a geometry\n" );
                    return false;
                }
            }
        }
    }
    
    // Import the visual scene
    l3m::Scene* modelScene = model->CreateComponent<l3m::Scene>("scene");
    ImportScene ( sce, modelScene );

    return true;
}

bool import_blender ( int argc, const char** argv, const char* file, l3m::Model* model )
{
    startup_blender(argc, argv);
    BlendFileData* data = BLO_read_from_file(file, NULL);
    if ( data == 0 )
        return false;
    
    return import_blender ( data->curscene, model );
}

bool import_blender ( int argc, const char** argv, std::istream& is, l3m::Model* model )
{
    startup_blender(argc, argv);

    u32 memSize = 4096;
    char* mem = new char [ memSize ];
    
    char temp [ 512 ];
    u32 totalSize = 0;
    u32 currentSize;
    
    while ( !is.eof () )
    {
        is.read ( temp, sizeof(temp) );
        currentSize = is.gcount();

        if ( currentSize > 0 )
        {
            if ( totalSize + currentSize > memSize )
            {
                memSize *= 2;
                char* newMem = new char [ memSize ];
                memcpy ( newMem, mem, totalSize );
                delete [] mem;
                mem = newMem;
            }
            memcpy ( &mem[totalSize], temp, currentSize );
            totalSize += currentSize;
        }
    }
    
    BlendFileData* data = BLO_read_from_memory(mem, totalSize, NULL);
    if ( data == 0 )
    {
        delete [] mem;
        return false;
    }
    
    bool ret = import_blender ( data->curscene, model );
    delete [] mem;
    return ret;
}
