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
	${OBJECTDIR}/components/scene.o \
	${OBJECTDIR}/components/unknown.o \
	${OBJECTDIR}/stream.o \
	${OBJECTDIR}/model.o \
	${OBJECTDIR}/components/material.o \
	${OBJECTDIR}/components/geometry.o \
	${OBJECTDIR}/components/require.o \
	${OBJECTDIR}/components/texture.o \
	${OBJECTDIR}/components/camera.o \
	${OBJECTDIR}/components/factory.o


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

${OBJECTDIR}/components/scene.o: components/scene.cpp 
	${MKDIR} -p ${OBJECTDIR}/components
	${RM} $@.d
	$(COMPILE.cc) -O2 -DUSE_THREADS -I.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/components/scene.o components/scene.cpp

${OBJECTDIR}/components/unknown.o: components/unknown.cpp 
	${MKDIR} -p ${OBJECTDIR}/components
	${RM} $@.d
	$(COMPILE.cc) -O2 -DUSE_THREADS -I.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/components/unknown.o components/unknown.cpp

${OBJECTDIR}/stream.o: stream.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -DUSE_THREADS -I.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/stream.o stream.cpp

${OBJECTDIR}/model.o: model.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -DUSE_THREADS -I.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/model.o model.cpp

${OBJECTDIR}/components/material.o: components/material.cpp 
	${MKDIR} -p ${OBJECTDIR}/components
	${RM} $@.d
	$(COMPILE.cc) -O2 -DUSE_THREADS -I.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/components/material.o components/material.cpp

${OBJECTDIR}/components/geometry.o: components/geometry.cpp 
	${MKDIR} -p ${OBJECTDIR}/components
	${RM} $@.d
	$(COMPILE.cc) -O2 -DUSE_THREADS -I.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/components/geometry.o components/geometry.cpp

${OBJECTDIR}/components/require.o: components/require.cpp 
	${MKDIR} -p ${OBJECTDIR}/components
	${RM} $@.d
	$(COMPILE.cc) -O2 -DUSE_THREADS -I.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/components/require.o components/require.cpp

${OBJECTDIR}/components/texture.o: components/texture.cpp 
	${MKDIR} -p ${OBJECTDIR}/components
	${RM} $@.d
	$(COMPILE.cc) -O2 -DUSE_THREADS -I.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/components/texture.o components/texture.cpp

${OBJECTDIR}/components/camera.o: components/camera.cpp 
	${MKDIR} -p ${OBJECTDIR}/components
	${RM} $@.d
	$(COMPILE.cc) -O2 -DUSE_THREADS -I.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/components/camera.o components/camera.cpp

${OBJECTDIR}/components/factory.o: components/factory.cpp 
	${MKDIR} -p ${OBJECTDIR}/components
	${RM} $@.d
	$(COMPILE.cc) -O2 -DUSE_THREADS -I.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/components/factory.o components/factory.cpp

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
