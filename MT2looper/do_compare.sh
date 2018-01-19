#!/bin/bash

make -j 12 || return $?

OUTDIR=output/2016vs2017_16/
LOGDIR=logs_2016vs2017_16/
mkdir -p ${OUTDIR}
mkdir -p ${LOGDIR}

#most recent MC
INDIR=/nfs-6/userdata/mt2/V00-08-18_mc_skim_base_mt2gt200_ZinvV6_JECs/
declare -a Samples=(ttsl ttdl singletop qcd_ht ttw_mg_lo ttz_mg_lo ttg tth dyjetsll_ht gjets_dr0p05_ht wjets_ht100to200 wjets_ht1200to2500 wjets_ht200to400 wjets_ht2500toInf wjets_ht400to600 wjets_ht600to800 wjets_ht800to1200 zinv_ht)
#declare -a Samples=(wz_1l3n wz_3lnu wz_lnqq ww_2l2nu ww_lnuqq www_incl_amcnlo wwz_incl_amcnlo wzz_incl_amcnlo zzz_incl_amcnlo zz_2l2n zz_2l2q zz_2q2n zz_4l) #diboson and triboson samples

for SAMPLE in ${Samples[@]}; do
    echo nice -n 10 ./runLooper ${INDIR} ${SAMPLE} ${OUTDIR}
    eval "nohup nice -n 10 ./runLooper ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &"
done

#signal
INDIR=/nfs-6/userdata/mt2/V00-08-18_mc_skim_base_mt2gt200_ZinvV6_JECs/
declare -a Samples=(T1bbbb T1qqqq T1tttt T2bb T2qq T2tt)

for SAMPLE in ${Samples[@]}; do
    echo nice -n 10 ./runLooper ${INDIR} ${SAMPLE} ${OUTDIR}
    eval "nohup nice -n 10 ./runLooper ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &"
done

#most recent run2016H and re-reco
INDIR=/nfs-6/userdata/mt2/V00-08-18_skim_base_mt2gt200_ZinvV6_JECs/
declare -a Samples=(data_Run2016B data_Run2016C data_Run2016D data_Run2016E data_Run2016F data_Run2016G data_Run2016H)

for SAMPLE in ${Samples[@]}; do
    echo nice -n 10 ./runLooper ${INDIR} ${SAMPLE} ${OUTDIR}
    eval "nohup nice -n 10 ./runLooper ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &"
done
