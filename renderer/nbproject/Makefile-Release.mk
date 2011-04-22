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
	${OBJECTDIR}/renderers/opengl3/renderer.o \
	${OBJECTDIR}/extensible_entity.o \
	${OBJECTDIR}/renderers/opengl3/program.o \
	${OBJECTDIR}/renderers/opengl3/shader.o \
	${OBJECTDIR}/entity.o


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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/librenderer.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/librenderer.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/librenderer.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/librenderer.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/librenderer.a

${OBJECTDIR}/renderers/opengl3/renderer.o: renderers/opengl3/renderer.cpp 
	${MKDIR} -p ${OBJECTDIR}/renderers/opengl3
	${RM} $@.d
	$(COMPILE.cc) -O2 -I.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/renderers/opengl3/renderer.o renderers/opengl3/renderer.cpp

${OBJECTDIR}/extensible_entity.o: extensible_entity.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/extensible_entity.o extensible_entity.cpp

${OBJECTDIR}/renderers/opengl3/program.o: renderers/opengl3/program.cpp 
	${MKDIR} -p ${OBJECTDIR}/renderers/opengl3
	${RM} $@.d
	$(COMPILE.cc) -O2 -I.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/renderers/opengl3/program.o renderers/opengl3/program.cpp

${OBJECTDIR}/renderers/opengl3/shader.o: renderers/opengl3/shader.cpp 
	${MKDIR} -p ${OBJECTDIR}/renderers/opengl3
	${RM} $@.d
	$(COMPILE.cc) -O2 -I.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/renderers/opengl3/shader.o renderers/opengl3/shader.cpp

${OBJECTDIR}/entity.o: entity.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/entity.o entity.cpp

# Subprojects
.build-subprojects:
	cd ../l3m && ${MAKE}  -f Makefile CONF=Release

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/librenderer.a

# Subprojects
.clean-subprojects:
	cd ../l3m && ${MAKE}  -f Makefile CONF=Release clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
