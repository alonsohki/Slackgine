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
	${OBJECTDIR}/main.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-ansi -pipe
CXXFLAGS=-ansi -pipe

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=../core/dist/Debug/GNU-Linux-x86/libcore.a ../l3m/dist/Debug/GNU-Linux-x86/libl3m.a ../renderer/dist/Debug/GNU-Linux-x86/librenderer.a ../math/dist/Debug/GNU-Linux-x86/libmath.a ../shared/dist/Debug/GNU-Linux-x86/libshared.a `pkg-config --libs libpng` -lpthread  `pkg-config --libs gl` `pkg-config --libs glu` `pkg-config --libs glew` -lglut -lrt  

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/test

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/test: ../core/dist/Debug/GNU-Linux-x86/libcore.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/test: ../l3m/dist/Debug/GNU-Linux-x86/libl3m.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/test: ../renderer/dist/Debug/GNU-Linux-x86/librenderer.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/test: ../math/dist/Debug/GNU-Linux-x86/libmath.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/test: ../shared/dist/Debug/GNU-Linux-x86/libshared.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/test: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/test ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -DDEBUG -DDEBUG_VERY_VERBOSE -I.. `pkg-config --cflags libpng` `pkg-config --cflags gl` `pkg-config --cflags glu` `pkg-config --cflags glew`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:
	cd ../core && ${MAKE}  -f Makefile CONF=Debug
	cd ../l3m && ${MAKE}  -f Makefile CONF=Debug
	cd ../renderer && ${MAKE}  -f Makefile CONF=Debug
	cd ../math && ${MAKE}  -f Makefile CONF=Debug
	cd ../shared && ${MAKE}  -f Makefile CONF=Debug
	cd ../shared && ${MAKE}  -f Makefile CONF=Debug
	cd ../core && ${MAKE}  -f Makefile CONF=Debug
	cd ../math && ${MAKE}  -f Makefile CONF=Debug
	cd ../l3m && ${MAKE} -j9 -f Makefile CONF=Debug

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/test

# Subprojects
.clean-subprojects:
	cd ../core && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../l3m && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../renderer && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../math && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../shared && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../shared && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../core && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../math && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../l3m && ${MAKE} -j9 -f Makefile CONF=Debug clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
