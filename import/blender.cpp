/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        blender.cpp
// PURPOSE:     Import models from .blend files.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//
/* copied from BLI_utildefines.h */
#define STRINGIFY_ARG(x) #x
#define STRINGIFY(x) STRINGIFY_ARG(x)


#if defined(__linux__) && defined(__GNUC__)
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <fenv.h>
#endif

#if (defined(__APPLE__) && (defined(__i386__) || defined(__x86_64__)))
#define OSX_SSE_FPE
#include <xmmintrin.h>
#endif

#include <stdlib.h>
#include <stddef.h>
#include <string.h>

/* for setuid / getuid */
#ifdef __sgi
#include <sys/types.h>
#include <unistd.h>
#endif

/* This little block needed for linking to Blender... */

#include "MEM_guardedalloc.h"

#ifdef WIN32
#include "BLI_winstuff.h"
#endif

#include "BLI_args.h"
#include "BLI_threads.h"
#include "BLI_scanfill.h" // for BLI_setErrorCallBack, TODO, move elsewhere
#include "BLI_utildefines.h"
#include "BLI_callbacks.h"

#include "DNA_ID.h"
#include "DNA_scene_types.h"

#include "BLI_blenlib.h"

#include "BKE_utildefines.h"
#include "BKE_blender.h"
#include "BKE_context.h"
#include "BKE_depsgraph.h" // for DAG_on_visible_update
#include "BKE_font.h"
#include "BKE_global.h"
#include "BKE_main.h"
#include "BKE_material.h"
#include "BKE_packedFile.h"
#include "BKE_scene.h"
#include "BKE_node.h"
#include "BKE_report.h"
#include "BKE_sound.h"

extern "C" {
#include "IMB_imbuf.h"	// for IMB_init
#include "IMB_imbuf_types.h"
}

#ifdef WITH_PYTHON
#include "BPY_extern.h"
#endif

#include "RE_pipeline.h"

//XXX #include "playanim_ext.h"
#include "ED_datafiles.h"

#include "WM_api.h"

#include "RNA_define.h"

#include "GPU_draw.h"
#include "GPU_extensions.h"

#ifdef WITH_BUILDINFO_HEADER
#define BUILD_DATE
#endif

/* for passing information between creator and gameengine */
#ifdef WITH_GAMEENGINE
#include "BL_System.h"
#else /* dummy */
#define SYS_SystemHandle int
#endif

#include <signal.h>

#ifdef __FreeBSD__
# include <sys/types.h>
# include <floatingpoint.h>
# include <sys/rtprio.h>
#endif

#ifdef WITH_BINRELOC
#include "binreloc.h"
#endif

#include <algorithm>
#include <set>
#include <string>
#include <sstream>
#include "BLO_readfile.h"
#include "DNA_armature_types.h"
#include "DNA_camera_types.h"
#include "DNA_object_types.h"
#include "DNA_material_types.h"
#include "DNA_mesh_types.h"
#include "DNA_meshdata_types.h"
#include "DNA_scene_types.h"
#include "DNA_packedFile_types.h"
#include "BKE_customdata.h"
#include "BKE_object.h"
#include "BLI_math.h"
#include "shared/platform.h"
#include "math/vector.h"
#include "math/transform.h"
#include "math/util.h"
#include "l3m/l3m.h"
#include "l3m/Components/components.h"
#include "renderer/mesh.h"

extern void startup_blender (int argc, const char** argv);

static std::map<std::string, l3m::Material*> gMaterials;

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
	return translate_id(id_name(ob->data)) + "";
}

std::string get_light_id(Object *ob)
{
	return translate_id(id_name(ob)) + "";
}

std::string get_joint_id(Bone *bone, Object *ob_arm)
{
	return translate_id(/*id_name(ob_arm) + "_" +*/ bone->name);
}

std::string get_camera_id(Object *ob)
{
	return translate_id(id_name(ob)) + "";
}

std::string get_material_id(Material *mat)
{
	return translate_id(id_name(mat)) + "";
}

