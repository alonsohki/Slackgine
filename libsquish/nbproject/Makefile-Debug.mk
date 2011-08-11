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
	${OBJECTDIR}/maths.o \
	${OBJECTDIR}/colourfit.o \
	${OBJECTDIR}/singlecolourfit.o \
	${OBJECTDIR}/clusterfit.o \
	${OBJECTDIR}/squish.o \
	${OBJECTDIR}/rangefit.o \
	${OBJECTDIR}/colourblock.o \
	${OBJECTDIR}/alpha.o \
	${OBJECTDIR}/colourset.o


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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/liblibsquish.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/liblibsquish.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/liblibsquish.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/liblibsquish.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/liblibsquish.a

${OBJECTDIR}/maths.o: maths.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I. -I.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/maths.o maths.cpp

${OBJECTDIR}/colourfit.o: colourfit.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I. -I.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/colourfit.o colourfit.cpp

${OBJECTDIR}/singlecolourfit.o: singlecolourfit.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I. -I.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/singlecolourfit.o singlecolourfit.cpp

${OBJECTDIR}/clusterfit.o: clusterfit.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I. -I.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/clusterfit.o clusterfit.cpp

${OBJECTDIR}/squish.o: squish.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I. -I.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/squish.o squish.cpp

${OBJECTDIR}/rangefit.o: rangefit.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I. -I.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/rangefit.o rangefit.cpp

${OBJECTDIR}/colourblock.o: colourblock.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I. -I.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/colourblock.o colourblock.cpp

${OBJECTDIR}/alpha.o: alpha.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I. -I.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/alpha.o alpha.cpp

${OBJECTDIR}/colourset.o: colourset.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I. -I.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/colourset.o colourset.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/liblibsquish.a

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
