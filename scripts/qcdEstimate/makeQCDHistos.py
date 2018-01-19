#! /usr/bin/env python
## run this on the output of QCDLooper to produce barebones histograms of rphi, fj, rb
## output is stored in directories in a root file. Pass this to makeQCDPlots to make
## pretty plots

import os
import sys
import numpy as np
import ROOT

indir = open('inputs.txt').readlines()[0].strip()
tag = indir.split("/")[-1]
outdir = "output/"+tag

try:
    os.makedirs(outdir)
except:
    pass

qcd_sample = "qcd_ht"
nqcd_samples = ["nonqcd"]
data_sample = "data_Run2016"

ht_reg_names_tmp = ["ht250to450","ht450to575","ht575to1000","ht1000to1500","ht1500toInf","ht1000toInf"]
nj_reg_names_tmp = ["j2to3","j4to6","j7toInf","j2to6","j4toInf","j2toInf"]

ht_reg_names = []
nj_reg_names = []
for name in ht_reg_names_tmp:
    ht_reg_names.append(name+"cent")
    ht_reg_names.append(name+"forw")
for name in nj_reg_names_tmp:
    nj_reg_names.append(name+"cent")
    nj_reg_names.append(name+"forw")

fout = ROOT.TFile(os.path.join(outdir,"qcdHistos.root"),"RECREATE")

