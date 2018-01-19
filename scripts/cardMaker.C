#include <iostream>
#include <utility>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <set>
#include <regex>
#include <sys/stat.h>

#include "TROOT.h"
#include "TLatex.h"
#include "TString.h"
#include "TFile.h"
#include "TH1.h"
#include "TH3.h"
#include "TList.h"
#include "TCollection.h"
#include "TKey.h"
#include "TBenchmark.h"
#include "RooHistError.h"
#include "TError.h"

using namespace std;

TFile* f_lostlep = 0;
TFile* f_zinv = 0;
TFile* f_zinvDY = 0;
TFile* f_zgratio = 0;
TFile* f_purity = 0;
TFile* f_qcd = 0;
TFile* f_sig = 0;
TFile* f_data = 0;

const bool verbose = true;

const bool suppressZeroBins = false;

const bool suppressZeroTRs = true;

const bool doSuperSignalRegions = false;

const float dummy_alpha = 1.; // dummy value for gmN when there are no SR events

const bool uncorrelatedZGratio = false; // treat ZGratio uncertainty as fully uncorrelated

const bool fourNuisancesPerBinZGratio = true;

const bool integratedZinvEstimate = true;

const bool doDummySignalSyst = false;

const bool subtractSignalContam = true;

const bool doZinvFromDY = true; //if false, will take estimate from GJets

const bool decorrelateLostlepNuisances = false; //if true, new lostlep nuisances will be decorrelated in all regions

const bool doSimpleLostlepNuisances = false; //if true, reverts to ICHEP lostlep nuisances (only alpha & lepeff)

const bool printTable = false; //if true, prints additional .txt files with the data & bkg yields and uncertainties for plotmaking

const bool suppressUHmt2bin = true; //if true, skips the lowest mt2bin in the UH HT region

double last_zinv_ratio = 0.5;
double last_lostlep_transfer = 2.;
double last_zinvDY_transfer = 2.;

inline bool FileExists(const TString name) {
  struct stat buffer;   
  return (stat (name.Data(), &buffer) == 0); 
}

//_______________________________________________________________________________
void ReplaceString(std::string& subject, const std::string& search, const std::string& replace) {
    size_t pos = 0;
    while((pos = subject.find(search, pos)) != std::string::npos) {
         subject.replace(pos, search.length(), replace);
         pos += replace.length();
    }
}

//_______________________________________________________________________________
double round(double d)
{
  return floor(d + 0.5);
}

