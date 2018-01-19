import os
import sys

d_ds2name = {
    "backgrounds": {
### Wjets
        "/WJetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"                         : "wjets_incl",
        "/WJetsToLNu_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"             : "wjets_ht100to200_nonext",    
        "/WJetsToLNu_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM"        : "wjets_ht100to200_ext1",
        "/WJetsToLNu_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"             : "wjets_ht200to400_nonext",
        "/WJetsToLNu_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM"        : "wjets_ht200to400_ext1",
        "/WJetsToLNu_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext2-v1/MINIAODSIM"        : "wjets_ht200to400_ext2",
        "/WJetsToLNu_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"             : "wjets_ht400to600_nonext",
        "/WJetsToLNu_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM"        : "wjets_ht400to600_ext1",
        "/WJetsToLNu_HT-600To800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"             : "wjets_ht600to800_nonext",
        "/WJetsToLNu_HT-600To800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM"        : "wjets_ht600to800_ext1",
        "/WJetsToLNu_HT-800To1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM"       : "wjets_ht800to1200_ext1",
        "/WJetsToLNu_HT-1200To2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"           : "wjets_ht1200to2500_nonext",
        "/WJetsToLNu_HT-2500ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"            : "wjets_ht2500toInf_nonext",
        "/WJetsToLNu_HT-2500ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM"       : "wjets_ht2500toInf_ext1",
### DYToLL
        "/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v2/MINIAODSIM"               : "dyjetsll_incl",
        "/DYJetsToLL_M-50_HT-100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"        : "dyjetsll_ht100to200_nonext",
        "/DYJetsToLL_M-50_HT-200to400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"        : "dyjetsll_ht200to400_nonext",
        "/DYJetsToLL_M-50_HT-200to400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM"   : "dyjetsll_ht200to400_ext1",
        "/DYJetsToLL_M-50_HT-400to600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"        : "dyjetsll_ht400to600_nonext",
        "/DYJetsToLL_M-50_HT-400to600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM"   : "dyjetsll_ht400to600_ext1",
        "/DYJetsToLL_M-50_HT-600to800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2/MINIAODSIM"        : "dyjetsll_ht600to800_nonext",
        "/DYJetsToLL_M-50_HT-800to1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"       : "dyjetsll_ht800to1200_nonext",
        "/DYJetsToLL_M-50_HT-1200to2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"      : "dyjetsll_ht1200to2500_nonext",
        "/DYJetsToLL_M-50_HT-2500toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"       : "dyjetsll_ht2500toInf_nonext",
### zinv
        "/ZJetsToNuNu_HT-100To200_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"                                    : "zinv_ht100to200_nonext",
        "/ZJetsToNuNu_HT-200To400_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"                                    : "zinv_ht200to400_nonext",
        "/ZJetsToNuNu_HT-400To600_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM"                               : "zinv_ht400to600_ext1",
        "/ZJetsToNuNu_HT-600To800_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"                                    : "zinv_ht600to800_nonext",
        "/ZJetsToNuNu_HT-800To1200_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"                                   : "zinv_ht800to1200_nonext",
        "/ZJetsToNuNu_HT-1200To2500_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"                                  : "zinv_ht1200to2500_nonext",
        "/ZJetsToNuNu_HT-2500ToInf_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"                                   : "zinv_ht2500toInf_nonext",
### GJets dr0p4
        "/GJets_DR-0p4_HT-40To100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_qcut19_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"     : "gjets_dr0p4_ht40to100",
        "/GJets_DR-0p4_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_qcut19_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"    : "gjets_dr0p4_ht100to200",
        "/GJets_DR-0p4_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_qcut19_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"    : "gjets_dr0p4_ht200to400",
        "/GJets_DR-0p4_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_qcut19_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"    : "gjets_dr0p4_ht400to600",
        "/GJets_DR-0p4_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_qcut19_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"    : "gjets_dr0p4_ht600toInf",
### GJets dr0p05
        "/GJets_HT-40To100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM"              : "gjets_dr0p05_ht40to100_ext1",
        "/GJets_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"                  : "gjets_dr0p05_ht100to200_nonext",
        "/GJets_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM"             : "gjets_dr0p05_ht200to400_ext1",
        "/GJets_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"                  : "gjets_dr0p05_ht400to600_nonext",
        "/GJets_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"                  : "gjets_dr0p05_ht600toInf_nonext",
        "/GJets_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM"             : "gjets_dr0p05_ht600toInf_ext1",
### ttbar
        "/TTJets_SingleLeptFromT_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"             : "ttsl_mg_lo_top_nonext",
        "/TTJets_SingleLeptFromT_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM"        : "ttsl_mg_lo_top_ext1",
        "/TTJets_SingleLeptFromTbar_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"          : "ttsl_mg_lo_tbar_nonext", 
        "/TTJets_SingleLeptFromTbar_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM"     : "ttsl_mg_lo_tbar_ext1", 
        "/TTJets_DiLept_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"                      : "ttdl_mg_lo_nonext",
        "/TTJets_DiLept_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM"                 : "ttdl_mg_lo_ext1",
### single top
        "/ST_s-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"                    : "singletop_amcatnlo_4f_schan", 
        "/ST_t-channel_antitop_4f_inclusiveDecays_13TeV-powhegV2-madspin-pythia8_TuneCUETP8M1/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM" : "singletop_powheg_4f_tbartchan",
        "/ST_tW_top_5f_NoFullyHadronicDecays_13TeV-powheg_TuneCUETP8M1/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"                        : "singletop_powheg_5f_tWchan_l",
        "/ST_tW_antitop_5f_NoFullyHadronicDecays_13TeV-powheg_TuneCUETP8M1/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"                    : "singletop_powheg_5f_tbarWchan_l",
        "/ST_tWll_5f_LO_13TeV-MadGraph-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"                                                : "twz_ll_mg_lo",
### diboson
        "/WWTo2L2Nu_13TeV-powheg/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"                          : "ww_2l2nu_powheg",
#         "/WWToLNuQQ_13TeV-powheg/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"                          : "ww_lnuqq_powheg",
#         "/WZTo1L1Nu2Q_13TeV_amcatnloFXFX_madspin_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v3/MINIAODSIM"  : "wz_lnqq_amcnlo",
#         "/WZTo1L3Nu_13TeV_amcatnloFXFX_madspin_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"    : "wz_1l3n_amcnlo",
#         "/WZTo3LNu_TuneCUETP8M1_13TeV-powheg-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"      : "wz_3lnu_powheg",
#         "/ZZTo2L2Nu_13TeV_powheg_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"                  : "zz_2l2n_powheg",
#         "/ZZTo2L2Q_13TeV_amcatnloFXFX_madspin_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"     : "zz_2l2q_powheg",
#         "/ZZTo2Q2Nu_13TeV_amcatnloFXFX_madspin_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"    : "zz_2q2n_amcnlo",
#         "/ZZTo4L_13TeV_powheg_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"                     : "zz_4l_powheg",
# ### triboson
#         "/WWW_4F_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"  : "www_incl_amcnlo",
#         "/WWZ_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"     : "wwz_incl_amcnlo",
#         "/WZZ_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"     : "wzz_incl_amcnlo",
#         "/ZZZ_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"     : "zzz_incl_amcnlo",
### ttV
        "/TTGJets_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"                   : "ttg_amcatnlo",
        "/ttZJets_13TeV_madgraphMLM/RunIISummer16MiniAODv2-80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"                                                             : "ttz_mg_lo",
        "/ttWJets_13TeV_madgraphMLM/RunIISummer16MiniAODv2-80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"                                                             : "ttw_mg_lo",
        "/ttHTobb_M125_13TeV_powheg_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"                                         : "tth_bb",
        "/ttHToNonbb_M125_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"             : "tth_nonbb",
        "/TTTT_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"                                  : "tttt_amcatnlo",
### QCD HT-binned
        "/QCD_HT100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"                     : "qcd_ht100to200_nonext", 
        "/QCD_HT200to300_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"                     : "qcd_ht200to300_nonext", 
        "/QCD_HT200to300_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM"                : "qcd_ht200to300_ext1", 
        "/QCD_HT300to500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"                     : "qcd_ht300to500_nonext", 
        "/QCD_HT300to500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM"                : "qcd_ht300to500_ext1", 
        "/QCD_HT500to700_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"                     : "qcd_ht500to700_nonext", 
        "/QCD_HT500to700_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v2/MINIAODSIM"                : "qcd_ht500to700_ext1", 
        "/QCD_HT700to1000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"                    : "qcd_ht700to1000_nonext", 
        "/QCD_HT700to1000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM"               : "qcd_ht700to1000_ext1", 
        "/QCD_HT1000to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"                   : "qcd_ht1000to1500_nonext", 
        "/QCD_HT1000to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM"              : "qcd_ht1000to1500_ext1", 
        "/QCD_HT1500to2000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"                   : "qcd_ht1500to2000_nonext", 
        "/QCD_HT1500to2000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM"              : "qcd_ht1500to2000_ext1", 
        "/QCD_HT2000toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"                    : "qcd_ht2000toInf_nonext", 
        "/QCD_HT2000toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM"               : "qcd_ht2000toInf_ext1",
    },
    "data": {
        "/JetHT/Run2016B-03Feb2017_ver2-v2/MINIAOD"           : "data_Run2016B_JetHT_03Feb2017",
        "/HTMHT/Run2016B-03Feb2017_ver2-v2/MINIAOD"           : "data_Run2016B_HTMHT_03Feb2017",
        "/MET/Run2016B-03Feb2017_ver2-v2/MINIAOD"             : "data_Run2016B_MET_03Feb2017",
        "/SingleElectron/Run2016B-03Feb2017_ver2-v2/MINIAOD"  : "data_Run2016B_SingleElectron_03Feb2017",
        "/SingleMuon/Run2016B-03Feb2017_ver2-v2/MINIAOD"      : "data_Run2016B_SingleMuon_03Feb2017",
        "/SinglePhoton/Run2016B-03Feb2017_ver2-v2/MINIAOD"    : "data_Run2016B_SinglePhoton_03Feb2017",
        "/DoubleEG/Run2016B-03Feb2017_ver2-v2/MINIAOD"        : "data_Run2016B_DoubleEG_03Feb2017",
        "/DoubleMuon/Run2016B-03Feb2017_ver2-v2/MINIAOD"      : "data_Run2016B_DoubleMuon_03Feb2017",
        "/MuonEG/Run2016B-03Feb2017_ver2-v2/MINIAOD"          : "data_Run2016B_MuonEG_03Feb2017",

        "/JetHT/Run2016C-03Feb2017-v1/MINIAOD"           : "data_Run2016C_JetHT_03Feb2017",
        "/HTMHT/Run2016C-03Feb2017-v1/MINIAOD"           : "data_Run2016C_HTMHT_03Feb2017",
        "/MET/Run2016C-03Feb2017-v1/MINIAOD"             : "data_Run2016C_MET_03Feb2017",
        "/SingleElectron/Run2016C-03Feb2017-v1/MINIAOD"  : "data_Run2016C_SingleElectron_03Feb2017",
        "/SingleMuon/Run2016C-03Feb2017-v1/MINIAOD"      : "data_Run2016C_SingleMuon_03Feb2017",
        "/SinglePhoton/Run2016C-03Feb2017-v1/MINIAOD"    : "data_Run2016C_SinglePhoton_03Feb2017",
        "/DoubleEG/Run2016C-03Feb2017-v1/MINIAOD"        : "data_Run2016C_DoubleEG_03Feb2017",
        "/DoubleMuon/Run2016C-03Feb2017-v1/MINIAOD"      : "data_Run2016C_DoubleMuon_03Feb2017",
        "/MuonEG/Run2016C-03Feb2017-v1/MINIAOD"          : "data_Run2016C_MuonEG_03Feb2017",

        "/JetHT/Run2016D-03Feb2017-v1/MINIAOD"           : "data_Run2016D_JetHT_03Feb2017",
        "/HTMHT/Run2016D-03Feb2017-v1/MINIAOD"           : "data_Run2016D_HTMHT_03Feb2017",
        "/MET/Run2016D-03Feb2017-v1/MINIAOD"             : "data_Run2016D_MET_03Feb2017",
        "/SingleElectron/Run2016D-03Feb2017-v1/MINIAOD"  : "data_Run2016D_SingleElectron_03Feb2017",
        "/SingleMuon/Run2016D-03Feb2017-v1/MINIAOD"      : "data_Run2016D_SingleMuon_03Feb2017",
        "/SinglePhoton/Run2016D-03Feb2017-v1/MINIAOD"    : "data_Run2016D_SinglePhoton_03Feb2017",
        "/DoubleEG/Run2016D-03Feb2017-v1/MINIAOD"        : "data_Run2016D_DoubleEG_03Feb2017",
        "/DoubleMuon/Run2016D-03Feb2017-v1/MINIAOD"      : "data_Run2016D_DoubleMuon_03Feb2017",
        "/MuonEG/Run2016D-03Feb2017-v1/MINIAOD"          : "data_Run2016D_MuonEG_03Feb2017",

        "/JetHT/Run2016E-03Feb2017-v1/MINIAOD"           : "data_Run2016E_JetHT_03Feb2017",
        "/HTMHT/Run2016E-03Feb2017-v1/MINIAOD"           : "data_Run2016E_HTMHT_03Feb2017",
        "/MET/Run2016E-03Feb2017-v1/MINIAOD"             : "data_Run2016E_MET_03Feb2017",
        "/SingleElectron/Run2016E-03Feb2017-v1/MINIAOD"  : "data_Run2016E_SingleElectron_03Feb2017",
        "/SingleMuon/Run2016E-03Feb2017-v1/MINIAOD"      : "data_Run2016E_SingleMuon_03Feb2017",
        "/SinglePhoton/Run2016E-03Feb2017-v1/MINIAOD"    : "data_Run2016E_SinglePhoton_03Feb2017",
        "/DoubleEG/Run2016E-03Feb2017-v1/MINIAOD"        : "data_Run2016E_DoubleEG_03Feb2017",
        "/DoubleMuon/Run2016E-03Feb2017-v1/MINIAOD"      : "data_Run2016E_DoubleMuon_03Feb2017",
        "/MuonEG/Run2016E-03Feb2017-v1/MINIAOD"          : "data_Run2016E_MuonEG_03Feb2017",

        "/JetHT/Run2016F-03Feb2017-v1/MINIAOD"           : "data_Run2016F_JetHT_03Feb2017",
        "/HTMHT/Run2016F-03Feb2017-v1/MINIAOD"           : "data_Run2016F_HTMHT_03Feb2017",
        "/MET/Run2016F-03Feb2017-v1/MINIAOD"             : "data_Run2016F_MET_03Feb2017",
        "/SingleElectron/Run2016F-03Feb2017-v1/MINIAOD"  : "data_Run2016F_SingleElectron_03Feb2017",
        "/SingleMuon/Run2016F-03Feb2017-v1/MINIAOD"      : "data_Run2016F_SingleMuon_03Feb2017",
        "/SinglePhoton/Run2016F-03Feb2017-v1/MINIAOD"    : "data_Run2016F_SinglePhoton_03Feb2017",
        "/DoubleEG/Run2016F-03Feb2017-v1/MINIAOD"        : "data_Run2016F_DoubleEG_03Feb2017",
        "/DoubleMuon/Run2016F-03Feb2017-v1/MINIAOD"      : "data_Run2016F_DoubleMuon_03Feb2017",
        "/MuonEG/Run2016F-03Feb2017-v1/MINIAOD"          : "data_Run2016F_MuonEG_03Feb2017",

        "/JetHT/Run2016G-03Feb2017-v1/MINIAOD"           : "data_Run2016G_JetHT_03Feb2017",
        "/HTMHT/Run2016G-03Feb2017-v1/MINIAOD"           : "data_Run2016G_HTMHT_03Feb2017",
        "/MET/Run2016G-03Feb2017-v1/MINIAOD"             : "data_Run2016G_MET_03Feb2017",
        "/SingleElectron/Run2016G-03Feb2017-v1/MINIAOD"  : "data_Run2016G_SingleElectron_03Feb2017",
        "/SingleMuon/Run2016G-03Feb2017-v1/MINIAOD"      : "data_Run2016G_SingleMuon_03Feb2017",
        "/SinglePhoton/Run2016G-03Feb2017-v1/MINIAOD"    : "data_Run2016G_SinglePhoton_03Feb2017",
        "/DoubleEG/Run2016G-03Feb2017-v1/MINIAOD"        : "data_Run2016G_DoubleEG_03Feb2017",
        "/DoubleMuon/Run2016G-03Feb2017-v1/MINIAOD"      : "data_Run2016G_DoubleMuon_03Feb2017",
        "/MuonEG/Run2016G-03Feb2017-v1/MINIAOD"          : "data_Run2016G_MuonEG_03Feb2017",

        "/JetHT/Run2016H-03Feb2017_ver2-v1/MINIAOD"          : "data_Run2016H_JetHT_03Feb2017_ver2-v1",
        "/JetHT/Run2016H-03Feb2017_ver3-v1/MINIAOD"          : "data_Run2016H_JetHT_03Feb2017_ver3-v1",
        "/HTMHT/Run2016H-03Feb2017_ver2-v1/MINIAOD"          : "data_Run2016H_HTMHT_03Feb2017_ver2-v1",
        "/HTMHT/Run2016H-03Feb2017_ver3-v1/MINIAOD"          : "data_Run2016H_HTMHT_03Feb2017_ver3-v1",
        "/MET/Run2016H-03Feb2017_ver2-v1/MINIAOD"            : "data_Run2016H_MET_03Feb2017_ver2-v1", 
        "/MET/Run2016H-03Feb2017_ver3-v1/MINIAOD"            : "data_Run2016H_MET_03Feb2017_ver3-v1",
        "/SingleElectron/Run2016H-03Feb2017_ver2-v1/MINIAOD" : "data_Run2016H_SingleElectron_03Feb2017_ver2-v1",
        "/SingleElectron/Run2016H-03Feb2017_ver3-v1/MINIAOD" : "data_Run2016H_SingleElectron_03Feb2017_ver3-v1",
        "/SingleMuon/Run2016H-03Feb2017_ver2-v1/MINIAOD"     : "data_Run2016H_SingleMuon_03Feb2017_ver2-v1",
        "/SingleMuon/Run2016H-03Feb2017_ver3-v1/MINIAOD"     : "data_Run2016H_SingleMuon_03Feb2017_ver3-v1",
        "/SinglePhoton/Run2016H-03Feb2017_ver2-v1/MINIAOD"   : "data_Run2016H_SinglePhoton_03Feb2017_ver2-v1",
        "/SinglePhoton/Run2016H-03Feb2017_ver3-v1/MINIAOD"   : "data_Run2016H_SinglePhoton_03Feb2017_ver3-v1",
        "/DoubleEG/Run2016H-03Feb2017_ver2-v1/MINIAOD"       : "data_Run2016H_DoubleEG_03Feb2017_ver2-v1",
        "/DoubleEG/Run2016H-03Feb2017_ver3-v1/MINIAOD"       : "data_Run2016H_DoubleEG_03Feb2017_ver3-v1",
        "/DoubleMuon/Run2016H-03Feb2017_ver2-v1/MINIAOD"     : "data_Run2016H_DoubleMuon_03Feb2017_ver2-v1",
        "/DoubleMuon/Run2016H-03Feb2017_ver3-v1/MINIAOD"     : "data_Run2016H_DoubleMuon_03Feb2017_ver3-v1",
        "/MuonEG/Run2016H-03Feb2017_ver2-v1/MINIAOD"         : "data_Run2016H_MuonEG_03Feb2017_ver2-v1",
        "/MuonEG/Run2016H-03Feb2017_ver3-v1/MINIAOD"         : "data_Run2016H_MuonEG_03Feb2017_ver3-v1"
    },
    "scans": {
        "/SMS-T1tttt_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16Fast_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM"         : "T1tttt",
        "/SMS-T1bbbb_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16Fast_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM"         : "T1bbbb",
        "/SMS-T1qqqq_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16Fast_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM"         : "T1qqqq",
        "/SMS-T2bb_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16Fast_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM"           : "T2bb",
        "/SMS-T2tt_mStop-400to1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16Fast_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM"   : "T2tt_mStop-400to1200",
        "/SMS-T2tt_mStop-150to250_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16Fast_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM"    : "T2tt_mStop-150to250",
        "/SMS-T2tt_mStop-250to350_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16Fast_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM"    : "T2tt_mStop-250to350",
        "/SMS-T2tt_mStop-350to400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16Fast_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM"    : "T2tt_mStop-350to400",
        "/SMS-T2qq_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16Fast_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM"           : "T2qq",
        "/SMS-T2cc_genHT-160_genMET-80_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16Fast_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM"  : "T2cc",
        "/SMS-T2bW_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16Fast_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM"           : "T2bW",
        "/SMS-T2bt_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16Fast_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM"           : "T2bt",
    }
}

# for t,samples in d_ds2name.items():
#     for sample in samples.keys():
#         print "%s: baby_remerge"%sample

def dataset_to_shortname(ds):
    for typ_values in d_ds2name.values():
        for dname in typ_values:
            if dname == ds:
                return typ_values[dname]
    return "unknown"

inputs_path = os.path.abspath(__file__).rsplit("/",1)[0]+"/job_input/"
sweepRoot_scripts = [inputs_path+"sweeproot.sh", inputs_path+"sweeproot_macro.C"]
merging_scripts = [inputs_path+"merge_script.sh", inputs_path+"merge_macro.C"]

tag = "test"
package = inputs_path+"package.tar.gz"
executable = inputs_path+"wrapper_auto.sh"
dashboard_name = "AutoTwopler_MT2babies"
baby_merged_dir = "/hadoop/cms/store/user/%s/AutoTwopler_babies/merged/"%(os.getenv("USER"))
merge_babies_on_condor = True

if not os.path.isfile(package):
    raise Exception("Woah! Did you run make_job_inputs.sh? I don't see the package.tar.gz file in %s" % inputs_path)


