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
	${OBJECTDIR}/src/Config.o \
	${OBJECTDIR}/src/DB.o \
	${OBJECTDIR}/src/base64.o \
	${OBJECTDIR}/src/main.o

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
CCFLAGS=-I libs/pistache/include $(shell pkg-config --cflags libfastjson) -I include
CXXFLAGS=-I libs/pistache/include $(shell pkg-config --cflags libfastjson) -I include

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
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/cpp ${OBJECTFILES} ${LDLIBSOPTIONS} -lboost_filesystem -lboost_system -lfastjson -lMPFDParser-1 -lRedisClient -I include

${OBJECTDIR}/src/Config.o: src/Config.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags libpistache` `pkg-config --cflags pthread-stubs` `pkg-config --cflags libfastjson` `pkg-config --cflags libcurl`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Config.o src/Config.cpp

${OBJECTDIR}/src/DB.o: src/DB.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags libpistache` `pkg-config --cflags pthread-stubs` `pkg-config --cflags libfastjson` `pkg-config --cflags libcurl`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/DB.o src/DB.cpp

${OBJECTDIR}/src/base64.o: src/base64.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags libpistache` `pkg-config --cflags pthread-stubs` `pkg-config --cflags libfastjson` `pkg-config --cflags libcurl`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/base64.o src/base64.cpp

${OBJECTDIR}/src/main.o: src/main.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags libpistache` `pkg-config --cflags pthread-stubs` `pkg-config --cflags libfastjson` `pkg-config --cflags libcurl`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/main.o src/main.cpp

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


${OBJECTDIR}/src/Config_nomain.o: ${OBJECTDIR}/src/Config.o src/Config.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/Config.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g `pkg-config --cflags libpistache` `pkg-config --cflags pthread-stubs` `pkg-config --cflags libfastjson` `pkg-config --cflags libcurl`   -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Config_nomain.o src/Config.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/Config.o ${OBJECTDIR}/src/Config_nomain.o;\
	fi

${OBJECTDIR}/src/DB_nomain.o: ${OBJECTDIR}/src/DB.o src/DB.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/DB.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g `pkg-config --cflags libpistache` `pkg-config --cflags pthread-stubs` `pkg-config --cflags libfastjson` `pkg-config --cflags libcurl`   -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/DB_nomain.o src/DB.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/DB.o ${OBJECTDIR}/src/DB_nomain.o;\
	fi

${OBJECTDIR}/src/base64_nomain.o: ${OBJECTDIR}/src/base64.o src/base64.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/base64.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g `pkg-config --cflags libpistache` `pkg-config --cflags pthread-stubs` `pkg-config --cflags libfastjson` `pkg-config --cflags libcurl`   -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/base64_nomain.o src/base64.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/base64.o ${OBJECTDIR}/src/base64_nomain.o;\
	fi

${OBJECTDIR}/src/main_nomain.o: ${OBJECTDIR}/src/main.o src/main.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/main.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g `pkg-config --cflags libpistache` `pkg-config --cflags pthread-stubs` `pkg-config --cflags libfastjson` `pkg-config --cflags libcurl`   -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/main_nomain.o src/main.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/main.o ${OBJECTDIR}/src/main_nomain.o;\
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
