#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=mkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=cof
DEBUGGABLE_SUFFIX=cof
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/SlaveO.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=cof
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/SlaveO.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../xlcd/putsxlcd.c ../xlcd/setcgram.c ../xlcd/openxlcd.c ../xlcd/setddram.c ../xlcd/writdata.c ../xlcd/readaddr.c ../xlcd/readdata.c ../xlcd/busyxlcd.c ../xlcd/putrxlcd.c ../xlcd/wcmdxlcd.c ../SlaveO.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/760475324/putsxlcd.o ${OBJECTDIR}/_ext/760475324/setcgram.o ${OBJECTDIR}/_ext/760475324/openxlcd.o ${OBJECTDIR}/_ext/760475324/setddram.o ${OBJECTDIR}/_ext/760475324/writdata.o ${OBJECTDIR}/_ext/760475324/readaddr.o ${OBJECTDIR}/_ext/760475324/readdata.o ${OBJECTDIR}/_ext/760475324/busyxlcd.o ${OBJECTDIR}/_ext/760475324/putrxlcd.o ${OBJECTDIR}/_ext/760475324/wcmdxlcd.o ${OBJECTDIR}/_ext/1472/SlaveO.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/760475324/putsxlcd.o.d ${OBJECTDIR}/_ext/760475324/setcgram.o.d ${OBJECTDIR}/_ext/760475324/openxlcd.o.d ${OBJECTDIR}/_ext/760475324/setddram.o.d ${OBJECTDIR}/_ext/760475324/writdata.o.d ${OBJECTDIR}/_ext/760475324/readaddr.o.d ${OBJECTDIR}/_ext/760475324/readdata.o.d ${OBJECTDIR}/_ext/760475324/busyxlcd.o.d ${OBJECTDIR}/_ext/760475324/putrxlcd.o.d ${OBJECTDIR}/_ext/760475324/wcmdxlcd.o.d ${OBJECTDIR}/_ext/1472/SlaveO.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/760475324/putsxlcd.o ${OBJECTDIR}/_ext/760475324/setcgram.o ${OBJECTDIR}/_ext/760475324/openxlcd.o ${OBJECTDIR}/_ext/760475324/setddram.o ${OBJECTDIR}/_ext/760475324/writdata.o ${OBJECTDIR}/_ext/760475324/readaddr.o ${OBJECTDIR}/_ext/760475324/readdata.o ${OBJECTDIR}/_ext/760475324/busyxlcd.o ${OBJECTDIR}/_ext/760475324/putrxlcd.o ${OBJECTDIR}/_ext/760475324/wcmdxlcd.o ${OBJECTDIR}/_ext/1472/SlaveO.o