//_______________________________________________________________________________
int printCard( string dir_str , int mt2bin , string signal, string output_dir, int scanM1 = -1, int scanM2 = -1) {

  if (scanM1 != 1000 || scanM2 != 600) return 0;

  // read off yields from h_mt2bins hist in each topological region
  if (verbose) cout<<"Looking at region "<<dir_str<<", mt2 bin "<<mt2bin<<endl;
  TString dir = TString(dir_str);

  // Used to test for directory existence. If a control region has zero counts, no directory is created for it.
  TDirectoryFile* dir_sig = (TDirectoryFile*) f_sig->Get(dir);
  TDirectoryFile* dir_lostlep = (TDirectoryFile*) f_lostlep->Get(dir);
  TDirectoryFile* dir_zinv = (TDirectoryFile*) f_zinv->Get(dir);
  TDirectoryFile* dir_zinvDY = (TDirectoryFile*) f_zinvDY->Get(dir);
  TDirectoryFile* dir_zgratio = (TDirectoryFile*) f_zgratio->Get(dir);
  TDirectoryFile* dir_purity = (TDirectoryFile*) f_purity->Get(dir);
  TDirectoryFile* dir_qcd = (TDirectoryFile*) f_qcd->Get(dir);

  // This should never happen because f_sig generates the list of dirs, but just in case...
  if (!dir_sig) {
    cout << "dir " << dir << " does not exist in f_sig. Aborting..." << endl;
    return 0;
  }

  TString fullhistname = dir + "/h_mt2bins";
  TString fullhistnameGenMet  = fullhistname+"_genmet";
  TString fullhistnameBtagsfHeavy  = fullhistname+"_btagsf_heavy_UP";
  TString fullhistnameBtagsfLight  = fullhistname+"_btagsf_light_UP";
  TString fullhistnameLepeff  = fullhistname+"_lepeff_UP";
  TString fullhistnameIsr  = fullhistname+"_isr_UP";
  TString fullhistnameScan  = fullhistname+"_sigscan";
  TString fullhistnameScanGenMet  = fullhistname+"_sigscan_genmet";
  TString fullhistnameScanBtagsfHeavy  = fullhistname+"_sigscan_btagsf_heavy_UP";
  TString fullhistnameScanBtagsfLight  = fullhistname+"_sigscan_btagsf_light_UP";
  TString fullhistnameScanLepeff  = fullhistname+"_sigscan_lepeff_UP";
  TString fullhistnameScanIsr  = fullhistname+"_sigscan_isr_UP";
  TString fullhistnameStat  = fullhistname+"Stat";
  TString fullhistnameMCStat  = fullhistname+"MCStat";
  TString fullhistnameCRyield  = fullhistname+"CRyield";
  TString fullhistnameCRyieldDatacard  = fullhistname+"CRyieldDatacard";
  TString fullhistnameLastbinHybrid  = dir+"/h_lastbinHybrid";
  TString fullhistnameMCExtrap  = dir+"/h_mt2binsMCExtrap";
  TString fullhistnameRatio  = fullhistname+"Ratio";
  TString fullhistnameRatioInt  = fullhistname+"RatioInt";
  TString fullhistnamePurity = dir + "/h_mt2binspurityFailSieieData";
  TString fullhistnamePurityInt = dir + "/h_mt2binspurityIntFailSieieData";
  TString fullhistnameAlpha  = fullhistname+"Alpha";
  TString fullhistnameFJRB  = fullhistname+"FJRBsyst";
  TString fullhistnameFitStat  = fullhistname+"FitStat";
  TString fullhistnameFitSyst  = fullhistname+"FitSyst";
  TString fullhistnameCRSL = TString(dir).ReplaceAll("sr","crsl") + "/h_mt2bins";
  TString fullhistnameCRSLGenMet = TString(dir).ReplaceAll("sr","crsl") + "/h_mt2bins_genmet";
  TString fullhistnameCRSLScan  = fullhistnameCRSL+"_sigscan";
  TString fullhistnameCRSLScanGenMet  = fullhistnameCRSL+"_sigscan_genmet";

  // check to see if this is a super signal region (20 and up)
  int sr_number = -1;
  std::regex sr_number_regex("[^0-9]*([0-9]+).*"); // finds an integer of any length
  std::smatch sr_number_match;
  if (std::regex_match(dir_str,sr_number_match, sr_number_regex)) {
    sr_number = stoi(sr_number_match[1].str());
    if (doSuperSignalRegions && sr_number < 20) return 0;
    else if (!doSuperSignalRegions && sr_number >= 20) return 0;
  }
  else {
    cout << "WARNING: couldn't get signal region number for region: " << dir_str << endl;
  }
  
  TString signame(signal);
  if (scanM1 >= 0 && scanM2 >= 0) {
    signame = Form("%s_%d_%d",signal.c_str(),scanM1,scanM2);
  }
  bool isSignalWithLeptons = bool(signame.Contains("T1tttt") || signame.Contains("T2tt"));

  double n_lostlep(0.);
  double n_lostlep_cr(0.);
  double lostlep_alpha(0.);
  double lostlep_alpha_topological(0.);
  double lostlep_alpha_lepeff_UP(0.);
  double lostlep_alpha_lepeff_DN(0.);
  double lostlep_alpha_btagsf_heavy_UP(0.);
  double lostlep_alpha_btagsf_heavy_DN(0.);
  double lostlep_alpha_btagsf_light_UP(0.);
  double lostlep_alpha_btagsf_light_DN(0.);
  double lostlep_alpha_tau1p_UP(0.);
  double lostlep_alpha_tau1p_DN(0.);
  double lostlep_alpha_tau3p_UP(0.);
  double lostlep_alpha_tau3p_DN(0.);
  double lostlep_alpha_renorm_UP(0.);
  double lostlep_alpha_renorm_DN(0.);
  double lostlep_MCExtrap(0.);
  double err_lostlep_mcstat(0.);
  double n_zinv(0.);
  double n_zinv_cr(0.);
  double err_zinv_mcstat(0.);
  double zinv_ratio_zg(0.);
  double zinv_purity(0.);
  double err_zinv_purity(0.);
  double n_qcd(0.);
  double n_qcd_cr(0.);
  double qcd_alpha(0.);
  double err_qcd_alpha(0.);
  double err_qcd_fjrb(0.);
  double err_qcd_fitstat(0.);
  double err_qcd_fitsyst(0.);
  double n_bkg(0.);
  double n_data(0.);
  int lostlep_lastbin_hybrid(1);
  double n_zinvDY(0.);
  double n_zinvDY_cr(0.);
  double zinvDY_alpha(0.);
  double zinvDY_alpha_topological(0.);
  double zinvDY_purity(0.);
  double err_zinvDY_mcstat(0.);
  double err_zinvDY_purity(0.);
  bool usingInclusiveTemplates(true);
  int zinvDY_lastbin_hybrid(1);

  double n_sig(0.);
  double n_sig_cor(0.);
  double n_sig_TR(0.);
  double n_sig_crsl(0.);
  double n_sig_crsl_genmet(0.);
  double err_sig_mcstat(0.);
  double err_sig_mcstat_rel(0.);
  double n_sig_genmet(0.);
  double n_sig_cor_genmet(0.);
  double n_sig_recogenaverage(0.);
  double n_sig_cor_recogenaverage(0.);
  double n_sig_btagsf_heavy_UP(0.);
  double n_sig_btagsf_light_UP(0.);
  double n_sig_lepeff_UP(0.);
  double n_sig_isr_UP(0.);
  double err_sig_recogenaverage(0.);

  TH1D* h_sig(0);
  TH1D* h_sig_crsl(0);
  TH1D* h_sig_crsl_genmet(0);
  TH1D* h_sig_genmet(0);
  TH1D* h_sig_btagsf_heavy_UP(0);
  TH1D* h_sig_btagsf_light_UP(0);
  TH1D* h_sig_lepeff_UP(0);
  TH1D* h_sig_isr_UP(0);
  // pick point out of signal scan
  if (scanM1 >= 0 && scanM2 >= 0) {
    TH3D* h_sigscan = (TH3D*) f_sig->Get(fullhistnameScan);
    if (!h_sigscan) return 0;
    int bin1 = h_sigscan->GetYaxis()->FindBin(scanM1);
    int bin2 = h_sigscan->GetZaxis()->FindBin(scanM2);
    h_sig = h_sigscan->ProjectionX(Form("h_mt2bins_%d_%d_%s",scanM1,scanM2,dir_str.c_str()),bin1,bin1,bin2,bin2);
    TH3D* h_sigscan_genmet = (TH3D*) f_sig->Get(fullhistnameScanGenMet);
    if (h_sigscan_genmet) h_sig_genmet = h_sigscan_genmet->ProjectionX(Form("h_mt2bins_genmet_%d_%d_%s",scanM1,scanM2,dir_str.c_str()),bin1,bin1,bin2,bin2);
    TH3D* h_sigscan_btagsf_heavy_UP = (TH3D*) f_sig->Get(fullhistnameScanBtagsfHeavy);
    if (h_sigscan_btagsf_heavy_UP) h_sig_btagsf_heavy_UP = h_sigscan_btagsf_heavy_UP->ProjectionX(Form("h_mt2bins_btagsf_heavy_UP_%d_%d_%s",scanM1,scanM2,dir_str.c_str()),bin1,bin1,bin2,bin2);
    TH3D* h_sigscan_btagsf_light_UP = (TH3D*) f_sig->Get(fullhistnameScanBtagsfLight);
    if (h_sigscan_btagsf_light_UP) h_sig_btagsf_light_UP = h_sigscan_btagsf_light_UP->ProjectionX(Form("h_mt2bins_btagsf_light_UP_%d_%d_%s",scanM1,scanM2,dir_str.c_str()),bin1,bin1,bin2,bin2);
    TH3D* h_sigscan_lepeff_UP = (TH3D*) f_sig->Get(fullhistnameScanLepeff);
    if (h_sigscan_lepeff_UP) h_sig_lepeff_UP = h_sigscan_lepeff_UP->ProjectionX(Form("h_mt2bins_lepeff_UP_%d_%d_%s",scanM1,scanM2,dir_str.c_str()),bin1,bin1,bin2,bin2);
    TH3D* h_sigscan_isr_UP = (TH3D*) f_sig->Get(fullhistnameScanIsr);
    if (h_sigscan_isr_UP) h_sig_isr_UP = h_sigscan_isr_UP->ProjectionX(Form("h_mt2bins_isr_UP_%d_%d_%s",scanM1,scanM2,dir_str.c_str()),bin1,bin1,bin2,bin2);
    if (subtractSignalContam) {
      TH3D* h_sigscan_crsl = (TH3D*) f_sig->Get(fullhistnameCRSLScan);
      TH3D* h_sigscan_crsl_genmet = (TH3D*) f_sig->Get(fullhistnameCRSLScanGenMet);
      if (h_sigscan_crsl) h_sig_crsl = h_sigscan_crsl->ProjectionX(Form("h_mt2bins_crsl_%d_%d_%s",scanM1,scanM2,dir_str.c_str()),bin1,bin1,bin2,bin2);
      if (h_sigscan_crsl_genmet) h_sig_crsl_genmet = h_sigscan_crsl_genmet->ProjectionX(Form("h_mt2bins_genmet_crsl_%d_%d_%s",scanM1,scanM2,dir_str.c_str()),bin1,bin1,bin2,bin2);
    }
  }
  // single point sample
  else {
    h_sig = (TH1D*) f_sig->Get(fullhistname);
    h_sig_genmet = (TH1D*) f_sig->Get(fullhistnameGenMet);
    h_sig_btagsf_heavy_UP = (TH1D*) f_sig->Get(fullhistnameBtagsfHeavy);
    h_sig_btagsf_light_UP = (TH1D*) f_sig->Get(fullhistnameBtagsfLight);
    h_sig_lepeff_UP = (TH1D*) f_sig->Get(fullhistnameLepeff);
    h_sig_isr_UP = (TH1D*) f_sig->Get(fullhistnameIsr);
    if (subtractSignalContam) {
      h_sig_crsl = (TH1D*) f_sig->Get(fullhistnameCRSL);
      h_sig_crsl_genmet = (TH1D*) f_sig->Get(fullhistnameCRSLGenMet);
    }
    // Trick to print out monojet regions even when running on signal without monojet events
    //    if (fullhistname.Contains("J")) 
    //      h_sig = (TH1D*) f_sig->Get("sr6M/h_mt2bins");
  }

  if (!h_sig) {
    if (verbose) cout<<"No signal found in this region"<<endl;
    if (suppressZeroBins || suppressZeroTRs) return 0;
  }
  else if (h_sig->Integral(0,-1) == 0) {
    if (verbose) cout<<"No signal found in this region"<<endl;
    if (suppressZeroBins || suppressZeroTRs) return 0;
  }
  else {
    n_sig = h_sig->GetBinContent(mt2bin);
    n_sig_TR = h_sig->Integral(0,-1);
    err_sig_mcstat = h_sig->GetBinError(mt2bin);
    err_sig_mcstat_rel = err_sig_mcstat/n_sig;
  }
  if (h_sig_genmet) n_sig_genmet = h_sig_genmet->GetBinContent(mt2bin);
  if (h_sig_btagsf_heavy_UP) n_sig_btagsf_heavy_UP = h_sig_btagsf_heavy_UP->GetBinContent(mt2bin);
  if (h_sig_btagsf_light_UP) n_sig_btagsf_light_UP = h_sig_btagsf_light_UP->GetBinContent(mt2bin);
  if (h_sig_lepeff_UP) n_sig_lepeff_UP = h_sig_lepeff_UP->GetBinContent(mt2bin);
  if (h_sig_isr_UP) n_sig_isr_UP = h_sig_isr_UP->GetBinContent(mt2bin);
  
  //Get variable boundaries for signal region.
  //Used to create datacard name.
  TH1D* h_ht_LOW = (TH1D*) f_sig->Get(dir+"/h_ht_LOW");
  TH1D* h_ht_HI = (TH1D*) f_sig->Get(dir+"/h_ht_HI");
  int ht_LOW = h_ht_LOW->GetBinContent(1);
  int ht_HI = h_ht_HI->GetBinContent(1);
  
  TH1D* h_nbjets_LOW = (TH1D*) f_sig->Get(dir+"/h_nbjets_LOW");
  TH1D* h_nbjets_HI = (TH1D*) f_sig->Get(dir+"/h_nbjets_HI");
  int nbjets_LOW = h_nbjets_LOW->GetBinContent(1);
  int nbjets_HI = h_nbjets_HI->GetBinContent(1);

  TH1D* h_njets_LOW = (TH1D*) f_sig->Get(dir+"/h_njets_LOW");
  TH1D* h_njets_HI = (TH1D*) f_sig->Get(dir+"/h_njets_HI");
  int njets_LOW = h_njets_LOW->GetBinContent(1);
  int njets_HI = h_njets_HI->GetBinContent(1);

  int mt2_LOW = h_sig ? h_sig->GetBinLowEdge(mt2bin) : 0;
  int mt2_HI = h_sig ? mt2_LOW + h_sig->GetBinWidth(mt2bin) : 1500;
  // hardcode the current edge of our highest bin..
  if ((mt2_HI == 1500) || (mt2_HI == 1800)) mt2_HI = -1;

  int nbjets_HI_mod = nbjets_HI;
  int njets_HI_mod = njets_HI;
  if(nbjets_HI != -1) nbjets_HI_mod--;
  if(njets_HI != -1) njets_HI_mod--;

  std::string ht_str = "HT" + to_string(ht_LOW) + "to" + to_string(ht_HI);
  std::string jet_str = (njets_HI_mod == njets_LOW) ? "j" + to_string(njets_LOW) : "j" + to_string(njets_LOW) + "to" + to_string(njets_HI_mod);
  std::string bjet_str = (nbjets_HI_mod == nbjets_LOW) ? "b" + to_string(nbjets_LOW) : "b" + to_string(nbjets_LOW) + "to" + to_string(nbjets_HI_mod);
  std::string mt2_str = "m" + to_string(mt2_LOW) + "to" + to_string(mt2_HI);
  std::string eta_str = dir_str.substr(dir_str.length()-4);
  
  cout << "eta_str = " << eta_str << endl;

  //Replace instances of "-1" with "inf" for variables with no upper bound.
  ReplaceString(ht_str, "-1", "Inf");
  ReplaceString(jet_str, "-1", "Inf");
  ReplaceString(bjet_str, "-1", "Inf");
  ReplaceString(mt2_str, "-1", "Inf");

  std::string channel = ht_str + "_" + jet_str + "_" + bjet_str + "_" + eta_str + "_" + mt2_str;
  std::string topologicalR = ht_str + "_" + jet_str + "_" + bjet_str + "_" + eta_str;
  
  // bin boundaries for CRSL, for lostlep systematic correlations
  int ht_LOW_crsl = 0;
  int ht_HI_crsl = 0;
  int nbjets_LOW_crsl = 0;
  int nbjets_HI_crsl = 0;
  int njets_LOW_crsl = 0;
  int njets_HI_crsl = 0;
  // Check if dir_lostlep exists, then update boundaries if so.
  if (dir_lostlep) {
    TH1D* h_ht_LOW_crsl = (TH1D*) f_lostlep->Get(dir+"/h_ht_LOW");
    TH1D* h_ht_HI_crsl = (TH1D*) f_lostlep->Get(dir+"/h_ht_HI");
    ht_LOW_crsl = h_ht_LOW_crsl->GetBinContent(1);
    ht_HI_crsl = h_ht_HI_crsl->GetBinContent(1);
  
    TH1D* h_nbjets_LOW_crsl = (TH1D*) f_lostlep->Get(dir+"/h_nbjets_LOW");
    TH1D* h_nbjets_HI_crsl = (TH1D*) f_lostlep->Get(dir+"/h_nbjets_HI");
    nbjets_LOW_crsl = h_nbjets_LOW_crsl->GetBinContent(1);
    nbjets_HI_crsl = h_nbjets_HI_crsl->GetBinContent(1);
    
    TH1D* h_njets_LOW_crsl = (TH1D*) f_lostlep->Get(dir+"/h_njets_LOW");
    TH1D* h_njets_HI_crsl = (TH1D*) f_lostlep->Get(dir+"/h_njets_HI");
    njets_LOW_crsl = h_njets_LOW_crsl->GetBinContent(1);
    njets_HI_crsl = h_njets_HI_crsl->GetBinContent(1);
  }
  int nbjets_HI_crsl_mod = nbjets_HI_crsl;
  int njets_HI_crsl_mod = njets_HI_crsl;
  if(nbjets_HI_crsl != -1) nbjets_HI_crsl_mod--;
  if(njets_HI_crsl != -1) njets_HI_crsl_mod--;
  
  std::string ht_str_crsl = "HT" + to_string(ht_LOW_crsl) + "to" + to_string(ht_HI_crsl);
  std::string jet_str_crsl = (njets_HI_crsl_mod == njets_LOW_crsl) ? "j" + to_string(njets_LOW_crsl) : "j" + to_string(njets_LOW_crsl) + "to" + to_string(njets_HI_crsl_mod);
  std::string bjet_str_crsl = (nbjets_HI_crsl_mod == nbjets_LOW_crsl) ? "b" + to_string(nbjets_LOW_crsl) : "b" + to_string(nbjets_LOW_crsl) + "to" + to_string(nbjets_HI_crsl_mod);
  
  //Replace instances of "-1" with "inf" for variables with no upper bound.
  ReplaceString(ht_str_crsl, "-1", "Inf");
  ReplaceString(jet_str_crsl, "-1", "Inf");
  ReplaceString(bjet_str_crsl, "-1", "Inf");
  
  TString cardname = Form("%s/datacard_%s_%s.txt",output_dir.c_str(),channel.c_str(),signame.Data());
  
  if (suppressZeroBins && ((n_sig < 0.1) || (n_sig/n_bkg < 0.02))) {
    //if ( (suppressZeroBins && (n_sig < 0.001)) || (suppressZeroTRs && (n_sig < 0.001)) ) {
    if (verbose) std::cout << "Zero signal, card not printed: " << cardname << std::endl;
    return 0;
  }
  
  // get yields for each sample
  // !!!!! HACK: set zero bins to 0.01 for now to make combine happy
  if (dir_lostlep) {
    TH1D* h_lostlep = (TH1D*) f_lostlep->Get(fullhistname);
    if (h_lostlep != 0) {
      n_lostlep = h_lostlep->GetBinContent(mt2bin);
    } 
    TH1D* h_lostlep_mcstat = (TH1D*) f_lostlep->Get(fullhistnameMCStat);
    if (h_lostlep_mcstat != 0 && h_lostlep_mcstat->GetBinContent(mt2bin) != 0) 
      err_lostlep_mcstat = h_lostlep_mcstat->GetBinError(mt2bin)/h_lostlep_mcstat->GetBinContent(mt2bin);
    TH1D* h_lostlep_cryield = (TH1D*) f_lostlep->Get(fullhistnameCRyieldDatacard);
    if (h_lostlep_cryield != 0) 
      n_lostlep_cr = round(h_lostlep_cryield->GetBinContent(mt2bin));
    TH1D* h_lostlep_alpha = (TH1D*) f_lostlep->Get(fullhistnameAlpha);
    if (h_lostlep_alpha != 0) {
      lostlep_alpha = h_lostlep_alpha->GetBinContent(mt2bin);
      lostlep_alpha_topological = h_lostlep_alpha->Integral(0,-1);
    }
    TH1D* h_lostlep_alpha_lepeff_UP = (TH1D*) f_lostlep->Get(fullhistnameAlpha+"_lepeff_UP");
    if (h_lostlep_alpha_lepeff_UP != 0) 
      lostlep_alpha_lepeff_UP = h_lostlep_alpha_lepeff_UP->Integral(0,-1);
    TH1D* h_lostlep_alpha_lepeff_DN = (TH1D*) f_lostlep->Get(fullhistnameAlpha+"_lepeff_DN");
    if (h_lostlep_alpha_lepeff_DN != 0) 
      lostlep_alpha_lepeff_DN = h_lostlep_alpha_lepeff_DN->Integral(0,-1);
    TH1D* h_lostlep_alpha_btagsf_heavy_UP = (TH1D*) f_lostlep->Get(fullhistnameAlpha+"_btagsf_heavy_UP");
    if (h_lostlep_alpha_btagsf_heavy_UP != 0) 
      lostlep_alpha_btagsf_heavy_UP = h_lostlep_alpha_btagsf_heavy_UP->Integral(0,-1);
    TH1D* h_lostlep_alpha_btagsf_heavy_DN = (TH1D*) f_lostlep->Get(fullhistnameAlpha+"_btagsf_heavy_DN");
    if (h_lostlep_alpha_btagsf_heavy_DN != 0) 
      lostlep_alpha_btagsf_heavy_DN = h_lostlep_alpha_btagsf_heavy_DN->Integral(0,-1);
    TH1D* h_lostlep_alpha_btagsf_light_UP = (TH1D*) f_lostlep->Get(fullhistnameAlpha+"_btagsf_light_UP");
    if (h_lostlep_alpha_btagsf_light_UP != 0) 
      lostlep_alpha_btagsf_light_UP = h_lostlep_alpha_btagsf_light_UP->Integral(0,-1);
    TH1D* h_lostlep_alpha_btagsf_light_DN = (TH1D*) f_lostlep->Get(fullhistnameAlpha+"_btagsf_light_DN");
    if (h_lostlep_alpha_btagsf_light_DN != 0) 
      lostlep_alpha_btagsf_light_DN = h_lostlep_alpha_btagsf_light_DN->Integral(0,-1);
    TH1D* h_lostlep_alpha_tau1p_UP = (TH1D*) f_lostlep->Get(fullhistnameAlpha+"_tau1p_UP");
    if (h_lostlep_alpha_tau1p_UP != 0) 
      lostlep_alpha_tau1p_UP = h_lostlep_alpha_tau1p_UP->Integral(0,-1);
    TH1D* h_lostlep_alpha_tau1p_DN = (TH1D*) f_lostlep->Get(fullhistnameAlpha+"_tau1p_DN");
    if (h_lostlep_alpha_tau1p_DN != 0) 
      lostlep_alpha_tau1p_DN = h_lostlep_alpha_tau1p_DN->Integral(0,-1);
    TH1D* h_lostlep_alpha_tau3p_UP = (TH1D*) f_lostlep->Get(fullhistnameAlpha+"_tau3p_UP");
    if (h_lostlep_alpha_tau3p_UP != 0) 
      lostlep_alpha_tau3p_UP = h_lostlep_alpha_tau3p_UP->Integral(0,-1);
    TH1D* h_lostlep_alpha_tau3p_DN = (TH1D*) f_lostlep->Get(fullhistnameAlpha+"_tau3p_DN");
    if (h_lostlep_alpha_tau3p_DN != 0) 
      lostlep_alpha_tau3p_DN = h_lostlep_alpha_tau3p_DN->Integral(0,-1);
    TH1D* h_lostlep_alpha_renorm_UP = (TH1D*) f_lostlep->Get(fullhistnameAlpha+"_renorm_UP");
    if (h_lostlep_alpha_renorm_UP != 0) 
      lostlep_alpha_renorm_UP = h_lostlep_alpha_renorm_UP->Integral(0,-1);
    TH1D* h_lostlep_alpha_renorm_DN = (TH1D*) f_lostlep->Get(fullhistnameAlpha+"_renorm_DN");
    if (h_lostlep_alpha_renorm_DN != 0) 
      lostlep_alpha_renorm_DN = h_lostlep_alpha_renorm_DN->Integral(0,-1);
    TH1D* h_lostlep_lastbin_hybrid = (TH1D*) f_lostlep->Get(fullhistnameLastbinHybrid);
    if (h_lostlep_lastbin_hybrid != 0)
      lostlep_lastbin_hybrid = h_lostlep_lastbin_hybrid->GetBinContent(1);
    TH1D* h_lostlep_MCExtrap = (TH1D*) f_lostlep->Get(fullhistnameMCExtrap);
    if (h_lostlep_MCExtrap != 0)
      lostlep_MCExtrap = h_lostlep_MCExtrap->GetBinContent(mt2bin);
  }
  //calculate lostlep_alpha errors
  //lepeff
  double lostlep_alpha_lepeff_ERR = 0;
  const double lostlep_alpha_lepeff_ERRup = abs(1-lostlep_alpha_lepeff_UP/lostlep_alpha_topological);
  const double lostlep_alpha_lepeff_ERRdn = abs(1-lostlep_alpha_lepeff_DN/lostlep_alpha_topological);
  lostlep_alpha_lepeff_ERR = max(lostlep_alpha_lepeff_ERRup, lostlep_alpha_lepeff_ERRdn);

  //btagsf heavy & light
  double lostlep_alpha_btagsf_heavy_ERR = 0;
  const double lostlep_alpha_btagsf_heavy_ERRup = abs(1-lostlep_alpha_btagsf_heavy_UP/lostlep_alpha_topological);
  const double lostlep_alpha_btagsf_heavy_ERRdn = abs(1-lostlep_alpha_btagsf_heavy_DN/lostlep_alpha_topological);
  lostlep_alpha_btagsf_heavy_ERR = max(lostlep_alpha_btagsf_heavy_ERRup, lostlep_alpha_btagsf_heavy_ERRdn);
  double lostlep_alpha_btagsf_light_ERR = 0;
  const double lostlep_alpha_btagsf_light_ERRup = abs(1-lostlep_alpha_btagsf_light_UP/lostlep_alpha_topological);
  const double lostlep_alpha_btagsf_light_ERRdn = abs(1-lostlep_alpha_btagsf_light_DN/lostlep_alpha_topological);
  lostlep_alpha_btagsf_light_ERR = max(lostlep_alpha_btagsf_light_ERRup, lostlep_alpha_btagsf_light_ERRdn);
  const double lostlep_alpha_btagsf_ERR = max(lostlep_alpha_btagsf_heavy_ERR, lostlep_alpha_btagsf_light_ERR);

  //tau 1p & 3p
  double lostlep_alpha_tau1p_ERR = 0;
  const double lostlep_alpha_tau1p_ERRup = abs(1-lostlep_alpha_tau1p_UP/lostlep_alpha_topological);
  const double lostlep_alpha_tau1p_ERRdn = abs(1-lostlep_alpha_tau1p_DN/lostlep_alpha_topological);
  lostlep_alpha_tau1p_ERR = max(lostlep_alpha_tau1p_ERRup, lostlep_alpha_tau1p_ERRdn);
  double lostlep_alpha_tau3p_ERR = 0;
  const double lostlep_alpha_tau3p_ERRup = abs(1-lostlep_alpha_tau3p_UP/lostlep_alpha_topological);
  const double lostlep_alpha_tau3p_ERRdn = abs(1-lostlep_alpha_tau3p_DN/lostlep_alpha_topological);
  lostlep_alpha_tau3p_ERR = max(lostlep_alpha_tau3p_ERRup, lostlep_alpha_tau3p_ERRdn);
  const double lostlep_alpha_tau_ERR = max(lostlep_alpha_tau1p_ERR, lostlep_alpha_tau3p_ERR);

  //renorm
  double lostlep_alpha_renorm_ERR = 0;
  const double lostlep_alpha_renorm_ERRup = abs(1-lostlep_alpha_renorm_UP/lostlep_alpha_topological);
  const double lostlep_alpha_renorm_ERRdn = abs(1-lostlep_alpha_renorm_DN/lostlep_alpha_topological);
  lostlep_alpha_renorm_ERR = max(lostlep_alpha_renorm_ERRup, lostlep_alpha_renorm_ERRdn);

  //MC extrapolation for MT2 shape
  double lostlep_shape_ERR = 0;
  if (n_lostlep > 0.) lostlep_shape_ERR = lostlep_MCExtrap / n_lostlep;

  int n_mt2bins = 1;
  // Need this scoping of h_zinv for hybrid calculation to compile, but calculation
  // won't happen for default n_mt2bins and zinv_lastbin_hybrid values.
  TH1D* h_zinv(0); 
  if (dir_zinv != 0) {
    h_zinv = (TH1D*) f_zinv->Get(fullhistname);
    if (h_zinv != 0) n_zinv = h_zinv->GetBinContent(mt2bin);
    if (h_zinv != 0) n_mt2bins = h_zinv->GetNbinsX();    
    // MC stat unc based on #Z/#g
    TH1D* h_zinv_mcstat = (TH1D*) f_zinv->Get(fullhistnameRatio);
    if (integratedZinvEstimate) h_zinv_mcstat = (TH1D*) f_zinv->Get(fullhistnameRatioInt);
    if (h_zinv_mcstat != 0 && h_zinv_mcstat->GetBinContent(mt2bin) != 0) {
      err_zinv_mcstat = h_zinv_mcstat->GetBinError(mt2bin)/h_zinv_mcstat->GetBinContent(mt2bin);
      zinv_ratio_zg = h_zinv_mcstat->GetBinContent(mt2bin);
      last_zinv_ratio = zinv_ratio_zg;
    }
    else { // catch zeroes (there shouldn't be any)
      err_zinv_mcstat = 1.;
      zinv_ratio_zg = last_zinv_ratio;
    }
  }
  // Pure GJet yield (useful when extracting G_i/G_int for integral estimate)
  TH1D* h_gjetyield = (TH1D*) f_zinv->Get(fullhistnameCRyield);

  if (dir_purity) {
    // Photon yield (includes GJetPrompt+QCDPrompt+QCDFake, or Data)
    TH1D* h_zinv_cryield = (TH1D*) f_purity->Get(fullhistname);
    if (h_zinv_cryield != 0) {
      n_zinv_cr = round(h_zinv_cryield->GetBinContent(mt2bin));
      if (integratedZinvEstimate)  n_zinv_cr = round(h_zinv_cryield->Integral(0,-1));
    }
 
    // Purity and uncertainty (bin-by-bin estimate)
    TH1D* h_zinv_purity = (TH1D*) f_purity->Get(fullhistnamePurity);
    zinv_purity = 1.;
    int mt2bin_tmp = mt2bin;
    if (integratedZinvEstimate) {
      // When using the integrated estimate (over MT2), should use the integrated purity.
      h_zinv_purity = (TH1D*) f_purity->Get(fullhistnamePurityInt);
      mt2bin_tmp = 1;
    }
    
    if (h_zinv_purity != 0 && h_zinv_purity->GetBinContent(mt2bin_tmp) != 0) {
      zinv_purity *= h_zinv_purity->GetBinContent(mt2bin_tmp);
      err_zinv_purity = h_zinv_purity->GetBinError(mt2bin_tmp)/h_zinv_purity->GetBinContent(mt2bin_tmp);
    }
  }

  //-----------------------------------
  //----- Zinv Estimate using DY ------
  //-----------------------------------

  // Need this scoping of h_zinvDY for hybrid calculation to compile, but calculation 
  // won't happen if dir_zinvDY doesn't exist (n_mt2bins and zinvDY_lastbin_hybrid 
  // default values cause condition for hybrid calculation to happen to fail).
  TH1D* h_zinvDY(0);
  if (dir_zinvDY) {
    h_zinvDY  = (TH1D*) f_zinvDY->Get(dir+"/hybridEstimate"); //the final hybrid estimate
    TH1D* purityCard = (TH1D*) f_zinvDY->Get(dir+"/purityCard"); //purity histogram
    TH1D* ratioCard  = (TH1D*) f_zinvDY->Get(dir+"/ratioCard"); //alpha histogram
    TH1D* h_zinvDY_cryield = (TH1D*) f_zinvDY->Get(dir+"/CRyieldCard"); //CR yield histogram
    if (h_zinvDY != 0) {
      n_mt2bins = h_zinvDY->GetNbinsX();
      n_zinvDY = h_zinvDY->GetBinContent(mt2bin);
    }
    //get mcstat from alpha histogram
    //has additional uncertainties baked in from Zinvmaker
    if (ratioCard != 0 && ratioCard->GetBinContent(mt2bin) != 0) 
      err_zinvDY_mcstat = ratioCard->GetBinError(mt2bin)/ratioCard->GetBinContent(mt2bin);
    if (h_zinvDY_cryield != 0) {
      n_zinvDY_cr = round(h_zinvDY_cryield->GetBinContent(mt2bin)); 
      //check if using inclusive templates by looking at content of CRyieldCard
      //if all bins are not equal, not using inclusive templates
      int crYieldFirstBin = -1;
      for (int ibin = 1; ibin <  h_zinvDY_cryield->GetNbinsX(); ibin++) {
	int thiscrYield = round(h_zinvDY_cryield->GetBinContent(ibin));
	if (ibin == 1) crYieldFirstBin = round(h_zinvDY_cryield->GetBinContent(ibin));
	if (crYieldFirstBin != thiscrYield) {
	  usingInclusiveTemplates = false;
	  break;
	}
      }
    }
    TH1D* h_zinvDY_alpha = (TH1D*) f_zinvDY->Get(dir+"/ratioCard");
    if (h_zinvDY_alpha != 0) {
      //multiply in purity histogram to get final alpha
      if (purityCard != 0) {
	zinvDY_purity = purityCard->GetBinContent(mt2bin);
	h_zinvDY_alpha->Multiply(purityCard);
	err_zinvDY_purity = purityCard->GetBinContent(mt2bin) > 0 ? purityCard->GetBinError(mt2bin)/purityCard->GetBinContent(mt2bin) : 0;
      }
      zinvDY_alpha = h_zinvDY_alpha->GetBinContent(mt2bin);
      zinvDY_alpha_topological = h_zinvDY_alpha->Integral(0,-1);
    }
    TH1D* h_zinvDY_lastbin_hybrid = (TH1D*) f_zinvDY->Get(fullhistnameLastbinHybrid);
    if (h_zinvDY_lastbin_hybrid != 0)
      zinvDY_lastbin_hybrid = h_zinvDY_lastbin_hybrid->GetBinContent(1);
  }
  
  float zllgamma_nj = 1., zllgamma_nb = 1., zllgamma_ht = 1., zllgamma_ht2 = 1., zllgamma_mt2 = 1.;
  TString notFound = "";
  if (fourNuisancesPerBinZGratio) {
    // Load NJ, NB, HT, MT2 histograms of R(Z/Gamma)
    TH1D* h_zllgamma_nj  = (TH1D*) f_zgratio->Get("h_njbinsRatio");
    TH1D* h_zllgamma_nb  = (TH1D*) f_zgratio->Get("h_nbjbinsRatio");
    TH1D* h_zllgamma_ht  = (TH1D*) f_zgratio->Get("h_htbinsRatio");
    TH1D* h_zllgamma_ht2  = (TH1D*) f_zgratio->Get("h_htbins2Ratio");
    TH1D* h_zllgamma_mt2 = (TH1D*) f_zgratio->Get("h_mt2binsRatio");
    // Extract values corresponding to this bin
    if (h_zllgamma_nj == 0 || h_zllgamma_nb == 0 || h_zllgamma_ht == 0 || h_zllgamma_ht2 == 0 || h_zllgamma_mt2 == 0) {
      cout<<"Trying fourNuisancesPerBinZGratio, but could not find inclusive Zll/Gamma ratio plots for nuisance parameters"<<endl;
      return 0;
    }
 
    int bin_nj   = h_zllgamma_nj ->FindBin(njets_LOW + 0.5);     
    int bin_nb   = h_zllgamma_nb ->FindBin(nbjets_LOW + 0.5);
    if (nbjets_LOW >= 3) bin_nb   = h_zllgamma_nb ->FindBin(nbjets_LOW - 1 + 0.5);
    int bin_ht   = h_zllgamma_ht ->FindBin(ht_LOW + 1);      
    int bin_ht2   = h_zllgamma_ht2 ->FindBin(ht_LOW + 1);      
    int bin_mt2  = h_zllgamma_mt2->FindBin(mt2_LOW + 1);     
    // (set to 100% if the ratio doesn't exist)
    zllgamma_nj  = h_zllgamma_nj ->GetBinContent( bin_nj  ) > 0 ? h_zllgamma_nj ->GetBinError( bin_nj  ) / h_zllgamma_nj ->GetBinContent( bin_nj  ) : 1.;
    zllgamma_nb  = h_zllgamma_nb ->GetBinContent( bin_nb  ) > 0 ? h_zllgamma_nb ->GetBinError( bin_nb  ) / h_zllgamma_nb ->GetBinContent( bin_nb  ) : 1.;
    if (nbjets_LOW >= 3) zllgamma_nb *= 2;
    zllgamma_ht  = h_zllgamma_ht ->GetBinContent( bin_ht  ) > 0 ? h_zllgamma_ht ->GetBinError( bin_ht  ) / h_zllgamma_ht ->GetBinContent( bin_ht  ) : 1.;
    zllgamma_ht2 = h_zllgamma_ht2->GetBinContent( bin_ht2 ) > 0 ? h_zllgamma_ht2->GetBinError( bin_ht2 ) / h_zllgamma_ht2->GetBinContent( bin_ht2 ) : 1.;
    zllgamma_mt2 = h_zllgamma_mt2->GetBinContent( bin_mt2 ) > 0 ? h_zllgamma_mt2->GetBinError( bin_mt2 ) / h_zllgamma_mt2->GetBinContent( bin_mt2 ) : 1.;
    //cout<<"Looking at bin "<<channel<<endl;
    //cout<<"Corresponding bins are: "<<bin_nj<<", "<<bin_nb<<", "<<bin_ht<<", "<<bin_mt2<<endl;
    //cout<<"Corresponding Zll/Gamma uncertainties are: "<<zllgamma_nj<<", "<<zllgamma_nb<<", "<<zllgamma_ht<<", "<<zllgamma_mt2<<endl;
  }

  if (dir_qcd) {
    TH1D* h_qcd = (TH1D*) f_qcd->Get(fullhistname);
    if (h_qcd != 0) n_qcd = h_qcd->GetBinContent(mt2bin);

    cout << topologicalR << endl;
    cout << channel << endl;
    h_qcd->Print("all");
    cout << "n_qcd = " << n_qcd << endl;

    //  if (n_qcd < 0.01) n_qcd = 0.01;
    TH1D* h_qcd_cryield = (TH1D*) f_qcd->Get(fullhistnameCRyield);
    if (h_qcd_cryield != 0) 
      n_qcd_cr = round(h_qcd_cryield->GetBinContent(mt2bin));
    TH1D* h_qcd_alpha = (TH1D*) f_qcd->Get(fullhistnameAlpha);
    if (h_qcd_alpha != 0) {
      qcd_alpha = h_qcd_alpha->GetBinContent(mt2bin);
      err_qcd_alpha = h_qcd_alpha->GetBinError(mt2bin); // for 1j, 50%.  For multijet, not used
    }
    TH1D* h_qcd_fjrb = (TH1D*) f_qcd->Get(fullhistnameFJRB);
    if (h_qcd_fjrb != 0) {
      err_qcd_fjrb = h_qcd_fjrb->GetBinContent(mt2bin); // multijet only
    }
    TH1D* h_qcd_fitstat = (TH1D*) f_qcd->Get(fullhistnameFitStat);
    if (h_qcd_fitstat != 0) {
      err_qcd_fitstat = h_qcd_fitstat->GetBinContent(mt2bin); // multijet only
    }
    TH1D* h_qcd_fitsyst = (TH1D*) f_qcd->Get(fullhistnameFitSyst);
    if (h_qcd_fitsyst != 0) {
      err_qcd_fitsyst = h_qcd_fitsyst->GetBinContent(mt2bin); // multijet only
    }
  }
  
  int n_syst = 0;
  // ----- lost lepton bkg uncertainties
  double lostlep_shape = lostlep_shape_ERR;
  double lostlep_mcstat = 1. + err_lostlep_mcstat; // transfer factor stat uncertainty
  double lostlep_lepeff = 1.0 + lostlep_alpha_lepeff_ERR; // transfer factor uncertainty from lepton eff
  double lostlep_mtcut = 1.03; // transfer factor uncertainty from mt cut
  double lostlep_taueff = 1.0 + lostlep_alpha_tau_ERR; // transfer factor uncertainty from tau efficiency
  double lostlep_btageff = 1.0 + lostlep_alpha_btagsf_ERR; // transfer factor uncertainty from btag efficiency
  double lostlep_jec = 1.0 + 0.02; // transfer factor uncertainty from JEC unc
  double lostlep_renorm = 1.0 + lostlep_alpha_renorm_ERR; // transfer factor uncertainty from renorm & factorization scales
  double lostlep_alphaerr = 1. + 0.10; // only used if doSimpleLostlepNuisances
 
  // nuisances decorrelated depending on extrapolation in hybrid method
  TString name_lostlep_shape      = Form("llep_shape_%s_%s_%s"   , ht_str_crsl.c_str(), jet_str_crsl.c_str(), bjet_str_crsl.c_str());
  TString name_lostlep_crstat     = Form("llep_CRstat_%s_%s_%s"  , ht_str_crsl.c_str(), jet_str_crsl.c_str(), bjet_str_crsl.c_str());
  TString name_lostlep_alphaerr = Form("llep_alpha_%s_%s_%s"     , ht_str_crsl.c_str(), jet_str_crsl.c_str(), bjet_str_crsl.c_str()); // only used if doSimpleLostlepNuisances
  if (n_mt2bins > 1 && mt2bin >= lostlep_lastbin_hybrid) {
    // extrapolation: need shape uncertainty
    n_syst++;  // lostlep_shape
  } else {
    // bin-by-bin is used: no shape uncertainty, decorrelated CR uncertainty
    name_lostlep_crstat     = Form("llep_CRstat_%s"  , channel.c_str());
  }

  // nuisances decorrelated across all bins
  TString name_lostlep_mcstat     = Form("llep_MCstat_%s"        , channel.c_str());

  // nuisances correlated across all bins
  TString name_lostlep_lepeff     = Form("lep_eff");
  TString name_lostlep_taueff     = Form("llep_taueff");
  TString name_lostlep_btageff    = Form("llep_btageff");
  TString name_lostlep_jec        = Form("jec");
  TString name_lostlep_renorm     = Form("llep_renorm");
  TString name_lostlep_mtcut      = Form("llep_mtcut");
  if (decorrelateLostlepNuisances) {
    //name_lostlep_lepeff     = Form("llep_lepeff_%s"      , channel.c_str()); //this nuisance is always totally correlated with Zinv
    name_lostlep_taueff     = Form("llep_taueff_%s"      , channel.c_str());
    name_lostlep_btageff    = Form("llep_btageff_%s"     , channel.c_str());
    //name_lostlep_jec        = Form("llep_jec_%s"         , channel.c_str()); //this nuisance is always totally correlated with Zinv
    name_lostlep_renorm     = Form("llep_renorm_%s"      , channel.c_str());
    name_lostlep_mtcut      = Form("llep_mtcut_%s"       , channel.c_str());
  }

  if (doSimpleLostlepNuisances) {
    //overwrite lepeff nuisance to old version
    TString name_lostlep_lepeff = Form("llep_lepeff_%s_%s_%s", ht_str_crsl.c_str(), jet_str_crsl.c_str(), bjet_str_crsl.c_str());
    lostlep_lepeff = 1.0 + 0.12;
    if ( njets_LOW == 7 && nbjets_LOW >= 3) lostlep_alphaerr = 1.18; // due to btag eff
  }
  
  // note that if n_lostlep_cr == 0, we will just use lostlep_alpha (straight from MC) in the datacard
  if (n_lostlep_cr > 0.) {
    if (lostlep_alpha > 3.) {
      lostlep_alpha = 3.; // hard bound to avoid statistical fluctuations
      n_lostlep = n_lostlep_cr * lostlep_alpha;
    }
  }
  if (lostlep_alpha > 0.) last_lostlep_transfer = lostlep_alpha; // cache last good alpha value
  else if (n_lostlep == 0) lostlep_alpha = 0;
  else lostlep_alpha = last_lostlep_transfer;   // if alpha is 0: use alpha from previous (MT2) bin
  if (doSimpleLostlepNuisances) n_syst += 4; // lostlep_crstat, lostlep_mcstat, lostlep_alphaerr, lostlep_lepeff
  else n_syst += 8; // lostlep_crstat, lostlep_mcstat, lostlep_lepeff, lostlep_mtcut, lostlep_taueff, lostlep_btageff, jec, lostlep_renorm

  // ----- zinv bkg uncertainties - depend on signal region, b selection
  TString perChannel(channel.c_str());
  TString perTopoRegion(topologicalR.c_str());

  //zinv estimate from GJ nuisances
  TString name_zinv_crstat     = integratedZinvEstimate ? "zinv_CRstat_"    +perTopoRegion : "zinv_CRstat_"+perChannel    ;
  TString name_zinv_alphaerr   = integratedZinvEstimate ? "zinv_alphaErr_"  +perTopoRegion : "zinv_alphaErr_"+perChannel  ;
  TString name_zinv_purityerr  = integratedZinvEstimate ? "zinv_purity_"    +perTopoRegion : "zinv_purity_"+perChannel    ;
  TString name_zinv_puritysyst = integratedZinvEstimate ? "zinv_puritySyst_"+perTopoRegion : "zinv_puritySyst_"+perChannel;
  TString name_zinv_zgamma = "zinv_ZGratio";
  if (uncorrelatedZGratio) name_zinv_zgamma = Form("zinv_ZGratio_%s",channel.c_str());
  TString name_zinv_zgamma_nj  = Form("zinv_ZGratio_%s" , jet_str.c_str()  );
  TString name_zinv_zgamma_nb  = Form("zinv_ZGratio_%s" , bjet_str.c_str() );
  TString name_zinv_zgamma_ht  = Form("zinv_ZGratio_%s" , ht_str.c_str()   );
  // Only a low edge for MT2, since we want to maintain correlations between differently sized MT2 bins with the same lower edge
  TString name_zinv_zgamma_mt2 = "zinv_ZGratio_m" + to_string(mt2_LOW); 
  TString name_zinv_doubleRatioOffset = "zinv_doubleRatioOffset";
  TString name_zinv_mcsyst = Form("zinv_MC_%s",channel.c_str());
  TString name_zinv_shape = "zinv_shape_"+perTopoRegion;
  
  double zinv_alpha = 1.;
  double zinv_alphaerr = 1. + err_zinv_mcstat;
  double zinv_purityerr = 1. + err_zinv_purity;
  double zinv_puritysyst = 1.10;
  double zinv_doubleRatioOffset = 1.11;
  double zinv_zgamma = -1.;
  double zinv_zgamma_nj = 1., zinv_zgamma_nb = 1., zinv_zgamma_ht = 1., zinv_zgamma_mt2 = 1.;
  double zinv_mcsyst = -1.;
  double zinv_shape = 1.;

  //zinv Estimate from DY nuisances
  // nuisances decorrelated across all bins
  TString name_zinvDY_alphaErr      = Form("zinvDY_alphaErr_%s"        , channel.c_str());
  // nuisances decorrelated depending on extrapolation in hybrid method
  TString name_zinvDY_purity      = "zinvDY_purity_"+perTopoRegion;
  TString name_zinvDY_rsfof       = "zinvDY_Rsfof";
  TString name_zinvDY_shape       = "zinvDY_shape_"+perTopoRegion;
  //TString name_zinvDY_jec         = "jec"; // name defined in lostlep estimate
  TString name_zinvDY_crstat      = "zinvDY_CRstat_"    +perTopoRegion;
  if (mt2bin < zinvDY_lastbin_hybrid && !usingInclusiveTemplates) {
    // bin-by-bin is used: no shape uncertainty, decorrelated CR uncertainty
    name_zinvDY_crstat     = Form("zinvDY_CRstat_%s"  , channel.c_str());
  }
  //  double zinvDY_mcsyst = -1.;
  double zinvDY_shape = 1.;
  double zinvDY_puritystat = 1. + err_zinvDY_purity; // transfer factor purity stat uncertainty
  double zinvDY_rsfof = 1. + (1-zinvDY_purity)*0.15; // transfer factor uncertainty due to R(SF/OF)
  double zinvDY_alphaErr = 1. + err_zinvDY_mcstat; // transfer factor stat uncertainty
  double zinvDY_lepeff = 1. + 0.05; // lepeff uncertainty
  double zinvDY_jec = (ht_HI == 450) ?  1. + 0.05 : 1. + 0.02; // transfer factor jec uncertainty

  //  note that if n_zinvDY_cr == 0, we will just use zinvDY_alpha (straight from MC) in the datacard
  if (n_zinvDY_cr > 0.) {
    if (zinvDY_alpha > 10.) {
      zinvDY_alpha = 10.; // hard bound to avoid statistical fluctuations
      n_zinvDY = n_zinvDY_cr * zinvDY_alpha;
    }
  }
  if (zinvDY_alpha > 0.) last_zinvDY_transfer = zinvDY_alpha; // cache last good alpha value
  else if (n_zinvDY == 0) zinvDY_alpha = 0;
  else zinvDY_alpha = last_zinvDY_transfer;   // if alpha is 0: use alpha from previous (MT2) bin

  if (zinvDY_alpha != n_zinvDY/n_zinvDY_cr && n_zinvDY_cr != 0) {
    if (verbose) cout << "zinvDY_alpha does not add up! alpha: " << zinvDY_alpha << " n_cr: " << n_zinvDY_cr << " n: " << n_zinvDY << endl;
    zinvDY_alpha = n_zinvDY/n_zinvDY_cr;
    if (verbose) cout << "Setting zinvDY_alpha to " << zinvDY_alpha << endl;
  }
  n_syst += 6; // zinvDY_crstat, zinvDY_alphaErr, zinvDY_purity, zinvDY_rsfof, lep_eff, jec

  // in hybrid method, or normal extrapolation: shape uncertainty only for bins with MT2 extrapolation
  const float last_bin_relerr_zinvDY = 0.4;
  // default value of n_mt2bins is 1, default of zinvDY_lastbin_hybrid is 1, 
  // so the condition fails if no observation (ie if dir_zinv or dir_zinvDY don't exist)
  int n_extrap_bins_zinvDY = n_mt2bins - zinvDY_lastbin_hybrid;
  if (n_extrap_bins_zinvDY > 0 && mt2bin >= zinvDY_lastbin_hybrid) {
    if (mt2bin == zinvDY_lastbin_hybrid && n_zinvDY > 0.) {
      // first bin needs to compensate normalization from the rest
      float increment = 0.;
      for (int ibin=zinvDY_lastbin_hybrid+1; ibin<=h_zinvDY->GetNbinsX(); ibin++) 
	increment += last_bin_relerr_zinvDY / (n_extrap_bins_zinvDY) * (ibin - zinvDY_lastbin_hybrid) * h_zinvDY->GetBinContent(ibin);
      zinvDY_shape = 1. - increment/n_zinvDY;
    }
    else
      zinvDY_shape = 1. + last_bin_relerr_zinvDY / (n_extrap_bins_zinvDY) * (mt2bin - zinvDY_lastbin_hybrid);
    n_syst++;  // zinvDY_shape
  }

  
  // 2+b: pure MC estimate (not used when using integrated estimate)
  if ( !integratedZinvEstimate && nbjets_LOW >= 2 ) {
    zinv_mcsyst = 2.;
    ++n_syst;
  }
  else {
    // err_zinv_mcstat, zinv_ratio_zg, n_zinv_cr
    zinv_alpha =  zinv_ratio_zg * zinv_purity * 0.92; // 0.92 is a fixed factor for "f = GJetPrompt / (GJetPrompt+QCDPrompt)"
    if (f_data) zinv_alpha *= 0.93; // data-driven correction, based on the double-ratio R(Zll/Gamma)
    if (zinv_alpha > 0.5) zinv_alpha = 0.5; // Hard bound to avoid statistical fluctuations
    n_syst += 5; // 1: zinv_alphaerr (stat on ratio). 2: gamma function. 3: purity stat unc. 4: puritySyst. 5: doubleRatioOffset
    if (integratedZinvEstimate && n_mt2bins > 1) {
      if (mt2bin == 1 && n_zinv > 0) {
	// first bin needs to compensate normalization from the rest
	float increment = 0.;
	for (int ibin=2; ibin<=h_zinv->GetNbinsX(); ibin++) 
	  increment += 0.4 / (n_mt2bins - 1) * (ibin - 1) * h_zinv->GetBinContent(ibin);
	zinv_shape = 1. - increment/n_zinv;
      }
      else
	zinv_shape = 1. + 0.4 / (n_mt2bins - 1) * (mt2bin - 1);
      n_syst++;
    }
    if (fourNuisancesPerBinZGratio) {
      zinv_zgamma_nj  =  1. + zllgamma_nj ;     
      zinv_zgamma_nb  =  1. + zllgamma_nb ;
      zinv_zgamma_ht  =  1. + zllgamma_ht ; 
      if (njets_LOW == 1) zinv_zgamma_ht  =  1. + zllgamma_ht2 ; // special double ratio for 1-jet
      else n_syst += 3;
      if (!integratedZinvEstimate) {
	zinv_zgamma_mt2 =  1. + zllgamma_mt2;	
	n_syst++;
      }
    }
    else {
      zinv_zgamma = 1.20; // old 20% uncertainty
      n_syst++;
    }
    n_zinv = n_zinv_cr * zinv_alpha; // don't use Zinv MC as central value any more!
  }

  
  // ----- qcd bkg uncertainties from data driven rphi method (or monojet method)
  double qcd_crstat = qcd_alpha; // transfer factor
  double qcd_alphaerr = 1. + (err_qcd_alpha / qcd_alpha); // for 1j, 50%.  For multijet, not used
  double qcd_fjrbsyst = 1. + err_qcd_fjrb; // multijet only, error on fj+rb
  double qcd_fitstat = 1. + err_qcd_fitstat; // multijet only, stat error on fit
  double qcd_fitsyst = 1. + err_qcd_fitsyst; // multijet only, syst error on fit
 
  // fully uncorrelated for all except fit related, those are correlated within HT bins
  TString name_qcd_crstat = "qcd_CRstat_"+perChannel;
  TString name_qcd_alphaerr = "qcd_alphaErr_"+perChannel;
  TString name_qcd_fjrbsyst = "qcd_FJRBsyst_"+perChannel;
  TString name_qcd_fitstat = Form("qcd_RPHIstat_%s",ht_str.c_str());
  TString name_qcd_fitsyst = Form("qcd_RPHIsyst_%s",ht_str.c_str());

  if (njets_LOW == 1) n_syst += 2; // crstat, alphaerr
  else n_syst += 4; // crstat, fjrbsyst, fitstat, fitsyst

  
  //gen-reco averaging
  n_sig_recogenaverage = (n_sig_genmet + n_sig)/2;
  err_sig_recogenaverage = (n_sig-n_sig_genmet)/2/n_sig_recogenaverage;
  
  // correction for signal contamination:
  //   find how much bkg prediction is increased for this MT2 bin by signal in CR (n_lostlep_extra)
  //   subtract n_lostlep_extra from n_sig and use for reduced signal efficiency
  //first initialize corrected yields to initial values
  n_sig_cor = n_sig;
  n_sig_cor_genmet = n_sig_genmet;
  n_sig_cor_recogenaverage = n_sig_recogenaverage;
     
  if (subtractSignalContam) {
    if (h_sig_crsl) {
      // integrated shape
      if (mt2bin >= lostlep_lastbin_hybrid) {
	n_sig_crsl = h_sig_crsl->Integral(lostlep_lastbin_hybrid,-1);
	if (h_sig_crsl_genmet) n_sig_crsl_genmet = h_sig_crsl_genmet->Integral(lostlep_lastbin_hybrid,-1);
      }
      // bin-by-bin
      else {
	n_sig_crsl = h_sig_crsl->GetBinContent(mt2bin);
	if (h_sig_crsl_genmet) n_sig_crsl_genmet = h_sig_crsl_genmet->GetBinContent(mt2bin);
      }
      double n_lostlep_extra = n_sig_crsl * lostlep_alpha;
      double n_lostlep_extra_genmet = n_sig_crsl_genmet * lostlep_alpha;
      double n_lostlep_extra_recogenaverage = (n_lostlep_extra + n_lostlep_extra_genmet)/2;
      n_sig_cor = std::max(0.,n_sig - n_lostlep_extra);
      n_sig_cor_genmet = std::max(0.,n_sig_genmet - n_lostlep_extra_genmet);
      n_sig_cor_recogenaverage = std::max(0., n_sig_recogenaverage - n_lostlep_extra_recogenaverage);
      if (verbose) {
	cout << "correcting down signal yield from " << n_sig << " to " << n_sig_cor
	     << ", n_sig_crsl: " << n_sig_crsl
	     << ", alpha: " << lostlep_alpha << ", extra bkg pred: " << n_lostlep_extra
	     << ", lostlep_lastbin_hybrid: " << lostlep_lastbin_hybrid << endl;
	cout << "correcting down genmet yield from " << n_sig_genmet << " to " << n_sig_cor_genmet
	     << ", n_sig_crsl: " << n_sig_crsl
	     << ", alpha: " << lostlep_alpha << ", extra bkg pred: " << n_lostlep_extra_genmet
	     << ", lostlep_lastbin_hybrid: " << lostlep_lastbin_hybrid << endl;
	cout << "correcting down recogenaverage yield from " << n_sig_recogenaverage << " to " << n_sig_cor_recogenaverage
	     << ", n_sig_crsl: " << n_sig_crsl
	     << ", alpha: " << lostlep_alpha << ", extra bkg pred: " << n_lostlep_extra_recogenaverage
	     << ", lostlep_lastbin_hybrid: " << lostlep_lastbin_hybrid << endl;
      }
      err_sig_recogenaverage = (n_sig_cor_recogenaverage > 0.) ? abs(n_sig_cor-n_sig_cor_genmet)/2/n_sig_cor_recogenaverage : 0;
    }
    else {
      cout << "tried to subtract signal contamination but couldn't find sig_crsl hist" << endl;
    }
  } // if (subtractSignalContam)  
  
  // ----- sig uncertainties
  double sig_syst         = 1.10; // dummy 10% from early MC studies
  double sig_lumi         = 1.026; // 2.6% lumi uncertainty, Moriond 2016
  double sig_pu           = 1.046; // 4.6% pile-up uncertainty
  double sig_mcstat       = (n_sig > 0.) ? 1. + sqrt(pow(err_sig_mcstat_rel,2) + 0.005) : 1.071; // MC stat err +  quadrature sum of 5% for JES, 5% for renorm/fact scales
  double sig_genmet       = (n_sig > 0.) ? 1. + err_sig_recogenaverage: 1.00; // reco-gen MET averaging
  double sig_btagsf_heavy = (n_sig > 0.) ? n_sig_btagsf_heavy_UP/n_sig : 1.00; // btagsf heavy, eff UP
  double sig_btagsf_light = (n_sig > 0.) ? n_sig_btagsf_light_UP/n_sig : 1.00; // btagsf light, eff UP
  double sig_lepeff       = (n_sig > 0.) ? n_sig_lepeff_UP/n_sig : 1.00; // lepton eff UP
  double sig_isr          = (n_sig > 0.) ? n_sig_isr_UP/n_sig : 1.00; // isr weight UP

  // fully correlated for lumi, btagsf, lepeff, isr.  Fully uncorrelated for stats and other systs
  TString name_sig_syst         = "sig_syst";
  TString name_sig_lumi         = "lumi_syst";
  TString name_sig_pu           = "sig_PUsyst";
  TString name_sig_mcstat       = "sig_MCstat_"+perChannel;
  TString name_sig_genmet       = "sig_gensyst";
  TString name_sig_isr          = "sig_isrSyst";
  TString name_sig_btagsf_heavy = "sig_bTagHeavySyst";
  TString name_sig_btagsf_light = "sig_bTagLightSyst";
  TString name_sig_lepeff       = "sig_lepEffSyst";
  
  if (doDummySignalSyst) {
    // dummy: just 1 nuisance, correlated
    ++n_syst;
  }
  // otherwise do "real" signal systematics
  else {
    n_syst += 7; // mcstat (including gen scales and JEC), lumi, pileup, btagsf_heavy, btagsf_light, isr, reco-gen averaging
    if (isSignalWithLeptons) ++n_syst; // lepeff
  }

  if (doZinvFromDY)  n_bkg = n_lostlep+n_zinvDY+n_qcd;
  else               n_bkg = n_lostlep+n_zinv+n_qcd;
  
  if (n_bkg < 0.001) n_qcd = 0.01;

  if (f_data) {
    // n_data is 0 by default
    TH1D* h_data = (TH1D*) f_data->Get(fullhistname);
    if (h_data != 0) {
      n_data = h_data->GetBinContent(mt2bin);
    } 
  } else {
    // using only MC: take observation == total bkg
    n_data = n_bkg;
  }
 
  ofstream ofile;
  ofile.open(cardname);

  // Uncorrelate error if nonzero CR events and zero SR events
  bool uncorr_zinvDY = n_zinvDY_cr > 0 && n_zinvDY == 0;
  bool uncorr_lostlep = n_lostlep_cr > 0 && n_lostlep == 0;
  bool uncorr_qcd = n_qcd_cr > 0 && n_qcd == 0;

  // Set alpha to 2.0 if zero cr events and zero alpha
  bool zero_zinvDY = zinvDY_alpha == 0.0 && n_zinvDY_cr == 0.0;
  bool zero_lostlep = lostlep_alpha == 0.0 && n_lostlep_cr == 0.0;
  bool zero_qcd = qcd_crstat == 0.0 && n_qcd_cr == 0.0;

  // Allow ad hoc modification of counts and alphas by setting scale parameters.
  // Set alphas = 0 to 2.0 (ad hoc) in certain cases 
  float sig_scale = 1.0;
  float n_sig_cor_recogenaverage_towrite=sig_scale * n_sig_cor_recogenaverage;
  float scale = 1.0;
  float n_zinvDY_towrite=scale*n_zinvDY;
  float n_lostlep_towrite=scale*n_lostlep;
  float n_qcd_towrite=scale*n_qcd;
  // If alpha is 0 then set it to 2.0
  float zinvDY_alpha_towrite = ( (uncorr_zinvDY || zero_zinvDY) ? 2.0 : zinvDY_alpha) * scale;
  float lostlep_alpha_towrite = ( (uncorr_lostlep || zero_lostlep) ? 2.0 : lostlep_alpha) * scale;
  float qcd_crstat_towrite = ( (uncorr_qcd || zero_qcd) ? 2.0 : qcd_crstat) * scale;

  float n_zinvDY_cr_towrite = n_zinvDY_cr;  
  if (uncorr_zinvDY) {
    name_zinvDY_crstat += "_" + mt2_str;
    n_zinvDY_cr_towrite = 0;
  }
  float n_lostlep_cr_towrite = n_lostlep_cr;  
  if (uncorr_lostlep) {
    name_lostlep_crstat += "_" + mt2_str;
    n_lostlep_cr_towrite = 0;
  }
  float n_qcd_cr_towrite = n_qcd_cr;  
  if (uncorr_qcd) {
    name_qcd_crstat += "_" + mt2_str;
    n_qcd_cr_towrite = 0;
  }

  cout << "n_qcd_towrite = " << n_qcd_towrite << endl;


  ofile <<  "imax 1  number of channels"                                                    << endl;
  ofile <<  "jmax 3  number of backgrounds"                                                 << endl;
  ofile <<  "kmax *"                                                                        << endl;
  ofile <<  "------------"                                                                  << endl;
  ofile <<  Form("bin         %s",channel.c_str())                                          << endl;
  ofile <<  Form("observation %.3f",n_data)                                                 << endl;
  ofile <<  "------------"                                                                  << endl;
  ofile <<  Form("bin             %s   %s   %s   %s",channel.c_str(),channel.c_str(),channel.c_str(),channel.c_str()) << endl;
  ofile <<  "process          sig       zinv        llep      qcd"                                      << endl; 
  ofile <<  "process           0         1           2         3"                                      << endl;
  if (doZinvFromDY) ofile <<  Form("rate            %.3f    %.3f      %.3f      %.3f",n_sig_cor_recogenaverage_towrite,n_zinvDY_towrite,n_lostlep_towrite,n_qcd_towrite) << endl;
  else ofile <<  Form("rate            %.3f    %.3f      %.3f      %.3f",n_sig_cor_recogenaverage,n_zinv,n_lostlep,n_qcd) << endl;
  ofile <<  "------------"                                                                  << endl;
 
  // ---- sig systs
  if (doDummySignalSyst) {
    ofile <<  Form("%s                                lnN   %.3f    -      -     - ",name_sig_syst.Data(),sig_syst)  << endl;
  }
  // full signal systematics
  else {
    ofile <<  Form("%s                    lnN    %.3f   -    -    - ",name_sig_lumi.Data(),sig_lumi)  << endl;
    ofile <<  Form("%s                    lnN    %.3f   -    -    - ",name_sig_pu.Data(),sig_pu)  << endl;
    ofile <<  Form("%s     lnN    %.3f   -    -    - ",name_sig_mcstat.Data(),sig_mcstat)  << endl;
    ofile <<  Form("%s                  lnU    %.3f   -    -    - ",name_sig_genmet.Data(),sig_genmet)  << endl;
    ofile <<  Form("%s                  lnN    %.3f   -    -    - ",name_sig_isr.Data(),sig_isr)  << endl;
    ofile <<  Form("%s            lnN    %.3f   -    -    - ",name_sig_btagsf_heavy.Data(),sig_btagsf_heavy)  << endl;
    ofile <<  Form("%s            lnN    %.3f   -    -    - ",name_sig_btagsf_light.Data(),sig_btagsf_light)  << endl;
    if (isSignalWithLeptons) ofile <<  Form("%s               lnN    %.3f   -    -    - ",name_sig_lepeff.Data(),sig_lepeff)  << endl;
  }

  // ---- Zinv systs
  if (doZinvFromDY) {
    ofile <<  Form("%s        gmN %.0f    -   %.5f   -   - ",name_zinvDY_crstat.Data(),n_zinvDY_cr_towrite,zinvDY_alpha_towrite)  << endl;
    ofile <<  Form("%s        lnN    -   %.3f   -   - ",name_zinvDY_alphaErr.Data(),zinvDY_alphaErr)  << endl;
    ofile <<  Form("%s        lnN    -   %.3f   -   - ",name_zinvDY_purity.Data(),zinvDY_puritystat)  << endl;
    ofile <<  Form("%s        lnN    -   %.3f   -   - ",name_zinvDY_rsfof.Data(),zinvDY_rsfof)  << endl;
    if (n_extrap_bins_zinvDY > 0 && mt2bin >= zinvDY_lastbin_hybrid)
      ofile <<  Form("%s    lnN    -   %.3f   -   - ",name_zinvDY_shape.Data(),zinvDY_shape)  << endl;
  }
  else {
    if ( !integratedZinvEstimate && nbjets_LOW >= 2) {
      ofile <<  Form("%s       lnN    -   %.3f   -    - ",name_zinv_mcsyst.Data(),zinv_mcsyst)  << endl;
    }
    else {
      ofile <<  Form("%s       lnN    -   %.3f   -    - ",name_zinv_doubleRatioOffset.Data(),zinv_doubleRatioOffset )  << endl;
      if (fourNuisancesPerBinZGratio) {
	ofile <<  Form("%s      lnN    -   %.3f    -    - ",name_zinv_zgamma_ht.Data() ,zinv_zgamma_ht )  << endl;
	ofile <<  Form("%s      lnN    -   %.3f    -    - ",name_zinv_zgamma_nj.Data() ,zinv_zgamma_nj )  << endl;
	ofile <<  Form("%s      lnN    -   %.3f    -    - ",name_zinv_zgamma_nb.Data() ,zinv_zgamma_nb )  << endl;
	if (!integratedZinvEstimate)
	  ofile <<  Form("%s      lnN    -   %.3f    -    - ",name_zinv_zgamma_mt2.Data(),zinv_zgamma_mt2)  << endl;
      }
      else {
	ofile <<  Form("%s      lnN    -   %.3f    -    - ",name_zinv_zgamma.Data(),zinv_zgamma)  << endl;     
      }
      ofile <<  Form("%s       lnN    -   %.3f   -    - ",name_zinv_puritysyst.Data(),zinv_puritysyst)  << endl;
      ofile <<  Form("%s       lnN    -   %.3f   -    - ",name_zinv_purityerr.Data(),zinv_purityerr)  << endl;
      ofile <<  Form("%s     gmN %.0f    -  %.5f   -   - ",name_zinv_crstat.Data(),n_zinv_cr,zinv_alpha)  << endl;
      if (integratedZinvEstimate && n_mt2bins > 1)
	ofile <<  Form("%s       lnN    -   %.3f   -    - ",name_zinv_shape.Data(),zinv_shape)  << endl;
      ofile <<  Form("%s       lnN    -   %.3f   -    - ",name_zinv_alphaerr.Data(),zinv_alphaerr)  << endl;
    }
  }
  
  // ---- lostlep systs
  if (doSimpleLostlepNuisances) {
    ofile <<  Form("%s        lnN    -    -    %.3f    - ",name_lostlep_lepeff.Data(),lostlep_lepeff)  << endl;
    ofile <<  Form("%s        lnN    -    -    %.3f    - ",name_lostlep_alphaerr.Data(),lostlep_alphaerr)  << endl;
  }
  else {
    ofile <<  Form("%s        lnN    -    -    %.3f    - ",name_lostlep_mtcut.Data(),lostlep_mtcut)  << endl;
    ofile <<  Form("%s        lnN    -    -    %.3f    - ",name_lostlep_taueff.Data(),lostlep_taueff)  << endl;
    ofile <<  Form("%s        lnN    -    -    %.3f    - ",name_lostlep_btageff.Data(),lostlep_btageff)  << endl;
    if (!doZinvFromDY) ofile <<  Form("%s        lnN    -    -    %.3f    - ",name_lostlep_jec.Data(),lostlep_jec)  << endl;
    ofile <<  Form("%s        lnN    -    -    %.3f    - ",name_lostlep_renorm.Data(),lostlep_renorm)  << endl;
  }
  ofile <<  Form("%s        gmN %.0f    -    -    %.5f     - ",name_lostlep_crstat.Data(),n_lostlep_cr_towrite,lostlep_alpha_towrite)  << endl;
  ofile <<  Form("%s        lnN    -    -    %.3f    - ",name_lostlep_mcstat.Data(),lostlep_mcstat)  << endl;
  if (n_mt2bins > 1 && mt2bin >= lostlep_lastbin_hybrid)
    ofile <<  Form("%s    lnN    -    -   %.3f     - ",name_lostlep_shape.Data(),lostlep_shape)  << endl;
  //ofile <<  Form("%s        lnN    -    -    %.3f    - ",name_lostlep_alphaerr.Data(),lostlep_alphaerr)  << endl;

  //lepeff & jec systematics for Zinv and lostlep
  if (doZinvFromDY) {
    ofile <<  Form("%s        lnN    -    %.3f    %.3f    - ",name_lostlep_jec.Data(),zinvDY_jec,lostlep_jec)  << endl;
    ofile <<  Form("%s        lnN    -    %.3f    %.3f    - ",name_lostlep_lepeff.Data(),zinvDY_lepeff,lostlep_lepeff)  << endl;
  }
  else {
    ofile <<  Form("%s        lnN    -    -    %.3f    - ",name_lostlep_jec.Data(),lostlep_jec)  << endl;
    ofile <<  Form("%s        lnN    -    -    %.3f    - ",name_lostlep_lepeff.Data(),lostlep_lepeff)  << endl;
  }
  
  // ---- QCD systs
  ofile <<  Form("%s        gmN %.0f    -    -    -   %.5f",name_qcd_crstat.Data(),n_qcd_cr_towrite,qcd_crstat_towrite)  << endl;
  if (njets_LOW == 1) {
    ofile <<  Form("%s        lnN    -    -    -   %.3f",name_qcd_alphaerr.Data(),qcd_alphaerr)  << endl;
  } else {
    ofile <<  Form("%s        lnN    -    -    -   %.3f",name_qcd_fjrbsyst.Data(),qcd_fjrbsyst)  << endl;
    ofile <<  Form("%s        lnN    -    -    -   %.3f",name_qcd_fitstat.Data(),qcd_fitstat)  << endl;
    ofile <<  Form("%s        lnN    -    -    -   %.3f",name_qcd_fitsyst.Data(),qcd_fitsyst)  << endl;
  }

  ofile.close();

  if (verbose) std::cout << "Wrote card: " << cardname << std::endl;

  //prints a table of background and CR yields, with systematic and statistical errors
  TString tablename = Form("%s/table_%s.txt",output_dir.c_str(),channel.c_str());
  if (printTable && !FileExists(tablename)) {
    //calculate the stat error for each background based on poisson interval
    double zinvDY_statUp, zinvDY_statDown;
    RooHistError::instance().getPoissonInterval(n_zinvDY_cr,zinvDY_statDown,zinvDY_statUp,1.);
    zinvDY_statUp   *= (n_zinvDY_cr>0) ? n_zinvDY/n_zinvDY_cr : (zinvDY_alpha>0) ?  zinvDY_alpha : last_zinvDY_transfer;
    zinvDY_statDown *= (n_zinvDY_cr>0) ? n_zinvDY/n_zinvDY_cr : (zinvDY_alpha>0) ?  zinvDY_alpha : last_zinvDY_transfer;  
    zinvDY_statUp    = abs(zinvDY_statUp - n_zinvDY);
    zinvDY_statDown  = abs(zinvDY_statDown - n_zinvDY);
    
    double lostlep_statUp, lostlep_statDown;
    RooHistError::instance().getPoissonInterval(n_lostlep_cr,lostlep_statDown,lostlep_statUp,1.);
    lostlep_statUp   *= (n_lostlep_cr>0) ? n_lostlep/n_lostlep_cr : (lostlep_alpha>0) ?  lostlep_alpha : last_lostlep_transfer;
    lostlep_statDown *= (n_lostlep_cr>0) ? n_lostlep/n_lostlep_cr : (lostlep_alpha>0) ?  lostlep_alpha : last_lostlep_transfer;  
    lostlep_statUp    = abs(lostlep_statUp - n_lostlep);
    lostlep_statDown  = abs(lostlep_statDown - n_lostlep);

    double qcd_statUp, qcd_statDown;
    RooHistError::instance().getPoissonInterval(n_qcd_cr,qcd_statDown,qcd_statUp,1.);
    qcd_statUp   *= (n_qcd_cr>0) ? n_qcd/n_qcd_cr : (qcd_alpha>0) ?  qcd_alpha : 0;
    qcd_statDown *= (n_qcd_cr>0) ? n_qcd/n_qcd_cr : (qcd_alpha>0) ?  qcd_alpha : 0;  
    qcd_statUp    = abs(qcd_statUp - n_qcd);
    qcd_statDown  = abs(qcd_statDown - n_qcd);

    //calculate the total syst error for each background by summing in quadrature
    double zinvDY_syst;
    if (n_extrap_bins_zinvDY > 0 && mt2bin >= zinvDY_lastbin_hybrid) {
      zinvDY_syst = n_zinvDY*pow(pow(1-zinvDY_alphaErr,2)+pow(1-zinvDY_puritystat,2)+pow(1-zinvDY_rsfof,2)+pow(1-zinvDY_shape,2),0.5);
    }
    else zinvDY_syst = n_zinvDY*pow(pow(1-zinvDY_alphaErr,2)+pow(1-zinvDY_puritystat,2)+pow(1-zinvDY_rsfof,2),0.5);

    
    double lostlep_syst;
    if (n_mt2bins > 1 && mt2bin >= lostlep_lastbin_hybrid) {
      lostlep_syst = n_lostlep*pow(pow(1-lostlep_lepeff,2)+pow(1-lostlep_mtcut,2)+pow(1-lostlep_taueff,2)+pow(1-lostlep_btageff,2)+pow(1-lostlep_jec,2)+pow(1-lostlep_renorm,2)+pow(1-lostlep_mcstat,2)+pow(1-lostlep_shape,2),0.5);
    }
    else lostlep_syst = n_lostlep*pow(pow(1-lostlep_lepeff,2)+pow(1-lostlep_mtcut,2)+pow(1-lostlep_taueff,2)+pow(1-lostlep_btageff,2)+pow(1-lostlep_jec,2)+pow(1-lostlep_renorm,2)+pow(1-lostlep_mcstat,2),0.5);

    double qcd_syst;
    if (njets_LOW == 1) {
      qcd_syst = n_qcd*pow(pow(1-qcd_alphaerr,2),0.5);
    }
    else qcd_syst = n_qcd*pow(pow(1-qcd_fjrbsyst,2)+pow(1-qcd_fitstat,2)+pow(1-qcd_fitsyst,2),0.5);
    
    //write the table
    ofstream tablefile;
    tablefile.open(tablename);
    tablefile <<  "### bg_name yield statUp statDown systUp systDown" << endl;
    tablefile << Form("zinv  %.1f  %.1f  %.1f  %.1f  %.1f ",n_zinvDY, zinvDY_statUp, zinvDY_statDown, zinvDY_syst, zinvDY_syst) << endl;
    tablefile << Form("llep  %.1f  %.1f  %.1f  %.1f  %.1f ",n_lostlep, lostlep_statUp, lostlep_statDown, lostlep_syst, lostlep_syst) << endl;
    tablefile << Form("qcd  %.1f  %.1f  %.1f  %.1f  %.1f ",n_qcd, qcd_statUp, qcd_statDown, qcd_syst, qcd_syst) << endl;
    tablefile << Form("data %.0f ", n_data) << endl;
    tablefile << Form("zinv_nCR %.0f ", n_zinvDY_cr) << endl;
    tablefile << Form("llep_nCR %.0f ", n_lostlep_cr) << endl;
    tablefile << Form("qcd_nCR %.0f ", n_qcd_cr) << endl;
    
    tablefile.close();
    
    if (verbose) std::cout << "Wrote table: " << tablename << std::endl;
  }//if printTable
  
  return 1;
}

