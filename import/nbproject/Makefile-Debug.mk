#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/startup_blender.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/blender.o \
	${OBJECTDIR}/post_process.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE -fopenmp -msse2 -msse -pipe -fPIC -funsigned-char -fno-strict-aliasing -Wall -Wno-invalid-offsetof -Wno-sign-compare -Wundef -O2 -DNDEBUG -DNDEBUG -pthread -rdynamic ../libblender/blender-build/lib/libbf_windowmanager.a ../libblender/blender-build/lib/libbf_editor_space_api.a ../libblender/blender-build/lib/libbf_editor_space_action.a ../libblender/blender-build/lib/libbf_editor_space_buttons.a ../libblender/blender-build/lib/libbf_editor_space_console.a ../libblender/blender-build/lib/libbf_editor_space_file.a ../libblender/blender-build/lib/libbf_editor_space_graph.a ../libblender/blender-build/lib/libbf_editor_space_image.a ../libblender/blender-build/lib/libbf_editor_space_info.a ../libblender/blender-build/lib/libbf_editor_space_logic.a ../libblender/blender-build/lib/libbf_editor_space_nla.a ../libblender/blender-build/lib/libbf_editor_space_node.a ../libblender/blender-build/lib/libbf_editor_space_outliner.a ../libblender/blender-build/lib/libbf_editor_space_script.a ../libblender/blender-build/lib/libbf_editor_space_sequencer.a ../libblender/blender-build/lib/libbf_editor_space_sound.a ../libblender/blender-build/lib/libbf_editor_space_time.a ../libblender/blender-build/lib/libbf_editor_space_userpref.a ../libblender/blender-build/lib/libbf_editor_space_view3d.a ../libblender/blender-build/lib/libbf_editor_text.a ../libblender/blender-build/lib/libbf_editor_transform.a ../libblender/blender-build/lib/libbf_editor_util.a ../libblender/blender-build/lib/libbf_editor_uvedit.a ../libblender/blender-build/lib/libbf_editor_curve.a ../libblender/blender-build/lib/libbf_editor_armature.a ../libblender/blender-build/lib/libbf_editor_gpencil.a ../libblender/blender-build/lib/libbf_editor_interface.a ../libblender/blender-build/lib/libbf_editor_mesh.a ../libblender/blender-build/lib/libbf_editor_metaball.a ../libblender/blender-build/lib/libbf_editor_object.a ../libblender/blender-build/lib/libbf_editor_physics.a ../libblender/blender-build/lib/libbf_editor_render.a ../libblender/blender-build/lib/libbf_editor_screen.a ../libblender/blender-build/lib/libbf_editor_sculpt_paint.a ../libblender/blender-build/lib/libbf_editor_sound.a ../libblender/blender-build/lib/libbf_editor_animation.a ../libblender/blender-build/lib/libbf_editor_datafiles.a ../libblender/blender-build/lib/libbf_render.a ../libblender/blender-build/lib/libbf_intern_opennl.a ../libblender/blender-build/lib/libbf_python.a ../libblender/blender-build/lib/libbf_python_ext.a ../libblender/blender-build/lib/libbf_python_mathutils.a ../libblender/blender-build/lib/libbf_ikplugin.a ../libblender/blender-build/lib/libbf_modifiers.a ../libblender/blender-build/lib/libbf_blenkernel.a ../libblender/blender-build/lib/libbf_nodes.a ../libblender/blender-build/lib/libbf_gpu.a ../libblender/blender-build/lib/libbf_blenloader.a ../libblender/blender-build/lib/libbf_blenlib.a ../libblender/blender-build/lib/libbf_intern_ghost.a ../libblender/blender-build/lib/libbf_intern_string.a ../libblender/blender-build/lib/libbf_blenpluginapi.a ../libblender/blender-build/lib/libbf_imbuf.a ../libblender/blender-build/lib/libbf_avi.a ../libblender/blender-build/lib/libbf_imbuf_cineon.a ../libblender/blender-build/lib/libbf_imbuf_openexr.a ../libblender/blender-build/lib/libbf_imbuf_dds.a ../libblender/blender-build/lib/libbf_intern_bsp.a ../libblender/blender-build/lib/libbf_intern_bop.a ../libblender/blender-build/lib/libbf_intern_decimate.a ../libblender/blender-build/lib/libbf_intern_elbeem.a ../libblender/blender-build/lib/libbf_intern_ik.a ../libblender/blender-build/lib/libbf_intern_memutil.a ../libblender/blender-build/lib/libbf_intern_guardedalloc.a ../libblender/blender-build/lib/libbf_intern_ctr.a ../libblender/blender-build/lib/libge_blen_routines.a ../libblender/blender-build/lib/libge_converter.a ../libblender/blender-build/lib/libge_phys_dummy.a ../libblender/blender-build/lib/libge_phys_bullet.a ../libblender/blender-build/lib/libbf_intern_smoke.a ../libblender/blender-build/lib/libextern_minilzo.a ../libblender/blender-build/lib/libextern_lzma.a ../libblender/blender-build/lib/libextern_recastnavigation.a ../libblender/blender-build/lib/libge_logic_ketsji.a ../libblender/blender-build/lib/libge_phys_common.a ../libblender/blender-build/lib/libge_logic.a ../libblender/blender-build/lib/libge_rasterizer.a ../libblender/blender-build/lib/libge_oglrasterizer.a ../libblender/blender-build/lib/libge_logic_expressions.a ../libblender/blender-build/lib/libge_scenegraph.a ../libblender/blender-build/lib/libge_logic_network.a ../libblender/blender-build/lib/libbf_python.a ../libblender/blender-build/lib/libge_logic_ngnetwork.a ../libblender/blender-build/lib/libextern_bullet.a ../libblender/blender-build/lib/libge_logic_loopbacknetwork.a ../libblender/blender-build/lib/libbf_intern_moto.a ../libblender/blender-build/lib/libge_videotex.a ../libblender/blender-build/lib/libbf_rna.a ../libblender/blender-build/lib/libbf_dna.a ../libblender/blender-build/lib/libbf_blenfont.a ../libblender/blender-build/lib/libbf_intern_audaspace.a ../libblender/blender-build/lib/libbf_intern_mikktspace.a ../libblender/blender-build/lib/libextern_recastnavigation.a ../libblender/blender-build/lib/libbf_editor_util.a ../libblender/blender-build/lib/libextern_glew.a ../libblender/blender-build/lib/libbf_imbuf.a ../libblender/blender-build/lib/libextern_binreloc.a ../libblender/blender-build/lib/libbf_intern_itasc.a -lGL -lGLU -ljpeg -lpng -lz -lz -lutil -lc -lm -lpthread -lstdc++ -lX11 -lXi -ldl -Xlinker -export-dynamic -lpython3.2mu -lGLU -ljpeg -lpng -lz -lz -lfreetype -lopenal -lsamplerate -Wl,-Bstatic -lSDLmain -Wl,-Bdynamic -lSDL -lpthread -ltiff -lHalf -lIex -lIlmImf -lIlmThread -lImath -lopenjpeg -lutil -lc -lm -lpthread -lstdc++ -lX11 -lXi -ldl -lpython3.2mu -lfreetype -lopenal -lsamplerate -Wl,-Bstatic -lSDLmain -Wl,-Bdynamic -lSDL -lpthread -ltiff -lHalf -lIex -lIlmImf -lIlmThread -lImath -lopenjpeg -lspnav ../l3m/dist/Debug/GNU-Linux-x86/libl3m.a ../renderer/dist/Debug/GNU-Linux-x86/librenderer.a ../shared/dist/Debug/GNU-Linux-x86/libshared.a ../libsquish/dist/Debug/GNU-Linux-x86/liblibsquish.a

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/import

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/import: ../l3m/dist/Debug/GNU-Linux-x86/libl3m.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/import: ../renderer/dist/Debug/GNU-Linux-x86/librenderer.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/import: ../shared/dist/Debug/GNU-Linux-x86/libshared.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/import: ../libsquish/dist/Debug/GNU-Linux-x86/liblibsquish.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/import: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/import ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/startup_blender.o: startup_blender.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -DUSE_THREADS -DDEBUG -DDEBUG_VERY_VERBOSE -I. -I.. -I../libblender/blender/source/blender/blenkernel -I../libblender/blender/source/blender/blenlib -I../libblender/blender/source/blender/makesdna -I../libblender/blender/source/blender/makesrna -I../libblender/blender/intern/guardedalloc -I../libblender/blender/source/blender/imbuf -I../libblender/blender/source/blender/windowmanager -I../libblender/blender/source/blender/gpu -I../libblender/blender/source/blender/render/extern/include -I../libblender/blender/source/blender/nodes -I../libblender/blender/source/blender/editors/include -I../libblender/blender/source/blender/blenloader -MMD -MP -MF $@.d -o ${OBJECTDIR}/startup_blender.o startup_blender.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -DUSE_THREADS -DDEBUG -DDEBUG_VERY_VERBOSE -I. -I.. -I../libblender/blender/source/blender/blenkernel -I../libblender/blender/source/blender/blenlib -I../libblender/blender/source/blender/makesdna -I../libblender/blender/source/blender/makesrna -I../libblender/blender/intern/guardedalloc -I../libblender/blender/source/blender/imbuf -I../libblender/blender/source/blender/windowmanager -I../libblender/blender/source/blender/gpu -I../libblender/blender/source/blender/render/extern/include -I../libblender/blender/source/blender/nodes -I../libblender/blender/source/blender/editors/include -I../libblender/blender/source/blender/blenloader -MMD -MP -MF $@.d -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/blender.o: blender.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -DUSE_THREADS -DDEBUG -DDEBUG_VERY_VERBOSE -I. -I.. -I../libblender/blender/source/blender/blenkernel -I../libblender/blender/source/blender/blenlib -I../libblender/blender/source/blender/makesdna -I../libblender/blender/source/blender/makesrna -I../libblender/blender/intern/guardedalloc -I../libblender/blender/source/blender/imbuf -I../libblender/blender/source/blender/windowmanager -I../libblender/blender/source/blender/gpu -I../libblender/blender/source/blender/render/extern/include -I../libblender/blender/source/blender/nodes -I../libblender/blender/source/blender/editors/include -I../libblender/blender/source/blender/blenloader -MMD -MP -MF $@.d -o ${OBJECTDIR}/blender.o blender.cpp