static Transform get_node_transform_ob(Object *ob, Matrix3* scaling)
{
    Transform transform;

    transform.translation() = Vector3 ( ob->obmat[3][0], ob->obmat[3][1], ob->obmat[3][2] );
    Matrix3 basis;
    for ( u8 i = 0; i < 3; ++i )
         for ( u8 j = 0; j < 3; ++j )
             basis.vector()[i*3+j] = ob->obmat[i][j];
    Matrix3 Q, R;
    Matrix3::QRDecompose ( basis, &Q, &R );
    transform.orientation() = Q;
    *scaling = R;

    return transform;
}




static void ImportVertex ( Renderer::Vertex* to, MVert* from )
{
    to->pos() = from->co;
    to->norm() = Vector3 ( from->no[0] / 32767.0f, from->no[1] / 32767.0f, from->no[2] / 32767.0f );
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
    // Import the material
    if ( me->mat != 0 && me->mat[mat_index] != 0 )
    {
        ::Material* ma = me->mat[mat_index];
        std::string material_id = get_material_id(ma);
        std::map<std::string, l3m::Material*>::iterator iter = gMaterials.find(material_id);
        if ( iter != gMaterials.end() )
        {
            mesh->material() = &iter->second->material();
        }
    }
    g->LoadMesh( mesh );
    
    return true;
}

static bool ImportGeometry ( Renderer::Geometry* g, Object* ob, l3m::Model* model )
{
    Mesh* me = (Mesh *)ob->data;
    MFace *faces = me->mface;
    MVert* verts = me->mvert;
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
    Renderer::Vertex* vertexArray = (Renderer::Vertex *)malloc ( sizeof(Renderer::Vertex) * actualVertexCount );
    memset ( vertexArray, 0, sizeof(Renderer::Vertex) * actualVertexCount );
    u32 curVertex = 0;
    
    for ( u32 i = 0; i < totface; ++i )
    {
        MFace* face = &faces[i];
        ImportVertex ( &vertexArray [ curVertex++ ], &verts[ face->v1 ] );
        ImportVertex ( &vertexArray [ curVertex++ ], &verts[ face->v2 ] );
        ImportVertex ( &vertexArray [ curVertex++ ], &verts[ face->v3 ] );

        if ( face->v4 != 0 )
        {
            ImportVertex ( &vertexArray [ curVertex++ ], &verts[ face->v1 ] );
            ImportVertex ( &vertexArray [ curVertex++ ], &verts[ face->v3 ] );
            ImportVertex ( &vertexArray [ curVertex++ ], &verts[ face->v4 ] );
        }
    }
    g->Set( vertexArray, actualVertexCount );
    
    // Import the geometry UV texture coordinates
    bool has_uvs = (bool)CustomData_has_layer(&me->fdata, CD_MTFACE);
    if ( has_uvs )
    {
        curVertex = 0;
        int layerCount = CustomData_number_of_layers(&me->fdata, CD_MTFACE);
        Vector2* uvData = (Vector2*)malloc ( sizeof(Vector2) * layerCount * actualVertexCount );
        
        for ( u32 l = 0; l < layerCount; ++l )
        {
            MTFace *tface = (MTFace*)CustomData_get_layer_n(&me->fdata, CD_MTFACE, l);
            
            for ( u32 i = 0; i < totface; ++i )
            {
                MFace* face = &faces[i];
                
                uvData [ curVertex++ ] = Vector2 ( tface[i].uv[0] );
                uvData [ curVertex++ ] = Vector2 ( tface[i].uv[1] );
                uvData [ curVertex++ ] = Vector2 ( tface[i].uv[2] );

                if ( face->v4 != 0 )
                {
                    uvData [ curVertex++ ] = Vector2 ( tface[i].uv[0] );
                    uvData [ curVertex++ ] = Vector2 ( tface[i].uv[2] );
                    uvData [ curVertex++ ] = Vector2 ( tface[i].uv[3] );
                }
            }
        }
        
        g->CreateVertexLayer( "uv", layerCount, uvData, sizeof(Vector2) );
        free ( uvData );
    }
    
    // Import the vertex colors
    bool has_color = (bool)CustomData_has_layer(&me->fdata, CD_MCOL);
    if ( has_color )
    {
        Color* colorData = (Color *)malloc( sizeof(Color) * actualVertexCount );
        int index = CustomData_get_active_layer_index(&me->fdata, CD_MCOL);
        curVertex = 0;

        MCol *mcol = (MCol*)me->fdata.layers[index].data;
        
        for ( u32 i = 0; i < totface; ++i, mcol += 4 )
        {
            colorData [ curVertex++ ] = Color ( mcol[0].r, mcol[0].g, mcol[0].b, mcol[0].a );
            colorData [ curVertex++ ] = Color ( mcol[1].r, mcol[1].g, mcol[1].b, mcol[1].a );
            colorData [ curVertex++ ] = Color ( mcol[2].r, mcol[2].g, mcol[2].b, mcol[2].a );
            
            if ( faces[i].v4 != 0 )
            {
                colorData [ curVertex++ ] = Color ( mcol[0].r, mcol[0].g, mcol[0].b, mcol[0].a );
                colorData [ curVertex++ ] = Color ( mcol[2].r, mcol[2].g, mcol[2].b, mcol[2].a );
                colorData [ curVertex++ ] = Color ( mcol[3].r, mcol[3].g, mcol[3].b, mcol[3].a );
            }
        }
        
        g->CreateVertexLayer("color", 1, colorData, sizeof(Color) );
        free ( colorData );
    }
    
    // Load every mesh in this geometry
    if ( !totcol )
        return ImportMesh ( g, g->name(), 0, ob, model );
    else
    {
        for ( u32 i = 0; i < totcol; ++i )
        {
            char name [ 512 ];
            snprintf ( name, sizeof(name), "%s-%u", g->name().c_str(), i );
            if ( !ImportMesh ( g, name, i, ob, model ) )
                return false;
        }
        return true;
    }
}

