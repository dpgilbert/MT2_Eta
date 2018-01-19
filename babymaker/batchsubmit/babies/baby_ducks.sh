#!/bin/bash

CMSSW_VER=$1
SCRAM_VER=$2
DATASET=$3
FILENAME=$4
ANALYSIS=$5
BABY_TAG=$6
SHORTNAME=$7
NEVENTS=$8
OUTPUT_NAMES=$9
EXE_ARGS=${10}
IMERGED=$(echo $FILENAME | sed 's/.*merged_ntuple_\([0-9]\+\)\.root/\1/')

echo cmssw_ver: $CMSSW_VER
echo scram_ver: $SCRAM_VER
echo dataset: $DATASET
echo filename: $FILENAME
echo analysis: $ANALYSIS
echo baby_tag: $BABY_TAG
echo shortname: $SHORTNAME
echo nevents: $NEVENTS
echo output_names: $OUTPUT_NAMES
echo exe_args: $EXE_ARGS
echo imerged: $IMERGED




echo Extracting package tar file
tar -xzf package.tar.gz

echo Before executable
echo Date: $(date +%s)
hostname
ls -l

# ----------------- BEGIN USER EXECUTABLE -----------------
#!/bin/bash

#
# args
#

FILEID=${OUTPUT_NAMES}
FILE=${FILENAME}
COPYDIR=${OUTPUT_NAMES}

echo "[wrapper] FILEID    = " ${FILEID}
echo "[wrapper] FILE      = " ${FILE}
echo "[wrapper] COPYDIR   = " ${COPYDIR}

#
# set up environment
#
CMSSW_VERSION=CMSSW_8_0_5

###version using cvmfs install of CMSSW
echo "[wrapper] setting env"
export SCRAM_ARCH=slc6_amd64_gcc530
source /cvmfs/cms.cern.ch/cmsset_default.sh
OLDDIR=`pwd`
cd /cvmfs/cms.cern.ch/$SCRAM_ARCH/cms/cmssw/$CMSSW_VERSION/src
#cmsenv
eval `scramv1 runtime -sh`
cd $OLDDIR

# ### version using nfs install of CMSSW
# echo "[wrapper] setting env"
# export SCRAM_ARCH=slc6_amd64_gcc493
# source /nfs-7/cmssoft/cms.cern.ch/cmssw/cmsset_default.sh
# OLDDIR=`pwd`
# cd /nfs-7/cmssoft/cms.cern.ch/cmssw/slc6_amd64_gcc493/cms/cmssw-patch/$CMSSW_VERSION/src
# #cmsenv
# eval `scramv1 runtime -sh`
# cd $OLDDIR

echo

echo "[wrapper] printing env"
printenv
echo 

echo "[wrapper] hostname  = " `hostname`
echo "[wrapper] date      = " `date`
echo "[wrapper] linux timestamp = " `date +%s`
echo "[wrapper] checking input file with ls"
ls -alrth ${FILE}

# catch exit code
if [ $? -ne 0 ]; then
    echo "[wrapper] could not find input file, trying xrootd instead"
    FILESHORT=${FILE#/hadoop/cms}
    xrdfs xrootd.t2.ucsd.edu ls ${FILESHORT}
    if [ $? -ne 0 ]; then
	echo "[wrapper] could not find input file with xrootd either, exiting"
	exit 1
    else
	echo "[wrapper] found file with xrootd, will proceed"
	FILE="root://xrootd.t2.ucsd.edu/"${FILESHORT}
    fi
fi

#
# untar input sandbox
#

echo "[wrapper] extracting input sandbox"
tar -zxf package.tar.gz

#source job_input/setupenv.sh
#printenv

#cd job_input
echo "[wrapper] input contents are"
ls -a

echo "[wrapper] directory contents are"
ls

#
# run it
#
echo "[wrapper] running: ./processBaby ${FILEID} ${FILE}"

./processBaby ${FILEID} ${FILE}

#
# do something with output
#

echo "[wrapper] output is"
ls

OUTPUT=`ls | grep ${FILEID}`
echo "[wrapper] OUTPUT = " ${OUTPUT}
mv ${OUTPUT} output.root

# Rigorous sweeproot which checks ALL branches for ALL events.
# If GetEntry() returns -1, then there was an I/O problem, so we will delete it
cat > rigorousSweepRoot.py << EOL
import ROOT as r
import os, sys

f1 = r.TFile("output.root")
if not f1 or not f1.IsOpen() or f1.IsZombie():
    print "[RSR] removing zombie output.root because it does not deserve to live"
    os.system("rm output.root")
    sys.exit()

t = f1.Get("mt2")
print "[RSR] ntuple has %i events" % t.GetEntries()

foundBad = False
for i in range(0,t.GetEntries(),1):
    if t.GetEntry(i) < 0:
        foundBad = True
        print "[RSR] found bad event %i" % i
        break

if foundBad:
    print "[RSR] removing output.root because it does not deserve to live"
    os.system("rm output.root")
else:
    print "[RSR] passed the rigorous sweeproot"
EOL

date +%s
echo "[wrapper] running rigorousSweepRoot.py"
python rigorousSweepRoot.py
date +%s

#
# do skim
#

echo "[wrapper] doing skim"
root -b -q skim_macro.C

#
# do rphi skim
#

echo "[wrapper] doing rphi skim"
root -b -q skim_rphi_macro.C

#
# clean up
#

echo "[wrapper] copying files"

# if [ ! -d "${COPYDIR}" ]; then
#     echo "creating output directory " ${COPYDIR}
#     mkdir ${COPYDIR}
# fi

# gfal-copy -p -f -t 4200 --verbose file:`pwd`/${OUTPUT} srm://bsrm-3.t2.ucsd.edu:8443/srm/v2/server?SFN=${COPYDIR}/${OUTPUT}

# echo "[wrapper] cleaning up"
# for FILE in `find . -not -name "*stderr" -not -name "*stdout"`; do rm -rf $FILE; done
# echo "[wrapper] cleaned up"
# ls

# ----------------- END USER EXECUTABLE -----------------


echo After executable
echo Date: $(date +%s)
ls -l

gfal-copy -p -f -t 4200 file://`pwd`/output.root gsiftp://gftp.t2.ucsd.edu/hadoop/cms/store/user/dpgilber/AutoTwopler_babies/${ANALYSIS}_${BABY_TAG}/${SHORTNAME}//output/output_${IMERGED}.root --checksum ADLER32

gfal-copy -p -f -t 4200 file://`pwd`/skim.root gsiftp://gftp.t2.ucsd.edu/hadoop/cms/store/user/dpgilber/AutoTwopler_babies/${ANALYSIS}_${BABY_TAG}/${SHORTNAME}//skim/skim_${IMERGED}.root --checksum ADLER32

gfal-copy -p -f -t 4200 file://`pwd`/skim_rphi.root gsiftp://gftp.t2.ucsd.edu/hadoop/cms/store/user/dpgilber/AutoTwopler_babies/${ANALYSIS}_${BABY_TAG}/${SHORTNAME}//skim_rphi/skim_rphi_${IMERGED}.root --checksum ADLER32

echo After copy
echo Date: $(date +%s)
echo Cleaning up
for FILE in `find . -not -name "*stderr" -not -name "*stdout"`; do rm -rf $FILE; done
echo Cleaned up
ls -l