${OBJECTDIR}/post_process.o: post_process.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -DUSE_THREADS -DDEBUG -DDEBUG_VERY_VERBOSE -I. -I.. -I../libblender/blender/source/blender/blenkernel -I../libblender/blender/source/blender/blenlib -I../libblender/blender/source/blender/makesdna -I../libblender/blender/source/blender/makesrna -I../libblender/blender/intern/guardedalloc -I../libblender/blender/source/blender/imbuf -I../libblender/blender/source/blender/windowmanager -I../libblender/blender/source/blender/gpu -I../libblender/blender/source/blender/render/extern/include -I../libblender/blender/source/blender/nodes -I../libblender/blender/source/blender/editors/include -I../libblender/blender/source/blender/blenloader -MMD -MP -MF $@.d -o ${OBJECTDIR}/post_process.o post_process.cpp

# Subprojects
.build-subprojects:
	cd ../l3m && ${MAKE}  -f Makefile CONF=Debug
	cd ../renderer && ${MAKE}  -f Makefile CONF=Debug
	cd ../shared && ${MAKE}  -f Makefile CONF=Debug
	cd ../libsquish && ${MAKE}  -f Makefile CONF=Debug
	cd ../shared && ${MAKE}  -f Makefile CONF=Debug

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/import

# Subprojects
.clean-subprojects:
	cd ../l3m && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../renderer && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../shared && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../libsquish && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../shared && ${MAKE}  -f Makefile CONF=Debug clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
