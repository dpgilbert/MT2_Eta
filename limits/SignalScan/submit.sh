#!/bin/bash

SAMPLE=$1
DATE=$2
MODELFOLDER=$3
COMBINED=$4

while  ! voms-proxy-info -exist
do echo "No Proxy found issuing \"voms-proxy-init -voms cms\""
   voms-proxy-init -hours 168 -voms cms 
done

#MODEL=$(echo "$SAMPLE"|awk -F- 'split($1,a,"_")&&$0=a[1]')
MODEL=$(echo "$SAMPLE"|awk -F_ 'split($1,a,"_")&&$0=a[1]')

echo $MODEL $SAMPLE $DATE $MODELFOLDER

INPUT="job_input.tar.gz, cards_${MODELFOLDER}_${DATE}/cards_$SAMPLE.tar.gz"
SITE="T2_US_UCSD"
PROXY=$(voms-proxy-info -path)
SUBMITLOGDIR="${PWD}/submit_logs_$MODELFOLDER"
JOBLOGDIR="/data/tmp/$USER/job_logs/mt2Limits_$MODELFOLDER"
JOBCFGDIR="${PWD}/job_cfg_$MODELFOLDER"
LOG="${SUBMITLOGDIR}/condor_submit.log"
OUT="${JOBLOGDIR}/1e.\$(Cluster).\$(Process).out"
ERR="${JOBLOGDIR}/1e.\$(Cluster).\$(Process).err"

OUTPUTDIR=/hadoop/cms/store/user/$USER/combine/limits/${MODELFOLDER}_${DATE}

if [ ! -d "${SUBMITLOGDIR}" ]; then
    mkdir -p ${SUBMITLOGDIR}
fi

if [ ! -d "${JOBLOGDIR}" ]; then
    mkdir -p ${JOBLOGDIR}
fi

if [ ! -d "${JOBCFGDIR}" ]; then
    mkdir -p ${JOBCFGDIR}
fi

if [ $COMBINED == 1 ]
then
    EXE=condorExecutableCombined.sh
else
    EXE=condorExecutable.sh
fi

# outdated settings
#universe=grid
#Grid_Resource=condor cmssubmit-r1.t2.ucsd.edu glidein-collector.t2.ucsd.edu

echo "
universe=vanilla
when_to_transfer_output = ON_EXIT
transfer_input_files=${INPUT}
transfer_output_files = /dev/null
+DESIRED_Sites=\"${SITE}\"
+remote_DESIRED_Sites=\"T2_US_UCSD\"
+Owner = undefined
log=${LOG}
output=${OUT}
error =${ERR}
notification=Never
x509userproxy=${PROXY}
executable=${EXE}
transfer_executable=True
arguments=$SAMPLE $OUTPUTDIR 
queue
" > ${JOBCFGDIR}/condor_$SAMPLE.cmd

echo "condor_submit ${JOBCFGDIR}/condor_$SAMPLE.cmd"
condor_submit ${JOBCFGDIR}/condor_$SAMPLE.cmd