# Source Files
SOURCEFILES=../xlcd/putsxlcd.c ../xlcd/setcgram.c ../xlcd/openxlcd.c ../xlcd/setddram.c ../xlcd/writdata.c ../xlcd/readaddr.c ../xlcd/readdata.c ../xlcd/busyxlcd.c ../xlcd/putrxlcd.c ../xlcd/wcmdxlcd.c ../SlaveO.c


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/SlaveO.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=18F8722
MP_PROCESSOR_OPTION_LD=18f8722
MP_LINKER_DEBUG_OPTION= -u_DEBUGCODESTART=0x1fd30 -u_DEBUGCODELEN=0x2d0 -u_DEBUGDATASTART=0xef4 -u_DEBUGDATALEN=0xb
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/760475324/putsxlcd.o: ../xlcd/putsxlcd.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/760475324 
	@${RM} ${OBJECTDIR}/_ext/760475324/putsxlcd.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -p$(MP_PROCESSOR_OPTION) -ml -oa-  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/760475324/putsxlcd.o   ../xlcd/putsxlcd.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/760475324/putsxlcd.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/760475324/putsxlcd.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/760475324/setcgram.o: ../xlcd/setcgram.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/760475324 
	@${RM} ${OBJECTDIR}/_ext/760475324/setcgram.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -p$(MP_PROCESSOR_OPTION) -ml -oa-  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/760475324/setcgram.o   ../xlcd/setcgram.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/760475324/setcgram.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/760475324/setcgram.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/760475324/openxlcd.o: ../xlcd/openxlcd.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/760475324 
	@${RM} ${OBJECTDIR}/_ext/760475324/openxlcd.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -p$(MP_PROCESSOR_OPTION) -ml -oa-  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/760475324/openxlcd.o   ../xlcd/openxlcd.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/760475324/openxlcd.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/760475324/openxlcd.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/760475324/setddram.o: ../xlcd/setddram.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/760475324 
	@${RM} ${OBJECTDIR}/_ext/760475324/setddram.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -p$(MP_PROCESSOR_OPTION) -ml -oa-  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/760475324/setddram.o   ../xlcd/setddram.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/760475324/setddram.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/760475324/setddram.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/760475324/writdata.o: ../xlcd/writdata.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/760475324 
	@${RM} ${OBJECTDIR}/_ext/760475324/writdata.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -p$(MP_PROCESSOR_OPTION) -ml -oa-  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/760475324/writdata.o   ../xlcd/writdata.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/760475324/writdata.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/760475324/writdata.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/760475324/readaddr.o: ../xlcd/readaddr.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/760475324 
	@${RM} ${OBJECTDIR}/_ext/760475324/readaddr.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -p$(MP_PROCESSOR_OPTION) -ml -oa-  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/760475324/readaddr.o   ../xlcd/readaddr.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/760475324/readaddr.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/760475324/readaddr.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/760475324/readdata.o: ../xlcd/readdata.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/760475324 
	@${RM} ${OBJECTDIR}/_ext/760475324/readdata.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -p$(MP_PROCESSOR_OPTION) -ml -oa-  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/760475324/readdata.o   ../xlcd/readdata.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/760475324/readdata.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/760475324/readdata.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/760475324/busyxlcd.o: ../xlcd/busyxlcd.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/760475324 
	@${RM} ${OBJECTDIR}/_ext/760475324/busyxlcd.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -p$(MP_PROCESSOR_OPTION) -ml -oa-  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/760475324/busyxlcd.o   ../xlcd/busyxlcd.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/760475324/busyxlcd.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/760475324/busyxlcd.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/760475324/putrxlcd.o: ../xlcd/putrxlcd.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/760475324 
	@${RM} ${OBJECTDIR}/_ext/760475324/putrxlcd.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -p$(MP_PROCESSOR_OPTION) -ml -oa-  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/760475324/putrxlcd.o   ../xlcd/putrxlcd.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/760475324/putrxlcd.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/760475324/putrxlcd.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/760475324/wcmdxlcd.o: ../xlcd/wcmdxlcd.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/760475324 
	@${RM} ${OBJECTDIR}/_ext/760475324/wcmdxlcd.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -p$(MP_PROCESSOR_OPTION) -ml -oa-  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/760475324/wcmdxlcd.o   ../xlcd/wcmdxlcd.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/760475324/wcmdxlcd.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/760475324/wcmdxlcd.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/SlaveO.o: ../SlaveO.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/SlaveO.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -p$(MP_PROCESSOR_OPTION) -ml -oa-  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/1472/SlaveO.o   ../SlaveO.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/SlaveO.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/SlaveO.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