static bool ImportSceneObject ( l3m::Model* model, Object* ob, ::Scene* sce, l3m::Scene* modelScene )
{
    switch ( ob->type )
    {
        case OB_MESH:
        {
            Mesh* me = (Mesh*)ob->data;
            l3m::Scene::Node& node = modelScene->CreateGeometryNode();
            node.url = get_geometry_id(ob);
            
            // Get the transform
            Matrix3 scale;
            node.transform = get_node_transform_ob(ob, &scale );
            
            // Apply the scale and mirroring
            // Find the geometry associated to this url.
            for ( l3m::Model::componentVector::iterator miter = model->components().begin();
                  miter != model->components().end();
                  ++miter )
            {
                if ( (*miter)->type() == "geometry" && static_cast < l3m::Geometry* > ( *miter )->geometry().name() == node.url )
                {
                    Renderer::Geometry& g = static_cast<l3m::Geometry *>(*miter)->geometry();

                    // Apply the scaling to every vertex.
                    Renderer::Vertex* v = g.vertices();
                    for ( u32 i = 0; i < g.numVertices(); ++i )
                    {
                        Vector3 pos = v[i].pos() * scale;
                        v[i].pos() = pos;
                    }
                }
            }
     
            // Import the set of textures
            bool has_uvs = (bool)CustomData_has_layer(&me->fdata, CD_MTFACE);
            if ( has_uvs )
            {
                int layerCount = CustomData_number_of_layers(&me->fdata, CD_MTFACE);
                for ( u32 l = 0; l < layerCount; ++l )
                {
                    MTFace *tface = (MTFace*)CustomData_get_layer_n(&me->fdata, CD_MTFACE, l);
                    Image* image = tface->tpage;
                    if ( image != 0 )
                        node.textures.push_back(translate_id(id_name(image)));
                }
            }
            
            break;
        }
    }
    
    return true;
}

static bool ImportScene ( l3m::Model* model, ::Scene* sce, l3m::Scene* modelScene )
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
                            if ( ImportSceneObject(model, ob, sce, modelScene) == false )
                                return false;
                            break;
            }

            base= base->next;
    }
    
    // Import the scene camera
    if ( sce->camera != 0 )
        modelScene->camera() = get_camera_id ( sce->camera );
    
    // Import the scene rendering size
    modelScene->width() = sce->r.xsch;
    modelScene->height() = sce->r.ysch;
    
    return true;
}

