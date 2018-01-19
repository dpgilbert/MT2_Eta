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
### ttbar semilep
        "/TTJets_SingleLeptFromT_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"             : "ttsl_mg_lo_top_nonext",
        "/TTJets_SingleLeptFromT_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM"        : "ttsl_mg_lo_top_ext1",
        "/TTJets_SingleLeptFromTbar_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"          : "ttsl_mg_lo_tbar_nonext", 
        "/TTJets_SingleLeptFromTbar_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM"     : "ttsl_mg_lo_tbar_ext1"
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


