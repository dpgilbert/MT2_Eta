## for use within makeQCDPlots.py

import os
import sys
import numpy as np
import ROOT

def makeRbPlot(h_mc, h_data, nj_reg, outfile):

    nj_list_pref = ["j2to3","j4to6","j7toInf","j2to6","j4toInf","j2toInf"]
    nj_list = []
    for name in nj_list_pref:
        nj_list.append(name+"cent")
        nj_list.append(name+"forw")

    nj_bounds_tmp = [(2,3),(4,6),(2,6),(7,"Inf"),(4,"Inf"),(2,"Inf")]
    nj_bounds = []
    for bound in nj_bounds_tmp:
        nj_bounds.append(bound)
        nj_bounds.append(bound)

    nj_reg = nj_list.index(nj_reg)

    h_mcerr = h_mc.Clone("h_mcerr")

    ROOT.gStyle.SetOptStat(0)
    c = ROOT.TCanvas("c1","c1",600,650)
    c.SetLeftMargin(0.12)
    c.SetTopMargin(0.08)
    c.SetLogy(1)
    c.SetTicky(1)

    h_mc.SetLineColor(ROOT.kRed+1)
    h_mc.SetLineWidth(2)
    h_mc.SetTitle("")
    h_mc.GetXaxis().SetTitle("N(bjets)")
    h_mc.GetXaxis().SetTitleOffset(1.3)
    h_mc.GetXaxis().SetNdivisions(6)
    h_mc.GetYaxis().SetTitle("r_{b}")
    h_mc.GetYaxis().SetTitleOffset(1.4)
    h_mc.GetYaxis().SetRangeUser(1e-3,1.2)

    h_data.SetMarkerStyle(20)
    h_data.SetMarkerColor(ROOT.kBlack)
    h_data.SetLineColor(ROOT.kBlack)

    h_mcerr.SetFillStyle(3244)
    h_mcerr.SetFillColor(ROOT.kGray+1)

    h_mc.Draw("HIST")
    h_mcerr.Draw("SAME E2")
    h_data.Draw("SAME P E0")

    leg = ROOT.TLegend(0.6,0.75,0.89,0.89)
    leg.SetFillStyle(0)
    leg.SetTextFont(62)
    leg.SetHeader("{0} #leq N(jets) #leq {1} ".format(*(nj_bounds[nj_reg])))
    leg.AddEntry(h_data,"Data",'p')
    leg.AddEntry(h_mc,"QCD MC",'l')
    leg.SetBorderSize(0)
    leg.Draw()

    drawHeaders(c, True)

    c.SaveAs(outfile+".pdf")
    c.SaveAs(outfile+".png")
    

def makeFjPlot(h_mc, h_data, ht_reg, outfile):

    ht_list_tmp = ["ht250to450","ht450to575","ht575to1000","ht1000to1500","ht1500toInf","ht1000toInf"]
    ht_list = []
    for name in ht_list_tmp:
        ht_list.append(name+"forw")
        ht_list.append(name+"cent")

    ht_reg = ht_list.index(ht_reg)

    ht_bounds_tmp = [(250,450),(450,575),(575,1000),(1000,1500),(1500,"Inf"),(1000,"Inf")]
    ht_bounds = []
    for bound in ht_bounds_tmp:
        ht_bounds.append(bound)
        ht_bounds.append(bound)

    h_mcerr = h_mc.Clone("h_mcerr")

    ROOT.gStyle.SetOptStat(0)
    c = ROOT.TCanvas("c1","c1",600,650)
    c.SetLeftMargin(0.12)
    c.SetTopMargin(0.08)
    c.SetTicky(1)

    h_mc.SetLineColor(ROOT.kRed+1)
    h_mc.SetLineWidth(2)
    h_mc.SetTitle("")
    h_mc.GetXaxis().SetTitle("N(jets)")
    h_mc.GetXaxis().SetTitleOffset(1.3)
    h_mc.GetXaxis().SetNdivisions(9)
    h_mc.GetYaxis().SetTitle("f_{j}")
    h_mc.GetYaxis().SetTitleOffset(1.4)
    h_mc.GetYaxis().SetRangeUser(0,1.2)

    h_data.SetMarkerStyle(20)
    h_data.SetMarkerColor(ROOT.kBlack)
    h_data.SetLineColor(ROOT.kBlack)

    h_mcerr.SetFillStyle(3244)
    h_mcerr.SetFillColor(ROOT.kGray+1)

    h_mc.Draw("HIST")
    h_mcerr.Draw("SAME E2")
    h_data.Draw("SAME P E0")


    leg = ROOT.TLegend(0.6,0.75,0.89,0.89)
    leg.SetFillStyle(0)
    leg.SetTextFont(62)
    leg.SetHeader("{0} < H_{{T}} < {1} GeV".format(*(ht_bounds[ht_reg])))
    leg.AddEntry(h_data,"Data",'p')
    leg.AddEntry(h_mc,"QCD MC",'l')
    leg.SetBorderSize(0)
    leg.Draw()

    drawHeaders(c, True)

    c.SaveAs(outfile+".pdf")
    c.SaveAs(outfile+".png")
    

