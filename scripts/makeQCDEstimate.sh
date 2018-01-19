#!/bin/bash 

INDIR=/home/users/dpgilber/MT2AnalysisEdit/MT2looper/output/PseudoJetEta/

QCDFILE=data_Run2016

#QCDESTIMATE=./inputs_new/qcdEstimateData_Mar07
QCDESTIMATE=./qcdEstimate/output/full2016_PseudoJetEta/qcdEstimate
#QCDMONOJET=./inputs_new/qcdEstimateMonojet_Mar07
QCDMONOJET=./inputs/qcdEstimateMonojet
echo "root -b -q qcdRphiMaker.C+(${INDIR},${QCDFILE},${QCDESTIMATE},${QCDMONOJET})"
root -b -q "qcdRphiMaker.C+(\"${INDIR}\",\"${QCDFILE}\",\"${QCDESTIMATE}\",\"${QCDMONOJET}\")" >> dataDrivenEstimates.log
echo "done"
