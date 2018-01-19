#!/bin/bash

INDIR=/home/users/dpg/MT2Analysis/MT2looper/output/test

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

QCDESTIMATE=./inputs/qcdEstimateData
QCDMONOJET=./inputs/qcdEstimateMonojet

if [ ! -d "$INDIR" ]; then
  echo "Input directory does not exist" 
fi

cd $INDIR

echo "hadd -f data_Run2016.root data_Run2016B.root data_Run2016C.root data_Run2016D.root data_Run2016E.root data_Run2016F.root data_Run2016G.root data_Run2016H.root"
hadd -f data_Run2016.root data_Run2016B.root data_Run2016C.root data_Run2016D.root data_Run2016E.root data_Run2016F.root data_Run2016G.root data_Run2016H.root > dataDrivenEstimates.log

# combine wjets_ht bins
echo "hadd -f wjets_ht.root wjets_ht100to200.root wjets_ht200to400.root wjets_ht400to600.root wjets_ht600to800.root wjets_ht800to1200.root wjets_ht1200to2500.root wjets_ht2500toInf.root"
hadd -f wjets_ht.root wjets_ht100to200.root wjets_ht200to400.root wjets_ht400to600.root wjets_ht600to800.root wjets_ht800to1200.root wjets_ht1200to2500.root wjets_ht2500toInf.root >> dataDrivenEstimates.log

# (temporarily replaced tth with ttg, because of sample availability)
echo "hadd -f top.root ttsl.root ttdl.root singletop.root ttw.root ttz.root ttg.root"
hadd -f top.root ttsl.root ttdl.root singletop.root ttw_mg_lo.root ttz_mg_lo.root ttg.root >> dataDrivenEstimates.log

# make "lostlep" from sum of MC lostlep backgrounds (except QCD, to avoid spikes)
echo "hadd -f lostlep.root ttsl.root ttdl.root singletop.root ttw.root ttz.root tth.root wjets_ht.root"
hadd -f lostlep.root ttsl.root ttdl.root singletop.root ttw_mg_lo.root ttz_mg_lo.root tth.root wjets_ht.root >> dataDrivenEstimates.log

# echo "hadd -f wz.root wz_1l3n.root wz_3lnu.root wz_lnqq.root"
# hadd -f wz.root wz_1l3n.root wz_3lnu.root wz_lnqq.root >> dataDrivenEstimates.log

# echo "hadd -f ww.root ww_2l2nu.root ww_lnuqq.root"
# hadd -f ww.root ww_2l2nu.root ww_lnuqq.root >> dataDrivenEstimates.log

# echo "hadd -f zz.root zz_2l2n.root zz_2l2q.root zz_2q2n.root zz_4l.root"
# hadd -f zz.root zz_2l2n.root zz_2l2q.root zz_2q2n.root zz_4l.root >> dataDrivenEstimates.log

# echo "hadd -f vv.root ww_2l2nu.root ww_lnuqq.root zz_2l2n.root zz_2l2q.root zz_2q2n.root zz_4l.root"
# hadd -f vv.root ww_2l2nu.root ww_lnuqq.root zz_2l2n.root zz_2l2q.root zz_2q2n.root zz_4l.root >> dataDrivenEstimates.log

# echo "hadd -f vvv.root  www_incl_amcnlo.root wwz_incl_amcnlo.root wzz_incl_amcnlo.root zzz_incl_amcnlo.root"
# hadd -f vvv.root  www_incl_amcnlo.root wwz_incl_amcnlo.root wzz_incl_amcnlo.root zzz_incl_amcnlo.root >> dataDrivenEstimates.log

## make "lostlep" from sum of MC lostlep backgrounds and diboson (except QCD, to avoid spikes)
# echo "hadd -f lostlep_diboson.root ttsl.root ttdl.root singletop.root ttw.root ttz.root ttg.root wjets_ht.root ww.root wz.root" ## (temporarily replaced tth with ttg, because of sample availability)
# hadd -f lostlep_diboson.root ttsl.root ttdl.root singletop.root ttw_mg_lo.root ttz_mg_lo.root ttg.root wjets_ht.root ww.root wz.root >> dataDrivenEstimates.log


cd $THISDIR

#this script scales the HI and LOW boundary histograms by 1/numSamples since we don't want these hadd'ed
echo "root -b -q rescaleBoundaryHists.C+(${INDIR}/data_Run2016.root,7)"
root -b -q "rescaleBoundaryHists.C+(\"${INDIR}/data_Run2016.root\",7)" >> dataDrivenEstimates.log

echo "root -b -q rescaleBoundaryHists.C+(${INDIR}/wjets_ht.root,7)"
root -b -q "rescaleBoundaryHists.C+(\"${INDIR}/wjets_ht.root\",7)" >> dataDrivenEstimates.log

echo "root -b -q rescaleBoundaryHists.C+(${INDIR}/top.root,6)"
root -b -q "rescaleBoundaryHists.C+(\"${INDIR}/top.root\",6)" >> dataDrivenEstimates.log

# echo "root -b -q rescaleBoundaryHists.C+(${INDIR}/wz.root,3)"
# root -b -q "rescaleBoundaryHists.C+(\"${INDIR}/wz.root\",3)" >> dataDrivenEstimates.log

# echo "root -b -q rescaleBoundaryHists.C+(${INDIR}/ww.root,2)"
# root -b -q "rescaleBoundaryHists.C+(\"${INDIR}/ww.root\",2)" >> dataDrivenEstimates.log

# echo "root -b -q rescaleBoundaryHists.C+(${INDIR}/zz.root,4)"
# root -b -q "rescaleBoundaryHists.C+(\"${INDIR}/zz.root\",4)" >> dataDrivenEstimates.log

# echo "root -b -q rescaleBoundaryHists.C+(${INDIR}/vv.root,6)"
# root -b -q "rescaleBoundaryHists.C+(\"${INDIR}/vv.root\",6)" >> dataDrivenEstimates.log

# echo "root -b -q rescaleBoundaryHists.C+(${INDIR}/vvv.root,4)"
# root -b -q "rescaleBoundaryHists.C+(\"${INDIR}/vvv.root\",4)" >> dataDrivenEstimates.log

echo "root -b -q rescaleBoundaryHists.C+(${INDIR}/lostlep.root,13)"
root -b -q "rescaleBoundaryHists.C+(\"${INDIR}/lostlep.root\",13)" >> dataDrivenEstimates.log

# echo "root -b -q rescaleBoundaryHists.C+(${INDIR}/lostlep_diboson.root,18)"
# root -b -q "rescaleBoundaryHists.C+(\"${INDIR}/lostlep_diboson.root\",18)" >> dataDrivenEstimates.log

# make the data driven background files
echo "root -b -q lostlepMaker.C+(${INDIR},${LOSTLEPFILE})"
root -b -q "lostlepMaker.C+(\"${INDIR}\",\"${LOSTLEPFILE}\")" >> dataDrivenEstimates.log
echo "root -b -q ZinvMaker.C+(${INDIR})"
root -b -q "ZinvMaker.C+(\"${INDIR}\")" >> dataDrivenEstimates.log
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