## r_phi
for ireg,reg in enumerate(ht_reg_names):
  print "Doing r_phi for region",reg
  
  dirname = "rphi_"+reg
  tdir = fout.mkdir(dirname)

  ## get num/denom histos
  
  # QCD MC
  fin = ROOT.TFile(os.path.join(indir,qcd_sample+".root"))
  try:
    h_qcd_num = fin.Get("rphi_{0}/h_mt2_num".format(reg))
    h_qcd_den = fin.Get("rphi_{0}/h_mt2_den".format(reg))
    h_qcd_num.SetDirectory(0)    
    h_qcd_den.SetDirectory(0)
  except TypeError:
    raise Exception("ERROR: could not get histos from qcd file!!")
  fin.Close()

  # Data
  fin = ROOT.TFile(os.path.join(indir,data_sample+".root"))
  try:
    h_data_num = fin.Get("rphi_{0}/h_mt2_num".format(reg))
    h_data_den = fin.Get("rphi_{0}/h_mt2_den".format(reg))
    h_data_num.SetDirectory(0)
    h_data_den.SetDirectory(0)
  except TypeError:
    raise Exception("ERROR: could not get histos from data file!!")
  fin.Close()

  # non-QCD MC
  h_nonqcd_num = h_qcd_num.Clone("h_nonqcd_num")
  h_nonqcd_num.Reset()
  h_nonqcd_den = h_qcd_num.Clone("h_nonqcd_den")
  h_nonqcd_den.Reset()
  for s in nqcd_samples:
    fin = ROOT.TFile(os.path.join(indir,s+".root"))
    try:
      h_nonqcd_num.Add(fin.Get("rphi_{0}/h_mt2_num".format(reg)))
      h_nonqcd_den.Add(fin.Get("rphi_{0}/h_mt2_den".format(reg)))
    except TypeError:
      ## sometimes some bkg process wont have any events, generates error
      print "WARNING: could not get {0} rphi hists for sample {1}".format(reg,s)
    fin.Close()


  # MCall and non-qcd-subtracted data
  h_mcall_num = h_qcd_num.Clone("h_mcall_num")
  h_mcall_den = h_qcd_den.Clone("h_mcall_den")
  h_datasub_num = h_data_num.Clone("h_datasub_num")
  h_datasub_den = h_data_den.Clone("h_datasub_den")

  h_mcall_num.Add(h_nonqcd_num)
  h_mcall_den.Add(h_nonqcd_den)

  ## GET DATA/MC RATIO
  dataMCsf = h_data_num.Integral(0,-1)/h_mcall_num.Integral(0,-1)

  # print "Data/MC scale factor for region {0} is {1}".format(reg, dataMCsf)
  
  h_datasub_num.Add(h_nonqcd_num,-1*dataMCsf)
  h_datasub_den.Add(h_nonqcd_den,-1*dataMCsf)
 
  #rebin using the following bin edges    
  mt2bins = [50,55,60,65,70,75,80,85,90,95,100,125,200,300,450,800,1500]
  mt2bins = np.array(mt2bins, dtype=float)
  
  h_qcd_num = h_qcd_num.Rebin(mt2bins.size-1, "h_qcd_num", mt2bins)
  h_qcd_den = h_qcd_den.Rebin(mt2bins.size-1, "h_qcd_den", mt2bins)
  h_data_num = h_data_num.Rebin(mt2bins.size-1, "h_data_num", mt2bins)
  h_data_den = h_data_den.Rebin(mt2bins.size-1, "h_data_den", mt2bins)
  h_mcall_num = h_mcall_num.Rebin(mt2bins.size-1, "h_mcall_num", mt2bins)
  h_mcall_den = h_mcall_den.Rebin(mt2bins.size-1, "h_mcall_den", mt2bins)
  h_datasub_num = h_datasub_num.Rebin(mt2bins.size-1, "h_datasub_num", mt2bins)
  h_datasub_den = h_datasub_den.Rebin(mt2bins.size-1, "h_datasub_den", mt2bins)

  # compute rphi
  h_qcd_rphi = h_qcd_num.Clone("h_qcd_rphi")
  h_qcd_rphi.Divide(h_qcd_den)
  h_data_rphi = h_data_num.Clone("h_data_rphi")
  h_data_rphi.Divide(h_data_den)
  h_mcall_rphi = h_mcall_num.Clone("h_mcall_rphi")
  h_mcall_rphi.Divide(h_mcall_den)
  h_datasub_rphi = h_datasub_num.Clone("h_datasub_rphi")
  h_datasub_rphi.Divide(h_datasub_den)

  # perform the fits
  fit_mc = ROOT.TF1("fit_mc","[0]*x^[1]",30,1500)
  fit_mc.SetParameter(0,100)
  fit_mc.SetParLimits(0,20,1e9)
  fit_data = ROOT.TF1("fit_data","[0]*x^[1]",30,1500)
  fit_data.SetParameter(0,100)
  fit_data.SetParLimits(0,20,1e9)
  lowbound = 60 if ireg in [0,1,2] else 70
  highbound = 100
  h_qcd_rphi.Fit("fit_mc","QN","goff",lowbound,highbound)
  h_datasub_rphi.Fit("fit_data","QN","goff",lowbound,highbound)

  # perform the fits with up variation
  fit_mc_systUp = ROOT.TF1("fit_mc_systUp","[0]*x^[1]",30,1500)
  fit_mc_systUp.SetParameter(0,100)
  fit_mc_systUp.SetParLimits(0,20,1e9)
  fit_data_systUp = ROOT.TF1("fit_data_systUp","[0]*x^[1]",30,1500)
  fit_data_systUp.SetParameter(0,100)
  fit_data_systUp.SetParLimits(0,20,1e9)
  lowbound = 60 if ireg in [0,1,2] else 70
  highbound = 125
  h_qcd_rphi.Fit("fit_mc_systUp","QN","goff",lowbound,highbound)
  h_datasub_rphi.Fit("fit_data_systUp","QN","goff",lowbound,highbound)

  # perform the fits with down variation
  fit_mc_systDown = ROOT.TF1("fit_mc_systDown","[0]*x^[1]",30,1500)
  fit_mc_systDown.SetParameter(0,100)
  fit_mc_systDown.SetParLimits(0,20,1e9)
  fit_data_systDown = ROOT.TF1("fit_data_systDown","[0]*x^[1]",30,1500)
  fit_data_systDown.SetParameter(0,100)
  fit_data_systDown.SetParLimits(0,20,1e9)
  lowbound = 55 if ireg in [0,1,2] else 65
  highbound = 100
  h_qcd_rphi.Fit("fit_mc_systDown","QN","goff",lowbound,highbound)
  h_datasub_rphi.Fit("fit_data_systDown","QN","goff",lowbound,highbound)

  # write to directory
  tdir.cd()
  h_qcd_rphi.Write()
  h_data_rphi.Write()
  h_mcall_rphi.Write()
  h_datasub_rphi.Write()
  fit_mc.Write()
  fit_data.Write()
  fit_mc_systUp.Write()
  fit_data_systUp.Write()
  fit_mc_systDown.Write()
  fit_data_systDown.Write()
  # h_qcd_num.Write()
  # h_data_num.Write()
  # h_mcall_num.Write()
  # h_datasub_num.Write()
  # h_qcd_den.Write()
  # h_data_den.Write()
  # h_mcall_den.Write()
  # h_datasub_den.Write()

