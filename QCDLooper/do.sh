#!/bin/bash

make -j 8

OUTDIR=output/full2016_PseudoJetEta

LOGDIR=logs_PseudoJetEta

mkdir -p ${OUTDIR}
mkdir -p ${LOGDIR}


#most recent MC
#INDIR=/nfs-6/userdata/mt2/rphi_skim_V00-08-10_JECandRenorm
INDIR=/nfs-6/userdata/mt2/V00-08-18_mc_skim_rphi/

declare -a Samples=(qcd_ht ttsl ttdl singletop ttw_mg_lo ttz_mg_lo ttg dyjetsll_ht wjets_ht100to200 wjets_ht1200to2500 wjets_ht200to400 wjets_ht2500toInf wjets_ht400to600 wjets_ht600to800 wjets_ht800to120 gjets_dr0p05_ht zinv_ht)

for SAMPLE in ${Samples[@]};
  do echo ./runLooper ${INDIR} ${SAMPLE} ${OUTDIR}
  nohup ./runLooper ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &
done

#except most recent run2016H
#INDIR=/nfs-6/userdata/mt2/rphi_skim_V00-08-12_json_271036-284044_23Sep2016ReReco_36p26fb 
INDIR=/nfs-6/userdata/mt2/V00-08-18_skim_rphi/

declare -a Samples=(data_Run2016B data_Run2016C data_Run2016D data_Run2016E data_Run2016F data_Run2016G)

for SAMPLE in ${Samples[@]};
  do echo ./runLooper ${INDIR} ${SAMPLE} ${OUTDIR}
  nohup ./runLooper ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &
done
