/*
 * $Id: creator.c 38290 2011-07-10 18:54:02Z campbellbarton $
 *
 * ***** BEGIN GPL LICENSE BLOCK *****
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * The Original Code is Copyright (C) 2001-2002 by NaN Holding BV.
 * All rights reserved.
 *
 * The Original Code is: all of this file.
 *
 * Contributor(s): none yet.
 *
 * ***** END GPL LICENSE BLOCK *****
 */
extern "C" {
/** \file creator/creator.c
 *  \ingroup creator
 */
/* copied from BLI_utildefines.h */
#define STRINGIFY_ARG(x) #x
#define STRINGIFY(x) STRINGIFY_ARG(x)



#define BUILD_REV 38518
#define BUILD_DATE 2011-07-20
#define BUILD_TIME 19:33:14

char build_date[]= STRINGIFY(BUILD_DATE);
char build_time[]= STRINGIFY(BUILD_TIME);
char build_rev[]= STRINGIFY(BUILD_REV);

char build_platform[]= STRINGIFY(BUILD_PLATFORM);
char build_type[]= STRINGIFY(BUILD_TYPE);

char build_cflags[]= "unmaintained buildsystem alert!";
char build_cxxflags[]= "unmaintained buildsystem alert!";
char build_linkflags[]= "unmaintained buildsystem alert!";
char build_system[]= "unmaintained buildsystem alert!";



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

#include "IMB_imbuf.h"	// for IMB_init

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


/* for the callbacks: */

extern int pluginapi_force_ref(void);  /* from blenpluginapi:pluginapi.c */

char bprogname[FILE_MAX]; /* from blenpluginapi:pluginapi.c */
char btempdir[FILE_MAX];

#define BLEND_VERSION_STRING_FMT "Blender %d.%02d (sub %d)\n", BLENDER_VERSION/100, BLENDER_VERSION%100, BLENDER_SUBVERSION

/* set breakpoints here when running in debug mode, useful to catch floating point errors */
#if defined(__sgi) || defined(__linux__) || defined(_WIN32) || defined(OSX_SSE_FPE)
static void fpe_handler(int UNUSED(sig))
{
	// printf("SIGFPE trapped\n");
}
#endif

#ifndef WITH_PYTHON_MODULE
/* handling ctrl-c event in console */
static void blender_esc(int sig)
{
	static int count = 0;
	
	G.afbreek = 1;	/* forces render loop to read queue, not sure if its needed */
	
	if (sig == 2) {
		if (count) {
			printf("\nBlender killed\n");
			exit(2);
		}
		printf("\nSent an internal break event. Press ^C again to kill Blender\n");
		count++;
	}
}
#endif

/* buildinfo can have quotes */
#ifdef BUILD_DATE
static void strip_quotes(char *str)
{
	if(str[0] == '"') {
		int len= strlen(str) - 1;
		memmove(str, str+1, len);
		if(str[len-1] == '"') {
			str[len-1]= '\0';
		}
	}
}
#endif

double PIL_check_seconds_timer(void);

static int end_arguments(int UNUSED(argc), const char **UNUSED(argv), void *UNUSED(data))
{
	return -1;
}

static int enable_python(int UNUSED(argc), const char **UNUSED(argv), void *UNUSED(data))
{
	G.f |= G_SCRIPT_AUTOEXEC;
	G.f |= G_SCRIPT_OVERRIDE_PREF;
	return 0;
}

static int disable_python(int UNUSED(argc), const char **UNUSED(argv), void *UNUSED(data))
{
	G.f &= ~G_SCRIPT_AUTOEXEC;
	G.f |= G_SCRIPT_OVERRIDE_PREF;
	return 0;
}

static int background_mode(int UNUSED(argc), const char **UNUSED(argv), void *UNUSED(data))
{
	G.background = 1;
	return 0;
}

static int debug_mode(int UNUSED(argc), const char **UNUSED(argv), void *data)
{
	G.f |= G_DEBUG;		/* std output printf's */
	printf(BLEND_VERSION_STRING_FMT);
	MEM_set_memory_debug();

#ifdef NAN_BUILDINFO
	printf("Build: %s %s %s %s\n", build_date, build_time, build_platform, build_type);
#endif // NAN_BUILDINFO

	BLI_argsPrint((bArgs*)data);
	return 0;
}

static int set_fpe(int UNUSED(argc), const char **UNUSED(argv), void *UNUSED(data))
{
#if defined(__sgi) || defined(__linux__) || defined(_WIN32) || defined(OSX_SSE_FPE)
	/* zealous but makes float issues a heck of a lot easier to find!
	 * set breakpoints on fpe_handler */
	signal(SIGFPE, fpe_handler);

# if defined(__linux__) && defined(__GNUC__)
	feenableexcept(FE_DIVBYZERO | FE_INVALID | FE_OVERFLOW );
# endif	/* defined(__linux__) && defined(__GNUC__) */
# if defined(OSX_SSE_FPE)
	/* OSX uses SSE for floating point by default, so here 
	 * use SSE instructions to throw floating point exceptions */
	_MM_SET_EXCEPTION_MASK(_MM_MASK_MASK &~
			(_MM_MASK_OVERFLOW|_MM_MASK_INVALID|_MM_MASK_DIV_ZERO));
# endif	/* OSX_SSE_FPE */
# if defined(_WIN32) && defined(_MSC_VER)
	_controlfp_s(NULL, 0, _MCW_EM); /* enables all fp exceptions */
	_controlfp_s(NULL, _EM_DENORMAL | _EM_UNDERFLOW | _EM_INEXACT, _MCW_EM); /* hide the ones we don't care about */
# endif /* _WIN32 && _MSC_VER */
#endif

	return 0;
}

static int set_factory_startup(int UNUSED(argc), const char **UNUSED(argv), void *UNUSED(data))
{
	G.factory_startup= 1;
	return 0;
}

static int set_env(int argc, const char **argv, void *UNUSED(data))
{
	/* "--env-system-scripts" --> "BLENDER_SYSTEM_SCRIPTS" */

	char env[64]= "BLENDER";
	char *ch_dst= env + 7; /* skip BLENDER */
	const char *ch_src= argv[0] + 5; /* skip --env */

	if (argc < 2) {
		printf("%s requires one argument\n", argv[0]);
		exit(1);
	}

	for(; *ch_src; ch_src++, ch_dst++) {
		*ch_dst= (*ch_src == '-') ? '_' : (*ch_src)-32; /* toupper() */
	}

	*ch_dst= '\0';
	BLI_setenv(env, argv[1]);
	return 1;
}

static int playback_mode(int UNUSED(argc), const char **UNUSED(argv), void *UNUSED(data))
{
	/* not if -b was given first */
	if (G.background == 0) {

// XXX				playanim(argc, argv); /* not the same argc and argv as before */
		exit(0);
	}

	return -2;
}

static int prefsize(int argc, const char **argv, void *UNUSED(data))
{
	int stax, stay, sizx, sizy;

	if (argc < 5) {
		printf ("-p requires four arguments\n");
		exit(1);
	}

	stax= atoi(argv[1]);
	stay= atoi(argv[2]);
	sizx= atoi(argv[3]);
	sizy= atoi(argv[4]);

	WM_setprefsize(stax, stay, sizx, sizy);

	return 4;
}

static int with_borders(int UNUSED(argc), const char **UNUSED(argv), void *UNUSED(data))
{
	WM_setinitialstate_normal();
	return 0;
}

static int without_borders(int UNUSED(argc), const char **UNUSED(argv), void *UNUSED(data))
{
	WM_setinitialstate_fullscreen();
	return 0;
}

extern int wm_start_with_console; // blender/windowmanager/intern/wm_init_exit.c
static int start_with_console(int UNUSED(argc), const char **UNUSED(argv), void *UNUSED(data))
{
	wm_start_with_console = 1;
	return 0;
}

static int register_extension(int UNUSED(argc), const char **UNUSED(argv), void *data)
{
#ifdef WIN32
	if (data)
		G.background = 1;
	RegisterBlendExtension();
#else
	(void)data; /* unused */
#endif
	return 0;
}

static int no_joystick(int UNUSED(argc), const char **UNUSED(argv), void *data)
{
#ifndef WITH_GAMEENGINE
	(void)data;
#else
	SYS_SystemHandle *syshandle = data;

	/**
	 	don't initialize joysticks if user doesn't want to use joysticks
		failed joystick initialization delays over 5 seconds, before game engine start
	*/
	SYS_WriteCommandLineInt(*syshandle, "nojoystick",1);
	if (G.f & G_DEBUG) printf("disabling nojoystick\n");
#endif

	return 0;
}

static int no_glsl(int UNUSED(argc), const char **UNUSED(argv), void *UNUSED(data))
{
	GPU_extensions_disable();
	return 0;
}

static int no_audio(int UNUSED(argc), const char **UNUSED(argv), void *UNUSED(data))
{
	sound_force_device(0);
	return 0;
}

static int set_audio(int argc, const char **argv, void *UNUSED(data))
{
	if (argc < 1) {
		printf("-setaudio require one argument\n");
		exit(1);
	}

	sound_force_device(sound_define_from_str(argv[1]));
	return 1;
}

static int set_output(int argc, const char **argv, void *data)
{
	bContext *C = (bContext*)data;
	if (argc >= 1){
		if (CTX_data_scene(C)) {
			Scene *scene= CTX_data_scene(C);
			BLI_strncpy(scene->r.pic, argv[1], sizeof(scene->r.pic));
		} else {
			printf("\nError: no blend loaded. cannot use '-o / --render-output'.\n");
		}
		return 1;
	} else {
		printf("\nError: you must specify a path after '-o  / --render-output'.\n");
		return 0;
	}
}

static int set_engine(int argc, const char **argv, void *data)
{
	bContext *C = (bContext *)data;
	if (argc >= 1)
	{
		if (!strcmp(argv[1],"help"))
		{
			RenderEngineType *type = NULL;

			for( type = (RenderEngineType*)R_engines.first; type; type = (RenderEngineType*)type->next )
			{
				printf("\t%s\n", type->idname);
			}
			exit(0);
		}
		else
		{
			if (CTX_data_scene(C)==NULL)
			{
				printf("\nError: no blend loaded. order the arguments so '-E  / --engine ' is after a blend is loaded.\n");
			}
			else {
				Scene *scene= CTX_data_scene(C);
				RenderData *rd = &scene->r;

				if(BLI_findstring(&R_engines, argv[1], offsetof(RenderEngineType, idname))) {
					BLI_strncpy(rd->engine, argv[1], sizeof(rd->engine));
				}
			}
		}

		return 1;
	}
	else
	{
		printf("\nEngine not specified.\n");
		return 0;
	}
}

static int set_image_type(int argc, const char **argv, void *data)
{
	bContext *C = (bContext *)data;
	if (argc >= 1){
		const char *imtype = argv[1];
		if (CTX_data_scene(C)==NULL) {
			printf("\nError: no blend loaded. order the arguments so '-F  / --render-format' is after the blend is loaded.\n");
		} else {
			Scene *scene= CTX_data_scene(C);
			if      (!strcmp(imtype,"TGA")) scene->r.imtype = R_TARGA;
			else if (!strcmp(imtype,"IRIS")) scene->r.imtype = R_IRIS;
#ifdef WITH_DDS
			else if (!strcmp(imtype,"DDS")) scene->r.imtype = R_DDS;
#endif
			else if (!strcmp(imtype,"JPEG")) scene->r.imtype = R_JPEG90;
			else if (!strcmp(imtype,"IRIZ")) scene->r.imtype = R_IRIZ;
			else if (!strcmp(imtype,"RAWTGA")) scene->r.imtype = R_RAWTGA;
			else if (!strcmp(imtype,"AVIRAW")) scene->r.imtype = R_AVIRAW;
			else if (!strcmp(imtype,"AVIJPEG")) scene->r.imtype = R_AVIJPEG;
			else if (!strcmp(imtype,"PNG")) scene->r.imtype = R_PNG;
			else if (!strcmp(imtype,"AVICODEC")) scene->r.imtype = R_AVICODEC;
			else if (!strcmp(imtype,"QUICKTIME")) scene->r.imtype = R_QUICKTIME;
			else if (!strcmp(imtype,"BMP")) scene->r.imtype = R_BMP;
#ifdef WITH_HDR
			else if (!strcmp(imtype,"HDR")) scene->r.imtype = R_RADHDR;
#endif
#ifdef WITH_TIFF
			else if (!strcmp(imtype,"TIFF")) scene->r.imtype = R_TIFF;
#endif
#ifdef WITH_OPENEXR
			else if (!strcmp(imtype,"EXR")) scene->r.imtype = R_OPENEXR;
			else if (!strcmp(imtype,"MULTILAYER")) scene->r.imtype = R_MULTILAYER;
#endif
			else if (!strcmp(imtype,"MPEG")) scene->r.imtype = R_FFMPEG;
			else if (!strcmp(imtype,"FRAMESERVER")) scene->r.imtype = R_FRAMESERVER;
#ifdef WITH_CINEON
			else if (!strcmp(imtype,"CINEON")) scene->r.imtype = R_CINEON;
			else if (!strcmp(imtype,"DPX")) scene->r.imtype = R_DPX;
#endif
#ifdef WITH_OPENJPEG
			else if (!strcmp(imtype,"JP2")) scene->r.imtype = R_JP2;
#endif
			else printf("\nError: Format from '-F / --render-format' not known or not compiled in this release.\n");
		}
		return 1;
	} else {
		printf("\nError: you must specify a format after '-F  / --render-foramt'.\n");
		return 0;
	}
}

static int set_threads(int argc, const char **argv, void *UNUSED(data))
{
	if (argc >= 1) {
		if(G.background) {
			RE_set_max_threads(atoi(argv[1]));
		} else {
			printf("Warning: threads can only be set in background mode\n");
		}
		return 1;
	} else {
		printf("\nError: you must specify a number of threads between 0 and 8 '-t  / --threads'.\n");
		return 0;
	}
}

static int set_extension(int argc, const char **argv, void *data)
{
	bContext *C =(bContext*) data;
	if (argc >= 1) {
		if (CTX_data_scene(C)) {
			Scene *scene= CTX_data_scene(C);
			if (argv[1][0] == '0') {
				scene->r.scemode &= ~R_EXTENSION;
			} else if (argv[1][0] == '1') {
				scene->r.scemode |= R_EXTENSION;
			} else {
				printf("\nError: Use '-x 1 / -x 0' To set the extension option or '--use-extension'\n");
			}
		} else {
			printf("\nError: no blend loaded. order the arguments so '-o ' is after '-x '.\n");
		}
		return 1;
	} else {
		printf("\nError: you must specify a path after '- '.\n");
		return 0;
	}
}

static int set_ge_parameters(int argc, const char **argv, void *data)
{
	int a = 0;
#ifdef WITH_GAMEENGINE
	SYS_SystemHandle syshandle = *(SYS_SystemHandle*)data;
#else
	(void)data;
#endif

/**
gameengine parameters are automaticly put into system
-g [paramname = value]
-g [boolparamname]
example:
-g novertexarrays
-g maxvertexarraysize = 512
*/

	if(argc >= 1)
	{
		const char *paramname = argv[a];
		/* check for single value versus assignment */
		if (a+1 < argc && (*(argv[a+1]) == '='))
		{
			a++;
			if (a+1 < argc)
			{
				a++;
				/* assignment */
#ifdef WITH_GAMEENGINE
				SYS_WriteCommandLineString(syshandle,paramname,argv[a]);
#endif
			}  else
			{
				printf("error: argument assignment (%s) without value.\n",paramname);
				return 0;
			}
			/* name arg eaten */

		} else {
#ifdef WITH_GAMEENGINE
			SYS_WriteCommandLineInt(syshandle,argv[a],1);
#endif
			/* doMipMap */
			if (!strcmp(argv[a],"nomipmap"))
			{
				GPU_set_mipmap(0); //doMipMap = 0;
			}
			/* linearMipMap */
			if (!strcmp(argv[a],"linearmipmap"))
			{
				GPU_set_linear_mipmap(1); //linearMipMap = 1;
			}


		} /* if (*(argv[a+1]) == '=') */
	}

	return a;
}

static int render_frame(int argc, const char **argv, void *data)
{
	bContext *C = (bContext *)data;
	if (CTX_data_scene(C)) {
		Main *bmain= CTX_data_main(C);
		Scene *scene= CTX_data_scene(C);

		if (argc > 1) {
			Render *re = RE_NewRender(scene->id.name);
			int frame;
			ReportList reports;

			switch(*argv[1]) {
			case '+':
				frame= scene->r.sfra + atoi(argv[1]+1);
				break;
			case '-':
				frame= (scene->r.efra - atoi(argv[1]+1)) + 1;
				break;
			default:
				frame= atoi(argv[1]);
				break;
			}

			BKE_reports_init(&reports, RPT_PRINT);

			frame = MIN2(MAXFRAME, MAX2(MINAFRAME, frame));

			RE_SetReports(re, &reports);
			RE_BlenderAnim(re, bmain, scene, NULL, scene->lay, frame, frame, scene->r.frame_step);
			RE_SetReports(re, NULL);
			return 1;
		} else {
			printf("\nError: frame number must follow '-f / --render-frame'.\n");
			return 0;
		}
	} else {
		printf("\nError: no blend loaded. cannot use '-f / --render-frame'.\n");
		return 0;
	}
}

static int render_animation(int UNUSED(argc), const char **UNUSED(argv), void *data)
{
	bContext *C = (bContext *)data;
	if (CTX_data_scene(C)) {
		Main *bmain= CTX_data_main(C);
		Scene *scene= CTX_data_scene(C);
		Render *re= RE_NewRender(scene->id.name);
		ReportList reports;
		BKE_reports_init(&reports, RPT_PRINT);
		RE_SetReports(re, &reports);
		RE_BlenderAnim(re, bmain, scene, NULL, scene->lay, scene->r.sfra, scene->r.efra, scene->r.frame_step);
		RE_SetReports(re, NULL);
	} else {
		printf("\nError: no blend loaded. cannot use '-a'.\n");
	}
	return 0;
}

static int set_scene(int argc, const char **argv, void *data)
{
	if(argc > 1) {
		bContext *C= (bContext *)data;
		Scene *sce= set_scene_name(CTX_data_main(C), argv[1]);
		if(sce) {
			CTX_data_scene_set(C, sce);
		}
		return 1;
	} else {
		printf("\nError: Scene name must follow '-S / --scene'.\n");
		return 0;
	}
}

static int set_start_frame(int argc, const char **argv, void *data)
{
	bContext *C = (bContext *)data;
	if (CTX_data_scene(C)) {
		Scene *scene= CTX_data_scene(C);
		if (argc > 1) {
			int frame = atoi(argv[1]);
			(scene->r.sfra) = CLAMPIS(frame, MINFRAME, MAXFRAME);
			return 1;
		} else {
			printf("\nError: frame number must follow '-s / --frame-start'.\n");
			return 0;
		}
	} else {
		printf("\nError: no blend loaded. cannot use '-s / --frame-start'.\n");
		return 0;
	}
}

static int set_end_frame(int argc, const char **argv, void *data)
{
	bContext *C = (bContext *)data;
	if (CTX_data_scene(C)) {
		Scene *scene= CTX_data_scene(C);
		if (argc > 1) {
			int frame = atoi(argv[1]);
			(scene->r.efra) = CLAMPIS(frame, MINFRAME, MAXFRAME);
			return 1;
		} else {
			printf("\nError: frame number must follow '-e / --frame-end'.\n");
			return 0;
		}
	} else {
		printf("\nError: no blend loaded. cannot use '-e / --frame-end'.\n");
		return 0;
	}
}

static int set_skip_frame(int argc, const char **argv, void *data)
{
	bContext *C = (bContext*)data;
	if (CTX_data_scene(C)) {
		Scene *scene= CTX_data_scene(C);
		if (argc > 1) {
			int frame = atoi(argv[1]);
			(scene->r.frame_step) = CLAMPIS(frame, 1, MAXFRAME);
			return 1;
		} else {
			printf("\nError: number of frames to step must follow '-j / --frame-jump'.\n");
			return 0;
		}
	} else {
		printf("\nError: no blend loaded. cannot use '-j / --frame-jump'.\n");
		return 0;
	}
}

/* macro for ugly context setup/reset */
#ifdef WITH_PYTHON
#define BPY_CTX_SETUP(_cmd) \
{ \
	wmWindowManager *wm= CTX_wm_manager(C); \
	wmWindow *prevwin= CTX_wm_window(C); \
	Scene *prevscene= CTX_data_scene(C); \
	if(wm->windows.first) { \
		CTX_wm_window_set(C, wm->windows.first); \
		_cmd; \
		CTX_wm_window_set(C, prevwin); \
	} \
	else { \
		fprintf(stderr, "Python script \"%s\" running with missing context data.\n", argv[1]); \
		_cmd; \
	} \
	CTX_data_scene_set(C, prevscene); \
} \

#endif /* WITH_PYTHON */

static int run_python(int argc, const char **argv, void *data)
{
#ifdef WITH_PYTHON
	bContext *C = data;

	/* workaround for scripts not getting a bpy.context.scene, causes internal errors elsewhere */
	if (argc > 1) {
		/* Make the path absolute because its needed for relative linked blends to be found */
		char filename[FILE_MAXDIR + FILE_MAXFILE];
		BLI_strncpy(filename, argv[1], sizeof(filename));
		BLI_path_cwd(filename);

		BPY_CTX_SETUP(BPY_filepath_exec(C, filename, NULL))

		return 1;
	} else {
		printf("\nError: you must specify a Python script after '-P / --python'.\n");
		return 0;
	}
#else
	(void)argc; (void)argv; (void)data; /* unused */
	printf("This blender was built without python support\n");
	return 0;
#endif /* WITH_PYTHON */
}

static int run_python_console(int UNUSED(argc), const char **argv, void *data)
{
#ifdef WITH_PYTHON
	bContext *C = data;

	BPY_CTX_SETUP(BPY_string_exec(C, "__import__('code').interact()"))

	return 0;
#else
	(void)argv; (void)data; /* unused */
	printf("This blender was built without python support\n");
	return 0;
#endif /* WITH_PYTHON */
}

static int set_addons(int argc, const char **argv, void *data)
{
	/* workaround for scripts not getting a bpy.context.scene, causes internal errors elsewhere */
	if (argc > 1) {
#ifdef WITH_PYTHON
		const int slen= strlen(argv[1]) + 128;
		char *str= malloc(slen);
		bContext *C= data;
		BLI_snprintf(str, slen, "[__import__('addon_utils').enable(i, default_set=False) for i in '%s'.split(',')]", argv[1]);
		BPY_CTX_SETUP(BPY_string_exec(C, str));
		free(str);
#else
		(void)argv; (void)data; /* unused */
#endif /* WITH_PYTHON */
		return 1;
	}
	else {
		printf("\nError: you must specify a comma separated list after '--addons'.\n");
		return 0;
	}
}

} // extern "C"

void startup_blender (int argc, const char** argv)
{
	SYS_SystemHandle syshandle;
	bArgs *ba;

	BLI_where_am_i(bprogname, sizeof(bprogname), argv[0]);
	BLI_threadapi_init();

	RNA_init();
	RE_engines_init();

	pluginapi_force_ref();

	init_nodesystem();
	
	initglobals();	/* blender.c */

	IMB_init();

	BLI_cb_init();

	syshandle= 0;
}
