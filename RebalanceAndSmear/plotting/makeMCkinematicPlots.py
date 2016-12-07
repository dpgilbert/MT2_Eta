import ROOT
import os
import pyRootPlotMaker as ppm

ROOT.gROOT.SetBatch(1)

dir = "looper_output/RebalanceAndSmear_V00-08-12_tail50"
dir_noRS = "looper_output/RebalanceAndSmear_V00-08-12_noRS"

frse = ROOT.TFile(os.path.join(dir,"qcd_ht_ext.root"))
frsne = ROOT.TFile(os.path.join(dir,"qcd_ht_nonext.root"))
fnrse = ROOT.TFile(os.path.join(dir_noRS,"qcd_ht_ext.root"))
fnrsne = ROOT.TFile(os.path.join(dir_noRS,"qcd_ht_nonext.root"))

vars = ["met","ht","mt2","nJet30","nBJet20","J0pt","J1pt","deltaPhiMin","diffMetMhtOverMet"]
names = ["E_{T}^{miss}","H_{T}","M_{T2}","nJet30","nBJet20","p_{T}(jet1)","p_{T}(jet2)","#Delta#phi_{min}","|met - mht|/met"]
units = ["GeV","GeV","GeV",None,None,"GeV","GeV",None,None]
rebin = [2,2,2,1,1,2,2,1,2]

for tdir in ["ht450to1000","ht1000toInf"]:
    for ivar,var in enumerate(vars):
        
        hrs = frse.Get("{0}/h_{1}".format(tdir, var))
        hrs.Scale(0.686)
        hrs.Add(frsne.Get("{0}/h_{1}".format(tdir,var)), 1-0.686)

        hnrs = fnrse.Get("{0}/h_{1}".format(tdir, var))
        hnrs.Scale(0.686)
        hnrs.Add(fnrsne.Get("{0}/h_{1}".format(tdir,var)), 1-0.686)

        hrs.Rebin(rebin[ivar])
        hnrs.Rebin(rebin[ivar])

        for ext in [".pdf",".png"]:
            saveAs = "/home/users/bemarsh/public_html/mt2/RebalanceAndSmear/MCtests/tmp/{0}/{0}_{1}{2}".format(tdir,var,ext)
            ppm.plotDataMC([hnrs], ["QCD MC"], h_data=hrs, dataTitle="R&S from MC", saveAs=saveAs, 
                           xAxisTitle=names[ivar], xAxisUnit=units[ivar])
