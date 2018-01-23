#! /usr/bin/env python
## run this on the output of makeQCDHistos.py to get pretty plots

import os
import sys
import ROOT
import QCDPlotMaker as pm

ROOT.gROOT.SetBatch(1)

indir = open('inputs.txt').readlines()[0].strip()
tag = indir.split("/")[-1]

infile = "output/{0}/qcdHistos.root".format(tag)
outdir = "qcd_plots/"+tag

try:
    os.makedirs(outdir)
except:
    pass

fin = ROOT.TFile(infile)

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

## r_phi
for reg in ht_reg_names:
    h_qcd = fin.Get("rphi_{0}/h_qcd_rphi".format(reg))
    h_all = fin.Get("rphi_{0}/h_mcall_rphi".format(reg))
    fit_mc = fin.Get("rphi_{0}/fit_mc".format(reg))
    fit_mc_systUp = fin.Get("rphi_{0}/fit_mc_systUp".format(reg))
    fit_mc_systDown = fin.Get("rphi_{0}/fit_mc_systDown".format(reg))
    if type(h_qcd)!=type(ROOT.TH1D()) or type(h_all)!=type(ROOT.TH1D()):
        raise Exception("ERROR: could not get MC rphi hists!!")
    outfile = os.path.join(outdir,"rphi_MC_{0}".format(reg))
    pm.makeRphiPlot(h_all, h_qcd, fit_mc, reg, False, outfile, fit_systUp=fit_mc_systUp, fit_systDown=fit_mc_systDown)

    h_qcd = fin.Get("rphi_{0}/h_datasub_rphi".format(reg))
    h_all = fin.Get("rphi_{0}/h_data_rphi".format(reg))
    fit_data = fin.Get("rphi_{0}/fit_data".format(reg))
    fit_data_systUp = fin.Get("rphi_{0}/fit_data_systUp".format(reg))
    fit_data_systDown = fin.Get("rphi_{0}/fit_data_systDown".format(reg))
    if type(h_qcd)!=type(ROOT.TH1D()) or type(h_all)!=type(ROOT.TH1D()):
        raise Exception("ERROR: could not get data rphi hists!!")
    outfile = os.path.join(outdir,"rphi_data_{0}".format(reg))
    pm.makeRphiPlot(h_all, h_qcd, fit_data, reg, True, outfile, fit_systUp=fit_data_systUp, fit_systDown=fit_data_systDown)

## f_j
for reg in ht_reg_names:
    h_qcd = fin.Get("fj_{0}/h_qcd_fj".format(reg))
    h_data = fin.Get("fj_{0}/h_datasub_fj".format(reg))
    if type(h_qcd)!=type(ROOT.TH1D()) or type(h_data)!=type(ROOT.TH1D()):
        raise Exception("ERROR: could not get f_j hists!!")
    outfile = os.path.join(outdir,"fj_{0}".format(reg))
    pm.makeFjPlot(h_qcd,h_data, reg, outfile)

## r_b
for reg in nj_reg_names:
    h_qcd = fin.Get("rb_{0}/h_qcd_rb".format(reg))
    h_data = fin.Get("rb_{0}/h_datasub_rb".format(reg))
    if type(h_qcd)!=type(ROOT.TH1D()) or type(h_data)!=type(ROOT.TH1D()):
        raise Exception("ERROR: could not get r_b hists!!")
    outfile = os.path.join(outdir,"rb_{0}".format(reg))
    pm.makeRbPlot(h_qcd,h_data, reg, outfile)

fin.Close()
