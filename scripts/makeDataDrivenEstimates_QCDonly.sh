#!/bin/bash

INDIR=/home/users/dpg/MT2AnalysisEdit/MT2looper/output/run1/

THISDIR=`pwd`

## to use data for lostlepton
LOSTLEPFILE=data_Run2016
## to use MC for lostlepton
#LOSTLEPFILE=lostlep

GJETFILE=data_Run2016
#GJETFILE=qcdplusgjet

RLFILE=data_Run2016
#RLFILE=removedlep

QCDFILE=data_Run2016
#QCDFILE=qcd_ht

QCDESTIMATE=./inputs/qcdEstimateData_2016_Moriond_ETH_SnT_merge
QCDMONOJET=./inputs_new/qcdEstimateMonojet_Mar07

if [ ! -d "$INDIR" ]; then
  echo "Input directory does not exist" 
fi

# make the data driven background files
cd $INDIR
echo "hadd qcdplusgjet.root gjets_dr0p05_ht.root qcd_ht.root"
hadd -f qcdplusgjet.root gjets_dr0p05_ht.root qcd_ht.root  >> $THISDIR/dataDrivenEstimates.log
echo "hadd CRRLbkg.root ttsl.root ttdl.root singletop.root" # should probably include QCD here
hadd -f CRRLbkg.root ttsl.root ttdl.root singletop.root  >> $THISDIR/dataDrivenEstimates.log
hadd -f removedlep.root wjets_ht.root CRRLbkg.root >> $THISDIR/dataDrivenEstimates.log
cd $THISDIR
echo "root -b -q rescaleBoundaryHists.C+(${INDIR}/qcdplusgjet.root,2)"
root -b -q "rescaleBoundaryHists.C+(\"${INDIR}/qcdplusgjet.root\",2)" >> dataDrivenEstimates.log
echo "root -b -q rescaleBoundaryHists.C+(${INDIR}/CRRLbkg.root,3)"
root -b -q "rescaleBoundaryHists.C+(\"${INDIR}/CRRLbkg.root\",3)" >> dataDrivenEstimates.log
echo "root -b -q rescaleBoundaryHists.C+(${INDIR}/removedlep.root,2)"
root -b -q "rescaleBoundaryHists.C+(\"${INDIR}/removedlep.root\",2)" >> dataDrivenEstimates.log
echo "root -b -q purity.C+(${INDIR})"
root -b -q "purity.C+(\"${INDIR}\",\"${GJETFILE}\")" >> dataDrivenEstimates.log
echo "root -b -q purityRL.C+(${INDIR}, ${RLFILE})"
root -b -q "purityRL.C+(\"${INDIR}\",\"${RLFILE}\")" >> dataDrivenEstimates.log
echo "root -b -q DataRZGammaRatioMaker.C+(${INDIR})"
root -b -q "DataRZGammaRatioMaker.C+(\"${INDIR}\",\"${GJETFILE}\")" >> dataDrivenEstimates.log
echo "root -b -q qcdRphiMaker.C+(${INDIR},${QCDFILE},${QCDESTIMATE},${QCDMONOJET})"
root -b -q "qcdRphiMaker.C+(\"${INDIR}\",\"${QCDFILE}\",\"${QCDESTIMATE}\",\"${QCDMONOJET}\")" >> dataDrivenEstimates.log
echo "done"


