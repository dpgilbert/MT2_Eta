import sys
sys.path.insert(0,"/home/users/dpgilber/MT2AnalysisEdit/NtupleTools/AutoTwopler")
import run
import params as p
import mt2 as mt2

# set tag
mt2.tag = "AllPF"

# make instructions
instructions = []

##
## make instructions by class of sample
##
samples_types = ["backgrounds", "scans"]
#samples_types = ["scans"]
for stype in samples_types:
    for ds in mt2.d_ds2name[stype].keys():
        instructions.append({"executable": mt2.executable, "package": mt2.package, "analysis": "MT2", "dataset": ds, "baby_tag": mt2.tag, "type": "BABY", "extra": [-1, "output.root,skim.root,skim_rphi.root"]})

##
## make instructions by sample shortname
##
#todo = "wjets_incl wjets_ht100to200 wjets_ht2500toInf ".strip().split()
#todo = "gjets_dr0p05_ht400to600 ".strip().split()
#instructions = [inst for inst in instructions if mt2.dataset_to_shortname(inst["dataset"]) in todo]

#print instructions
#sys.exit()

p.dataset_to_shortname = mt2.dataset_to_shortname
p.dashboard_name = mt2.dashboard_name
p.sweepRoot_scripts = mt2.sweepRoot_scripts
p.merging_scripts = mt2.merging_scripts
p.baby_merged_dir = mt2.baby_merged_dir
p.merge_babies_on_condor = mt2.merge_babies_on_condor
p.exit_when_done = True

run.main(instructions=instructions, params=p)

# instructions = [ 
#         {
#             "dataset": "/WJetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM",
#             "type": "BABY",
#             "extra": "",
#             "baby_tag": "v0.01",
#             "analysis": "MT2",
#             "package": ss.package,
#             "executable": ss.executable,
#             }
#         ]

# p.merging_scripts = ["frank/merge.sh", "frank/merge.C"]
# p.dashboard_name = "AutoTwopler_test"
# p.merge_babies_on_condor = True
# run.main(instructions=instructions, params=p, do_one_iteration=True)
