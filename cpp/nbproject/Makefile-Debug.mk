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
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/DB.o \
	${OBJECTDIR}/base64.o \
	${OBJECTDIR}/main.o

# Test Directory
TESTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/tests

# Test Files
TESTFILES= \
	${TESTDIR}/TestFiles/f1

# Test Object Files
TESTOBJECTFILES= \
	${TESTDIR}/tests/base64Test.o

# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-I libs/pistache/include $(shell pkg-config --cflags libfastjson)
CXXFLAGS=-I libs/pistache/include $(shell pkg-config --cflags libfastjson)

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=`pkg-config --libs libpistache` `pkg-config --libs pthread-stubs` `pkg-config --libs libfastjson` `pkg-config --libs libcurl`  

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/cpp

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/cpp: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/cpp ${OBJECTFILES} ${LDLIBSOPTIONS} -lboost_filesystem -lboost_system -lfastjson -lMPFDParser-1 -lRedisClient

${OBJECTDIR}/DB.o: DB.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags libpistache` `pkg-config --cflags pthread-stubs` `pkg-config --cflags libfastjson` `pkg-config --cflags libcurl`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/DB.o DB.cpp

${OBJECTDIR}/base64.o: base64.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags libpistache` `pkg-config --cflags pthread-stubs` `pkg-config --cflags libfastjson` `pkg-config --cflags libcurl`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/base64.o base64.cpp

${OBJECTDIR}/main.o: main.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags libpistache` `pkg-config --cflags pthread-stubs` `pkg-config --cflags libfastjson` `pkg-config --cflags libcurl`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:

# Build Test Targets
.build-tests-conf: .build-tests-subprojects .build-conf ${TESTFILES}
.build-tests-subprojects:

${TESTDIR}/TestFiles/f1: ${TESTDIR}/tests/base64Test.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc} -o ${TESTDIR}/TestFiles/f1 $^ ${LDLIBSOPTIONS} -lboost_filesystem -lboost_system -lfastjson -lMPFDParser-1 -lRedisClient  


${TESTDIR}/tests/base64Test.o: tests/base64Test.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -I. `pkg-config --cflags libpistache` `pkg-config --cflags pthread-stubs` `pkg-config --cflags libfastjson` `pkg-config --cflags libcurl`   -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/base64Test.o tests/base64Test.cpp


${OBJECTDIR}/DB_nomain.o: ${OBJECTDIR}/DB.o DB.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/DB.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g `pkg-config --cflags libpistache` `pkg-config --cflags pthread-stubs` `pkg-config --cflags libfastjson` `pkg-config --cflags libcurl`   -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/DB_nomain.o DB.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/DB.o ${OBJECTDIR}/DB_nomain.o;\
	fi

${OBJECTDIR}/base64_nomain.o: ${OBJECTDIR}/base64.o base64.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/base64.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g `pkg-config --cflags libpistache` `pkg-config --cflags pthread-stubs` `pkg-config --cflags libfastjson` `pkg-config --cflags libcurl`   -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/base64_nomain.o base64.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/base64.o ${OBJECTDIR}/base64_nomain.o;\
	fi

${OBJECTDIR}/main_nomain.o: ${OBJECTDIR}/main.o main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/main.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g `pkg-config --cflags libpistache` `pkg-config --cflags pthread-stubs` `pkg-config --cflags libfastjson` `pkg-config --cflags libcurl`   -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main_nomain.o main.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/main.o ${OBJECTDIR}/main_nomain.o;\
	fi

# Run Test Targets
.test-conf:
	@if [ "${TEST}" = "" ]; \
	then  \
	    ${TESTDIR}/TestFiles/f1 || true; \
	else  \
	    ./${TEST} || true; \
	fi

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