//_______________________________________________________________________________
void cardMaker(string signal, string input_dir, string output_dir, bool isScan = false, bool doData = true){

  gErrorIgnoreLevel=kError;

  // Benchmark
  TBenchmark *bmark = new TBenchmark();
  bmark->Start("benchmark");

  // ----------------------------------------
  //  samples definition
  // ----------------------------------------

  // set input files

  f_lostlep = new TFile(Form("%s/lostlepFromCRs.root",input_dir.c_str()));
  f_zinv = new TFile(Form("%s/zinvFromGJ.root",input_dir.c_str()));
  f_zinvDY = new TFile(Form("%s/zinvFromDY.root",input_dir.c_str()));
  f_zgratio = new TFile(Form("%s/doubleRatio.root",input_dir.c_str())); // zgratio from data
  //  f_zgratio = new TFile(Form("%s/zinvFromGJ.root",input_dir.c_str())); // zgratio from MC (with poisson uncertainty)
  f_purity = new TFile(Form("%s/purity.root",input_dir.c_str()));
  f_qcd = new TFile(Form("%s/qcdFromCRs.root",input_dir.c_str()));

  f_sig = new TFile(Form("%s/%s.root",input_dir.c_str(),signal.c_str()));

  if (doData) f_data = new TFile(Form("%s/data_Run2016.root",input_dir.c_str()));

  if( f_lostlep->IsZombie() || f_zinv->IsZombie() ||  f_zinvDY->IsZombie() || f_purity->IsZombie() || f_qcd->IsZombie() || f_sig->IsZombie() || f_zgratio ->IsZombie() || (doData && f_data->IsZombie()) ) {
  // Trick to look at estimates even if QCD prediction is broken
  //  if( f_lostlep->IsZombie() || f_zinv->IsZombie() || f_purity->IsZombie() || f_sig->IsZombie() || (doData && f_data->IsZombie()) ) {
    std::cout << "Input file does not exist" << std::endl;
    return;
  }

  // ----------------------------------------
  //  cards definitions
  // ----------------------------------------

  set<pair<int, int> > signal_points;  
  //Loop through list of every directory in the signal file.
  //if directory begins with "sr", excluding "srbase", make cards for it.
  TIter it(f_sig->GetListOfKeys());
  TKey* k;
  string keep = "sr";
  string skip = "srbase";
  while ((k = (TKey *)it())) {
    if (strncmp (k->GetTitle(), skip.c_str(), skip.length()) == 0) continue;
    if (strncmp (k->GetTitle(), keep.c_str(), keep.length()) == 0) {//it is a signal region
      string mt2_hist_name = (k->GetTitle());
      mt2_hist_name += "/h_n_mt2bins";
      TH1D* h_n_mt2bins = (TH1D*) f_sig->Get(TString(mt2_hist_name));
      int n_mt2bins = h_n_mt2bins->GetBinContent(1);
      for (int imt2 = 1; imt2 <= n_mt2bins; ++imt2) {//Make a separate card for each MT2 bin.
	if (isScan) {
	  int y_binwidth = 25;
	  int y_max = 1600;
	  if (signal.find("T2cc") != std::string::npos) {
	    y_binwidth = 5;
	    y_max = 800;
	  }
	  for (int im1 = 0; im1 <= 2300; im1 += 25) {
	    for (int im2 = 0; im2 <= y_max; im2 += y_binwidth) {
	      if (suppressUHmt2bin && TString(k->GetTitle()).Contains("UH") && imt2 == 1) continue;
	      int result = printCard(k->GetTitle(), imt2, signal, output_dir, im1, im2);   //MT2 and scan bins with no entries are handled by printCard function.
	      if (result > 0) signal_points.insert( make_pair(im1,im2) ); 
	    } // scanM2 loop
	  } // scanM1 loop
	} // if isScan
        else {
	  printCard(k->GetTitle(), imt2, signal, output_dir);   //MT2 bins with no entries are handled by printCard function.
	}
      }
    }
  }

  if (isScan) {
    TString filename = Form("%s/points_%s.txt",output_dir.c_str(),signal.c_str());
    ofstream ofile;
    ofile.open(filename);
    cout << "--------------------------------------------" << endl
	 << "- saw nonzero signal entries for the following points: " << endl;
    for (set<pair<int,int> >::iterator it=signal_points.begin(); it!=signal_points.end(); ++it) {
      cout << signal << "_" << (*it).first << "_" << (*it).second << endl;
      ofile << signal << "_" << (*it).first << "_" << (*it).second << endl;
    }
    ofile.close();
  }
  
  bmark->Stop("benchmark");
  cout << endl;
  cout << "------------------------------" << endl;
  cout << "CPU  Time:	" << Form( "%.01f s", bmark->GetCpuTime("benchmark")  ) << endl;
  cout << "Real Time:	" << Form( "%.01f s", bmark->GetRealTime("benchmark") ) << endl;
  cout << endl;
  delete bmark;

}
