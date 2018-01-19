#!/bin/bash

#wrapper script for submitting combine jobs to condor
#Define MODEL, DATE, INDIR, and COMBINED

#the model name
#MODEL="T1tttt"
#MODEL="T1bbbb"
MODEL="T1qqqq"
#MODEL="T2tt"
#MODEL="T2bb"
#MODEL="T2-4bd_scan"

#the suffix
#DATE="ETH_Feb2"
DATE="PseudoJetEta_NoNan"

#directory where cards and points_MODEL.txt can be found
#INDIR="/home/users/mderdzinski/winter2017/clean_master/scripts/cards_${MODEL}_${DATE}"
INDIR="/home/users/dpgilber/MT2AnalysisEdit/scripts/cards_PseudoJetEta_NolnNFix"

#bool, true if cards are already combined
COMBINED=0

source do_tar.sh ${MODEL} ${DATE} ${INDIR} ${COMBINED}

source submitAllJobs.sh ${MODEL} ${DATE} ${INDIR} ${COMBINED}
