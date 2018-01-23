#!/bin/bash

#OUTDIR=output/full2016_PseudoJetEta
OUTDIR=output/debug

pushd $OUTDIR

hadd -f data_Run2016.root data*

hadd -f nonqcd.root dyjetsll_ht.root gjets_dr0p05_ht.root singletop.root ttdl.root ttg.root ttsl.root ttw_mg_lo.root ttz_mg_lo.root wjets_ht*.root

popd