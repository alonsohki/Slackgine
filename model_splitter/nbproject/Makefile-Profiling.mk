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
	${OBJECTDIR}/main.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-pipe -pg
CXXFLAGS=-pipe -pg

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=../l3m/dist/Debug/GNU-Linux-x86/libl3m.a ../renderer/dist/Debug/GNU-Linux-x86/librenderer.a ../shared/dist/Debug/GNU-Linux-x86/libshared.a `pkg-config --libs glew` `pkg-config --libs libpng`  

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/model_splitter

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/model_splitter: ../l3m/dist/Debug/GNU-Linux-x86/libl3m.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/model_splitter: ../renderer/dist/Debug/GNU-Linux-x86/librenderer.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/model_splitter: ../shared/dist/Debug/GNU-Linux-x86/libshared.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/model_splitter: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/model_splitter ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -DDEBUG -DPROFILING -I. -I.. `pkg-config --cflags glew` `pkg-config --cflags libpng`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:
	cd ../l3m && ${MAKE}  -f Makefile CONF=Debug
	cd ../renderer && ${MAKE}  -f Makefile CONF=Debug
	cd ../shared && ${MAKE}  -f Makefile CONF=Debug
	cd ../l3m && ${MAKE} -j -f Makefile CONF=Profiling
	cd ../renderer && ${MAKE} -j -f Makefile CONF=Profiling
	cd ../shared && ${MAKE} -j -f Makefile CONF=Profiling

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/model_splitter

# Subprojects
.clean-subprojects:
	cd ../l3m && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../renderer && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../shared && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../l3m && ${MAKE} -j -f Makefile CONF=Profiling clean
	cd ../renderer && ${MAKE} -j -f Makefile CONF=Profiling clean
	cd ../shared && ${MAKE} -j -f Makefile CONF=Profiling clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