else
${OBJECTDIR}/_ext/760475324/putsxlcd.o: ../xlcd/putsxlcd.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/760475324 
	@${RM} ${OBJECTDIR}/_ext/760475324/putsxlcd.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -ml -oa-  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/760475324/putsxlcd.o   ../xlcd/putsxlcd.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/760475324/putsxlcd.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/760475324/putsxlcd.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/760475324/setcgram.o: ../xlcd/setcgram.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/760475324 
	@${RM} ${OBJECTDIR}/_ext/760475324/setcgram.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -ml -oa-  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/760475324/setcgram.o   ../xlcd/setcgram.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/760475324/setcgram.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/760475324/setcgram.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/760475324/openxlcd.o: ../xlcd/openxlcd.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/760475324 
	@${RM} ${OBJECTDIR}/_ext/760475324/openxlcd.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -ml -oa-  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/760475324/openxlcd.o   ../xlcd/openxlcd.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/760475324/openxlcd.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/760475324/openxlcd.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/760475324/setddram.o: ../xlcd/setddram.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/760475324 
	@${RM} ${OBJECTDIR}/_ext/760475324/setddram.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -ml -oa-  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/760475324/setddram.o   ../xlcd/setddram.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/760475324/setddram.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/760475324/setddram.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/760475324/writdata.o: ../xlcd/writdata.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/760475324 
	@${RM} ${OBJECTDIR}/_ext/760475324/writdata.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -ml -oa-  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/760475324/writdata.o   ../xlcd/writdata.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/760475324/writdata.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/760475324/writdata.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/760475324/readaddr.o: ../xlcd/readaddr.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/760475324 
	@${RM} ${OBJECTDIR}/_ext/760475324/readaddr.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -ml -oa-  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/760475324/readaddr.o   ../xlcd/readaddr.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/760475324/readaddr.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/760475324/readaddr.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/760475324/readdata.o: ../xlcd/readdata.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/760475324 
	@${RM} ${OBJECTDIR}/_ext/760475324/readdata.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -ml -oa-  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/760475324/readdata.o   ../xlcd/readdata.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/760475324/readdata.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/760475324/readdata.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/760475324/busyxlcd.o: ../xlcd/busyxlcd.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/760475324 
	@${RM} ${OBJECTDIR}/_ext/760475324/busyxlcd.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -ml -oa-  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/760475324/busyxlcd.o   ../xlcd/busyxlcd.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/760475324/busyxlcd.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/760475324/busyxlcd.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/760475324/putrxlcd.o: ../xlcd/putrxlcd.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/760475324 
	@${RM} ${OBJECTDIR}/_ext/760475324/putrxlcd.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -ml -oa-  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/760475324/putrxlcd.o   ../xlcd/putrxlcd.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/760475324/putrxlcd.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/760475324/putrxlcd.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/760475324/wcmdxlcd.o: ../xlcd/wcmdxlcd.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/760475324 
	@${RM} ${OBJECTDIR}/_ext/760475324/wcmdxlcd.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -ml -oa-  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/760475324/wcmdxlcd.o   ../xlcd/wcmdxlcd.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/760475324/wcmdxlcd.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/760475324/wcmdxlcd.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/SlaveO.o: ../SlaveO.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/SlaveO.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -ml -oa-  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/1472/SlaveO.o   ../SlaveO.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/SlaveO.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/SlaveO.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/SlaveO.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_LD} $(MP_EXTRA_LD_PRE)   -p$(MP_PROCESSOR_OPTION_LD)  -w -x -u_DEBUG -m"${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map"  -z__MPLAB_BUILD=1  -u_CRUNTIME -z__MPLAB_DEBUG=1 -z__MPLAB_DEBUGGER_ICD3=1 $(MP_LINKER_DEBUG_OPTION) -l ${MP_CC_DIR}/../lib  -o dist/${CND_CONF}/${IMAGE_TYPE}/SlaveO.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}   
else
dist/${CND_CONF}/${IMAGE_TYPE}/SlaveO.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_LD} $(MP_EXTRA_LD_PRE)   -p$(MP_PROCESSOR_OPTION_LD)  -w  -m"${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map"  -z__MPLAB_BUILD=1  -u_CRUNTIME -l ${MP_CC_DIR}/../lib  -o dist/${CND_CONF}/${IMAGE_TYPE}/SlaveO.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}   
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell "${PATH_TO_IDE_BIN}"mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
