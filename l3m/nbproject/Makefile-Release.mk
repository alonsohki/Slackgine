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
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/Components/camera.o \
	${OBJECTDIR}/Components/require.o \
	${OBJECTDIR}/Components/geometry.o \
	${OBJECTDIR}/Components/scene.o \
	${OBJECTDIR}/stream.o \
	${OBJECTDIR}/model.o \
	${OBJECTDIR}/Components/texture.o \
	${OBJECTDIR}/Components/factory.o


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
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libl3m.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libl3m.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libl3m.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libl3m.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libl3m.a

${OBJECTDIR}/Components/camera.o: Components/camera.cpp 
	${MKDIR} -p ${OBJECTDIR}/Components
	${RM} $@.d
	$(COMPILE.cc) -O2 -DUSE_THREADS -I.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/Components/camera.o Components/camera.cpp

${OBJECTDIR}/Components/require.o: Components/require.cpp 
	${MKDIR} -p ${OBJECTDIR}/Components
	${RM} $@.d
	$(COMPILE.cc) -O2 -DUSE_THREADS -I.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/Components/require.o Components/require.cpp

${OBJECTDIR}/Components/geometry.o: Components/geometry.cpp 
	${MKDIR} -p ${OBJECTDIR}/Components
	${RM} $@.d
	$(COMPILE.cc) -O2 -DUSE_THREADS -I.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/Components/geometry.o Components/geometry.cpp

${OBJECTDIR}/Components/scene.o: Components/scene.cpp 
	${MKDIR} -p ${OBJECTDIR}/Components
	${RM} $@.d
	$(COMPILE.cc) -O2 -DUSE_THREADS -I.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/Components/scene.o Components/scene.cpp

${OBJECTDIR}/stream.o: stream.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -DUSE_THREADS -I.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/stream.o stream.cpp

${OBJECTDIR}/model.o: model.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -DUSE_THREADS -I.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/model.o model.cpp

${OBJECTDIR}/Components/texture.o: Components/texture.cpp 
	${MKDIR} -p ${OBJECTDIR}/Components
	${RM} $@.d
	$(COMPILE.cc) -O2 -DUSE_THREADS -I.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/Components/texture.o Components/texture.cpp

${OBJECTDIR}/Components/factory.o: Components/factory.cpp 
	${MKDIR} -p ${OBJECTDIR}/Components
	${RM} $@.d
	$(COMPILE.cc) -O2 -DUSE_THREADS -I.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/Components/factory.o Components/factory.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libl3m.a

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
