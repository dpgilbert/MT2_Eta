#!/bin/bash

make -j 8 || return $?

#./processBaby data_json_test /home/users/jgran/temp/74x_sync/CMSSW_7_4_3/src/CMS3/NtupleMaker/ntuple_data.root
#./processBaby data_test  /nfs-7/userdata/jgran/74x_sync/ntuple_data.root
#./processBaby data_json_fixrho  /home/users/jgran/temp/electronID/CMSSW_7_4_1_patch1/src/CMS3/NtupleMaker/ntuple_rereco.root
#./processBaby qcd_CMS3-07-04-03_test  /hadoop/cms/store/group/snt/run2_25ns/QCD_Pt_300to470_TuneCUETP8M1_13TeV_pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/V07-04-03/merged_ntuple_31.root
#./processBaby qcd_test_noJEC_fixjetID ./ntuple_QCD.root
#./processBaby qcd_test_noJEC_fixjetID_fixgamma /home/users/gzevi/ntupling/CMSSW_7_4_1_patch1/src/CMS3/NtupleMaker/ntupleQCD_Pt_1000to1400_10000.root

##### Run2 datasets for sync - note that these do NOT have the postprocessing vars
#./processBaby test_ttbar_1k /nfs-7/userdata/olivito/mt2/run2_25ns_MiniAODv2/ntuple_ttbar_25ns_1k.root
#./processBaby test_ttbar_10k /nfs-7/userdata/olivito/mt2/run2_25ns_MiniAODv2/ntuple_ttbar_25ns_10k.root
#./processBaby test_data_JetHT_1k /nfs-7/userdata/olivito/mt2/run2_25ns_MiniAODv2/ntuple_data_Run2015D_JetHT_PromptReco-v4_1k.root
#./processBaby test_data_JetHT_10k /nfs-7/userdata/olivito/mt2/run2_25ns_MiniAODv2/ntuple_data_Run2015D_JetHT_PromptReco-v4_10k.root

#./processBaby test_T2tt /hadoop/cms/store/group/snt/run2_fastsim_v1/SMS-T2tt_mStop-600-950_mLSP-1to450_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15FSPremix-MCRUN2_74_V9-v1/V07-04-12_miniaodv1_FS/merged_ntuple_1.root

#./processBaby test_ttbar_80X /nfs-7/userdata/bemarsh/scratch/TTJets_SingleLeptFromT_25ns_80X_All.root

./processBaby test_AllTrack /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv2_fastsim/SMS-T1tttt_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring16MiniAODv2-PUSpring16Fast_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/V08-00-09/merged_ntuple_1.root 1000