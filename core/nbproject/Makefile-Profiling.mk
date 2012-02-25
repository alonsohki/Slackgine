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
CND_CONF=Profiling
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/shader.o \
	${OBJECTDIR}/world.o \
	${OBJECTDIR}/time.o \
	${OBJECTDIR}/shader_manager.o \
	${OBJECTDIR}/model_manager.o \
	${OBJECTDIR}/transformable.o \
	${OBJECTDIR}/texture_manager.o \
	${OBJECTDIR}/camera.o \
	${OBJECTDIR}/slackgine.o \
	${OBJECTDIR}/entity.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-pipe -ansi -pg
CXXFLAGS=-pipe -ansi -pg

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libcore.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libcore.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libcore.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libcore.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libcore.a

${OBJECTDIR}/shader.o: shader.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -DDEBUG -DDEBUG_VERY_VERBOSE -DPROFILING -I.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/shader.o shader.cpp

${OBJECTDIR}/world.o: world.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -DDEBUG -DDEBUG_VERY_VERBOSE -DPROFILING -I.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/world.o world.cpp

${OBJECTDIR}/time.o: time.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -DDEBUG -DDEBUG_VERY_VERBOSE -DPROFILING -I.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/time.o time.cpp

${OBJECTDIR}/shader_manager.o: shader_manager.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -DDEBUG -DDEBUG_VERY_VERBOSE -DPROFILING -I.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/shader_manager.o shader_manager.cpp

${OBJECTDIR}/model_manager.o: model_manager.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -DDEBUG -DDEBUG_VERY_VERBOSE -DPROFILING -I.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/model_manager.o model_manager.cpp

${OBJECTDIR}/transformable.o: transformable.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -DDEBUG -DDEBUG_VERY_VERBOSE -DPROFILING -I.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/transformable.o transformable.cpp

${OBJECTDIR}/texture_manager.o: texture_manager.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -DDEBUG -DDEBUG_VERY_VERBOSE -DPROFILING -I.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/texture_manager.o texture_manager.cpp

${OBJECTDIR}/camera.o: camera.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -DDEBUG -DDEBUG_VERY_VERBOSE -DPROFILING -I.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/camera.o camera.cpp

${OBJECTDIR}/slackgine.o: slackgine.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -DDEBUG -DDEBUG_VERY_VERBOSE -DPROFILING -I.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/slackgine.o slackgine.cpp

${OBJECTDIR}/entity.o: entity.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -DDEBUG -DDEBUG_VERY_VERBOSE -DPROFILING -I.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/entity.o entity.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libcore.a

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