static bool ImportImages ( ::Scene* sce, const char* filename, l3m::Model* model )
{
    std::vector<std::string> mImages;

    // For each mesh...
    Base *base= (Base*) sce->base.first;
    while(base) {
            Object *ob = base->object;

            switch(ob->type)
            {
                case OB_MESH:
                {
                    // For each mesh uv...
                    Mesh* me = (Mesh *)ob->data;
                    bool has_uvs = (bool)CustomData_has_layer(&me->fdata, CD_MTFACE);
                    if ( has_uvs )
                    {
                        int layerCount = CustomData_number_of_layers(&me->fdata, CD_MTFACE);
                        for ( u32 l = 0; l < layerCount; ++l )
                        {
                            MTFace *tface = (MTFace*)CustomData_get_layer_n(&me->fdata, CD_MTFACE, l);
                            Image* image = tface->tpage;
                            if ( image == 0 )
                                continue;
                            
                            std::string name(id_name(image));
                            name = translate_id(name);
                            if (std::find(mImages.begin(), mImages.end(), name) == mImages.end())
                            {
                                mImages.push_back(name);
                                
                                // The image is packed
                                if (image->packedfile)
                                {
                                    int flag = IB_rect|IB_multilayer;
                                    if(image->flag & IMA_DO_PREMUL) flag |= IB_premul;
                                    ImBuf* ibuf = IMB_ibImageFromMemory((unsigned char*)image->packedfile->data, (size_t)image->packedfile->size, flag);
                                    if (ibuf)
                                    {
                                        Pixmap pix;
                                        pix.Create( ibuf->x, ibuf->y, (Color *)ibuf->rect );
                                        l3m::Texture* tex = model->CreateComponent<l3m::Texture>("texture");
                                        tex->id() = name;
                                        tex->pixmap() = pix;
                                    }
                                }
                                else
                                {
                                    char rel[FILE_MAX];
                                    char abs[FILE_MAX];
                                    char dir[FILE_MAX];

                                    BLI_split_dirfile(filename, dir, NULL);
                                    BKE_rebase_path(abs, sizeof(abs), rel, sizeof(rel), G.main->name, image->name, dir);


                                    Pixmap pix;
                                    if ( !pix.Load(abs) )
                                    {
                                        fprintf ( stderr, "Warning: Cannot open the image: %s\n", abs );
                                        continue;
                                    }
                                    l3m::Texture* tex = model->CreateComponent<l3m::Texture>("texture");
                                    tex->id() = name;
                                    tex->pixmap() = pix;
                                }
                            }
                        }
                    }
                    break;
                }
                default: break;
            }

            base= base->next;
    }
    
    return true;
}

static bool ImportMaterials ( ::Scene* sce, l3m::Model* model )
{
    std::vector < std::string > mMat;
    
    // For each mesh object...
    Base *base= (Base*) sce->base.first;
    while(base)
    {
        Object *ob = base->object;

        switch(ob->type)
        {
            case OB_MESH:
            {
                for ( u32 a = 0; a < ob->totcol; ++a )
                {
                    ::Material *ma = give_current_material(ob, a+1);
                    if (!ma)
                        continue;

                    std::string translated_id = get_material_id(ma);
                    if (std::find(mMat.begin(), mMat.end(), translated_id) == mMat.end())
                    {
                        l3m::Material* comp = model->CreateComponent<l3m::Material>("material");
                        
                        Renderer::Material mat;
                        Color ambient ( ma->ambr * 255.0f, ma->ambg * 255.0f, ma->ambb * 255.0f, 255.0f );
                        Color diffuse ( ma->r * ma->ref * 255.0f, ma->g * ma->ref * 255.0f, ma->b * ma->ref * 255.0f, 255.0f );
                        Color specular ( ma->specr * 255.0f, ma->specg * 255.0f, ma->specb * 255.0f, 255.0f );
                        Color emission ( ma->r * ma->emit * 255.0f, ma->g * ma->emit * 255.0f, ma->b * ma->emit * 255.0f, 255.0f );
                        float shininess = ma->har;
                        
                        mat.name() = translated_id;
                        mat.ambient() = ambient;
                        mat.diffuse() = diffuse;
                        mat.specular() = specular;
                        mat.emission() = emission;
                        mat.shininess() = shininess;
                        mat.isShadeless() = ma->vol.shade_type == MA_VOL_SHADE_SHADELESS;
                        
                        comp->material() = mat;
                        
                        gMaterials[translated_id] = comp;
                        mMat.push_back(translated_id);
                    }
                }
            }
        }
        
        base= base->next;
    }
    
    return true;
}