# f_j
for reg in ht_reg_names:
  print "Doing f_j for region",reg

  dirname = "fj_"+reg
  tdir = fout.mkdir(dirname)

  # QCD MC
  fin = ROOT.TFile(os.path.join(indir,qcd_sample+".root"))
  h_qcd_fj = fin.Get("fj_{0}/h_njets".format(reg))
  h_qcd_fj.SetDirectory(0)
  if type(h_qcd_fj)!=type(ROOT.TH1D()):
    raise Exception("ERROR: could not get {0} fj data from qcd sample!!".format(reg))
  fin.Close()

  # data
  fin = ROOT.TFile(os.path.join(indir,data_sample+".root"))
  h_data_fj = fin.Get("fj_{0}/h_njets".format(reg))
  h_data_fj.SetDirectory(0)
  if type(h_data_fj)!=type(ROOT.TH1D()):
    raise Exception("ERROR: could not get {0} fj data from data sample!!".format(reg))
  fin.Close()

  # non-QCD MC
  h_nonqcd_fj = h_qcd_fj.Clone("h_nonqcd_num")
  h_nonqcd_fj.Reset()
  for s in nqcd_samples:
    fin = ROOT.TFile(os.path.join(indir,s+".root"))
    try:
      h_nonqcd_fj.Add(fin.Get("fj_{0}/h_njets".format(reg)))
    except TypeError:
      ## sometimes some bkg process wont have any events, generates error
      print "WARNING: could not get {0} fj hists for sample {1}".format(reg,s)
    fin.Close()

  # MCall and non-qcd-subtracted data
  h_mcall_fj = h_qcd_fj.Clone("h_mcall_fj")
  h_datasub_fj = h_data_fj.Clone("h_datasub_fj")

  h_mcall_fj.Add(h_nonqcd_fj)

  ## GET DATA/MC RATIO
  dataMCsf = h_data_fj.Integral(0,-1)/h_mcall_fj.Integral(0,-1)
  
  h_datasub_fj.Add(h_nonqcd_fj,-1*dataMCsf)

  #normalize to get proportions
  h_qcd_fj.Scale(1.0/h_qcd_fj.Integral(0,-1))
  h_data_fj.Scale(1.0/h_data_fj.Integral(0,-1))
  h_mcall_fj.Scale(1.0/h_mcall_fj.Integral(0,-1))
  h_datasub_fj.Scale(1.0/h_datasub_fj.Integral(0,-1))

  # rebin
  njbins = [2,4,7,11]
  njbins = np.array(njbins, dtype=float)

  h_qcd_fj = h_qcd_fj.Rebin(njbins.size-1, "h_qcd_fj", njbins)
  h_data_fj = h_data_fj.Rebin(njbins.size-1, "h_data_fj", njbins)
  h_mcall_fj = h_mcall_fj.Rebin(njbins.size-1, "h_mcall_fj", njbins)
  h_datasub_fj = h_datasub_fj.Rebin(njbins.size-1, "h_datasub_fj", njbins)

  hists = [h_qcd_fj, h_data_fj, h_mcall_fj, h_datasub_fj]
  for h in hists:
    h.SetBinError(1, ROOT.TMath.Sqrt(h.GetBinError(1)**2 + (0.25*h.GetBinContent(1))**2))
    h.SetBinError(2, ROOT.TMath.Sqrt(h.GetBinError(2)**2 + (0.07*h.GetBinContent(2))**2))
    h.SetBinError(3, ROOT.TMath.Sqrt(h.GetBinError(3)**2 + (0.20*h.GetBinContent(3))**2))

  tdir.cd()
  h_qcd_fj.Write()
  h_data_fj.Write()
  h_mcall_fj.Write()
  h_datasub_fj.Write()