def makeRphiPlot(h_all, h_qcd, fit, ht_reg, isData, outfile, fit_systUp=None, fit_systDown=None):

    ht_list_tmp = ["ht250to450","ht450to575","ht575to1000","ht1000to1500","ht1500toInf","ht1000toInf"]
    ht_list = []
    for name in ht_list_tmp:
        ht_list.append(name+"forw")
        ht_list.append(name+"cent")

    ht_reg = ht_list.index(ht_reg)

    ht_bounds_tmp = [(250,450),(450,575),(575,1000),(1000,1500),(1500,"Inf"),(1000,"Inf")]
    ht_bounds = []
    for bound in ht_bounds_tmp:
        ht_bounds.append(bound)
        ht_bounds.append(bound)

    # h_errBand = ROOT.TH1D("h_errBand","",294,30,1500)
    # h_errBand.SetFillColor(ROOT.kGray)
    # h_errBand.SetFillStyle(3001)
    # ROOT.TVirtualFitter.GetFitter().GetConfidenceIntervals(h_errBand, 0.68)

    ROOT.gStyle.SetOptStat(0)
    c = ROOT.TCanvas("c1","c1",600,650)
    c.SetLogx(1)
    c.SetLogy(1)
    c.SetLeftMargin(0.12)
    c.SetTopMargin(0.08)
    c.SetTicky(1)
    
    h_qcd.SetMarkerStyle(24)
    h_qcd.SetLineColor(ROOT.kBlack)
    h_all.SetMarkerStyle(20)
    h_all.SetLineColor(ROOT.kBlack)

    h_qcd.SetTitle("")
    h_qcd.GetXaxis().SetRangeUser(50,1200)
    h_qcd.GetXaxis().SetTitle("M_{T2} [GeV]")
    h_qcd.GetXaxis().SetTitleOffset(1.3)
    h_qcd.GetYaxis().SetRangeUser(0.01,200)
    h_qcd.GetYaxis().SetTitle("r_{#phi}")
    h_qcd.GetYaxis().SetTitleOffset(1.4)
    
    if fit_systUp != None:
        fit_systUp.SetLineWidth(2)
        fit_systUp.SetLineColor(ROOT.kBlue+1)

    if fit_systDown != None:
        fit_systDown.SetLineWidth(2)
        fit_systDown.SetLineColor(ROOT.kGreen+2)

    fit.SetLineWidth(2)
    fit.SetLineColor(ROOT.kRed+1)

    h_qcd.Draw("PE0")

    line = ROOT.TLine()
    line.SetLineColor(ROOT.kBlack)
    line.SetLineWidth(1)
    line.SetLineStyle(2)
    lowbound = 60 if ht_reg in [0,1,2] else 70
    line.DrawLine(lowbound,0.01,lowbound,200)
    line.DrawLine(100,0.01,100,200)
    
    # h_errBand.Draw("SAME E3")
    if fit_systUp != None:
        fit_systUp.Draw("SAME")
    if fit_systDown != None:
        fit_systDown.Draw("SAME")
    fit.Draw("SAME")
    h_qcd.Draw("SAMEPE0")
    h_all.Draw("SAMEPE0")

    leg = ROOT.TLegend(0.45,0.7,0.89,0.89)
    leg.SetFillStyle(0)
    leg.SetTextFont(62)
    leg.SetHeader("{0} < H_{{T}} < {1} GeV".format(*(ht_bounds[ht_reg])))
    if isData:
        leg.AddEntry(h_all,"Data (all)",'p')
        leg.AddEntry(h_qcd,"Data (non-QCD subtracted)",'p')
    else:
        leg.AddEntry(h_all,"MC (all)",'p')
        leg.AddEntry(h_qcd,"MC (QCD only)",'p')
    leg.AddEntry(fit,"Fit",'l')
    leg.SetBorderSize(0)
    leg.Draw()

    # text = ROOT.TLatex()
    # text.SetNDC(1)
    # text.SetTextFont(62)
    # text.SetTextSize(0.035)
    # text.SetTextAlign(13)
    # text.DrawLatex(0.15,0.89,"a = {0:.1f} #pm {1:.1f}".format(fit.GetParameter(0),fit.GetParError(0)))
    # text.DrawLatex(0.15,0.85,"b = {0:.2f} #pm {1:.2f}".format(fit.GetParameter(1),fit.GetParError(1)))

    drawHeaders(c, isData)

    c.SaveAs(outfile+".pdf")
    c.SaveAs(outfile+".png")



def drawHeaders(canvas, isData):
    cmsText = "CMS Simulation"
    text = ROOT.TLatex()
    text.SetNDC(1)
    #lumi
    if(isData):
        text.SetTextAlign(31)
        text.SetTextSize(0.035)
        text.SetTextFont(42)
        text.DrawLatex(0.89,0.93,"{0} {1}^{{-1}} ({2} TeV)".format(12.9, 'fb', 13))
        cmsText = "CMS Preliminary"
    #CMS text
    text.SetTextSize(0.035)
    text.SetTextAlign(11)
    text.SetTextFont(62)
    text.DrawLatex(0.12,0.93,cmsText)
        