static bool ImportCamera ( l3m::Camera* cam, ::Object* ob, ::Scene* sce )
{
    // Get the transform
    Transform transform;
    float orientation[9];
    
    // Blender uses Z as up direction, but we use Y as up direction.
    transform.translation() = Vector3 ( ob->obmat[3][0], ob->obmat[3][1], ob->obmat[3][2] );
    
    for ( u32 i = 0; i < 3; ++i )
        for ( u32 j = 0; j < 3; ++j )
            orientation[i*3+j] = ob->obmat[i][j];
    // WARNING: Here we rotate the camera because, for some reason, Blender cameras look forward by looking downwards.
    transform.orientation() = Matrix3(orientation) * RotationMatrix(EulerAngles(0.0f,deg2rad(-90.0f), 0.0f));
    cam->transform() = transform;
    
    // Get the camera type
    ::Camera* c = (::Camera *)ob->data;
    if (!c)
        return false;
    
    // Import the data according to the type
    switch ( c->type )
    {
        case CAM_PERSP:
            cam->type() = l3m::Camera::CAMERA_PERSPECTIVE;
            cam->perspectiveData().aspect = (float)(sce->r.xsch)/(float)(sce->r.ysch);
            cam->perspectiveData().fov = lens_to_angle(c->lens);
            cam->perspectiveData().near = c->clipsta;
            cam->perspectiveData().far = c->clipend;
            break;
        case CAM_ORTHO:
            cam->type() = l3m::Camera::CAMERA_ORTHOGRAPHIC;
            cam->orthographicData().left = -c->ortho_scale * (float)(sce->r.xsch)/(float)(sce->r.ysch);
            cam->orthographicData().right = -cam->orthographicData().left;
            cam->orthographicData().top = c->ortho_scale;
            cam->orthographicData().bottom = -c->ortho_scale;
            cam->orthographicData().near = c->clipsta;
            cam->orthographicData().far = c->clipend;
            break;
        default:
            cam->type() = l3m::Camera::CAMERA_UNKNOWN;
            break;
    }

    
    return true;
}

static bool import_blender ( ::Scene* sce, const char* filename, l3m::Model* model )
{
    // Import the materials
    if ( !ImportMaterials ( sce, model ) )
    {
        fprintf ( stderr, "Error importing the model materials\n" );
        return false;
    }
    
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
    
    // Import the scene cameras
    std::set<std::string> exportedCameras;
    for ( Base* base = (Base *)sce->base.first; base != 0; base = base->next )
    {
        Object* ob = base->object;
        if ( ob->type == OB_CAMERA && ob->data )
        {
            std::string cam_id = get_camera_id(ob);
            if (exportedCameras.find(cam_id) == exportedCameras.end())
            {
                exportedCameras.insert(cam_id);
                l3m::Camera* cam = model->CreateComponent<l3m::Camera>("camera");
                cam->name() = cam_id;

                if ( ! ImportCamera ( cam, ob, sce ) )
                {
                    fprintf ( stderr, "Error importing a camera\n" );
                    return false;
                }
            }
        }
    }
    
    // Import the visual scene
    l3m::Scene* modelScene = model->CreateComponent<l3m::Scene>("scene");
    if ( !ImportScene ( model, sce, modelScene ) )
    {
        fprintf ( stderr, "Error importing the visual scene\n" );
        return false;
    }
    
    // Import the images
    if ( !ImportImages ( sce, filename, model ) )
    {
        fprintf ( stderr, "Error importing the scene images\n" );
        return false;
    }

    return true;
}

bool import_blender ( int argc, const char** argv, const char* file, l3m::Model* model )
{
    startup_blender(argc, argv);
    BlendFileData* data = BLO_read_from_file(file, NULL);
    if ( data == 0 )
    {
        fprintf ( stderr, "Unable to read the blender file.\n" );
        return false;
    }
    
    return import_blender ( data->curscene, file, model );
}

#if 0
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
    
    bool ret = import_blender ( data->curscene, argv[1], model );
    delete [] mem;
    return ret;
}
#endif