# r_b
for reg in nj_reg_names:
  print "Doing r_b for region",reg

  dirname = "rb_"+reg
  tdir = fout.mkdir(dirname)

  # QCD MC
  fin = ROOT.TFile(os.path.join(indir,qcd_sample+".root"))
  h_qcd_rb = fin.Get("rb_{0}/h_nbjets".format(reg))
  h_qcd_rb.SetDirectory(0)
  if type(h_qcd_rb)!=type(ROOT.TH1D()):
    raise Exception("ERROR: could not get {0} rb data from qcd sample!!".format(reg))
  fin.Close()

  # data
  fin = ROOT.TFile(os.path.join(indir,data_sample+".root"))
  h_data_rb = fin.Get("rb_{0}/h_nbjets".format(reg))
  h_data_rb.SetDirectory(0)
  if type(h_data_rb)!=type(ROOT.TH1D()):
    raise Exception("ERROR: could not get {0} rb data from data sample!!".format(reg))
  fin.Close()

  # non-QCD MC
  h_nonqcd_rb = h_qcd_rb.Clone("h_nonqcd_num")
  h_nonqcd_rb.Reset()
  for s in nqcd_samples:
    fin = ROOT.TFile(os.path.join(indir,s+".root"))
    try:
      h_nonqcd_rb.Add(fin.Get("rb_{0}/h_nbjets".format(reg)))
    except TypeError:
      ## sometimes some bkg process wont have any events, generates error
      print "WARNING: could not get {0} rb hists for sample {1}".format(reg,s)
    fin.Close()


  # MCall and non-qcd-subtracted data
  h_mcall_rb = h_qcd_rb.Clone("h_mcall_rb")
  h_datasub_rb = h_data_rb.Clone("h_datasub_rb")

  h_mcall_rb.Add(h_nonqcd_rb)

  ## GET DATA/MC RATIO
  dataMCsf = h_data_rb.Integral(0,-1)/h_mcall_rb.Integral(0,-1)
  
  h_datasub_rb.Add(h_nonqcd_rb,-1*dataMCsf)

  # normalize to get proportion
  h_qcd_rb.Scale(1.0/h_qcd_rb.Integral(0,-1))
  h_data_rb.Scale(1.0/h_data_rb.Integral(0,-1))
  h_mcall_rb.Scale(1.0/h_mcall_rb.Integral(0,-1))
  h_datasub_rb.Scale(1.0/h_datasub_rb.Integral(0,-1))

  nbjbins = [0,1,2,3,6]
  nbjbins = np.array(nbjbins, dtype=float)

  h_qcd_rb = h_qcd_rb.Rebin(nbjbins.size-1, "h_qcd_rb", nbjbins)
  h_data_rb = h_data_rb.Rebin(nbjbins.size-1, "h_data_rb", nbjbins)
  h_mcall_rb = h_mcall_rb.Rebin(nbjbins.size-1, "h_mcall_rb", nbjbins)
  h_datasub_rb = h_datasub_rb.Rebin(nbjbins.size-1, "h_datasub_rb", nbjbins)

  hists = [h_qcd_rb, h_data_rb, h_mcall_rb, h_datasub_rb]
  for h in hists:
    h.SetBinError(1, ROOT.TMath.Sqrt(h.GetBinError(1)**2 + (0.08*h.GetBinContent(1))**2))
    h.SetBinError(2, ROOT.TMath.Sqrt(h.GetBinError(2)**2 + (0.20*h.GetBinContent(2))**2))
    h.SetBinError(3, ROOT.TMath.Sqrt(h.GetBinError(3)**2 + (0.35*h.GetBinContent(3))**2))
    h.SetBinError(4, ROOT.TMath.Sqrt(h.GetBinError(4)**2 + (0.70*h.GetBinContent(4))**2))

  tdir.cd()
  h_qcd_rb.Write()
  h_data_rb.Write()
  h_mcall_rb.Write()
  h_datasub_rb.Write()


