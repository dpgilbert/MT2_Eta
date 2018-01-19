// C++
#include <iostream>
#include <vector>
#include <set>
#include <cmath>
#include <sstream>
#include <stdexcept>

// ROOT
#include "TDirectory.h"
#include "TTreeCache.h"
#include "Math/VectorUtil.h"
#include "TVector2.h"
#include "TBenchmark.h"
#include "TMath.h"
#include "TLorentzVector.h"
#include "TF1.h"

#include "RooRealVar.h"
#include "RooDataSet.h"

// Tools
#include "../CORE/Tools/utils.h"
#include "../CORE/Tools/goodrun.h"
#include "../CORE/Tools/dorky/dorky.h"
#include "../CORE/Tools/badEventFilter.h"

// header
#include "MT2Looper.h"

//MT2
#include "../MT2CORE/Plotting/PlotUtilities.h"
#include "../MT2CORE/applyWeights.h"

using namespace std;
using namespace mt2;
using namespace duplicate_removal;
using namespace RooFit;

class mt2tree;
class SR;

std::string toString(float in){
  std::stringstream ss;
  ss << in;
  return ss.str();
}

// generic binning for signal scans - need arrays since mt2 dimension will be variable
//   assuming here: 25 GeV binning, m1 from 0-2300, m2 from 0-2300
//   in Loop, also account for 5 GeV binning from 0-800 in T2cc scan
int n_m1bins = 93;
float* m1bins;
int n_m2bins = 93;
float* m2bins;

const int n_htbins = 5;
const float htbins[n_htbins+1] = {250, 450., 575., 1000., 1500., 3000.};
const int n_htbins2 = 6;
const float htbins2[n_htbins2+1] = {250., 350., 450., 575., 700., 1000., 3000.};
const int n_njbins = 4;
const float njbins[n_njbins+1] = {1, 2, 4, 7, 12};
const int n_nbjbins = 4;
const float nbjbins[n_nbjbins+1] = {0, 1, 2, 3, 6};
const int n_mt2bins_SRBase = 8;
const float SRBase_mt2bins[n_mt2bins_SRBase+1] = {200, 300, 400, 500, 600, 800, 1000, 1200, 1800}; 
const int n_ptVbins = 19;
float logstep(int i) {
  return TMath::Power(10, 2+4.5e-02*i);
}
const float ptVbins[n_ptVbins+1] = {100, logstep(1), logstep(2), logstep(3), logstep(4), logstep(5), logstep(6), 200, logstep(8), logstep(9), 
				    logstep(10), logstep(11), logstep(12), logstep(13), logstep(14), logstep(15), logstep(16), logstep(17), 800, 1200};

float doubleRatioWeight(float pTv) {
  if (pTv < 200 || pTv > 400) return 1.;
  else return (1/(0.79 + 0.00045*pTv));
}

//RooRealVars for unbinned data hist
RooRealVar* x_ = new RooRealVar( "x", "", 0., 50.);
RooRealVar* w_ = new RooRealVar( "w", "", 0., 1000.);

bool verbose = false;
bool useDRforGammaQCDMixing = true; // requires GenParticles
// turn on to apply weights to central value
bool applyWeights = false;
// turn on to apply btag sf to central value
bool applyBtagSF = true; // default true
// turn on to apply lepton sf to central value - take from babies
bool applyLeptonSFfromBabies = false;
// turn on to apply lepton sf to central value - reread from files
bool applyLeptonSFfromFiles = true; // default true
// turn on to apply lepton sf to central value also for 0L SR. values chosen by options above
bool applyLeptonSFtoSR = false;
// turn on to apply reweighting to ttbar based on top pt
bool applyTopPtReweight = false;
// add weights to correct for photon trigger efficiencies
bool applyPhotonTriggerWeights = false; //not needed since we apply trigger safe H/E cut
// add weights to correct for dilepton trigger efficiencies
bool applyDileptonTriggerWeights = true;
// use 2016 ICHEP ISR weights based on nisrMatch, signal and ttbar only
bool applyISRWeights = true;
// turn on to enable plots of MT2 with systematic variations applied. will only do variations for applied weights
bool doSystVariationPlots = true;
// turn on to apply Nvtx reweighting to MC
bool doNvtxReweight = false;
// turn on to apply nTrueInt reweighting to MC
bool doNTrueIntReweight = true;
// turn on to apply json file to data
bool applyJSON = true;
// veto on jets with pt > 30, |eta| > 3.0
bool doHFJetVeto = false;
// get signal scan nevents from file
bool doScanWeights = true;
// doesn't plot data for MT2 > 200 in signal regions
bool doBlindData = false;
// make variation histograms for tau efficiency
bool doGenTauVars = true;
// make variation histograms for renormalization and factorization scales
bool doRenormFactScaleReweight = true;
// apply JEC variations, 0 = none, -1 = down, 1 = up
int doJECVars = 0;
// make variation histograms for e+mu efficiency
bool doLepEffVars = true;
// make only minimal hists needed for results
bool doMinimalPlots = false;
// apply shape correction to GJ as a function of pTV
bool doubleRatioShapeCorrection = false;
// ignore scale1fb to run over test samples
bool ignoreScale1fb = false;
// print qcd CR event list
bool print_qcd_event_list = false;

//include pseudojet1 eta binning
bool include_eta_binning = false;

// load rphi fits to perform r_effective calculation.
bool doReffCalculation = true;
string rphi_file_name = "/home/users/fgolf/mt2/devel/MT2Analysis/scripts/qcdEstimate/output/full2016/qcdHistos.root";
TFile* rphi_file;
vector<TF1*> rphi_fits_data;
vector<TF1*> rphi_fits_mc;

std::ofstream fqcdlist;

TString stringsample;

MT2Looper::MT2Looper(){

}
MT2Looper::~MT2Looper(){

};

void MT2Looper::SetSignalRegions(){
  //SRVec =  getSignalRegionsZurich_jetpt30(); //same as getSignalRegionsZurich(), but with j1pt and j2pt cuts changed to 30 GeV
  //  SRVec =  getSignalRegionsJamboree(); //adds HT 200-450 regions
  if (! include_eta_binning) SRVec =  getSignalRegions2016(); //adds 2 bins at UH HT, for 3b
  else 
    {
      SRVec =  getSignalRegions2016Edit(); // adds eta binning
      cout << "Loaded Signal Regions" << endl;
    }
  SRVecMonojet = getSignalRegionsMonojet2016(); // first pass of monojet regions

  //store histograms with cut values for all variables
  for(unsigned int i = 0; i < SRVec.size(); i++){
    std::vector<std::string> vars = SRVec.at(i).GetListOfVariables();
    TDirectory * dir = (TDirectory*)outfile_->Get(("sr"+SRVec.at(i).GetName()).c_str());
    if (dir == 0) {
      dir = outfile_->mkdir(("sr"+SRVec.at(i).GetName()).c_str());
    } 
    dir->cd();
    for(unsigned int j = 0; j < vars.size(); j++){
      plot1D("h_"+vars.at(j)+"_"+"LOW",  1, SRVec.at(i).GetLowerBound(vars.at(j)), SRVec.at(i).srHistMap, "", 1, 0, 2);
      plot1D("h_"+vars.at(j)+"_"+"HI",   1, SRVec.at(i).GetUpperBound(vars.at(j)), SRVec.at(i).srHistMap, "", 1, 0, 2);
    }
    plot1D("h_n_mt2bins",  1, SRVec.at(i).GetNumberOfMT2Bins(), SRVec.at(i).srHistMap, "", 1, 0, 2);
    // fill with dummy value of weight 0 just to force it to make the histogram. need the binning info later
    plot1D("h_mt2bins",  -1, 0, SRVec.at(i).srHistMap, "; M_{T2} [GeV]", SRVec.at(i).GetNumberOfMT2Bins(), SRVec.at(i).GetMT2Bins());

    dir = (TDirectory*)outfile_->Get(("crsl"+SRVec.at(i).GetName()).c_str());
    if (dir == 0) {
      dir = outfile_->mkdir(("crsl"+SRVec.at(i).GetName()).c_str());
    } 
    dir->cd();
    std::vector<std::string> varsCRSL = SRVec.at(i).GetListOfVariablesCRSL();
    for(unsigned int j = 0; j < varsCRSL.size(); j++){
      plot1D("h_"+varsCRSL.at(j)+"_"+"LOW",  1, SRVec.at(i).GetLowerBoundCRSL(varsCRSL.at(j)), SRVec.at(i).crslHistMap, "", 1, 0, 2);
      plot1D("h_"+varsCRSL.at(j)+"_"+"HI",   1, SRVec.at(i).GetUpperBoundCRSL(varsCRSL.at(j)), SRVec.at(i).crslHistMap, "", 1, 0, 2);
    }
    plot1D("h_n_mt2bins",  1, SRVec.at(i).GetNumberOfMT2Bins(), SRVec.at(i).crslHistMap, "", 1, 0, 2);

    dir = (TDirectory*)outfile_->Get(("crgj"+SRVec.at(i).GetName()).c_str());
    if (dir == 0) {
      dir = outfile_->mkdir(("crgj"+SRVec.at(i).GetName()).c_str());
    } 
    dir->cd();
    for(unsigned int j = 0; j < vars.size(); j++){
      plot1D("h_"+vars.at(j)+"_"+"LOW",  1, SRVec.at(i).GetLowerBound(vars.at(j)), SRVec.at(i).crgjHistMap, "", 1, 0, 2);
      plot1D("h_"+vars.at(j)+"_"+"HI",   1, SRVec.at(i).GetUpperBound(vars.at(j)), SRVec.at(i).crgjHistMap, "", 1, 0, 2);
    }
    plot1D("h_n_mt2bins",  1, SRVec.at(i).GetNumberOfMT2Bins(), SRVec.at(i).crgjHistMap, "", 1, 0, 2);

    dir = (TDirectory*)outfile_->Get(("crrl"+SRVec.at(i).GetName()).c_str());
    if (dir == 0) {
      dir = outfile_->mkdir(("crrl"+SRVec.at(i).GetName()).c_str());
    } 
    dir->cd();
    for(unsigned int j = 0; j < vars.size(); j++){
      plot1D("h_"+vars.at(j)+"_"+"LOW",  1, SRVec.at(i).GetLowerBound(vars.at(j)), SRVec.at(i).crrlHistMap, "", 1, 0, 2);
      plot1D("h_"+vars.at(j)+"_"+"HI",   1, SRVec.at(i).GetUpperBound(vars.at(j)), SRVec.at(i).crrlHistMap, "", 1, 0, 2);
    }
    plot1D("h_n_mt2bins",  1, SRVec.at(i).GetNumberOfMT2Bins(), SRVec.at(i).crrlHistMap, "", 1, 0, 2);

    outfile_->cd();
  }

  //monojet regions: store histograms with cut values for all variables
  for(unsigned int i = 0; i < SRVecMonojet.size(); i++){
    std::vector<std::string> vars = SRVecMonojet.at(i).GetListOfVariables();
    TDirectory * dir = (TDirectory*)outfile_->Get(("sr"+SRVecMonojet.at(i).GetName()).c_str());
    if (dir == 0) {
      dir = outfile_->mkdir(("sr"+SRVecMonojet.at(i).GetName()).c_str());
    } 
    dir->cd();
    for(unsigned int j = 0; j < vars.size(); j++){
      plot1D("h_"+vars.at(j)+"_"+"LOW",  1, SRVecMonojet.at(i).GetLowerBound(vars.at(j)), SRVecMonojet.at(i).srHistMap, "", 1, 0, 2);
      plot1D("h_"+vars.at(j)+"_"+"HI",   1, SRVecMonojet.at(i).GetUpperBound(vars.at(j)), SRVecMonojet.at(i).srHistMap, "", 1, 0, 2);
    }
    plot1D("h_n_mt2bins",  1, SRVecMonojet.at(i).GetNumberOfMT2Bins(), SRVecMonojet.at(i).srHistMap, "", 1, 0, 2);

    dir = (TDirectory*)outfile_->Get(("crsl"+SRVecMonojet.at(i).GetName()).c_str());
    if (dir == 0) {
      dir = outfile_->mkdir(("crsl"+SRVecMonojet.at(i).GetName()).c_str());
    } 
    dir->cd();
    std::vector<std::string> varsCRSL = SRVecMonojet.at(i).GetListOfVariablesCRSL();
    for(unsigned int j = 0; j < varsCRSL.size(); j++){
      plot1D("h_"+varsCRSL.at(j)+"_"+"LOW",  1, SRVecMonojet.at(i).GetLowerBoundCRSL(varsCRSL.at(j)), SRVecMonojet.at(i).crslHistMap, "", 1, 0, 2);
      plot1D("h_"+varsCRSL.at(j)+"_"+"HI",   1, SRVecMonojet.at(i).GetUpperBoundCRSL(varsCRSL.at(j)), SRVecMonojet.at(i).crslHistMap, "", 1, 0, 2);
    }
    plot1D("h_n_mt2bins",  1, SRVecMonojet.at(i).GetNumberOfMT2Bins(), SRVecMonojet.at(i).crslHistMap, "", 1, 0, 2);

    dir = (TDirectory*)outfile_->Get(("crgj"+SRVecMonojet.at(i).GetName()).c_str());
    if (dir == 0) {
      dir = outfile_->mkdir(("crgj"+SRVecMonojet.at(i).GetName()).c_str());
    } 
    dir->cd();
    for(unsigned int j = 0; j < vars.size(); j++){
      plot1D("h_"+vars.at(j)+"_"+"LOW",  1, SRVecMonojet.at(i).GetLowerBound(vars.at(j)), SRVecMonojet.at(i).crgjHistMap, "", 1, 0, 2);
      plot1D("h_"+vars.at(j)+"_"+"HI",   1, SRVecMonojet.at(i).GetUpperBound(vars.at(j)), SRVecMonojet.at(i).crgjHistMap, "", 1, 0, 2);
    }
    plot1D("h_n_mt2bins",  1, SRVecMonojet.at(i).GetNumberOfMT2Bins(), SRVecMonojet.at(i).crgjHistMap, "", 1, 0, 2);
    outfile_->cd();
  }

  SRBase.SetName("srbase");
  SRBase.SetVar("mt2", 200, -1);
  SRBase.SetVar("j1pt", 30, -1);
  SRBase.SetVar("j2pt", 30, -1);
  SRBase.SetVar("deltaPhiMin", 0.3, -1);
  SRBase.SetVar("diffMetMhtOverMet", 0, 0.5);
  SRBase.SetVar("nlep", 0, 1);
  SRBase.SetVar("passesHtMet", 1, 2);
  SRBase.SetVarCRSL("mt2", 200, -1);
  SRBase.SetVarCRSL("j1pt", 30, -1);
  SRBase.SetVarCRSL("j2pt", 30, -1);
  SRBase.SetVarCRSL("deltaPhiMin", 0.3, -1);
  SRBase.SetVarCRSL("diffMetMhtOverMet", 0, 0.5);
  SRBase.SetVarCRSL("nlep", 1, 2);
  SRBase.SetVarCRSL("passesHtMet", 1, 2);
  SRBase.SetMT2Bins(n_mt2bins_SRBase, (float*) SRBase_mt2bins);

  std::vector<std::string> vars = SRBase.GetListOfVariables();
  TDirectory * dir = (TDirectory*)outfile_->Get((SRBase.GetName()).c_str());
  if (dir == 0) {
    dir = outfile_->mkdir((SRBase.GetName()).c_str());
  } 
  dir->cd();
  for(unsigned int j = 0; j < vars.size(); j++){
    plot1D("h_"+vars.at(j)+"_"+"LOW",  1, SRBase.GetLowerBound(vars.at(j)), SRBase.srHistMap, "", 1, 0, 2);
    plot1D("h_"+vars.at(j)+"_"+"HI",   1, SRBase.GetUpperBound(vars.at(j)), SRBase.srHistMap, "", 1, 0, 2);
  }
  plot1D("h_n_mt2bins",  1, SRBase.GetNumberOfMT2Bins(), SRBase.srHistMap, "", 1, 0, 2);
  outfile_->cd();

  std::vector<std::string> varsCRSL = SRBase.GetListOfVariablesCRSL();
  TDirectory * dirCRSL = (TDirectory*)outfile_->Get("crslbase");
  if (dirCRSL == 0) {
    dirCRSL = outfile_->mkdir("crslbase");
  } 
  dirCRSL->cd();
  for(unsigned int j = 0; j < varsCRSL.size(); j++){
    plot1D("h_"+varsCRSL.at(j)+"_"+"LOW",  1, SRBase.GetLowerBoundCRSL(varsCRSL.at(j)), SRBase.crslHistMap, "", 1, 0, 2);
    plot1D("h_"+varsCRSL.at(j)+"_"+"HI",   1, SRBase.GetUpperBoundCRSL(varsCRSL.at(j)), SRBase.crslHistMap, "", 1, 0, 2);
  }
  plot1D("h_n_mt2bins",  1, SRBase.GetNumberOfMT2Bins(), SRBase.crslHistMap, "", 1, 0, 2);
  outfile_->cd();

  std::vector<std::string> varsCRRL = SRBase.GetListOfVariables();
  TDirectory * dirCRRL = (TDirectory*)outfile_->Get("crrlbase");
  if (dirCRRL == 0) {
    dirCRRL = outfile_->mkdir("crrlbase");
  } 
  dirCRRL->cd();
  for(unsigned int j = 0; j < varsCRRL.size(); j++){
    plot1D("h_"+varsCRRL.at(j)+"_"+"LOW",  1, SRBase.GetLowerBound(varsCRRL.at(j)), SRBase.crrlHistMap, "", 1, 0, 2);
    plot1D("h_"+varsCRRL.at(j)+"_"+"HI",   1, SRBase.GetUpperBound(varsCRRL.at(j)), SRBase.crrlHistMap, "", 1, 0, 2);
  }
  plot1D("h_n_mt2bins",  1, SRBase.GetNumberOfMT2Bins(), SRBase.crrlHistMap, "", 1, 0, 2);
  outfile_->cd();

  //setup inclusive regions
  SR InclusiveHT250to450 = SRBase;
  InclusiveHT250to450.SetName("srbaseVL");
  InclusiveHT250to450.SetVar("ht", 250, 450);
  InclusiveHT250to450.SetVarCRSL("ht", 250, 450);
  InclusiveRegions.push_back(InclusiveHT250to450);

  SR InclusiveHT450to575 = SRBase;
  InclusiveHT450to575.SetName("srbaseL");
  InclusiveHT450to575.SetVar("ht", 450, 575);
  InclusiveHT450to575.SetVarCRSL("ht", 450, 575);
  InclusiveRegions.push_back(InclusiveHT450to575);

  SR InclusiveHT575to1000 = SRBase;
  InclusiveHT575to1000.SetName("srbaseM");
  InclusiveHT575to1000.SetVar("ht", 575, 1000);
  InclusiveHT575to1000.SetVarCRSL("ht", 575, 1000);
  InclusiveRegions.push_back(InclusiveHT575to1000);

  SR InclusiveHT1000to1500 = SRBase;
  InclusiveHT1000to1500.SetName("srbaseH");
  InclusiveHT1000to1500.SetVar("ht", 1000, 1500);
  InclusiveHT1000to1500.SetVarCRSL("ht", 1000, 1500);
  InclusiveRegions.push_back(InclusiveHT1000to1500);

  SR InclusiveHT1500toInf = SRBase;
  InclusiveHT1500toInf.SetName("srbaseUH");
  InclusiveHT1500toInf.SetVar("ht", 1500, -1);
  InclusiveHT1500toInf.SetVarCRSL("ht", 1500, -1);
  InclusiveRegions.push_back(InclusiveHT1500toInf);

  SR InclusiveNJets2to3 = SRBase;
  InclusiveNJets2to3.SetName("InclusiveNJets2to3");
  InclusiveNJets2to3.SetVar("njets", 2, 4);
  InclusiveRegions.push_back(InclusiveNJets2to3);

  SR InclusiveNJets4to6 = SRBase;
  InclusiveNJets4to6.SetName("InclusiveNJets4to6");
  InclusiveNJets4to6.SetVar("njets", 4, 7);
  InclusiveRegions.push_back(InclusiveNJets4to6);

  SR InclusiveNJets7toInf = SRBase;
  InclusiveNJets7toInf.SetName("InclusiveNJets7toInf");
  InclusiveNJets7toInf.SetVar("njets", 7, -1);
  InclusiveRegions.push_back(InclusiveNJets7toInf);

  SR InclusiveNBJets0 = SRBase;
  InclusiveNBJets0.SetName("InclusiveNBJets0");
  InclusiveNBJets0.SetVar("nbjets", 0, 1);
  InclusiveRegions.push_back(InclusiveNBJets0);

  SR InclusiveNBJets1 = SRBase;
  InclusiveNBJets1.SetName("InclusiveNBJets1");
  InclusiveNBJets1.SetVar("nbjets", 1, 2);
  InclusiveRegions.push_back(InclusiveNBJets1);

  SR InclusiveNBJets2 = SRBase;
  InclusiveNBJets2.SetName("InclusiveNBJets2");
  InclusiveNBJets2.SetVar("nbjets", 2, 3);
  InclusiveRegions.push_back(InclusiveNBJets2);

  SR InclusiveNBJets3toInf = SRBase;
  InclusiveNBJets3toInf.SetName("InclusiveNBJets3toInf");
  InclusiveNBJets3toInf.SetVar("nbjets", 3, -1);
  InclusiveRegions.push_back(InclusiveNBJets3toInf);

  for(unsigned int i=0; i<InclusiveRegions.size(); i++){
    TDirectory * dir = (TDirectory*)outfile_->Get((InclusiveRegions.at(i).GetName()).c_str());
    if (dir == 0) {
      dir = outfile_->mkdir((InclusiveRegions.at(i).GetName()).c_str());
    } 
  }

  // CRSL inclusive regions to isolate W+jets and ttbar
  CRSL_WJets = SRBase;
  CRSL_WJets.SetName("crslwjets");
  CRSL_WJets.SetVarCRSL("nbjets", 0, 1);
  CRSL_WJets.crslHistMap.clear();

  CRSL_TTbar = SRBase;
  CRSL_TTbar.SetName("crslttbar");
  CRSL_TTbar.SetVarCRSL("nbjets", 2, -1);
  CRSL_TTbar.crslHistMap.clear();

  // ----- monojet base regions

  SRBaseMonojet.SetName("srbaseJ");
  SRBaseMonojet.SetVar("ht", 250, -1);
  SRBaseMonojet.SetVar("njets", 1, 2);
  SRBaseMonojet.SetVar("nlep", 0, 1);
  SRBaseMonojet.SetVar("met", 250, -1);
  SRBaseMonojet.SetVar("deltaPhiMin", 0.3, -1);
  SRBaseMonojet.SetVar("diffMetMhtOverMet", 0, 0.5);
  SRBaseMonojet.SetVarCRSL("ht", 250, -1);
  SRBaseMonojet.SetVarCRSL("njets", 1, 2);
  SRBaseMonojet.SetVarCRSL("nlep", 1, 2);
  SRBaseMonojet.SetVarCRSL("met", 250, -1);
  SRBaseMonojet.SetVarCRSL("deltaPhiMin", 0.3, -1);
  SRBaseMonojet.SetVarCRSL("diffMetMhtOverMet", 0, 0.5);
  SRBaseMonojet.SetVarCRQCD("ht", 250, -1);
  SRBaseMonojet.SetVarCRQCD("j1pt", 250, -1);
  SRBaseMonojet.SetVarCRQCD("j2pt", 30, -1);
  SRBaseMonojet.SetVarCRQCD("nlep", 0, 1);
  SRBaseMonojet.SetVarCRQCD("njets", 2, 3);
  SRBaseMonojet.SetVarCRQCD("nbjets", 0, -1);
  SRBaseMonojet.SetVarCRQCD("met", 240, -1);
  SRBaseMonojet.SetVarCRQCD("deltaPhiMin", 0., 0.3);
  SRBaseMonojet.SetVarCRQCD("diffMetMhtOverMet", 0, 0.5);
  float SRBaseMonojet_mt2bins[8] = {250, 300, 400, 500, 600, 800, 1000, 1500};
  SRBaseMonojet.SetMT2Bins(7, SRBaseMonojet_mt2bins);

  vars = SRBaseMonojet.GetListOfVariables();
  dir = (TDirectory*)outfile_->Get((SRBaseMonojet.GetName()).c_str());
  if (dir == 0) {
    dir = outfile_->mkdir((SRBaseMonojet.GetName()).c_str());
  } 
  dir->cd();
  for(unsigned int j = 0; j < vars.size(); j++){
    plot1D("h_"+vars.at(j)+"_"+"LOW",  1, SRBaseMonojet.GetLowerBound(vars.at(j)), SRBaseMonojet.srHistMap, "", 1, 0, 2);
    plot1D("h_"+vars.at(j)+"_"+"HI",   1, SRBaseMonojet.GetUpperBound(vars.at(j)), SRBaseMonojet.srHistMap, "", 1, 0, 2);
  }
  plot1D("h_n_mt2bins",  1, SRBaseMonojet.GetNumberOfMT2Bins(), SRBaseMonojet.srHistMap, "", 1, 0, 2);
  outfile_->cd();

  varsCRSL = SRBaseMonojet.GetListOfVariablesCRSL();
  dirCRSL = (TDirectory*)outfile_->Get("crslbaseJ");
  if (dirCRSL == 0) {
    dirCRSL = outfile_->mkdir("crslbaseJ");
  } 
  dirCRSL->cd();
  for(unsigned int j = 0; j < varsCRSL.size(); j++){
    plot1D("h_"+varsCRSL.at(j)+"_"+"LOW",  1, SRBaseMonojet.GetLowerBoundCRSL(varsCRSL.at(j)), SRBaseMonojet.crslHistMap, "", 1, 0, 2);
    plot1D("h_"+varsCRSL.at(j)+"_"+"HI",   1, SRBaseMonojet.GetUpperBoundCRSL(varsCRSL.at(j)), SRBaseMonojet.crslHistMap, "", 1, 0, 2);
  }
  plot1D("h_n_mt2bins",  1, SRBaseMonojet.GetNumberOfMT2Bins(), SRBaseMonojet.crslHistMap, "", 1, 0, 2);
  outfile_->cd();

  dir = (TDirectory*)outfile_->Get("crgjbaseJ");
  if (dir == 0) {
    dir = outfile_->mkdir("crgjbaseJ");
  } 
  dir->cd();
  for(unsigned int j = 0; j < vars.size(); j++){
    plot1D("h_"+vars.at(j)+"_"+"LOW",  1, SRBaseMonojet.GetLowerBound(vars.at(j)), SRBaseMonojet.crgjHistMap, "", 1, 0, 2);
    plot1D("h_"+vars.at(j)+"_"+"HI",   1, SRBaseMonojet.GetUpperBound(vars.at(j)), SRBaseMonojet.crgjHistMap, "", 1, 0, 2);
  }
  plot1D("h_n_mt2bins",  1, SRBaseMonojet.GetNumberOfMT2Bins(), SRBaseMonojet.crgjHistMap, "", 1, 0, 2);
  outfile_->cd();

  dir = (TDirectory*)outfile_->Get("crdybaseJ");
  if (dir == 0) {
    dir = outfile_->mkdir("crdybaseJ");
  } 
  dir->cd();
  for(unsigned int j = 0; j < vars.size(); j++){
    plot1D("h_"+vars.at(j)+"_"+"LOW",  1, SRBaseMonojet.GetLowerBound(vars.at(j)), SRBaseMonojet.crdyHistMap, "", 1, 0, 2);
    plot1D("h_"+vars.at(j)+"_"+"HI",   1, SRBaseMonojet.GetUpperBound(vars.at(j)), SRBaseMonojet.crdyHistMap, "", 1, 0, 2);
  }
  plot1D("h_n_mt2bins",  1, SRBaseMonojet.GetNumberOfMT2Bins(), SRBaseMonojet.crdyHistMap, "", 1, 0, 2);
  outfile_->cd();

  dir = (TDirectory*)outfile_->Get("crrlbaseJ");
  if (dir == 0) {
    dir = outfile_->mkdir("crrlbaseJ");
  } 
  dir->cd();
  for(unsigned int j = 0; j < vars.size(); j++){
    plot1D("h_"+vars.at(j)+"_"+"LOW",  1, SRBaseMonojet.GetLowerBound(vars.at(j)), SRBaseMonojet.crrlHistMap, "", 1, 0, 2);
    plot1D("h_"+vars.at(j)+"_"+"HI",   1, SRBaseMonojet.GetUpperBound(vars.at(j)), SRBaseMonojet.crrlHistMap, "", 1, 0, 2);
  }
  plot1D("h_n_mt2bins",  1, SRBaseMonojet.GetNumberOfMT2Bins(), SRBaseMonojet.crrlHistMap, "", 1, 0, 2);
  outfile_->cd();

  vars = SRBaseMonojet.GetListOfVariablesCRQCD();
  dir = (TDirectory*)outfile_->Get("crqcdbaseJ");
  if (dir == 0) {
    dir = outfile_->mkdir("crqcdbaseJ");
  } 
  dir->cd();
  for(unsigned int j = 0; j < vars.size(); j++){
    plot1D("h_"+vars.at(j)+"_"+"LOW",  1, SRBaseMonojet.GetLowerBoundCRQCD(vars.at(j)), SRBaseMonojet.crqcdHistMap, "", 1, 0, 2);
    plot1D("h_"+vars.at(j)+"_"+"HI",   1, SRBaseMonojet.GetUpperBoundCRQCD(vars.at(j)), SRBaseMonojet.crqcdHistMap, "", 1, 0, 2);
  }
  plot1D("h_n_mt2bins",  1, SRBaseMonojet.GetNumberOfMT2Bins(), SRBaseMonojet.crqcdHistMap, "", 1, 0, 2);
  outfile_->cd();


  
  // inclusive in njets (mono+multi jet regions)
  SRBaseIncl.SetName("srbaseIncl");
  float SRBaseIncl_mt2bins[11] = {200, 250, 300, 400, 500, 600, 800, 1000, 1200, 1400, 1800};
  SRBaseIncl.SetMT2Bins(10, SRBaseIncl_mt2bins);

  dir = (TDirectory*)outfile_->Get((SRBaseIncl.GetName()).c_str());
  if (dir == 0) {
    dir = outfile_->mkdir((SRBaseIncl.GetName()).c_str());
  } 
  dir->cd();
  plot1D("h_n_mt2bins",  1, SRBaseIncl.GetNumberOfMT2Bins(), SRBaseIncl.srHistMap, "", 1, 0, 2);
  outfile_->cd();

  dirCRSL = (TDirectory*)outfile_->Get("crslbaseIncl");
  if (dirCRSL == 0) {
    dirCRSL = outfile_->mkdir("crslbaseIncl");
  } 
  dirCRSL->cd();
  plot1D("h_n_mt2bins",  1, SRBaseIncl.GetNumberOfMT2Bins(), SRBaseIncl.crslHistMap, "", 1, 0, 2);
  outfile_->cd();

  dir = (TDirectory*)outfile_->Get("crgjbaseIncl");
  if (dir == 0) {
    dir = outfile_->mkdir("crgjbaseIncl");
  } 
  dir->cd();
  plot1D("h_n_mt2bins",  1, SRBaseIncl.GetNumberOfMT2Bins(), SRBaseIncl.crgjHistMap, "", 1, 0, 2);
  outfile_->cd();

  dir = (TDirectory*)outfile_->Get("crdybaseIncl");
  if (dir == 0) {
    dir = outfile_->mkdir("crdybaseIncl");
  } 
  dir->cd();
  plot1D("h_n_mt2bins",  1, SRBaseIncl.GetNumberOfMT2Bins(), SRBaseIncl.crdyHistMap, "", 1, 0, 2);
  outfile_->cd();

  dir = (TDirectory*)outfile_->Get("crrlbaseIncl");
  if (dir == 0) {
    dir = outfile_->mkdir("crrlbaseIncl");
  } 
  dir->cd();
  plot1D("h_n_mt2bins",  1, SRBaseIncl.GetNumberOfMT2Bins(), SRBaseIncl.crrlHistMap, "", 1, 0, 2);
  outfile_->cd();

}


void MT2Looper::loop(TChain* chain, std::string sample, std::string output_dir){

  // Benchmark
  TBenchmark *bmark = new TBenchmark();
  bmark->Start("benchmark");

  gROOT->cd();
  TString output_name = Form("%s/%s.root",output_dir.c_str(),sample.c_str());
  cout << "[MT2Looper::loop] creating output file: " << output_name << endl;

  outfile_ = new TFile(output_name.Data(),"RECREATE") ; 

  // full 2016 dataset json, 36.26/fb:
  const char* json_file = "../babymaker/jsons/Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON_snt.txt";
  // to reproduce ICHEP, 12.9/fb:
  //  const char* json_file = "../babymaker/jsons/Cert_271036-276811_13TeV_PromptReco_Collisions16_JSON_snt.txt";
  if (applyJSON) {
    cout << "Loading json file: " << json_file << endl;
    set_goodrun_file(json_file);
  }

  // store the fits in a vector if we are doing r_eff calculation
  if(doReffCalculation){
    rphi_file = new TFile(rphi_file_name.c_str());
    if(rphi_file->IsZombie()){
      cout << "WARNING: could not open rphi file: " << rphi_file_name << endl;
      doReffCalculation = false;
    }else{
      string ht_strs[6] = {"ht250to450","ht450to575","ht575to1000","ht1000to1500","ht1500toInf","ht1000toInf"};
      string syst_strs[3] = {"","_systUp","_systDown"};
      for(int i=0; i<6; i++){
        for(int j=0; j<3; j++){
          rphi_fits_data.push_back((TF1*)rphi_file->Get(Form("rphi_%s/fit_data%s",ht_strs[i].c_str(),syst_strs[j].c_str())));
          rphi_fits_mc.push_back((TF1*)rphi_file->Get(Form("rphi_%s/fit_mc%s",ht_strs[i].c_str(),syst_strs[j].c_str())));
        }
      }
    }
      
  }

  eventFilter metFilterTxt;
  stringsample = sample;
  if (stringsample.Contains("data")) {
    cout<<"Loading bad event files ..."<<endl;
    // updated lists for full dataset
    metFilterTxt.loadBadEventList("/nfs-6/userdata/mt2utils/csc2015_Dec01.txt");
    metFilterTxt.loadBadEventList("/nfs-6/userdata/mt2utils/ecalscn1043093_Dec01.txt");
    metFilterTxt.loadBadEventList("/nfs-6/userdata/mt2utils/badResolutionTrack_Jan13.txt");
    metFilterTxt.loadBadEventList("/nfs-6/userdata/mt2utils/muonBadTrack_Jan13.txt");
    cout<<" ... finished!"<<endl;
  }

  h_nvtx_weights_ = 0;
  if (doNvtxReweight) {
    TFile* f_weights = new TFile("../babymaker/data/hists_reweight_zjets_Run2015B.root");
    TH1D* h_nvtx_weights_temp = (TH1D*) f_weights->Get("h_nVert_ratio");
    outfile_->cd();
    h_nvtx_weights_ = (TH1D*) h_nvtx_weights_temp->Clone("h_nvtx_weights");
    h_nvtx_weights_->SetDirectory(0);
    f_weights->Close();
    delete f_weights;
  }
  h_nTrueInt_weights_ = 0;
  if (doNTrueIntReweight) {
    TFile* f_weights = new TFile("../babymaker/data/puWeight2016.root");
    TH1D* h_nTrueInt_weights_temp = (TH1D*) f_weights->Get("pileupWeight");
    outfile_->cd();
    h_nTrueInt_weights_ = (TH1D*) h_nTrueInt_weights_temp->Clone("h_pileupWeight");
    h_nTrueInt_weights_->SetDirectory(0);
    f_weights->Close();
    delete f_weights;
  }
  if (verbose) cout<<__LINE__<<endl;
  h_sig_nevents_ = 0;
  h_sig_avgweight_btagsf_ = 0;
  h_sig_avgweight_btagsf_heavy_UP_ = 0;
  h_sig_avgweight_btagsf_light_UP_ = 0;
  h_sig_avgweight_btagsf_heavy_DN_ = 0;
  h_sig_avgweight_btagsf_light_DN_ = 0;
  h_sig_avgweight_isr_ = 0;
  h_sig_avgweight_isr_UP_ = 0;
  h_sig_avgweight_isr_DN_ = 0;
  if ((doScanWeights || applyBtagSF) && ((sample.find("T1") != std::string::npos) || (sample.find("T2") != std::string::npos) || (sample.find("T6") != std::string::npos))) {
    std::string scan_name = sample;
    if (sample.find("T1") != std::string::npos) scan_name = sample.substr(0,6);
    else if (sample.find("T2") != std::string::npos) scan_name = sample.substr(0,4);
    else if (sample.find("T6") != std::string::npos) scan_name = sample.substr(0,6);
    TFile* f_nsig_weights = new TFile(Form("../babymaker/data/nsig_weights_%s.root",scan_name.c_str()));
    TH2D* h_sig_nevents_temp = (TH2D*) f_nsig_weights->Get("h_nsig");
    TH2D* h_sig_avgweight_btagsf_temp = (TH2D*) f_nsig_weights->Get("h_avg_weight_btagsf");
    TH2D* h_sig_avgweight_btagsf_heavy_UP_temp = (TH2D*) f_nsig_weights->Get("h_avg_weight_btagsf_heavy_UP");
    TH2D* h_sig_avgweight_btagsf_light_UP_temp = (TH2D*) f_nsig_weights->Get("h_avg_weight_btagsf_light_UP");
    TH2D* h_sig_avgweight_btagsf_heavy_DN_temp = (TH2D*) f_nsig_weights->Get("h_avg_weight_btagsf_heavy_DN");
    TH2D* h_sig_avgweight_btagsf_light_DN_temp = (TH2D*) f_nsig_weights->Get("h_avg_weight_btagsf_light_DN");
    TH2D* h_sig_avgweight_isr_temp = (TH2D*) f_nsig_weights->Get("h_avg_weight_isr");
    TH2D* h_sig_avgweight_isr_UP_temp = (TH2D*) f_nsig_weights->Get("h_avg_weight_isr_UP");
    TH2D* h_sig_avgweight_isr_DN_temp = (TH2D*) f_nsig_weights->Get("h_avg_weight_isr_DN");
    h_sig_nevents_ = (TH2D*) h_sig_nevents_temp->Clone("h_sig_nevents");
    h_sig_avgweight_btagsf_ = (TH2D*) h_sig_avgweight_btagsf_temp->Clone("h_sig_avgweight_btagsf");
    h_sig_avgweight_btagsf_heavy_UP_ = (TH2D*) h_sig_avgweight_btagsf_heavy_UP_temp->Clone("h_sig_avgweight_btagsf_heavy_UP");
    h_sig_avgweight_btagsf_light_UP_ = (TH2D*) h_sig_avgweight_btagsf_light_UP_temp->Clone("h_sig_avgweight_btagsf_light_UP");
    h_sig_avgweight_btagsf_heavy_DN_ = (TH2D*) h_sig_avgweight_btagsf_heavy_DN_temp->Clone("h_sig_avgweight_btagsf_heavy_DN");
    h_sig_avgweight_btagsf_light_DN_ = (TH2D*) h_sig_avgweight_btagsf_light_DN_temp->Clone("h_sig_avgweight_btagsf_light_DN");
    h_sig_avgweight_isr_ = (TH2D*) h_sig_avgweight_isr_temp->Clone("h_sig_avgweight_isr");
    h_sig_avgweight_isr_UP_ = (TH2D*) h_sig_avgweight_isr_UP_temp->Clone("h_sig_avgweight_isr_UP");
    h_sig_avgweight_isr_DN_ = (TH2D*) h_sig_avgweight_isr_DN_temp->Clone("h_sig_avgweight_isr_DN");
    h_sig_nevents_->SetDirectory(0);
    h_sig_avgweight_btagsf_->SetDirectory(0);
    h_sig_avgweight_btagsf_heavy_UP_->SetDirectory(0);
    h_sig_avgweight_btagsf_light_UP_->SetDirectory(0);
    h_sig_avgweight_btagsf_heavy_DN_->SetDirectory(0);
    h_sig_avgweight_btagsf_light_DN_->SetDirectory(0);
    h_sig_avgweight_isr_->SetDirectory(0);
    h_sig_avgweight_isr_UP_->SetDirectory(0);
    h_sig_avgweight_isr_DN_->SetDirectory(0);
    f_nsig_weights->Close();
    delete f_nsig_weights;
  }
  if (verbose) cout<<__LINE__<<endl;

  if (applyLeptonSFfromFiles) {
    setElSFfile("../babymaker/lepsf/moriond17/scaleFactors_el_moriond_2017.root", "../babymaker/lepsf/moriond17/egammaEffi.txt_EGM2D.root" );
    setMuSFfile("../babymaker/lepsf/moriond17/TnP_NUM_LooseID_DENOM_generalTracks_VAR_map_pt_eta.root",
		"../babymaker/lepsf/moriond17/TnP_NUM_MiniIsoTight_DENOM_LooseID_VAR_map_pt_eta.root",
		"../babymaker/lepsf/moriond17/TnP_NUM_MediumIP2D_DENOM_LooseID_VAR_map_pt_eta.root",
		"../babymaker/lepsf/moriond17/Tracking_EfficienciesAndSF_BCDEFGH_hists.root");
    setVetoEffFile_fullsim("../babymaker/lepsf/vetoeff_emu_etapt_lostlep.root");  // same values for Moriond17 as ICHEP16
  }
  
  if (applyLeptonSFfromFiles && ((sample.find("T1") != std::string::npos) || (sample.find("T2") != std::string::npos))) {
    setElSFfile_fastsim("../babymaker/lepsf/moriond17/sf_el_vetoCB_mini01.root");  
    setMuSFfile_fastsim("../babymaker/lepsf/moriond17/sf_mu_looseID.root",
			"../babymaker/lepsf/moriond17/sf_mu_looseID_mini02.root",
			"../babymaker/lepsf/moriond17/sf_mu_mediumID_looseIP2D.root");
    setVetoEffFile_fastsim("../babymaker/lepsf/vetoeff_emu_etapt_T1tttt.root");  
  }
  if (verbose) cout<<__LINE__<<endl;

  // set up signal binning
  if (sample.find("T2cc") != std::string::npos) {
    // 5 GeV binning up to 800 GeV
    n_m2bins = 161;
  }
  m1bins = new float[n_m1bins+1];
  m2bins = new float[n_m2bins+1];
    
  for (int i = 0; i <= n_m1bins; ++i) {
    m1bins[i] = i*25.;
  }
  for (int i = 0; i <= n_m2bins; ++i) {
    // 5 GeV binning for T2cc
    if (sample.find("T2cc") != std::string::npos) m2bins[i] = i*5.;
    else m2bins[i] = i*25.;
  }
  
  cout << "[MT2Looper::loop] setting up histos" << endl;

  SetSignalRegions();

  SRNoCut.SetName("nocut");
  float SRNoCut_mt2bins[10] = {200, 300, 400, 500, 600, 800, 1000, 1200, 1400, 1800};
  SRNoCut.SetMT2Bins(9, SRNoCut_mt2bins);

  // open file to print list of QCD events
  if (print_qcd_event_list)
    fqcdlist.open(output_dir+"/qcd_event_list.txt", ios_base::app);  

  // These will be set to true if any SL GJ or DY control region plots are produced
  bool saveGJplots = false;
  bool saveDYplots = false;
  bool saveRLplots = false;
  bool saveRLELplots = false;
  bool saveRLMUplots = false;
  bool saveSLplots = false;
  bool saveSLMUplots = false;
  bool saveSLELplots = false;
  bool saveQCDplots = false;

  // File Loop
  int nDuplicates = 0;
  int nEvents = chain->GetEntries();
  unsigned int nEventsChain = nEvents;
  cout << "[MT2Looper::loop] running on " << nEventsChain << " events" << endl;
  unsigned int nEventsTotal = 0;
  TObjArray *listOfFiles = chain->GetListOfFiles();
  TIter fileIter(listOfFiles);
  TFile *currentFile = 0;
  while ( (currentFile = (TFile*)fileIter.Next()) ) {
    cout << "[MT2Looper::loop] running on file: " << currentFile->GetTitle() << endl;

    // Get File Content
    TFile f( currentFile->GetTitle() );
    TTree *tree = (TTree*)f.Get("mt2");
    TTreeCache::SetLearnEntries(10);
    tree->SetCacheSize(128*1024*1024);
    //mt2tree t(tree);
    
    // Use this to speed things up when not looking at genParticles
    //tree->SetBranchStatus("genPart_*", 0); 
    if (verbose) cout<<__LINE__<<endl;

    t.Init(tree);

    // Event Loop
    unsigned int nEventsTree = tree->GetEntriesFast();
    for( unsigned int event = 0; event < nEventsTree; ++event) {
      //      for( unsigned int event = 0; event < 10000; ++event) {
      
      t.GetEntry(event);

      //if (verbose && t.evt!=351710276) continue; 

      //---------------------
      // bookkeeping and progress report
      //---------------------
      ++nEventsTotal;
      if (nEventsTotal%10000==0) {
        ULong64_t i_permille = (int)floor(1000 * nEventsTotal / float(nEventsChain));
        if (isatty(1)) {
          printf("\015\033[32m ---> \033[1m\033[31m%4.1f%%"
                 "\033[0m\033[32m <---\033[0m\015", i_permille/10.);
          fflush(stdout);
        }
        else {
          cout<<i_permille/10.<<" ";
          if (nEventsTotal%100000==0) cout<<endl;
        }
      }
      if (verbose) cout<<__LINE__<<endl;

      //---------------------
      // skip duplicates -- needed when running on mutiple streams in data
      //---------------------
      if( t.isData ) {
	DorkyEventIdentifier id(t.run, t.evt, t.lumi);
	if (is_duplicate(id) ){
	  ++nDuplicates;
	  continue;
	}
      }
      if (verbose) cout<<__LINE__<<endl;

      //---------------------
      // basic event selection and cleaning
      //---------------------
      
      if( applyJSON && t.isData && !goodrun(t.run, t.lumi) ) continue;
      if (verbose) cout<<__LINE__<<endl;

      if (t.nVert == 0) continue;
      if (verbose) cout<<__LINE__<<endl;

      // MET filters (first 2 only in data)
      if (t.isData) {
        if (!t.Flag_globalTightHalo2016Filter) continue; 
        if (verbose) cout<<__LINE__<<endl;
        if (!t.Flag_badMuonFilterV2) continue;
	if (verbose) cout<<__LINE__<<endl;
	if (!t.Flag_eeBadScFilter) continue; 
	if (verbose) cout<<__LINE__<<endl;
      }
      if (!stringsample.Contains("2015")) { // several filters are not in 2015 MC
	if (!t.Flag_goodVertices) continue;
	if (verbose) cout<<__LINE__<<endl;
	if (!t.Flag_HBHENoiseFilter) continue;
	if (verbose) cout<<__LINE__<<endl;
	if (!t.Flag_HBHENoiseIsoFilter) continue;
	if (verbose) cout<<__LINE__<<endl;
	if (!t.Flag_EcalDeadCellTriggerPrimitiveFilter) continue;
	if (verbose) cout<<__LINE__<<endl;
	if (!t.Flag_badChargedHadronFilterV2) continue; 
	if (verbose) cout<<__LINE__<<endl;
      }

      // random events with ht or met=="inf" or "nan" that don't get caught by the filters...
      if(isinf(t.met_pt) || isnan(t.met_pt) || isinf(t.ht) || isnan(t.ht)){
        cout << "WARNING: bad event with infinite MET/HT! " << t.run << ":" << t.lumi << ":" << t.evt
             << ", met=" << t.met_pt << ", ht=" << t.ht << endl;
        continue;
      }

      // catch events with unphysical jet pt
      if(t.jet_pt[0] > 13000.){
	cout << "WARNING: bad event with unphysical jet pt! " << t.run << ":" << t.lumi << ":" << t.evt
	     << ", met=" << t.met_pt << ", ht=" << t.ht << ", jet_pt=" << t.jet_pt[0] << endl;
        continue;
      }

      // // txt MET filters (data only)
      // if (t.isData && metFilterTxt.eventFails(t.run, t.lumi, t.evt)) {
      // 	//cout<<"Found bad event in data: "<<t.run<<", "<<t.lumi<<", "<<t.evt<<endl;
      // 	continue;
      // }
      // if (verbose) cout<<__LINE__<<endl;


      // flag signal samples
      if (t.evt_id >= 1000) isSignal_ = true;
      else isSignal_ = false;
      if (verbose) cout<<__LINE__<<", signal "<<isSignal_<< endl;
      //isSignal_ = false;  //GZ needed to run over ETH babies

      // Jet ID Veto
      bool passJetID = true;
      //if (t.nJet30FailId > 0) continue;
      if (t.nJet30FailId > 0) passJetID = false;
      if (verbose) cout<<__LINE__<<endl;
      // filter for bad fastsim jets
      if (isSignal_ && t.nJet20BadFastsim > 0) continue;
      if (verbose) cout<<__LINE__<<endl;
      // remove low pt QCD samples 
      if (t.evt_id >= 100 && t.evt_id < 109) continue;
      // remove low HT QCD samples 
      if (t.evt_id >= 120 && t.evt_id < 123) continue;
      int maxQCD = 153; // We want to include 153 when making photon plots, but we start from 154 for everything else
      if (t.evt_id >= 151 && t.evt_id < maxQCD) continue;
      // note that ETH has an offset in QCD numbering..

      if (verbose) cout<<__LINE__<<endl;
      //if (isSignal_ && (t.GenSusyMScan1 != 1600 || t.GenSusyMScan2 != 0)) continue;
      
      // if (isSignal_ 
      // 	  && !(t.GenSusyMScan1 == 1400 && t.GenSusyMScan2 == 200 && sample  == "T1qqqq_1400_200")
      // 	  && !(t.GenSusyMScan1 == 800 && t.GenSusyMScan2 == 600 && sample  == "T1qqqq_800_600")
      // 	  && !(t.GenSusyMScan1 == 1000 && t.GenSusyMScan2 == 775 && sample  == "T1tttt_1000_775")
      // 	  && !(t.GenSusyMScan1 == 1700 && t.GenSusyMScan2 == 600 && sample  == "T1tttt_1700_600")
      // 	  && !(t.GenSusyMScan1 == 300 && t.GenSusyMScan2 == 200 && sample  == "T2tt_300_200")
      // 	  && !(t.GenSusyMScan1 == 800 && t.GenSusyMScan2 == 350 && sample  == "T2tt_800_350")
      // 	  && !(t.GenSusyMScan1 == 400 && t.GenSusyMScan2 == 200 && sample  == "T2qq_400_200")
      //   ) continue;
	  
      // note: this will double count some leptons, since reco leptons can appear as PFcands
      nlepveto_ = t.nMuons10 + t.nElectrons10 + t.nPFLep5LowMT + t.nPFHad10LowMT;

      //---------------------
      // set event level variables
      //---------------------
      
      //initialize the event variables
      mt2_           = t.mt2;
      ht_            = t.ht;
      met_pt_        = t.met_pt;
      met_phi_       = t.met_phi;
      mht_pt_        = t.mht_pt;
      mht_phi_       = t.mht_phi;
      jet1_pt_       = t.jet1_pt;
      jet2_pt_       = t.jet2_pt;
      deltaPhiMin_   = t.deltaPhiMin;
      diffMetMht_    = t.diffMetMht;
      nJet30_        = t.nJet30;
      nBJet20_       = t.nBJet20;
      
      //apply JEC variations
      if (doJECVars == 1) {
	mt2_           = t.mt2JECup;
	ht_            = t.htJECup;
	met_pt_        = t.met_ptJECup;
	met_phi_       = t.met_phiJECup;
	mht_pt_        = t.mht_ptJECup;
	mht_phi_       = t.mht_phiJECup;
	jet1_pt_       = t.jet1_ptJECup;
	jet2_pt_       = t.jet2_ptJECup;
	deltaPhiMin_   = t.deltaPhiMinJECup;
	diffMetMht_    = t.diffMetMhtJECup;
	nJet30_        = t.nJet30JECup;
	nBJet20_       = t.nBJet20JECup;
      }
      else if (doJECVars == -1) {
	mt2_           = t.mt2JECdn;
	ht_            = t.htJECdn;
	met_pt_        = t.met_ptJECdn;
	met_phi_       = t.met_phiJECdn;
	mht_pt_        = t.mht_ptJECdn;
	mht_phi_       = t.mht_phiJECdn;
	jet1_pt_       = t.jet1_ptJECdn;
	jet2_pt_       = t.jet2_ptJECdn;
	deltaPhiMin_   = t.deltaPhiMinJECdn;
	diffMetMht_    = t.diffMetMhtJECdn;
	nJet30_        = t.nJet30JECdn;
	nBJet20_       = t.nBJet20JECdn;
      }
      else if (doJECVars == 0) {}
      else { cerr << "WARNING: options doJECVars has illegal value!" << endl; }
      
      //---------------------
      // set weights and start making plots
      //---------------------
      outfile_->cd();
      // const float lumi = 12.9; //ICHEP 2016
      const float lumi = 35.867; // full 2016
    
      evtweight_ = 1.;
      if (verbose) cout<<__LINE__<<endl;
      // apply relevant weights to MC
      if (!t.isData) {
	if (isSignal_ && doScanWeights) {
	  int binx = h_sig_nevents_->GetXaxis()->FindBin(t.GenSusyMScan1);
	  int biny = h_sig_nevents_->GetYaxis()->FindBin(t.GenSusyMScan2);
	  double nevents = h_sig_nevents_->GetBinContent(binx,biny);
	  evtweight_ = lumi * t.evt_xsec*t.evt_filter*1000./nevents; // assumes xsec, filter are already filled correctly
	} else {
	  if (!ignoreScale1fb) evtweight_ = t.evt_scale1fb * lumi;
	}
	if (verbose) cout<<__LINE__<<endl;
	if (applyBtagSF) {
	  // remove events with 0 btag weight for now..
	  if (fabs(t.weight_btagsf) < 0.001) continue;
	  evtweight_ *= t.weight_btagsf;
	  if (isSignal_) {
	    int binx = h_sig_avgweight_btagsf_->GetXaxis()->FindBin(t.GenSusyMScan1);
	    int biny = h_sig_avgweight_btagsf_->GetYaxis()->FindBin(t.GenSusyMScan2);
	    float avgweight_btagsf = h_sig_avgweight_btagsf_->GetBinContent(binx,biny);
	    evtweight_ /= avgweight_btagsf;
	  }
	}
	// get pu weight from hist, restrict range to nvtx 4-31
	if (doNvtxReweight) {
	  int nvtx_input = t.nVert;
	  if (t.nVert > 31) nvtx_input = 31;
	  if (t.nVert < 4) nvtx_input = 4;
	  float puWeight = h_nvtx_weights_->GetBinContent(h_nvtx_weights_->FindBin(nvtx_input));
	  evtweight_ *= puWeight;
	}
	if (doNTrueIntReweight) {
	  int nTrueInt_input = t.nTrueInt;
	  float puWeight = h_nTrueInt_weights_->GetBinContent(h_nTrueInt_weights_->FindBin(nTrueInt_input));
	  evtweight_ *= puWeight;
	}
	// prioritize files for lepton SF, if we accidentally gave both options
	if (applyLeptonSFfromFiles) {
	  fillLepSFWeightsFromFile();
	  evtweight_ *= weight_lepsf_cr_;
	  fillLepCorSRfromFile(); // takes care of cases using applyLeptonSFtoSR
	  evtweight_ *= cor_lepeff_sr_;
	}
	// taking lepton SF from babies
	else if (applyLeptonSFfromBabies) {
	  weight_lepsf_cr_ = t.weight_lepsf;
	  weight_lepsf_cr_UP_ = t.weight_lepsf_UP;
	  weight_lepsf_cr_DN_ = t.weight_lepsf_DN;
	  evtweight_ *= weight_lepsf_cr_;
	  if (applyLeptonSFtoSR) {
	    cor_lepeff_sr_ = t.weight_lepsf_0l;
	    unc_lepeff_sr_UP_ = t.weight_lepsf_0l_UP;
	    unc_lepeff_sr_DN_ = t.weight_lepsf_0l_DN;
	    evtweight_ *= cor_lepeff_sr_;
	  }
	  // if not using 0l weights, try to correct uncertainties..
	  else {
	    cor_lepeff_sr_ = 1.;
	    unc_lepeff_sr_UP_ = t.weight_lepsf_0l_UP / t.weight_lepsf_0l;
	    unc_lepeff_sr_DN_ = t.weight_lepsf_0l_DN / t.weight_lepsf_0l;
	  }
	}
	if (isSignal_ && applyISRWeights) {
	  int binx = h_sig_avgweight_isr_->GetXaxis()->FindBin(t.GenSusyMScan1);
	  int biny = h_sig_avgweight_isr_->GetYaxis()->FindBin(t.GenSusyMScan2);
	  float avgweight_isr = h_sig_avgweight_isr_->GetBinContent(binx,biny);
	  evtweight_ *= t.weight_isr / avgweight_isr;
	}
	else if (applyISRWeights && t.evt_id >= 301 && t.evt_id <= 303) {
	  float avgweight_isr = getAverageISRWeight(t.evt_id, 0);
	  evtweight_ *= t.weight_isr / avgweight_isr;
	}
	if (applyTopPtReweight && t.evt_id >= 300 && t.evt_id < 400) {
	  evtweight_ *= t.weight_toppt;
	}

        // fix for 2015 dyjets xsecs
        if(stringsample.Contains("2015dyjetsll")){
          if(t.evt_id == 702) evtweight_ *= 1.0573;
          if(t.evt_id == 703) evtweight_ *= 0.9588;
          if(t.evt_id == 704) evtweight_ *= 1.0329;
          if(t.evt_id == 705) evtweight_ *= 0.9945;
        }
        // fix for 2015 wjets xsecs
        if(stringsample.Contains("2015wjets")){
          if(t.evt_id == 505) evtweight_ *= 12.05 / 18.77;
        }

      } // !isData

      //weights for renorm/factorization scale variations
      if (doRenormFactScaleReweight && t.LHEweight_wgt[0] != 0 && t.LHEweight_wgt[0] != -999) {
	if (!isSignal_) { 
	  evtweight_renormUp_ = evtweight_ /  t.LHEweight_wgt[0] *  t.LHEweight_wgt[4];
	  evtweight_renormDn_ = evtweight_ /  t.LHEweight_wgt[0] *  t.LHEweight_wgt[8];
	}
	else {
	  
	  evtweight_renormUp_ = evtweight_ ;
	  evtweight_renormDn_ = evtweight_ ;
	  
	  //commented out until sig_avgweight histogram is added
	  
	  // int binx = h_sig_avgweight_renorm_DN_->GetXaxis()->FindBin(t.GenSusyMScan1);
	  // int biny = h_sig_avgweight_renorm_DN_->GetYaxis()->FindBin(t.GenSusyMScan2);
	  // float weight_renorm_UP = evtweight_ /  t.LHEweight_wgt[0] *  t.LHEweight_wgt[4];
	  // float avgweight_renorm_UP = h_sig_avgweight_renorm_UP_->GetBinContent(binx,biny);
	  // float weight_renorm_DN = evtweight_ /  t.LHEweight_wgt[0] *  t.LHEweight_wgt[8];
	  // float avgweight_renorm_DN = h_sig_avgweight_renorm_DN_->GetBinContent(binx,biny);      
	  // evtweight_renormUp_ = weight_renorm_UP / avgweight_renorm_UP;
	  // evtweight_renormDn_ = weight_renorm_DN / avgweight_renorm_DN;
	}
      }
    
      if (verbose) cout<<__LINE__<<endl;

      plot1D("h_nvtx",       t.nVert,       evtweight_, h_1d_global, ";N(vtx)", 80, 0, 80);
      plot1D("h_mt2",       t.mt2,       evtweight_, h_1d_global, ";M_{T2} [GeV]", 80, 0, 800);

      // variables for single lep control region
      bool doSLplots = false;
      bool doSLMUplots = false;
      bool doSLELplots = false;
      leppt_ = -1.;
      mt_ = -1.;
      if (verbose) cout<<__LINE__<<endl;
      // count number of forward jets
      nJet30Eta3_ = 0;
      for (int ijet = 0; ijet < t.njet; ++ijet) {
	if (t.jet_pt[ijet] > 30. && fabs(t.jet_eta[ijet]) > 3.0) ++nJet30Eta3_;
      }
      if (verbose) cout<<__LINE__<<endl;

      // veto on forward jets
      if (doHFJetVeto && nJet30Eta3_ > 0) continue;
      if (verbose) cout<<__LINE__<<endl;

      // check jet id for monojet - don't apply to signal
      //  following ETH, just check leading jet (don't check if jet is central..)
      passMonojetId_ = false;
      if ( nJet30_ >= 1 && (isSignal_ || (t.jet_id[0] >= 4)) ) passMonojetId_ = true;

      // simple counter to check for 1L CR
      if (t.nLepLowMT == 1) {
	doSLplots = true;

	// find unique lepton to plot pt,MT and get flavor
	bool foundlep = false;
	int cand_pdgId = 0;

	// if reco leps, check those
	if (t.nlep > 0) {
      	  for (int ilep = 0; ilep < t.nlep; ++ilep) {
      	    float mt = sqrt( 2 * met_pt_ * t.lep_pt[ilep] * ( 1 - cos( met_phi_ - t.lep_phi[ilep]) ) );
	    if (mt > 100.) continue;

	    // good candidate: save
	    leppt_ = t.lep_pt[ilep];
	    mt_ = mt;
	    cand_pdgId = t.lep_pdgId[ilep];
	    foundlep = true;
	    break;
	  }
	} // t.nlep > 0

	// otherwise check PF leps that don't overlap with a reco lepton
	if (!foundlep && t.nPFLep5LowMT > 0) {
      	  for (int itrk = 0; itrk < t.nisoTrack; ++itrk) {
	    float pt = t.isoTrack_pt[itrk];
	    if (pt < 5.) continue;
      	    int pdgId = t.isoTrack_pdgId[itrk];
	    if ((abs(pdgId) != 11) && (abs(pdgId) != 13)) continue;
      	    if (t.isoTrack_absIso[itrk]/pt > 0.2) continue;
      	    float mt = sqrt( 2 * met_pt_ * pt * ( 1 - cos( met_phi_ - t.isoTrack_phi[itrk]) ) );
	    if (mt > 100.) continue;

	    // check overlap with reco leptons
	    bool overlap = false;
	    for(int ilep = 0; ilep < t.nlep; ilep++){
	      float thisDR = DeltaR(t.isoTrack_eta[itrk], t.lep_eta[ilep], t.isoTrack_phi[itrk], t.lep_phi[ilep]);
	      if (thisDR < 0.01) {
		overlap = true;
		break;
	      }
	    } // loop over reco leps
	    if (overlap) continue;

	    // good candidate: save
	    leppt_ = pt;
	    mt_ = mt;
	    cand_pdgId = pdgId;
	    foundlep = true;
	    break;
	  } // loop on isotracks
	}

	if (!foundlep) {
	  std::cout << "MT2Looper::Loop: WARNING! didn't find a lowMT candidate when expected: evt: " << t.evt
		    << ", nMuons10: " << t.nMuons10 << ", nElectrons10: " << t.nElectrons10 
		    << ", nPFLep5LowMT: " << t.nPFLep5LowMT << ", nLepLowMT: " << t.nLepLowMT << std::endl;
	}

	if (abs(cand_pdgId) == 11) doSLELplots = true;
	else if (abs(cand_pdgId) == 13) doSLMUplots = true;

      } // for 1L control region

      // Variables for gamma+jets control region
      if ( stringsample.Contains("gjet") ) t.evt_id = 201; //protection for samples with broken evt_id

      bool doGJplots = false;
      if (t.ngamma > 0) {
	if (t.isData) {
	  doGJplots = true;
	}
	else if (!useDRforGammaQCDMixing) {
	  if ( (t.evt_id < 200 && t.gamma_mcMatchId[0]>0  && t.gamma_genIso04[0]<5)    // Reject true photons from QCD (iso is always 0 for now)
	       || (t.evt_id >=200 && t.gamma_mcMatchId[0]==0 )                           // Reject unmatched photons from Gamma+Jets
	       || (t.evt_id >=200 && t.gamma_mcMatchId[0] >0 && t.gamma_genIso04[0]>5)    // Reject non-iso photons from Gamma+Jets
            )
          { doGJplots = false; }
	  else {
	    doGJplots = true;
	  }	    
	}
	else {
	  if ( (t.evt_id < 200 && t.gamma_mcMatchId[0]>0 && t.gamma_drMinParton[0] > 0.4)   // reject DR>0.4 photons from QCD
	       || (t.evt_id >=200 && t.gamma_mcMatchId[0]==0 )   // Reject unmatched photons from Gamma+Jets (should be small)    
	       || (t.evt_id >=200 && t.gamma_drMinParton[0]<0.4 )   // Reject fragmentation photons from Gamma+Jets (only there in samples with DR>0.05)
	       || (t.evt_id >=300) || (t.evt_id < 0) || (isSignal_) // Don't make plots for other samples
            )
          { doGJplots = false; }
	  else {
	    doGJplots = true;
	  }
	} //useDRforGammaQCDMixing
      } // ngamma > 0

      // Variables for Zll (DY) control region
      bool doDYplots = false;
      bool doLowPtSFplots = false;
      bool doOFplots = false;
      bool doLowPtOFplots = false;
      if (t.nlep == 2 && !isSignal_) {
	bool passSFtrig = (!t.isData || t.HLT_DoubleEl || t.HLT_DoubleMu || t.HLT_Photon165_HE10 || t.HLT_DoubleMu_NonIso || t.HLT_SingleMu_NonIso || t.HLT_DoubleEl33);
	bool passOFtrig =  (!t.isData || t.HLT_MuX_Ele12 || t.HLT_Mu8_EleX || t.HLT_Mu30_Ele30_NonIso || t.HLT_Mu33_Ele33_NonIso || t.HLT_Photon165_HE10 || t.HLT_SingleMu_NonIso);
	if ( (t.lep_charge[0] * t.lep_charge[1] == -1)
             && (abs(t.lep_pdgId[0]) == 13 ||  t.lep_tightId[0] > 0 )
             && (abs(t.lep_pdgId[1]) == 13 ||  t.lep_tightId[1] > 0 )
	     && t.lep_pt[0] > 100 && t.lep_pt[1] > 30               ) {
	  // no additional explicit lepton veto
	  // i.e. implicitly allow 3rd PF lepton or hadron
	  //nlepveto_ = 0; 
	  if (abs(t.lep_pdgId[0]) == abs(t.lep_pdgId[1]) && passSFtrig ) { // SF
	    if      (t.zll_pt > 200 && fabs(t.zll_mass - 91.19) < 20)                     doDYplots = true;
	    else if (t.zll_pt < 200 && fabs(t.zll_mass - 91.19) > 20 && t.zll_mass > 50.) doLowPtSFplots = true;
	  }
	  else if (abs(t.lep_pdgId[0]) != abs(t.lep_pdgId[1]) && passOFtrig ) { // OF
	    if      (t.zll_pt > 200 && fabs(t.zll_mass - 91.19) < 20)                     doOFplots = true;
	    else if (t.zll_pt < 200 && fabs(t.zll_mass - 91.19) > 20 && t.zll_mass > 50.) doLowPtOFplots = true;

	  }
	  if (!t.isData && applyDileptonTriggerWeights){
	    evtweight_ *= getDileptonTriggerWeight(t.lep_pt[0], t.lep_pdgId[0], t.lep_pt[1], t.lep_pdgId[1], 0);
	  }
	}
      } // nlep == 2
      
      // Variables for Removed single lepton (RL) region
      //muon only
      bool doRLplots = false;
      bool doRLMUplots = false;
      bool doRLELplots = false;
      if ( t.nlep == 1 && !isSignal_) {
	if ( t.lep_pt[0] > 30 && fabs(t.lep_eta[0])<2.5 && nBJet20_ == 0) { // raise threshold to avoid Ele23 in MC
	  if (abs(t.lep_pdgId[0])==13) { // muons
            if ( !t.isData || t.HLT_SingleMu || t.HLT_SingleMu_NonIso )  doRLMUplots = true;
          }
	  if (abs(t.lep_pdgId[0])==11) { // electrons
            if ( (!t.isData || t.HLT_SingleEl || t.HLT_SingleEl_NonIso )   // Ele23 trigger not present in MC. Need to keep lepton threshold high
                 && t.lep_relIso03[0]<0.1 // tighter selection for electrons
		 && t.lep_relIso03[0]*t.lep_pt[0]<5 // tighter selection for electrons
		 && t.lep_tightId[0]>2
		 && fabs(t.lep_eta[0])<2.1
              ) 
	      doRLELplots = true;
	  }
	  // no additional explicit lepton veto
	  // i.e. implicitly allow 2nd PF lepton or hadron
	  //nlepveto_ = 0; 
	  if (doRLMUplots || doRLELplots )  doRLplots = true;
	}
      } // nlep == 1

      // check for failing minDphi, for QCD CR
      bool doQCDplots = false;
      if (nJet30_ >= 2 && deltaPhiMin_ < 0.3) doQCDplots = true;

      ////////////////////////////////////
      /// done with overall selection  /// 
      ////////////////////////////////////
      ///   time to fill histograms    /// 
      ////////////////////////////////////

      if (doGJplots) {
        saveGJplots = true;
	if (t.gamma_nJet30FailId == 0) {

	  //// To test Madgraph fragmentation (need to remove drMinParton requirement from above) //
	  //if ( t.gamma_mcMatchId[0] > 0 ) {
	  //  if (t.evt_id < 200 || t.gamma_drMinParton[0]>0.4) fillHistosCRGJ("crgj"); // Prompt photon
	  //  if (t.evt_id >=200 && t.gamma_drMinParton[0]<0.4)  fillHistosCRGJ("crgj", "FragGJ");
	  //  if (t.evt_id < 200 && t.gamma_drMinParton[0]<0.05) fillHistosCRGJ("crgj", "FragGJ");
	  //}
	  //// End of Madgraph fragmentation tests //
	  if ( t.gamma_mcMatchId[0] > 0 || t.isData) fillHistosCRGJ("crgj"); // Prompt photon 
	  else fillHistosCRGJ("crgj", "Fake");
	}
      }
      if (verbose) cout<<__LINE__<<endl;

      if (!passJetID) continue;
      if (verbose) cout<<__LINE__<<endl;

      maxQCD = 154; // We want to include 153 when making photon plots, but we start from 154 for everything else
      if (t.evt_id >= 151 && t.evt_id < maxQCD) continue;

      if ( !(t.isData && doBlindData && mt2_ > 200) ) {
	if (verbose) cout<<__LINE__<<endl;

	fillHistos(SRNoCut.srHistMap, SRNoCut.GetNumberOfMT2Bins(), SRNoCut.GetMT2Bins(), SRNoCut.GetName(), "");

	fillHistosSignalRegion("sr");

	fillHistosSRBase();
	fillHistosInclusive();
      }

      if (doDYplots || doOFplots || doLowPtSFplots || doLowPtOFplots) {
        saveDYplots = true;
	if (verbose) cout<<__LINE__<<endl;
        if (doDYplots) fillHistosCRDY("crdy");
        if (doOFplots) fillHistosCRDY("crdy", "emu");
        if (doLowPtSFplots) fillHistosCRDY("crdy", "LowPt");
        if (doLowPtOFplots) fillHistosCRDY("crdy", "LowPtemu");
      }
      if (doRLplots) {
        saveRLplots = true;
	if (verbose) cout<<__LINE__<<endl;
        fillHistosCRRL("crrl");
      }
      if (doRLELplots && !doMinimalPlots) {
        saveRLELplots = true;
        fillHistosCRRL("crrlel");
      }
      if (doRLMUplots && !doMinimalPlots) {
        saveRLMUplots = true;
        fillHistosCRRL("crrlmu");
      }
      if (doSLplots) {
        saveSLplots = true;
	if (verbose) cout<<__LINE__<<endl;
        fillHistosCRSL("crsl");
      }
      if (doSLMUplots && !doMinimalPlots) {
        saveSLMUplots = true;
        fillHistosCRSL("crslmu");
      }
      if (doSLELplots && !doMinimalPlots) {
        saveSLELplots = true;
        fillHistosCRSL("crslel");
      }
      if (doQCDplots) {
        saveQCDplots = true;
	if (verbose) cout<<__LINE__<<endl;
        fillHistosCRQCD("crqcd");
      }


    }//end loop on events in a file
  
    delete tree;
    f.Close();
  }//end loop on files
  
  cout << "[MT2Looper::loop] processed " << nEventsTotal << " events" << endl;
  if ( nEventsChain != nEventsTotal ) {
    std::cout << "ERROR: number of events from files is not equal to total number of events" << std::endl;
  }

  cout << nDuplicates << " duplicate events were skipped." << endl;

  //---------------------
  // Save Plots
  //---------------------

  outfile_->cd();
  savePlotsDir(h_1d_global,outfile_,"");
  savePlotsDir(SRNoCut.srHistMap,outfile_,SRNoCut.GetName().c_str());
  savePlotsDir(SRBase.srHistMap,outfile_,SRBase.GetName().c_str());
  savePlotsDir(SRBase.crslHistMap,outfile_,"crslbase");
  savePlotsDir(SRBase.crrlHistMap,outfile_,"crrlbase");
  if (!doMinimalPlots) {
    savePlotsDir(CRSL_WJets.crslHistMap,outfile_,CRSL_WJets.GetName().c_str());
    savePlotsDir(CRSL_TTbar.crslHistMap,outfile_,CRSL_TTbar.GetName().c_str());
  }
  savePlotsDir(SRNoCut.crgjHistMap,outfile_,"crgjnocut");
  savePlotsDir(SRBase.crgjHistMap,outfile_,"crgjbase");

  for(unsigned int srN = 0; srN < SRVec.size(); srN++){
    if(!SRVec.at(srN).srHistMap.empty()){
      savePlotsDir(SRVec.at(srN).srHistMap, outfile_, ("sr"+SRVec.at(srN).GetName()).c_str());
    }
  }
  if (saveGJplots) {
    for(unsigned int srN = 0; srN < SRVec.size(); srN++){
      if(!SRVec.at(srN).crgjHistMap.empty()){
        savePlotsDir(SRVec.at(srN).crgjHistMap, outfile_, ("crgj"+SRVec.at(srN).GetName()).c_str());
	saveRooDataSetsDir(SRVec.at(srN).crgjRooDataSetMap, outfile_,  ("crgj"+SRVec.at(srN).GetName()).c_str());
      }
    }
  }
  if (saveDYplots) {
    for(unsigned int srN = 0; srN < SRVec.size(); srN++){
      if(!SRVec.at(srN).crdyHistMap.empty()){
        savePlotsDir(SRVec.at(srN).crdyHistMap, outfile_, ("crdy"+SRVec.at(srN).GetName()).c_str());
      }
    }
  }
  if (saveRLplots) {
    for(unsigned int srN = 0; srN < SRVec.size(); srN++){
      if(!SRVec.at(srN).crrlHistMap.empty()){
        savePlotsDir(SRVec.at(srN).crrlHistMap, outfile_, ("crrl"+SRVec.at(srN).GetName()).c_str());
      }
    }
  }
  if (saveRLELplots) {
    for(unsigned int srN = 0; srN < SRVec.size(); srN++){
      if(!SRVec.at(srN).crrlelHistMap.empty()){
        savePlotsDir(SRVec.at(srN).crrlelHistMap, outfile_, ("crrlel"+SRVec.at(srN).GetName()).c_str());
      }
    }
  }
  if (saveRLMUplots) {
    for(unsigned int srN = 0; srN < SRVec.size(); srN++){
      if(!SRVec.at(srN).crrlmuHistMap.empty()){
        savePlotsDir(SRVec.at(srN).crrlmuHistMap, outfile_, ("crrlmu"+SRVec.at(srN).GetName()).c_str());
      }
    }
  }
  if (saveSLplots) {
    for(unsigned int srN = 0; srN < SRVec.size(); srN++){
      if(!SRVec.at(srN).crslHistMap.empty()){
        savePlotsDir(SRVec.at(srN).crslHistMap, outfile_, ("crsl"+SRVec.at(srN).GetName()).c_str());
      }
    }
  }
  if (saveSLMUplots) {
    for(unsigned int srN = 0; srN < SRVec.size(); srN++){
      if(!SRVec.at(srN).crslmuHistMap.empty()){
        savePlotsDir(SRVec.at(srN).crslmuHistMap, outfile_, ("crslmu"+SRVec.at(srN).GetName()).c_str());
      }
    }
  }
  if (saveSLELplots) {
    for(unsigned int srN = 0; srN < SRVec.size(); srN++){
      if(!SRVec.at(srN).crslelHistMap.empty()){
        savePlotsDir(SRVec.at(srN).crslelHistMap, outfile_, ("crslel"+SRVec.at(srN).GetName()).c_str());
      }
    }
  }
  if (saveQCDplots) {
    for(unsigned int srN = 0; srN < SRVec.size(); srN++){
      if(!SRVec.at(srN).crqcdHistMap.empty()){
        savePlotsDir(SRVec.at(srN).crqcdHistMap, outfile_, ("crqcd"+SRVec.at(srN).GetName()).c_str());
      }
    }
  }


  //---------------------
  // Write and Close file
  //---------------------
  outfile_->Write();
  outfile_->Close();
  delete outfile_;

  delete m1bins;
  delete m2bins;

  // close file for qcd event list
  if (print_qcd_event_list)
    fqcdlist.close();

  bmark->Stop("benchmark");
  cout << endl;
  cout << nEventsTotal << " Events Processed" << endl;
  cout << "------------------------------" << endl;
  cout << "CPU  Time:	" << Form( "%.01f s", bmark->GetCpuTime("benchmark")  ) << endl;
  cout << "Real Time:	" << Form( "%.01f s", bmark->GetRealTime("benchmark") ) << endl;
  cout << endl;
  delete bmark;

  return;
}

void MT2Looper::fillHistosSRBase() {

  // trigger requirement on data
  if (t.isData && !(t.HLT_PFHT900 || t.HLT_PFJet450 || t.HLT_PFHT300_PFMET110 || t.HLT_PFMET120_PFMHT120 || t.HLT_PFMETNoMu120_PFMHTNoMu120)) return;

  // met/caloMet filter for additional cleaning
  if (t.met_miniaodPt / t.met_caloPt > 5.0) return;
  // ad-hoc RA2/b filter
  if (t.nJet200MuFrac50DphiMet > 0) return;

  std::map<std::string, float> values;
  values["deltaPhiMin"] = deltaPhiMin_;
  values["diffMetMhtOverMet"]  = diffMetMht_/met_pt_;
  values["nlep"]        = nlepveto_;
  values["j1pt"]        = jet1_pt_;
  values["j2pt"]        = jet2_pt_;
  values["mt2"]         = mt2_;
  values["passesHtMet"] = ( (ht_ > 250. && met_pt_ > 250.) || (ht_ > 1000. && met_pt_ > 30.) );

  if(SRBase.PassesSelection(values)) {
    fillHistos(SRBase.srHistMap, SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), SRBase.GetName(), "");
    if (t.isData && !doBlindData && met_pt_ > 2000.) {
      std::cout << "WARNING: found event with high MET: " << t.run << ":" <<  t.lumi << ":" << t.evt
		<< ", HT: " << ht_ << ", MET: " << met_pt_ << ", nJet30: " << nJet30_ << ", mt2: " << mt2_ << std::endl;
    }
  }

  // do monojet SRs
  bool passMonojet = false;
  if (passMonojetId_ && (!t.isData || t.HLT_PFHT900 || t.HLT_PFJet450 || t.HLT_PFHT300_PFMET110 || t.HLT_PFMET120_PFMHT120 || t.HLT_PFMETNoMu120_PFMHTNoMu120)) {
    std::map<std::string, float> values_monojet;
    values_monojet["deltaPhiMin"] = deltaPhiMin_;
    values_monojet["diffMetMhtOverMet"]  = diffMetMht_/met_pt_;
    values_monojet["nlep"]        = nlepveto_;
    values_monojet["ht"]          = ht_; // ETH uses ht instead of jet1_pt..
    values_monojet["njets"]       = nJet30_;
    values_monojet["met"]         = met_pt_;
    
    if(SRBaseMonojet.PassesSelection(values_monojet)) passMonojet = true;
    if (passMonojet) {
      fillHistos(SRBaseMonojet.srHistMap, SRBaseMonojet.GetNumberOfMT2Bins(), SRBaseMonojet.GetMT2Bins(), SRBaseMonojet.GetName(), "");
      if (t.isData && !doBlindData && met_pt_ > 2000.) {
	std::cout << "WARNING: found monojet event with high MET: " << t.run << ":" <<  t.lumi << ":" << t.evt
		  << ", HT: " << ht_ << ", MET: " << met_pt_ << ", nJet30: " << nJet30_ << ", mt2: " << mt2_ << std::endl;
      }
    }
  }
  if ((SRBase.PassesSelection(values)) || (passMonojet)) {
    fillHistos(SRBaseIncl.srHistMap, SRBaseIncl.GetNumberOfMT2Bins(), SRBaseIncl.GetMT2Bins(), SRBaseIncl.GetName(), "");
  }


  return;
}

void MT2Looper::fillHistosInclusive() {

  // trigger requirement on data
  if (t.isData && !(t.HLT_PFHT900 || t.HLT_PFJet450 || t.HLT_PFHT300_PFMET110 || t.HLT_PFMET120_PFMHT120 || t.HLT_PFMETNoMu120_PFMHTNoMu120)) return;

  // met/caloMet filter for additional cleaning
  if (t.met_miniaodPt / t.met_caloPt > 5.0) return;
  // ad-hoc RA2/b filter
  if (t.nJet200MuFrac50DphiMet > 0) return;

  std::map<std::string, float> values;
  values["deltaPhiMin"] = deltaPhiMin_;
  values["diffMetMhtOverMet"]  = diffMetMht_/met_pt_;
  values["nlep"]        = nlepveto_;
  values["j1pt"]        = jet1_pt_;
  values["j2pt"]        = jet2_pt_;
  values["mt2"]         = mt2_;
  values["passesHtMet"] = ( (ht_ > 250. && met_pt_ > 250.) || (ht_ > 1000. && met_pt_ > 30.) );

  for(unsigned int srN = 0; srN < InclusiveRegions.size(); srN++){
    std::map<std::string, float> values_temp = values;
    std::vector<std::string> vars = InclusiveRegions.at(srN).GetListOfVariables();
    for(unsigned int iVar=0; iVar<vars.size(); iVar++){
      if(vars.at(iVar) == "ht") values_temp["ht"] = ht_;
      else if(vars.at(iVar) == "njets") values_temp["njets"] = nJet30_;
      else if(vars.at(iVar) == "nbjets") values_temp["nbjets"] = nBJet20_;
    }
    if(InclusiveRegions.at(srN).PassesSelection(values_temp)){
      fillHistos(InclusiveRegions.at(srN).srHistMap, InclusiveRegions.at(srN).GetNumberOfMT2Bins(), InclusiveRegions.at(srN).GetMT2Bins(), InclusiveRegions.at(srN).GetName(), "");
    }
  }

  return;
}

void MT2Looper::fillHistosSignalRegion(const std::string& prefix, const std::string& suffix) {

  // trigger requirement on data
  if (t.isData && !(t.HLT_PFHT900 || t.HLT_PFJet450 || t.HLT_PFHT300_PFMET110 || t.HLT_PFMET120_PFMHT120 || t.HLT_PFMETNoMu120_PFMHTNoMu120)) return;
  
  // met/caloMet filter for additional cleaning
  if (t.met_miniaodPt / t.met_caloPt > 5.0) return;
  // ad-hoc RA2/b filter
  if (t.nJet200MuFrac50DphiMet > 0) return;

  std::map<std::string, float> values;
  values["deltaPhiMin"] = deltaPhiMin_;
  values["diffMetMhtOverMet"]  = diffMetMht_/met_pt_;
  values["nlep"]        = nlepveto_;
  values["j1pt"]        = jet1_pt_;
  values["j2pt"]        = jet2_pt_;
  values["njets"]       = nJet30_;
  values["nbjets"]      = nBJet20_;
  values["mt2"]         = mt2_;
  values["ht"]          = ht_;
  values["met"]         = met_pt_;
  //values["passesHtMet"] = ( (ht_ > 250. && met_pt_ > 250.) || (ht_ > 1000. && met_pt_ > 30.) );

  if (include_eta_binning) 
    {
      values["PseudoJet1_eta"] = fabs(t.pseudoJet1_eta);
    }

  for(unsigned int srN = 0; srN < SRVec.size(); srN++){
    if(SRVec.at(srN).PassesSelection(values)){
      //cout << "FOUNDEVENT:" << prefix+SRVec.at(srN).GetName() << ":" << t.run << ":" << t.lumi << ":" << t.evt << endl;
      fillHistos(SRVec.at(srN).srHistMap, SRVec.at(srN).GetNumberOfMT2Bins(), SRVec.at(srN).GetMT2Bins(), prefix+SRVec.at(srN).GetName(), suffix);
      //break;//signal regions are orthogonal, event cannot be in more than one --> not true now with super signal regions
    }
  }
  
  // do monojet SRs
  if (passMonojetId_ && (!t.isData || t.HLT_PFHT900 || t.HLT_PFJet450 || t.HLT_PFHT300_PFMET110 || t.HLT_PFMET120_PFMHT120 || t.HLT_PFMETNoMu120_PFMHTNoMu120)) {
    std::map<std::string, float> values_monojet;
    values_monojet["deltaPhiMin"] = deltaPhiMin_;
    values_monojet["diffMetMhtOverMet"]  = diffMetMht_/met_pt_;
    values_monojet["nlep"]        = nlepveto_;
    //values_monojet["j1pt"]        = jet1_pt_; // ETH doesn't cut on jet1_pt
    values_monojet["njets"]       = nJet30_;
    values_monojet["nbjets"]      = nBJet20_;
    values_monojet["ht"]          = ht_;
    values_monojet["met"]         = met_pt_;

    for(unsigned int srN = 0; srN < SRVecMonojet.size(); srN++){
      if(SRVecMonojet.at(srN).PassesSelection(values_monojet)){
        //cout << "FOUNDEVENT:" << prefix+SRVecMonojet.at(srN).GetName() << ":" << t.run << ":" << t.lumi << ":" << t.evt << endl;
	fillHistos(SRVecMonojet.at(srN).srHistMap, SRVecMonojet.at(srN).GetNumberOfMT2Bins(), SRVecMonojet.at(srN).GetMT2Bins(), prefix+SRVecMonojet.at(srN).GetName(), suffix);
	//break;//signal regions are orthogonal, event cannot be in more than one --> not true for Monojet, since we have inclusive regions
      }
    }
  } // monojet regions

  // genmet version of signal regions, for fastsim unc
  if (isSignal_ && doSystVariationPlots) {

    std::map<std::string, float> values_genmet;
    values_genmet["deltaPhiMin"] = t.deltaPhiMin_genmet;
    values_genmet["diffMetMhtOverMet"]  = t.diffMetMht_genmet/t.met_genPt;
    values_genmet["nlep"]        = nlepveto_;
    values_genmet["j1pt"]        = jet1_pt_;
    values_genmet["j2pt"]        = jet2_pt_;
    values_genmet["njets"]       = nJet30_;
    values_genmet["nbjets"]      = nBJet20_;
    values_genmet["mt2"]         = t.mt2_genmet;
    values_genmet["ht"]          = ht_;
    values_genmet["met"]         = t.met_genPt;

    if (include_eta_binning) values_genmet["PseudoJet1_eta"] = fabs(t.pseudoJet1_eta);

    for(unsigned int srN = 0; srN < SRVec.size(); srN++){
      if(SRVec.at(srN).PassesSelection(values_genmet)){
	fillHistosGenMET(SRVec.at(srN).srHistMap, SRVec.at(srN).GetNumberOfMT2Bins(), SRVec.at(srN).GetMT2Bins(), prefix+SRVec.at(srN).GetName(), suffix);
	//break;//signal regions are orthogonal, event cannot be in more than one --> not true now with super signal regions
      }
    }
  
    // do monojet SRs
    if (passMonojetId_) {
      std::map<std::string, float> values_monojet_genmet;
      values_monojet_genmet["deltaPhiMin"] = t.deltaPhiMin_genmet;
      values_monojet_genmet["diffMetMhtOverMet"]  = t.diffMetMht_genmet/t.met_genPt;
      values_monojet_genmet["nlep"]        = nlepveto_;
      //values_monojet_genmet["j1pt"]        = jet1_pt_; // ETH doesn't cut on jet1_pt
      values_monojet_genmet["njets"]       = nJet30_;
      values_monojet_genmet["nbjets"]      = nBJet20_;
      values_monojet_genmet["ht"]          = ht_;
      values_monojet_genmet["met"]         = t.met_genPt;

      for(unsigned int srN = 0; srN < SRVecMonojet.size(); srN++){
	if(SRVecMonojet.at(srN).PassesSelection(values_monojet_genmet)){
	  fillHistosGenMET(SRVecMonojet.at(srN).srHistMap, SRVecMonojet.at(srN).GetNumberOfMT2Bins(), SRVecMonojet.at(srN).GetMT2Bins(), prefix+SRVecMonojet.at(srN).GetName(), suffix);
	  //break;//signal regions are orthogonal, event cannot be in more than one --> not true for Monojet, since we have inclusive regions
	}
      }
    } // monojet regions
    
  } // if(isSignal_ && doSystVariationPlots)

  return;
}

// hists for single lepton control region
void MT2Looper::fillHistosCRSL(const std::string& prefix, const std::string& suffix) {

  // trigger requirement on data
  if (t.isData && !(t.HLT_PFHT900 || t.HLT_PFJet450 || t.HLT_PFHT300_PFMET110 || t.HLT_PFMET120_PFMHT120 || t.HLT_PFMETNoMu120_PFMHTNoMu120)) return;
  
  // met/caloMet filter for additional cleaning
  if (t.met_miniaodPt / t.met_caloPt > 5.0) return;
  // ad-hoc RA2/b filter
  if (t.nJet200MuFrac50DphiMet > 0) return;

  // first fill base region
  std::map<std::string, float> valuesBase;
  valuesBase["deltaPhiMin"] = deltaPhiMin_;
  valuesBase["diffMetMhtOverMet"]  = diffMetMht_/met_pt_;
  valuesBase["nlep"]        = t.nLepLowMT;
  valuesBase["j1pt"]        = jet1_pt_;
  valuesBase["j2pt"]        = jet2_pt_;
  valuesBase["mt2"]         = mt2_;
  valuesBase["passesHtMet"] = ( (ht_ > 250. && met_pt_ > 250.) || (ht_ > 1000. && met_pt_ > 30.) );

  if (SRBase.PassesSelectionCRSL(valuesBase)) {
    if(prefix=="crsl") fillHistosSingleLepton(SRBase.crslHistMap, SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crslbase", suffix);
    else if(prefix=="crslmu") fillHistosSingleLepton(SRBase.crslmuHistMap, SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crslmubase", suffix);
    else if(prefix=="crslel") fillHistosSingleLepton(SRBase.crslelHistMap, SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crslelbase", suffix);

    // only fill inclusive HT regions for inclusive lepton case
    if (prefix=="crsl") {
      if(ht_ > 250.  && ht_ < 450.)  fillHistosSingleLepton(InclusiveRegions.at(0).crslHistMap,   SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crslbaseVL", suffix);
      if(ht_ > 450.  && ht_ < 575.)  fillHistosSingleLepton(InclusiveRegions.at(1).crslHistMap,   SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crslbaseL", suffix);
      if(ht_ > 575.  && ht_ < 1000.) fillHistosSingleLepton(InclusiveRegions.at(2).crslHistMap,  SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crslbaseM", suffix);
      if(ht_ > 1000. && ht_ < 1500.) fillHistosSingleLepton(InclusiveRegions.at(3).crslHistMap, SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crslbaseH", suffix);
      if(ht_ > 1500.                 ) fillHistosSingleLepton(InclusiveRegions.at(4).crslHistMap,  SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crslbaseUH", suffix);
    }
  }

  // only fill wjets/ttbar histograms for inclusive lepton case
  if(!doMinimalPlots && prefix=="crsl") {

    // inclusive regions with btag cuts for wjets/ttbar
    std::map<std::string, float> valuesInc;
    valuesInc["deltaPhiMin"] = deltaPhiMin_;
    valuesInc["diffMetMhtOverMet"]  = diffMetMht_/met_pt_;
    valuesInc["nlep"]        = t.nLepLowMT;
    valuesInc["j1pt"]        = jet1_pt_;
    valuesInc["j2pt"]        = jet2_pt_;
    valuesInc["mt2"]         = mt2_;
    valuesInc["passesHtMet"] = ( (ht_ > 250. && met_pt_ > 250.) || (ht_ > 1000. && met_pt_ > 30.) );
    valuesInc["nbjets"]         = nBJet20_;
    if (CRSL_WJets.PassesSelectionCRSL(valuesInc)) {
      fillHistosSingleLepton(CRSL_WJets.crslHistMap, CRSL_WJets.GetNumberOfMT2Bins(), CRSL_WJets.GetMT2Bins(), CRSL_WJets.GetName().c_str(), suffix);
    }
    if (CRSL_TTbar.PassesSelectionCRSL(valuesInc)) {
      fillHistosSingleLepton(CRSL_TTbar.crslHistMap, CRSL_TTbar.GetNumberOfMT2Bins(), CRSL_TTbar.GetMT2Bins(), CRSL_TTbar.GetName().c_str(), suffix);
    }
  }

  // topological regions
  std::map<std::string, float> values;
  values["deltaPhiMin"] = deltaPhiMin_;
  values["diffMetMhtOverMet"]  = diffMetMht_/met_pt_;
  values["nlep"]        = t.nLepLowMT;
  values["j1pt"]        = jet1_pt_;
  values["j2pt"]        = jet2_pt_;
  values["njets"]       = nJet30_;
  values["nbjets"]      = nBJet20_;
  values["mt2"]         = mt2_;
  values["ht"]          = ht_;
  values["met"]         = met_pt_;

  if (include_eta_binning) 
    {
      values["PseudoJet1_eta"] = fabs(t.pseudoJet1_eta);
    }

  for(unsigned int srN = 0; srN < SRVec.size(); srN++){
    if(SRVec.at(srN).PassesSelectionCRSL(values)){
      if(prefix=="crsl"){
        // cout << "FOUNDEVENT:" << prefix+SRVec.at(srN).GetName() << ":" << t.run << ":" << t.lumi << ":" << t.evt << endl;
        fillHistosSingleLepton(SRVec.at(srN).crslHistMap,    SRVec.at(srN).GetNumberOfMT2Bins(), SRVec.at(srN).GetMT2Bins(), prefix+SRVec.at(srN).GetName(), suffix);
      }
      else if(prefix=="crslmu")  fillHistosSingleLepton(SRVec.at(srN).crslmuHistMap,  SRVec.at(srN).GetNumberOfMT2Bins(), SRVec.at(srN).GetMT2Bins(), prefix+SRVec.at(srN).GetName(), suffix);
      else if(prefix=="crslel")  fillHistosSingleLepton(SRVec.at(srN).crslelHistMap,  SRVec.at(srN).GetNumberOfMT2Bins(), SRVec.at(srN).GetMT2Bins(), prefix+SRVec.at(srN).GetName(), suffix);
      //      break;//control regions are not necessarily orthogonal
    }
  }

  // do monojet SRs
  if (passMonojetId_ && (!t.isData || t.HLT_PFHT900 || t.HLT_PFJet450 || t.HLT_PFHT300_PFMET110 || t.HLT_PFMET120_PFMHT120 || t.HLT_PFMETNoMu120_PFMHTNoMu120)) {

    // first fill base region
    std::map<std::string, float> valuesBase_monojet;
    valuesBase_monojet["deltaPhiMin"] = deltaPhiMin_;
    valuesBase_monojet["diffMetMhtOverMet"]  = diffMetMht_/met_pt_;
    valuesBase_monojet["nlep"]        = t.nLepLowMT;
    valuesBase_monojet["ht"]          = jet1_pt_; // don't include the lepton
    valuesBase_monojet["njets"]       = nJet30_;
    valuesBase_monojet["met"]         = met_pt_;

    if (SRBaseMonojet.PassesSelectionCRSL(valuesBase_monojet)) {
      if(prefix=="crsl") fillHistosSingleLepton(SRBaseMonojet.crslHistMap, SRBaseMonojet.GetNumberOfMT2Bins(), SRBaseMonojet.GetMT2Bins(), "crslbaseJ", suffix);
      else if(prefix=="crslmu") fillHistosSingleLepton(SRBaseMonojet.crslmuHistMap, SRBaseMonojet.GetNumberOfMT2Bins(), SRBaseMonojet.GetMT2Bins(), "crslmubaseJ", suffix);
      else if(prefix=="crslel") fillHistosSingleLepton(SRBaseMonojet.crslelHistMap, SRBaseMonojet.GetNumberOfMT2Bins(), SRBaseMonojet.GetMT2Bins(), "crslelbaseJ", suffix);
    }

    
    std::map<std::string, float> values_monojet;
    values_monojet["deltaPhiMin"] = deltaPhiMin_;
    values_monojet["diffMetMhtOverMet"]  = diffMetMht_/met_pt_;
    values_monojet["nlep"]        = t.nLepLowMT;
    //values_monojet["j1pt"]        = jet1_pt_; // ETH doesn't cut on jet1_pt..
    values_monojet["njets"]       = nJet30_;
    values_monojet["nbjets"]      = nBJet20_;
    values_monojet["ht"]          = jet1_pt_; // don't include the lepton 
    values_monojet["met"]         = met_pt_;

    for(unsigned int srN = 0; srN < SRVecMonojet.size(); srN++){
      if(SRVecMonojet.at(srN).PassesSelectionCRSL(values_monojet)){
        if(prefix=="crsl"){
          fillHistosSingleLepton(SRVecMonojet.at(srN).crslHistMap,    SRVecMonojet.at(srN).GetNumberOfMT2Bins(), SRVecMonojet.at(srN).GetMT2Bins(), prefix+SRVecMonojet.at(srN).GetName(), suffix);
          // cout << "FOUNDEVENT:" << prefix+SRVecMonojet.at(srN).GetName() << ":" << t.run << ":" << t.lumi << ":" << t.evt << endl;
        }
	else if(prefix=="crslmu")  fillHistosSingleLepton(SRVecMonojet.at(srN).crslmuHistMap,  SRVecMonojet.at(srN).GetNumberOfMT2Bins(), SRVecMonojet.at(srN).GetMT2Bins(), prefix+SRVecMonojet.at(srN).GetName(), suffix);
	else if(prefix=="crslel")  fillHistosSingleLepton(SRVecMonojet.at(srN).crslelHistMap,  SRVecMonojet.at(srN).GetNumberOfMT2Bins(), SRVecMonojet.at(srN).GetMT2Bins(), prefix+SRVecMonojet.at(srN).GetName(), suffix);
	//      break;//control regions are not necessarily orthogonal
      }
    }
  } // monojet regions


  // genmet version of signal regions, for fastsim unc
  if (isSignal_ && doSystVariationPlots) {

    // topological regions
    std::map<std::string, float> values_genmet;
    values_genmet["deltaPhiMin"] = t.deltaPhiMin_genmet;
    values_genmet["diffMetMhtOverMet"]  = t.diffMetMht_genmet/t.met_genPt;
    values_genmet["nlep"]        = t.nLepLowMT;
    values_genmet["j1pt"]        = jet1_pt_;
    values_genmet["j2pt"]        = jet2_pt_;
    values_genmet["njets"]       = nJet30_;
    values_genmet["nbjets"]      = nBJet20_;
    values_genmet["mt2"]         = t.mt2_genmet;
    values_genmet["ht"]          = ht_;
    values_genmet["met"]         = t.met_genPt;

    if (include_eta_binning) 
      {
	values_genmet["PseudoJet1_eta"] = fabs(t.pseudoJet1_eta);
      }

    for(unsigned int srN = 0; srN < SRVec.size(); srN++){
      if(SRVec.at(srN).PassesSelectionCRSL(values_genmet)){
	if(prefix=="crsl")    fillHistosGenMET(SRVec.at(srN).crslHistMap,    SRVec.at(srN).GetNumberOfMT2Bins(), SRVec.at(srN).GetMT2Bins(), prefix+SRVec.at(srN).GetName(), suffix);
	//      break;//control regions are not necessarily orthogonal
      }
    }

    // do monojet SRs
    if (passMonojetId_) {

      std::map<std::string, float> values_monojet_genmet;
      values_monojet_genmet["deltaPhiMin"] = t.deltaPhiMin_genmet;
      values_monojet_genmet["diffMetMhtOverMet"]  = t.diffMetMht_genmet/t.met_genPt;
      values_monojet_genmet["nlep"]        = t.nLepLowMT;
      //values_monojet_genmet["j1pt"]        = jet1_pt_; // ETH doesn't cut on jet1_pt..
      values_monojet_genmet["njets"]       = nJet30_;
      values_monojet_genmet["nbjets"]      = nBJet20_;
      values_monojet_genmet["ht"]          = jet1_pt_;
      values_monojet_genmet["met"]         = t.met_genPt;

      for(unsigned int srN = 0; srN < SRVecMonojet.size(); srN++){
	if(SRVecMonojet.at(srN).PassesSelectionCRSL(values_monojet_genmet)){
	  if(prefix=="crsl")    fillHistosGenMET(SRVecMonojet.at(srN).crslHistMap,    SRVecMonojet.at(srN).GetNumberOfMT2Bins(), SRVecMonojet.at(srN).GetMT2Bins(), prefix+SRVecMonojet.at(srN).GetName(), suffix);
	  //      break;//control regions are not necessarily orthogonal
	}
      }
    } // monojet regions
    
  } // if(isSignal_ && doSystVariationPlots)

  
  return;
}

// hists for Gamma+Jets control region
void MT2Looper::fillHistosCRGJ(const std::string& prefix, const std::string& suffix) {

  if (t.ngamma==0) return;

  // trigger requirement
  if ( ( t.isData || stringsample.Contains("2015")) && !t.HLT_Photon165_HE10) return;

  // additional cleaning for fakes and HLT-emulation (2016)
  if (fabs(t.gamma_eta[0])>2.4 || t.gamma_hOverE015[0]>0.1 ) return;

  // apply trigger weights to mc
  if (!t.isData && applyPhotonTriggerWeights){
    evtweight_ *= getPhotonTriggerWeight(t.gamma_eta[0], t.gamma_pt[0]);
  }
  
  bool passSieie = t.gamma_idCutBased[0] ? true : false; // just deal with the standard case now. Worry later about sideband in sieie

  // fill hists
  std::string add="";

  float passPtMT2 = false;
  if (mt2_ < 200 && t.gamma_pt[0]>180.) passPtMT2 = true;

  if (doubleRatioShapeCorrection)  evtweight_ *=  doubleRatioWeight(t.gamma_pt[0]);

  std::map<std::string, float> values;
  values["deltaPhiMin"] = t.gamma_deltaPhiMin;
  values["diffMetMhtOverMet"]  = t.gamma_diffMetMht/t.gamma_met_pt;
  values["nlep"]        = nlepveto_;
  values["j1pt"]        = t.gamma_jet1_pt;
  values["j2pt"]        = t.gamma_jet2_pt;
  values["njets"]       = t.gamma_nJet30;
  values["nbjets"]      = t.gamma_nBJet20;
  values["mt2"]         = t.gamma_mt2;
  values["ht"]          = t.gamma_ht;
  values["met"]         = t.gamma_met_pt;
  
  if (include_eta_binning)
    {
      values["PseudoJet1_eta"] = fabs(t.pseudoJet1_eta);
    }

  // Separate list for SRBASE
  std::map<std::string, float> valuesBase;
  valuesBase["deltaPhiMin"] = t.gamma_deltaPhiMin;
  valuesBase["diffMetMhtOverMet"]  = t.gamma_diffMetMht/t.gamma_met_pt;
  valuesBase["nlep"]        = nlepveto_;
  valuesBase["j1pt"]        = t.gamma_jet1_pt;
  valuesBase["j2pt"]        = t.gamma_jet2_pt;
  valuesBase["mt2"]         = t.gamma_mt2;
  valuesBase["passesHtMet"] = ( (t.gamma_ht > 250. && t.gamma_met_pt > 250.) || (t.gamma_ht > 1000. && t.gamma_met_pt > 30.) );
  bool passBase = SRBase.PassesSelection(valuesBase);

  std::map<std::string, float> valuesBase_monojet;
  valuesBase_monojet["deltaPhiMin"] = t.gamma_deltaPhiMin;
  valuesBase_monojet["diffMetMhtOverMet"]  = t.gamma_diffMetMht/t.gamma_met_pt;
  valuesBase_monojet["nlep"]        = nlepveto_;
  valuesBase_monojet["ht"]          = t.gamma_ht; // ETH doesn't cut on jet1_pt here, only ht
  valuesBase_monojet["njets"]       = t.gamma_nJet30;
  valuesBase_monojet["met"]         = t.gamma_met_pt;

  bool passBaseJ = SRBaseMonojet.PassesSelection(valuesBase_monojet);

  std::map<std::string, float> values_monojet;
  values_monojet["deltaPhiMin"] = t.gamma_deltaPhiMin;
  values_monojet["diffMetMhtOverMet"]  = t.gamma_diffMetMht/t.gamma_met_pt;
  values_monojet["nlep"]        = nlepveto_;
  //  values_monojet["j1pt"]        = t.gamma_jet1_pt;
  values_monojet["njets"]       = t.gamma_nJet30;
  values_monojet["nbjets"]      = t.gamma_nBJet20;
  values_monojet["ht"]          = t.gamma_ht;
  values_monojet["met"]         = t.gamma_met_pt;
  

  //float iso = t.gamma_chHadIso[0] + t.gamma_phIso[0];
  float iso = t.gamma_chHadIso[0];
  float isoCutTight = 2.5;
  float isoCutLoose = 10.;
  if (t.gamma_ht > 250) fillHistosGammaJets(SRNoCut.crgjHistMap, SRNoCut.crgjRooDataSetMap, SRNoCut.GetNumberOfMT2Bins(), SRNoCut.GetMT2Bins(), prefix+SRNoCut.GetName(), suffix+add+"All");

  if (iso>isoCutTight && iso < isoCutLoose) add += "LooseNotTight";
  if (iso>isoCutLoose) add += "NotLoose";
  if (!passSieie) add += "SieieSB"; // Keep Sigma IEta IEta sideband
  if (t.gamma_ht > 250) fillHistosGammaJets(SRNoCut.crgjHistMap, SRNoCut.crgjRooDataSetMap, SRNoCut.GetNumberOfMT2Bins(), SRNoCut.GetMT2Bins(), prefix+SRNoCut.GetName(), suffix+add);
  
  // Monojet Regions
  if ( passBaseJ && t.gamma_pt[0] > 180. ) {
    fillHistosGammaJets(SRBaseMonojet.crgjHistMap, SRBaseMonojet.crgjRooDataSetMap, SRBaseMonojet.GetNumberOfMT2Bins(), SRBaseMonojet.GetMT2Bins(), "crgjbaseJ", suffix+add);
    for(unsigned int srN = 0; srN < SRVecMonojet.size(); srN++){
      if(SRVecMonojet.at(srN).PassesSelection(values_monojet)){
        // if(add=="")
        // cout << "FOUNDEVENT:" << prefix+SRVecMonojet.at(srN).GetName() << ":" << t.run << ":" << t.lumi << ":" << t.evt << endl;
	fillHistosGammaJets(SRVecMonojet.at(srN).crgjHistMap, SRVecMonojet.at(srN).crgjRooDataSetMap,   SRVecMonojet.at(srN).GetNumberOfMT2Bins(), SRVecMonojet.at(srN).GetMT2Bins(), prefix+SRVecMonojet.at(srN).GetName(), suffix+add);
      }
    }
  }

  // Multijet Regions
  if(passBase && passPtMT2) {
    //if (nJet30_FailId > 0) cout<<"Event "<<t.evt<<" in run "<<t.run<<" fails jet ID"<<endl;
    //if (iso<isoCutTight && passSieie) {
    //  cout<<" "<<endl;
    //  cout<<"Event "<<t.evt<<" in run "<<t.run<<" passes baseline selection"<<endl;
    //}
    fillHistosGammaJets(SRBase.crgjHistMap, SRBase.crgjRooDataSetMap, SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crgjbase", suffix+add);
    if(passBase && t.gamma_ht > 250.  && t.gamma_ht < 450.)  fillHistosGammaJets(InclusiveRegions.at(0).crgjHistMap,   InclusiveRegions.at(0).crgjRooDataSetMap,   SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crgjbaseVL", suffix+add);
    if(passBase && t.gamma_ht > 450.  && t.gamma_ht < 575.)  fillHistosGammaJets(InclusiveRegions.at(1).crgjHistMap,   InclusiveRegions.at(1).crgjRooDataSetMap,   SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crgjbaseL", suffix+add);
    if(passBase && t.gamma_ht > 575.  && t.gamma_ht < 1000.) fillHistosGammaJets(InclusiveRegions.at(2).crgjHistMap,  InclusiveRegions.at(2).crgjRooDataSetMap,  SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crgjbaseM", suffix+add);
    if(passBase && t.gamma_ht > 1000. && t.gamma_ht < 1500.) fillHistosGammaJets(InclusiveRegions.at(3).crgjHistMap, InclusiveRegions.at(3).crgjRooDataSetMap, SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crgjbaseH", suffix+add);
    if(passBase && t.gamma_ht > 1500.                  ) fillHistosGammaJets(InclusiveRegions.at(4).crgjHistMap,  InclusiveRegions.at(4).crgjRooDataSetMap,  SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crgjbaseUH", suffix+add);
    for(unsigned int srN = 0; srN < SRVec.size(); srN++){
      if(SRVec.at(srN).PassesSelection(values)){
        // if(add=="")
        // cout << "FOUNDEVENT:" << prefix+SRVec.at(srN).GetName() << ":" << t.run << ":" << t.lumi << ":" << t.evt << endl;
	fillHistosGammaJets(SRVec.at(srN).crgjHistMap, SRVec.at(srN).crgjRooDataSetMap, SRVec.at(srN).GetNumberOfMT2Bins(), SRVec.at(srN).GetMT2Bins(), prefix+SRVec.at(srN).GetName(), suffix+add);
	break;//control regions are orthogonal, event cannot be in more than one
      }
    }//SRloop

  }

  // BaseInclusive
  if ((passBaseJ && t.gamma_pt[0] > 180.) || (passBase && passPtMT2)) {
    fillHistosGammaJets(SRBaseIncl.crgjHistMap, SRBaseIncl.crgjRooDataSetMap, SRBaseIncl.GetNumberOfMT2Bins(), SRBaseIncl.GetMT2Bins(), "crgjbaseIncl", suffix+add);
  }

  // Remake everything again to get "Loose"
  if (iso<isoCutLoose) {
    add = "Loose";
    if (!passSieie) add += "SieieSB"; // Keep Sigma IEta IEta sideband
    fillHistosGammaJets(SRNoCut.crgjHistMap, SRNoCut.crgjRooDataSetMap, SRNoCut.GetNumberOfMT2Bins(), SRNoCut.GetMT2Bins(), prefix+SRNoCut.GetName(), suffix+add);
    if ( passBaseJ && t.gamma_pt[0] > 180.) {
      fillHistosGammaJets(SRBaseMonojet.crgjHistMap, SRBaseMonojet.crgjRooDataSetMap, SRBaseMonojet.GetNumberOfMT2Bins(), SRBaseMonojet.GetMT2Bins(), "crgjbaseJ", suffix+add);
      for(unsigned int srN = 0; srN < SRVecMonojet.size(); srN++){
	if(SRVecMonojet.at(srN).PassesSelection(values_monojet)){
	  fillHistosGammaJets(SRVecMonojet.at(srN).crgjHistMap, SRVecMonojet.at(srN).crgjRooDataSetMap,   SRVecMonojet.at(srN).GetNumberOfMT2Bins(), SRVecMonojet.at(srN).GetMT2Bins(), prefix+SRVecMonojet.at(srN).GetName(), suffix+add);
	}
      }
    }
    if(passBase && passPtMT2) {
      fillHistosGammaJets(SRBase.crgjHistMap, SRBase.crgjRooDataSetMap, SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crgjbase", suffix+add);
      for(unsigned int srN = 0; srN < SRVec.size(); srN++){
	if(SRVec.at(srN).PassesSelection(values)){
	  fillHistosGammaJets(SRVec.at(srN).crgjHistMap, SRVec.at(srN).crgjRooDataSetMap, SRVec.at(srN).GetNumberOfMT2Bins(), SRVec.at(srN).GetMT2Bins(), prefix+SRVec.at(srN).GetName(), suffix+add);
	  break;//control regions are orthogonal, event cannot be in more than one
	}
      }//SRloop
    }
    if ((passBaseJ && t.gamma_pt[0] > 180.) || (passBase && passPtMT2)) {
      fillHistosGammaJets(SRBaseIncl.crgjHistMap, SRBaseIncl.crgjRooDataSetMap, SRBaseIncl.GetNumberOfMT2Bins(), SRBaseIncl.GetMT2Bins(), "crgjbaseIncl", suffix+add);
    }
  }
  if (!doMinimalPlots) {
    // Remake everything for inclusive isolation
    if (passSieie) {
      add = "AllIso";
      fillHistosGammaJets(SRNoCut.crgjHistMap, SRNoCut.crgjRooDataSetMap, SRNoCut.GetNumberOfMT2Bins(), SRNoCut.GetMT2Bins(), prefix+SRNoCut.GetName(), suffix+add);
      if ( passBaseJ && t.gamma_pt[0] > 180.) {
	fillHistosGammaJets(SRBaseMonojet.crgjHistMap, SRBaseMonojet.crgjRooDataSetMap, SRBaseMonojet.GetNumberOfMT2Bins(), SRBaseMonojet.GetMT2Bins(), "crgjbaseJ", suffix+add);
      }
      if(passBase && passPtMT2) {
	fillHistosGammaJets(SRBase.crgjHistMap, SRBase.crgjRooDataSetMap, SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crgjbase", suffix+add);
      }
    }

    //inclusive in Sieie to get sieie plots
    if(iso<isoCutLoose){
      add = "LooseAllSieie";
      if(passBase && passPtMT2) {
	fillHistosGammaJets(SRBase.crgjHistMap, SRBase.crgjRooDataSetMap, SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crgjbase", suffix+add);
      }
    }

  }
      

  return;
}

// hists for Zll control region
void MT2Looper::fillHistosCRDY(const std::string& prefix, const std::string& suffix) {

  if (t.nlep!=2) return;

  // trigger requirement on data and MC already implemented when defining doDYplots
  
  // met/caloMet filter for additional cleaning, only for pfmet > 200
  if ((t.met_miniaodPt > 200.) && (t.met_miniaodPt / t.met_caloPt > 5.0)) return;
  // ad-hoc RA2/b filter
  if (t.nJet200MuFrac50DphiMet > 0) return;
  
  std::map<std::string, float> values;
  values["deltaPhiMin"] = t.zll_deltaPhiMin;
  values["diffMetMhtOverMet"]  = t.zll_diffMetMht/t.zll_met_pt;
  values["nlep"]        = 0; // dummy value
  values["j1pt"]        = jet1_pt_;
  values["j2pt"]        = jet2_pt_;
  values["njets"]       = nJet30_;
  values["nbjets"]      = nBJet20_;
  values["mt2"]         = t.zll_mt2;
  values["ht"]          = t.zll_ht;
  values["met"]         = t.zll_met_pt;

  if (include_eta_binning) 
    {
      values["PseudoJet1_eta"] = fabs(t.pseudoJet1_eta);
    }

  // Separate list for SRBASE
  std::map<std::string, float> valuesBase;
  valuesBase["deltaPhiMin"] = t.zll_deltaPhiMin;
  valuesBase["diffMetMhtOverMet"]  = t.zll_diffMetMht/t.zll_met_pt;
  valuesBase["nlep"]        = 0; // dummy value
  valuesBase["j1pt"]        = jet1_pt_;
  valuesBase["j2pt"]        = jet2_pt_;
  valuesBase["mt2"]         = t.zll_mt2;
  valuesBase["passesHtMet"] = ( (t.zll_ht > 250. && t.zll_met_pt > 250.) || (t.zll_ht > 1000. && t.zll_met_pt > 30.) );
  bool passBase = SRBase.PassesSelection(valuesBase);

  std::map<std::string, float> valuesBase_monojet;
  valuesBase_monojet["deltaPhiMin"] = t.zll_deltaPhiMin;
  valuesBase_monojet["diffMetMhtOverMet"]  = t.zll_diffMetMht/t.zll_met_pt;
  valuesBase_monojet["nlep"]        = 0;
  valuesBase_monojet["ht"]          = t.zll_ht; 
  valuesBase_monojet["njets"]       = nJet30_;
  valuesBase_monojet["met"]         = t.zll_met_pt;

  bool passBaseJ = SRBaseMonojet.PassesSelection(valuesBase_monojet) && passMonojetId_;

  std::map<std::string, float> values_monojet;
  values_monojet["deltaPhiMin"] = t.zll_deltaPhiMin;
  values_monojet["diffMetMhtOverMet"]  = t.zll_diffMetMht/t.zll_met_pt;
  values_monojet["nlep"]        = 0;
  //  values_monojet["j1pt"]        = jet1_pt_; // ETH doesn't explictly cut on jet1_pt
  values_monojet["njets"]       = nJet30_;
  values_monojet["nbjets"]      = nBJet20_;
  values_monojet["ht"]          = t.zll_ht;
  values_monojet["met"]         = t.zll_met_pt;
  
  if (t.zll_ht > 250) fillHistosDY(SRNoCut.crdyHistMap, SRNoCut.GetNumberOfMT2Bins(), SRNoCut.GetMT2Bins(), prefix+SRNoCut.GetName(), suffix);
  if(passBase) fillHistosDY(SRBase.crdyHistMap, SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crdybase", suffix);
  if(passBaseJ) {
    fillHistosDY(SRBaseMonojet.crdyHistMap, SRBaseMonojet.GetNumberOfMT2Bins(), SRBaseMonojet.GetMT2Bins(), "crdybaseJ", suffix);
    for(unsigned int srN = 0; srN < SRVecMonojet.size(); srN++){
      if(SRVecMonojet.at(srN).PassesSelectionCRDY(values_monojet)){
        // cout << "FOUNDEVENT:" << prefix+SRVecMonojet.at(srN).GetName() << ":" << t.run << ":" << t.lumi << ":" << t.evt << endl;
	fillHistosDY(SRVecMonojet.at(srN).crdyHistMap, SRVecMonojet.at(srN).GetNumberOfMT2Bins(), SRVecMonojet.at(srN).GetMT2Bins(), prefix+SRVecMonojet.at(srN).GetName(), suffix); 
      }
    }
  }
  if(passBase || passBaseJ) {
    //cout<<t.run<<" "<<t.lumi<<" "<<t.evt<<endl;
    fillHistosDY(SRBaseIncl.crdyHistMap, SRBaseIncl.GetNumberOfMT2Bins(), SRBaseIncl.GetMT2Bins(), "crdybaseIncl", suffix);
  }

  if(passBase && t.zll_ht > 250.  && t.zll_ht < 450.)  fillHistosDY(InclusiveRegions.at(0).crdyHistMap, SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crdybaseVL", suffix);
  if(passBase && t.zll_ht > 450.  && t.zll_ht < 575.)  fillHistosDY(InclusiveRegions.at(1).crdyHistMap, SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crdybaseL", suffix);
  if(passBase && t.zll_ht > 575.  && t.zll_ht < 1000.) fillHistosDY(InclusiveRegions.at(2).crdyHistMap, SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crdybaseM", suffix);
  if(passBase && t.zll_ht > 1000. && t.zll_ht < 1500.) fillHistosDY(InclusiveRegions.at(3).crdyHistMap, SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crdybaseH", suffix);
  if(passBase && t.zll_ht > 1500.                  ) fillHistosDY(InclusiveRegions.at(4).crdyHistMap, SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crdybaseUH", suffix);

  for(unsigned int srN = 0; srN < SRVec.size(); srN++){
    if(SRVec.at(srN).PassesSelectionCRDY(values)){
      // cout << "FOUNDEVENT:" << prefix+SRVec.at(srN).GetName() << ":" << t.run << ":" << t.lumi << ":" << t.evt << endl;
      fillHistosDY(SRVec.at(srN).crdyHistMap, SRVec.at(srN).GetNumberOfMT2Bins(), SRVec.at(srN).GetMT2Bins(), prefix+SRVec.at(srN).GetName(), suffix);
      //break; //not orthogonal any more 
    }
  }

  return;
}

// hists for removed single lepton control region
void MT2Looper::fillHistosCRRL(const std::string& prefix, const std::string& suffix) {

  if (t.nlep!=1) return;

  // trigger requirement on data and MC already included in RL selection
  
  std::map<std::string, float> values;
  values["deltaPhiMin"] = t.rl_deltaPhiMin;
  values["diffMetMhtOverMet"]  = t.rl_diffMetMht/t.rl_met_pt;
  values["nlep"]        = 0; // dummy value
  values["j1pt"]        = jet1_pt_;
  values["j2pt"]        = jet2_pt_;
  values["njets"]       = nJet30_;
  values["nbjets"]      = nBJet20_;
  values["mt2"]         = t.rl_mt2;
  values["ht"]          = t.rl_ht;
  values["met"]         = t.rl_met_pt;

  if (include_eta_binning) 
    {
      values["PseudoJet1_eta"] = fabs(t.pseudoJet1_eta);
    }

  // Separate list for SRBASE
  std::map<std::string, float> valuesBase;
  valuesBase["deltaPhiMin"] = t.rl_deltaPhiMin;
  valuesBase["diffMetMhtOverMet"]  = t.rl_diffMetMht/t.rl_met_pt;
  valuesBase["nlep"]        = 0; // dummy value
  valuesBase["j1pt"]        = jet1_pt_;
  valuesBase["j2pt"]        = jet2_pt_;
  valuesBase["mt2"]         = t.rl_mt2;
  valuesBase["passesHtMet"] = ( (t.rl_ht > 250. && t.rl_met_pt > 250.) || (t.rl_ht > 1000. && t.rl_met_pt > 30.) );
  bool passBase = SRBase.PassesSelection(valuesBase);

  std::map<std::string, float> valuesBase_monojet;
  valuesBase_monojet["deltaPhiMin"] = t.rl_deltaPhiMin;
  valuesBase_monojet["diffMetMhtOverMet"]  = t.rl_diffMetMht/t.rl_met_pt;
  valuesBase_monojet["nlep"]        = 0;
  valuesBase_monojet["ht"]          = ht_; // ETH doesn't cut on jet1_pt, only ht
  valuesBase_monojet["njets"]       = nJet30_;
  valuesBase_monojet["met"]         = t.rl_met_pt;

  bool passBaseJ = SRBaseMonojet.PassesSelection(valuesBase_monojet);
  
  std::map<std::string, float> values_monojet;
  values_monojet["deltaPhiMin"] = t.rl_deltaPhiMin;
  values_monojet["diffMetMhtOverMet"]  = t.rl_diffMetMht/t.rl_met_pt;
  values_monojet["nlep"]        = 0;
  // values_monojet["j1pt"]        = jet1_pt_; // ETH only cuts on ht, not jet1_pt
  values_monojet["njets"]       = nJet30_;
  values_monojet["nbjets"]      = nBJet20_;
  values_monojet["ht"]          = t.rl_ht;
  values_monojet["met"]         = t.rl_met_pt;

  if (t.rl_ht > 250) {
    if(prefix=="crrl")        fillHistosRemovedLepton(SRNoCut.crrlHistMap,   SRNoCut.GetNumberOfMT2Bins(), SRNoCut.GetMT2Bins(), prefix+SRNoCut.GetName(), suffix);
    else if(prefix=="crrlmu") fillHistosRemovedLepton(SRNoCut.crrlmuHistMap, SRNoCut.GetNumberOfMT2Bins(), SRNoCut.GetMT2Bins(), prefix+SRNoCut.GetName(), suffix);
    else if(prefix=="crrlel") fillHistosRemovedLepton(SRNoCut.crrlelHistMap, SRNoCut.GetNumberOfMT2Bins(), SRNoCut.GetMT2Bins(), prefix+SRNoCut.GetName(), suffix);
  }

  if(passBase) {
    if(prefix=="crrl")        fillHistosRemovedLepton(SRBase.crrlHistMap,   SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crrlbase",   suffix);
    else if(prefix=="crrlmu") fillHistosRemovedLepton(SRBase.crrlmuHistMap, SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crrlmubase", suffix);
    else if(prefix=="crrlel") fillHistosRemovedLepton(SRBase.crrlelHistMap, SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crrlelbase", suffix);
  }
  if(passBaseJ) {
    if(prefix=="crrl")        fillHistosRemovedLepton(SRBaseMonojet.crrlHistMap,   SRBaseMonojet.GetNumberOfMT2Bins(), SRBaseMonojet.GetMT2Bins(), "crrlbaseJ",   suffix);
    else if(prefix=="crrlmu") fillHistosRemovedLepton(SRBaseMonojet.crrlmuHistMap, SRBaseMonojet.GetNumberOfMT2Bins(), SRBaseMonojet.GetMT2Bins(), "crrlmubaseJ", suffix);
    else if(prefix=="crrlel") fillHistosRemovedLepton(SRBaseMonojet.crrlelHistMap, SRBaseMonojet.GetNumberOfMT2Bins(), SRBaseMonojet.GetMT2Bins(), "crrlelbaseJ", suffix);
    for(unsigned int srN = 0; srN < SRVecMonojet.size(); srN++){
      if(SRVecMonojet.at(srN).PassesSelection(values_monojet)){
        if(prefix=="crrl"){
          // cout << "FOUNDEVENT:" << prefix+SRVecMonojet.at(srN).GetName() << ":" << t.run << ":" << t.lumi << ":" << t.evt << endl;         
          fillHistosRemovedLepton(SRVecMonojet.at(srN).crrlHistMap, SRVecMonojet.at(srN).GetNumberOfMT2Bins(), SRVecMonojet.at(srN).GetMT2Bins(), prefix+SRVecMonojet.at(srN).GetName(), suffix);
        }
      }
    }
  }
  if(passBase || passBaseJ) {
    if(prefix=="crrl")        fillHistosRemovedLepton(SRBaseIncl.crrlHistMap,   SRBaseIncl.GetNumberOfMT2Bins(), SRBaseIncl.GetMT2Bins(), "crrlbaseIncl",   suffix);
  }

  if(passBase && t.rl_ht > 250.  && t.rl_ht < 450.)  fillHistosRemovedLepton(InclusiveRegions.at(0).crrlHistMap,   SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crrlbaseVL", suffix);
  if(passBase && t.rl_ht > 450.  && t.rl_ht < 575.)  fillHistosRemovedLepton(InclusiveRegions.at(1).crrlHistMap,   SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crrlbaseL", suffix);
  if(passBase && t.rl_ht > 575.  && t.rl_ht < 1000.) fillHistosRemovedLepton(InclusiveRegions.at(2).crrlHistMap,  SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crrlbaseM", suffix);
  if(passBase && t.rl_ht > 1000. && t.rl_ht < 1500.) fillHistosRemovedLepton(InclusiveRegions.at(3).crrlHistMap, SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crrlbaseH", suffix);
  if(passBase && t.rl_ht > 1500.                 ) fillHistosRemovedLepton(InclusiveRegions.at(4).crrlHistMap,  SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crrlbaseUH", suffix);

  // if(passBase && t.evt_id == 400 ) fillHistosRemovedLepton(SRBase.crrlHistMap, SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crrlbase", "TsChan");
  // if(passBase && t.evt_id == 401 ) fillHistosRemovedLepton(SRBase.crrlHistMap, SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crrlbase", "TtChan");
  // if(passBase && t.evt_id == 402 ) fillHistosRemovedLepton(SRBase.crrlHistMap, SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crrlbase", "TtWChan");
  // if(passBase && t.evt_id == 403 ) fillHistosRemovedLepton(SRBase.crrlHistMap, SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crrlbase", "TbarsChan");
  // if(passBase && t.evt_id == 404 ) fillHistosRemovedLepton(SRBase.crrlHistMap, SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crrlbase", "TbartChan");
  // if(passBase && t.evt_id == 405 ) fillHistosRemovedLepton(SRBase.crrlHistMap, SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crrlbase", "TbartwChan");

  for(unsigned int srN = 0; srN < SRVec.size(); srN++){
    if(SRVec.at(srN).PassesSelection(values)){
      if(prefix=="crrl"){
        // cout << "FOUNDEVENT:" << prefix+SRVec.at(srN).GetName() << ":" << t.run << ":" << t.lumi << ":" << t.evt << endl;                        
        fillHistosRemovedLepton(SRVec.at(srN).crrlHistMap,   SRVec.at(srN).GetNumberOfMT2Bins(), SRVec.at(srN).GetMT2Bins(), prefix+SRVec.at(srN).GetName(), suffix);
      }
      else if(prefix=="crrlmu") fillHistosRemovedLepton(SRVec.at(srN).crrlmuHistMap, SRVec.at(srN).GetNumberOfMT2Bins(), SRVec.at(srN).GetMT2Bins(), prefix+SRVec.at(srN).GetName(), suffix);
      else if(prefix=="crrlel") fillHistosRemovedLepton(SRVec.at(srN).crrlelHistMap, SRVec.at(srN).GetNumberOfMT2Bins(), SRVec.at(srN).GetMT2Bins(), prefix+SRVec.at(srN).GetName(), suffix);
      break;//control regions are orthogonal, event cannot be in more than one
    }
  }
  
  return;
}

// hists for single lepton control region
void MT2Looper::fillHistosCRQCD(const std::string& prefix, const std::string& suffix) {

  // trigger requirement on data (also require to come from JetHT, HTMHT, or MET PD to match ETH)
  if (t.isData && !(t.HLT_PFHT900 || t.HLT_PFJet450 || t.HLT_PFHT300_PFMET110 || t.HLT_PFMET120_PFMHT120 || t.HLT_PFMETNoMu120_PFMHTNoMu120)) return;

  // met/caloMet filter for additional cleaning
  if (t.met_miniaodPt / t.met_caloPt > 5.0) return;
  // ad-hoc RA2/b filter
  if (t.nJet200MuFrac50DphiMet > 0) return;
  
  // topological regions
  std::map<std::string, float> values;
  values["deltaPhiMin"] = deltaPhiMin_;
  values["diffMetMhtOverMet"]  = diffMetMht_/met_pt_;
  values["nlep"]        = nlepveto_;
  values["j1pt"]        = jet1_pt_;
  values["j2pt"]        = jet2_pt_;
  values["mt2"]         = mt2_;
  values["ht"]          = ht_;
  values["met"]         = met_pt_;

  if (include_eta_binning) 
    {
      values["PseudoJet1_eta"] = fabs(t.pseudoJet1_eta);
    }

  for(unsigned int srN = 0; srN < SRVec.size(); srN++){
    if(SRVec.at(srN).PassesSelectionCRQCD(values)){
      // cout << "FOUNDEVENT:" << prefix+SRVec.at(srN).GetName() << ":" << t.run << ":" << t.lumi << ":" << t.evt << endl;
      fillHistosQCD(SRVec.at(srN).crqcdHistMap,    SRVec.at(srN).GetNumberOfMT2Bins(), SRVec.at(srN).GetMT2Bins(), prefix+SRVec.at(srN).GetName(), suffix);
      //      break;//control regions are not necessarily orthogonal
    }
  }

  // do monojet SRs
  if (passMonojetId_){

    std::map<std::string, float> values_monojet;
    values_monojet["deltaPhiMin"] = deltaPhiMin_;
    values_monojet["diffMetMhtOverMet"]  = diffMetMht_/met_pt_;
    values_monojet["nlep"]        = nlepveto_;
    values_monojet["j1pt"]        = jet1_pt_;
    values_monojet["j2pt"]        = jet2_pt_;
    values_monojet["njets"]       = nJet30_;
    values_monojet["nbjets"]      = nBJet20_;
    values_monojet["ht"]          = jet1_pt_; // only count jet1_pt for binning
    values_monojet["met"]         = met_pt_;

    for(unsigned int srN = 0; srN < SRVecMonojet.size(); srN++){
      if(SRVecMonojet.at(srN).PassesSelectionCRQCD(values_monojet)){
        // cout << "FOUNDEVENT:" << prefix+SRVecMonojet.at(srN).GetName() << ":" << t.run << ":" << t.lumi << ":" << t.evt << endl;
	fillHistosQCD(SRVecMonojet.at(srN).crqcdHistMap,    SRVecMonojet.at(srN).GetNumberOfMT2Bins(), SRVecMonojet.at(srN).GetMT2Bins(), prefix+SRVecMonojet.at(srN).GetName(), suffix);
	//      break;//control regions are not necessarily orthogonal
      }
    }

    if(SRBaseMonojet.PassesSelectionCRQCD(values_monojet)){
      fillHistosQCD(SRBaseMonojet.crqcdHistMap, SRBaseMonojet.GetNumberOfMT2Bins(), SRBaseMonojet.GetMT2Bins(), "crqcdbaseJ", suffix);
    }

  } // monojet regions
  
  return;
}

void MT2Looper::fillHistos(std::map<std::string, TH1*>& h_1d, int n_mt2bins, float* mt2bins, const std::string& dirname, const std::string& s) {
  TDirectory * dir = (TDirectory*)outfile_->Get(dirname.c_str());
  if (dir == 0) {
    dir = outfile_->mkdir(dirname.c_str());
  } 
  dir->cd();

  // workaround for monojet bins
  float mt2_temp = mt2_;
  if (nJet30_ == 1) mt2_temp = ht_;

  plot1D("h_Events"+s,  1, 1, h_1d, ";Events, Unweighted", 1, 0, 2);
  plot1D("h_Events_w"+s,  1,   evtweight_, h_1d, ";Events, Weighted", 1, 0, 2);
  plot1D("h_mt2bins"+s,       mt2_temp,   evtweight_, h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
  plot1D("h_mt2binsAll"+s,       mt2_temp,   evtweight_, h_1d, "; M_{T2} [GeV]", n_mt2bins_SRBase, SRBase_mt2bins);
  plot1D("h_htbins"+s,       ht_,   evtweight_, h_1d, ";H_{T} [GeV]", n_htbins, htbins);
  plot1D("h_htbins2"+s,       ht_,   evtweight_, h_1d, ";H_{T} [GeV]", n_htbins2, htbins2);
  plot1D("h_njbins"+s,       nJet30_,   evtweight_, h_1d, ";N(jets)", n_njbins, njbins);
  plot1D("h_nbjbins"+s,       nBJet20_,   evtweight_, h_1d, ";N(bjets)", n_nbjbins, nbjbins);
  plot1D("h_mt2"+s,       mt2_temp,   evtweight_, h_1d, "; M_{T2} [GeV]", 150, 0, 1500);

  // Templates for hybrid method
  //const std::string&  NB = nBJet20_ == 0 ? "_0" : ( nBJet20_ == 1 ? "_1" : ( nBJet20_ == 2 ? "_2" : "_3") );
  if (dirname=="srbaseVL") {
    if (nJet30_ < 4) {
      plot1D("h_mt2bins23J"+s,       mt2_temp,   evtweight_, h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
      //plot1D("h_mt2bins23J"+NB+s,       mt2_temp,   evtweight_, h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
    }
    else {
      plot1D("h_mt2bins4J"+s,       mt2_temp,   evtweight_, h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
      //plot1D("h_mt2bins4J"+NB+s,       mt2_temp,   evtweight_, h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
    }
    if (nJet30_ > 2) {
      plot1D("h_mt2bins3J"+s,       mt2_temp,   evtweight_, h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
      //plot1D("h_mt2bins3J"+NB+s,       mt2_temp,   evtweight_, h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
    }
  }
  if (dirname=="srbaseL" || dirname=="srbaseM" || dirname=="srbaseH" || dirname=="srbaseUH") {
    if (nJet30_ < 7) {
      plot1D("h_mt2bins26J"+s,       mt2_temp,   evtweight_, h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);  // used for 26J 3B
      //plot1D("h_mt2bins26J"+NB+s,       mt2_temp,   evtweight_, h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);  // used for 26J 3B
    }
    if (nJet30_ < 4) {
      plot1D("h_mt2bins23J"+s,       mt2_temp,   evtweight_, h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
      //plot1D("h_mt2bins23J"+NB+s,       mt2_temp,   evtweight_, h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
    }
    else if (nJet30_ < 7) {
      plot1D("h_mt2bins46J"+s,       mt2_temp,   evtweight_, h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
      //plot1D("h_mt2bins46J"+NB+s,       mt2_temp,   evtweight_, h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
    }
    else {
      plot1D("h_mt2bins7J"+s,       mt2_temp,   evtweight_, h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
      //plot1D("h_mt2bins7J"+NB+s,       mt2_temp,   evtweight_, h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
    }
    if (nJet30_ > 2 && nJet30_ < 7) {
      plot1D("h_mt2bins36J"+s,       mt2_temp,   evtweight_, h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
      //plot1D("h_mt2bins36J"+NB+s,       mt2_temp,   evtweight_, h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
    }
  }
  if (dirname=="sr20") { // Special plot to get MT2 template for 3B aggregate region
    if (nJet30_ > 2) {
      plot1D("h_mt2bins3J"+s,       mt2_temp,   evtweight_, h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
    }
  }

  if (!doMinimalPlots) {
    plot1D("h_met"+s,       met_pt_,   evtweight_, h_1d, ";E_{T}^{miss} [GeV]", 150, 0, 1500);
    plot1D("h_ht"+s,       ht_,   evtweight_, h_1d, ";H_{T} [GeV]", 120, 0, 3000);
    plot1D("h_nJet30"+s,       nJet30_,   evtweight_, h_1d, ";N(jets)", 15, 0, 15);
    plot1D("h_nJet30Eta3"+s,       nJet30Eta3_,   evtweight_, h_1d, ";N(jets, |#eta| > 3.0)", 10, 0, 10);
    plot1D("h_nBJet20"+s,      nBJet20_,   evtweight_, h_1d, ";N(bjets)", 6, 0, 6);
    plot1D("h_deltaPhiMin"+s,  deltaPhiMin_,   evtweight_, h_1d, ";#Delta#phi_{min}", 32, 0, 3.2);
    plot1D("h_diffMetMht"+s,   diffMetMht_,   evtweight_, h_1d, ";|E_{T}^{miss} - MHT| [GeV]", 120, 0, 300);
    plot1D("h_diffMetMhtOverMet"+s,   diffMetMht_/met_pt_,   evtweight_, h_1d, ";|E_{T}^{miss} - MHT| / E_{T}^{miss}", 100, 0, 2.);
    plot1D("h_minMTBMet"+s,   t.minMTBMet,   evtweight_, h_1d, ";min M_{T}(b, E_{T}^{miss}) [GeV]", 150, 0, 1500);
    plot1D("h_nlepveto"+s,     nlepveto_,   evtweight_, h_1d, ";N(leps)", 10, 0, 10);
    plot1D("h_J0pt"+s,       jet1_pt_,   evtweight_, h_1d, ";p_{T}(jet1) [GeV]", 150, 0, 1500);
    plot1D("h_J1pt"+s,       jet2_pt_,   evtweight_, h_1d, ";p_{T}(jet2) [GeV]", 150, 0, 1500);
  }

  TString directoryname(dirname);
  if (directoryname.Contains("nocut") && !t.isData && !doMinimalPlots) {
    
    float genHT = 0;
    int lep1idx = -1;
    int lep1id = -1;
    int lep2idx = -1;
    int Zidx = -1;
    int Gidx = -1;
    float minDR = 999;
    for ( int i = 0; i < t.ngenStat23; i++) {
      int ID = fabs(t.genStat23_pdgId[i]);
      if (ID==22) Gidx = i;
      if (ID==23 || ID==24) Zidx = i;
    }

    for ( int i = 0; i < t.ngenStat23; i++) {
      int ID = fabs(t.genStat23_pdgId[i]);
      if (ID<6 ||ID==21) {
	genHT += t.genStat23_pt[i];
	if (Gidx!=-1) {
	  float thisDR = DeltaR(t.genStat23_eta[Gidx],t.genStat23_eta[i],t.genStat23_phi[Gidx],t.genStat23_phi[i]);
	  if ( thisDR < minDR ) minDR = thisDR;
	}
      }
      if ( (ID==11 || ID==13) &&  t.genStat23_pt[i]>20 && fabs(t.genStat23_eta[i])<2.5 ) { 
	if (lep1idx==-1) {
	  lep1idx = i;
	  lep1id = ID;
	}
	else if ( ID == lep1id ) lep2idx = i;
      }
    }
    
    TLorentzVector V(0,0,0,0);
    if (lep2idx!=-1) {
      TLorentzVector lep1(0,0,0,0);
      lep1.SetPtEtaPhiM(t.genStat23_pt[lep1idx], t.genStat23_eta[lep1idx], t.genStat23_phi[lep1idx], t.genStat23_mass[lep1idx]);
      TLorentzVector lep2(0,0,0,0);
      lep2.SetPtEtaPhiM(t.genStat23_pt[lep2idx], t.genStat23_eta[lep2idx], t.genStat23_phi[lep2idx], t.genStat23_mass[lep2idx]);
      V = lep1 + lep2;
    }
    if (minDR < 0.4) Gidx = -1; // don't use low-DR photons
    if ((Gidx != -1 || Zidx != -1) && lep2idx==-1) {
      if (Zidx == -1) Zidx = Gidx;
      V.SetPtEtaPhiM(t.genStat23_pt[Zidx], t.genStat23_eta[Zidx], t.genStat23_phi[Zidx], t.genStat23_mass[Zidx]);
    }
    if (Zidx != -1 || Gidx != -1 || lep2idx!=-1) {
      float Vrapidity = V.Rapidity();
      plot1D("h_VpT"+s,       V.Pt(),   evtweight_, h_1d, "; p_{T}^{V} [GeV]", n_ptVbins, ptVbins);
      if (fabs(Vrapidity) < 1.4 ) plot1D("h_VpTcentral"+s,      V.Pt(),   evtweight_, h_1d, "; p_{T}^{V} [GeV]", n_ptVbins, ptVbins);
      if (genHT>250) plot1D("h_VpT_genHT250"+s,      V.Pt(),   evtweight_, h_1d, "; p_{T}^{V} [GeV]", n_ptVbins, ptVbins);
      if (fabs(Vrapidity) < 1.4 && genHT>250) plot1D("h_VpTcentral_genHT250"+s,       V.Pt(),   evtweight_, h_1d, "; p_{T}^{V} [GeV]", n_ptVbins, ptVbins);
      if (ht_>250) plot1D("h_VpT_HT250"+s,      V.Pt(),   evtweight_, h_1d, "; p_{T}^{V} [GeV]", 300, 0, 1500);

    }

  } // nocut region only

  if (isSignal_) {
    plot3D("h_mt2bins_sigscan"+s, mt2_temp, t.GenSusyMScan1, t.GenSusyMScan2, evtweight_, h_1d, ";M_{T2} [GeV];mass1 [GeV];mass2 [GeV]", n_mt2bins, mt2bins, n_m1bins, m1bins, n_m2bins, m2bins);
  }

  if (!t.isData && isSignal_ && applyISRWeights && doSystVariationPlots) {
    int binx = h_sig_avgweight_isr_->GetXaxis()->FindBin(t.GenSusyMScan1);
    int biny = h_sig_avgweight_isr_->GetYaxis()->FindBin(t.GenSusyMScan2);
    // remove central value ISR weight when doing variation
    float avgweight_isr = h_sig_avgweight_isr_->GetBinContent(binx,biny);
    float avgweight_isr_UP = h_sig_avgweight_isr_UP_->GetBinContent(binx,biny);
    float avgweight_isr_DN = h_sig_avgweight_isr_DN_->GetBinContent(binx,biny);
    plot1D("h_mt2bins_isr_UP"+s,       mt2_temp,   evtweight_ / t.weight_isr * avgweight_isr * t.weight_isr_UP / avgweight_isr_UP, h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
    plot1D("h_mt2bins_isr_DN"+s,       mt2_temp,   evtweight_ / t.weight_isr * avgweight_isr * t.weight_isr_DN / avgweight_isr_DN, h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
    plot3D("h_mt2bins_sigscan_isr_UP"+s, mt2_temp, t.GenSusyMScan1, t.GenSusyMScan2, evtweight_ / t.weight_isr * avgweight_isr * t.weight_isr_UP / avgweight_isr_UP, h_1d, ";M_{T2} [GeV];mass1 [GeV];mass2 [GeV]", n_mt2bins, mt2bins, n_m1bins, m1bins, n_m2bins, m2bins);
    plot3D("h_mt2bins_sigscan_isr_DN"+s, mt2_temp, t.GenSusyMScan1, t.GenSusyMScan2, evtweight_ / t.weight_isr * avgweight_isr * t.weight_isr_DN / avgweight_isr_DN, h_1d, ";M_{T2} [GeV];mass1 [GeV];mass2 [GeV]", n_mt2bins, mt2bins, n_m1bins, m1bins, n_m2bins, m2bins);
  }

  // ISR reweighting variation for ttbar
  else if (!t.isData && applyISRWeights && t.evt_id >= 301 && t.evt_id <= 303 && doSystVariationPlots) {
    // remove central value ISR weight when doing variation
    float avgweight_isr = getAverageISRWeight(t.evt_id);
    float avgweight_isr_UP = getAverageISRWeight(t.evt_id,1);
    float avgweight_isr_DN = getAverageISRWeight(t.evt_id,-1);
    plot1D("h_mt2bins_isr_UP"+s,       mt2_temp,   evtweight_ / t.weight_isr * avgweight_isr * t.weight_isr_UP / avgweight_isr_UP, h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
    plot1D("h_mt2bins_isr_DN"+s,       mt2_temp,   evtweight_ / t.weight_isr * avgweight_isr * t.weight_isr_DN / avgweight_isr_DN, h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
  }

  if (!t.isData && applyBtagSF && doSystVariationPlots) {

    int binx,biny;
    float avgweight_btagsf = 1.;
    float avgweight_heavy_UP = 1.;
    float avgweight_heavy_DN = 1.;
    float avgweight_light_UP = 1.;
    float avgweight_light_DN = 1.;

    if (isSignal_) {
      binx = h_sig_avgweight_btagsf_heavy_UP_->GetXaxis()->FindBin(t.GenSusyMScan1);
      biny = h_sig_avgweight_btagsf_heavy_UP_->GetYaxis()->FindBin(t.GenSusyMScan2);
      avgweight_btagsf = h_sig_avgweight_btagsf_->GetBinContent(binx,biny);
      avgweight_heavy_UP = h_sig_avgweight_btagsf_heavy_UP_->GetBinContent(binx,biny);
      avgweight_heavy_DN = h_sig_avgweight_btagsf_heavy_DN_->GetBinContent(binx,biny);
      avgweight_light_UP = h_sig_avgweight_btagsf_light_UP_->GetBinContent(binx,biny);
      avgweight_light_DN = h_sig_avgweight_btagsf_light_DN_->GetBinContent(binx,biny);
    }

    // assume weights are already applied to central value
    plot1D("h_mt2bins_btagsf_heavy_UP"+s,       mt2_temp,   evtweight_ / t.weight_btagsf * avgweight_btagsf * t.weight_btagsf_heavy_UP / avgweight_heavy_UP, h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
    plot1D("h_mt2bins_btagsf_light_UP"+s,       mt2_temp,   evtweight_ / t.weight_btagsf * avgweight_btagsf * t.weight_btagsf_light_UP / avgweight_light_UP, h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
    plot1D("h_mt2bins_btagsf_heavy_DN"+s,       mt2_temp,   evtweight_ / t.weight_btagsf * avgweight_btagsf * t.weight_btagsf_heavy_DN / avgweight_heavy_DN, h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
    plot1D("h_mt2bins_btagsf_light_DN"+s,       mt2_temp,   evtweight_ / t.weight_btagsf * avgweight_btagsf * t.weight_btagsf_light_DN / avgweight_light_DN, h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
    if (isSignal_) {
      plot3D("h_mt2bins_sigscan_btagsf_heavy_UP"+s, mt2_temp, t.GenSusyMScan1, t.GenSusyMScan2, evtweight_ / t.weight_btagsf * avgweight_btagsf * t.weight_btagsf_heavy_UP / avgweight_heavy_UP, h_1d, ";M_{T2} [GeV];mass1 [GeV];mass2 [GeV]", n_mt2bins, mt2bins, n_m1bins, m1bins, n_m2bins, m2bins);
      plot3D("h_mt2bins_sigscan_btagsf_light_UP"+s, mt2_temp, t.GenSusyMScan1, t.GenSusyMScan2, evtweight_ / t.weight_btagsf * avgweight_btagsf * t.weight_btagsf_light_UP / avgweight_light_UP, h_1d, ";M_{T2} [GeV];mass1 [GeV];mass2 [GeV]", n_mt2bins, mt2bins, n_m1bins, m1bins, n_m2bins, m2bins);
      plot3D("h_mt2bins_sigscan_btagsf_heavy_DN"+s, mt2_temp, t.GenSusyMScan1, t.GenSusyMScan2, evtweight_ / t.weight_btagsf * avgweight_btagsf * t.weight_btagsf_heavy_DN / avgweight_heavy_DN, h_1d, ";M_{T2} [GeV];mass1 [GeV];mass2 [GeV]", n_mt2bins, mt2bins, n_m1bins, m1bins, n_m2bins, m2bins);
      plot3D("h_mt2bins_sigscan_btagsf_light_DN"+s, mt2_temp, t.GenSusyMScan1, t.GenSusyMScan2, evtweight_ / t.weight_btagsf * avgweight_btagsf * t.weight_btagsf_light_DN / avgweight_light_DN, h_1d, ";M_{T2} [GeV];mass1 [GeV];mass2 [GeV]", n_mt2bins, mt2bins, n_m1bins, m1bins, n_m2bins, m2bins);
    }
  }

  if (!t.isData && doGenTauVars) {
    float unc_tau1p = 0.;
    float unc_tau3p = 0.;
    if (t.ngenTau1Prong > 0 || t.ngenTau3Prong > 0) {
      // loop on gen taus
      for (int itau = 0; itau < t.ngenTau; ++itau) {
	// check acceptance for veto: pt > 10
        if (t.genTau_leadTrackPt[itau] < 10.) continue;
        if (t.genTau_decayMode[itau] == 1) unc_tau1p += 0.18; // 18% relative uncertainty for missing a 1-prong tau
        else if (t.genTau_decayMode[itau] == 3) unc_tau3p += 0.08; // 8% relative uncertainty for missing a 3-prong tau
      }
    }
    
    plot1D("h_mt2bins_tau1p_UP"+s,       mt2_temp,   evtweight_ * (1. + unc_tau1p), h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
    plot1D("h_mt2bins_tau1p_DN"+s,       mt2_temp,   evtweight_ * (1. - unc_tau1p), h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
    plot1D("h_mt2bins_tau3p_UP"+s,       mt2_temp,   evtweight_ * (1. + unc_tau3p), h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
    plot1D("h_mt2bins_tau3p_DN"+s,       mt2_temp,   evtweight_ * (1. - unc_tau3p), h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
  }

  // lepton efficiency variation in signal region for fastsim: large uncertainty on leptons NOT vetoed
  if (!t.isData && doLepEffVars && applyLeptonSFtoSR && (applyLeptonSFfromFiles || applyLeptonSFfromBabies) && directoryname.Contains("sr")) {

    // if lepeff goes up, number of events in SR should go down. Already taken into account in unc_lepeff_sr_
    //  need to first remove lepeff SF
    plot1D("h_mt2bins_lepeff_UP"+s,       mt2_temp,   evtweight_ / cor_lepeff_sr_ * unc_lepeff_sr_UP_, h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
    plot1D("h_mt2bins_lepeff_DN"+s,       mt2_temp,   evtweight_ / cor_lepeff_sr_ * unc_lepeff_sr_DN_, h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
    plot3D("h_mt2bins_sigscan_lepeff_UP"+s, mt2_temp, t.GenSusyMScan1, t.GenSusyMScan2, evtweight_ / cor_lepeff_sr_ * unc_lepeff_sr_UP_, h_1d, ";M_{T2} [GeV];mass1 [GeV];mass2 [GeV]", n_mt2bins, mt2bins, n_m1bins, m1bins, n_m2bins, m2bins);
    plot3D("h_mt2bins_sigscan_lepeff_DN"+s, mt2_temp, t.GenSusyMScan1, t.GenSusyMScan2, evtweight_ / cor_lepeff_sr_ * unc_lepeff_sr_DN_, h_1d, ";M_{T2} [GeV];mass1 [GeV];mass2 [GeV]", n_mt2bins, mt2bins, n_m1bins, m1bins, n_m2bins, m2bins);
    
  }

  else if (!t.isData && doLepEffVars && !applyLeptonSFtoSR && (applyLeptonSFfromFiles || applyLeptonSFfromBabies) && directoryname.Contains("sr")) {
    // if lepeff goes up, number of events in SR should go down. Already taken into account in unc_lepeff_sr_
    plot1D("h_mt2bins_lepeff_UP"+s,       mt2_temp,   evtweight_ * unc_lepeff_sr_UP_, h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
    plot1D("h_mt2bins_lepeff_DN"+s,       mt2_temp,   evtweight_ * unc_lepeff_sr_DN_, h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
    plot3D("h_mt2bins_sigscan_lepeff_UP"+s, mt2_temp, t.GenSusyMScan1, t.GenSusyMScan2, evtweight_ * unc_lepeff_sr_UP_, h_1d, ";M_{T2} [GeV];mass1 [GeV];mass2 [GeV]", n_mt2bins, mt2bins, n_m1bins, m1bins, n_m2bins, m2bins);
    plot3D("h_mt2bins_sigscan_lepeff_DN"+s, mt2_temp, t.GenSusyMScan1, t.GenSusyMScan2, evtweight_ * unc_lepeff_sr_DN_, h_1d, ";M_{T2} [GeV];mass1 [GeV];mass2 [GeV]", n_mt2bins, mt2bins, n_m1bins, m1bins, n_m2bins, m2bins);
  }
  
  // lepton efficiency variation in control region: smallish uncertainty on leptons which ARE vetoed
  else if (!t.isData && doLepEffVars && (applyLeptonSFfromFiles || applyLeptonSFfromBabies) && directoryname.Contains("crsl")) {
    // lepsf was already applied as a central value, take it back out
    plot1D("h_mt2bins_lepeff_UP"+s,       mt2_temp,   evtweight_ / weight_lepsf_cr_ * weight_lepsf_cr_UP_, h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
    plot1D("h_mt2bins_lepeff_DN"+s,       mt2_temp,   evtweight_ / weight_lepsf_cr_ * weight_lepsf_cr_DN_, h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
    
  }
  
  if ( !t.isData && doRenormFactScaleReweight) {      
    plot1D("h_mt2bins_renorm_UP"+s,        mt2_temp,   evtweight_renormUp_ , h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
    plot1D("h_mt2bins_renorm_DN"+s,        mt2_temp,   evtweight_renormDn_ , h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
  }
    
  outfile_->cd();
  return;
}

void MT2Looper::fillHistosSingleLepton(std::map<std::string, TH1*>& h_1d, int n_mt2bins, float* mt2bins, const std::string& dirname, const std::string& s) {
  TDirectory * dir = (TDirectory*)outfile_->Get(dirname.c_str());
  if (dir == 0) {
    dir = outfile_->mkdir(dirname.c_str());
  } 
  dir->cd();

  if (!doMinimalPlots) {
    plot1D("h_leppt"+s,      leppt_,   evtweight_, h_1d, ";p_{T}(lep) [GeV]", 200, 0, 1000);
    plot1D("h_mt"+s,            mt_,   evtweight_, h_1d, ";M_{T} [GeV]", 200, 0, 1000);
  }
  
  outfile_->cd();

  fillHistos(h_1d, n_mt2bins, mt2bins, dirname, s);
  return;
}


void MT2Looper::fillHistosGammaJets(std::map<std::string, TH1*>& h_1d, std::map<std::string, RooDataSet*>& datasets, int n_mt2bins, float* mt2bins, const std::string& dirname, const std::string& s) {
  TDirectory * dir = (TDirectory*)outfile_->Get(dirname.c_str());
  if (dir == 0) {
    dir = outfile_->mkdir(dirname.c_str());
  } 
  dir->cd();

  //bins for FR
  const int n_FRhtbins = 6;
  const int n_FRptbins = 5;
  const int n_FRptbinsOne = 1;
  const int n_FRetabins = 2;
  const float FRhtbins[n_FRhtbins+1] = {0,250,450,1000,1500,2000,3000};
  const float FRptbins[n_FRptbins+1] = {0,150,300,450,600,1500};
  const float FRptbinsOne[n_FRptbinsOne+1] = {0,1500};
  const float FRetabins[n_FRetabins+1] = {0, 1.479, 2.5};

  float iso = t.gamma_chHadIso[0] + t.gamma_phIso[0];
  float chiso = t.gamma_chHadIso[0];

  //RooRealVars for unbinned data hist
  x_->setVal(chiso);
  w_->setVal(evtweight_);
 
  plot1D("h_iso"+s,      iso,   evtweight_, h_1d, ";iso [GeV]", 100, 0, 50);
  plot1D("h_chiso"+s,      chiso,   evtweight_, h_1d, ";iso [GeV]", 100, 0, 50);
  if (fabs(t.gamma_eta[0]) < 1.479) plot1D("h_chisoEB"+s,      chiso,   evtweight_, h_1d, ";iso [GeV]", 100, 0, 50);
  else plot1D("h_chisoEE"+s,      chiso,   evtweight_, h_1d, ";iso [GeV]", 100, 0, 50);
  plot1D("h_isoW1"+s,      iso,   1, h_1d, ";iso [GeV]", 100, 0, 50);
  plot1D("h_chisoW1"+s,      chiso,   1, h_1d, ";ch iso [GeV]", 100, 0, 50);

  plotRooDataSet("rds_chIso_"+s, x_, w_, evtweight_, datasets, "");

  float Sieie = t.gamma_sigmaIetaIeta[0];
  plot1D("h_SigmaIetaIeta"+s, Sieie, evtweight_, h_1d, "; Photon #sigma_{i#eta i#eta}", 160,0,0.04);
  if(fabs(t.gamma_eta[0]) < 1.479)
    plot1D("h_SigmaIetaIetaEB"+s, Sieie, evtweight_, h_1d, "; Photon #sigma_{i#eta i#eta}", 160,0,0.04);
  else
    plot1D("h_SigmaIetaIetaEE"+s, Sieie, evtweight_, h_1d, "; Photon #sigma_{i#eta i#eta}", 160,0,0.04);


  //for FR calculation
  //if( (t.evt_id>110 && t.evt_id<120) || t.isData){ //only use qcd samples with pt>=470 to compute FR
  plot2D("h2d_gammaht_gammapt"+s, t.gamma_ht, t.gamma_pt[0], evtweight_, h_1d, ";H_{T} [GeV];gamma p_{T} [GeV]",n_FRhtbins,FRhtbins,n_FRptbins,FRptbins);
  //plot2D("h2d_gammaht_gammaptW1"+s, t.gamma_ht, t.gamma_pt[0], 1, h_1d, ";H_{T} [GeV];gamma p_{T} [GeV]",n_FRhtbins,FRhtbins,n_FRptbins,FRptbins);
  plot2D("h2d_gammaht_gammaptSingleBin"+s, t.gamma_ht, t.gamma_pt[0], evtweight_, h_1d, ";H_{T} [GeV];gamma p_{T} [GeV]",1,0,3000,1,0,1500);
  //plot2D("h2d_gammaht_gammaptSingleBinW1"+s, t.gamma_ht, t.gamma_pt[0], 1, h_1d, ";H_{T} [GeV];gamma p_{T} [GeV]",1,0,3000,1,0,1500);
  plot2D("h2d_gammaht_gammaptDoubleBin"+s, fabs(t.gamma_eta[0]), t.gamma_pt[0], evtweight_, h_1d, ";gamma eta;gamma p_{T} [GeV]",n_FRetabins, FRetabins,n_FRptbinsOne,FRptbinsOne);
  //  }
  float gamma_mt2_temp = t.gamma_mt2;
  // workaround for monojet bins
  if (t.gamma_nJet30 == 1) gamma_mt2_temp = t.gamma_ht;

//  for (int i = 0; i < SRBase.GetNumberOfMT2Bins(); i++) {
//    if ( gamma_mt2_temp > SRBase.GetMT2Bins()[i] &&  gamma_mt2_temp < SRBase.GetMT2Bins()[i+1]) {
//      plotRooDataSet("rds_chIso_"+toString(SRBase.GetMT2Bins()[i])+s, x_, w_, evtweight_, datasets, "");
//      plot1D("h_chiso_mt2bin"+toString(SRBase.GetMT2Bins()[i])+s,  iso,  evtweight_, h_1d, "; iso", 100, 0, 50);
//      plot2D("h2d_gammaht_gammapt"+toString(SRBase.GetMT2Bins()[i])+s, t.gamma_ht, t.gamma_pt[0], evtweight_, h_1d, ";H_{T} [GeV];gamma p_{T} [GeV]",n_FRhtbins,FRhtbins,n_FRptbins,FRptbins);
//      //plot2D("h2d_gammaht_gammaptW1"+toString(SRBase.GetMT2Bins()[i])+s, t.gamma_ht, t.gamma_pt[0], evtweight_, h_1d, ";H_{T} [GeV];gamma p_{T} [GeV]",n_FRhtbins,FRhtbins,n_FRptbins,FRptbins);	
//      plot2D("h2d_gammaht_gammaptSingleBin"+toString(SRBase.GetMT2Bins()[i])+s, t.gamma_ht, t.gamma_pt[0], evtweight_, h_1d, ";H_{T} [GeV];gamma p_{T} [GeV]",1,0,3000,1,0,1500);
//      //plot2D("h2d_gammaht_gammaptSingleBinW1"+toString(SRBase.GetMT2Bins()[i])+s, t.gamma_ht, t.gamma_pt[0], evtweight_, h_1d, ";H_{T} [GeV];gamma p_{T} [GeV]",1,0,3000,1,0,1500);
//    }
//  }

  for (int i = 0; i < n_mt2bins; i++) {
    if ( gamma_mt2_temp > mt2bins[i] &&  gamma_mt2_temp < mt2bins[i+1]) {
      plotRooDataSet("rds_chIso_"+toString(mt2bins[i])+s, x_, w_, evtweight_, datasets, "");
      //plot1D("h_chiso_mt2bin"+toString(mt2bins[i])+s,  iso,  evtweight_, h_1d, "; iso", 100, 0, 50);
      plot2D("h2d_gammaht_gammapt"+toString(mt2bins[i])+s, t.gamma_ht, t.gamma_pt[0], evtweight_, h_1d, ";H_{T} [GeV];gamma p_{T} [GeV]",n_FRhtbins,FRhtbins,n_FRptbins,FRptbins);
      //plot2D("h2d_gammaht_gammaptW1"+toString(mt2bins[i])+s, t.gamma_ht, t.gamma_pt[0], evtweight_, h_1d, ";H_{T} [GeV];gamma p_{T} [GeV]",n_FRhtbins,FRhtbins,n_FRptbins,FRptbins);	
      plot2D("h2d_gammaht_gammaptSingleBin"+toString(mt2bins[i])+s, t.gamma_ht, t.gamma_pt[0], evtweight_, h_1d, ";H_{T} [GeV];gamma p_{T} [GeV]",1,0,3000,1,0,1500);
      //plot2D("h2d_gammaht_gammaptSingleBinW1"+toString(mt2bins[i])+s, t.gamma_ht, t.gamma_pt[0], evtweight_, h_1d, ";H_{T} [GeV];gamma p_{T} [GeV]",1,0,3000,1,0,1500);
    }
  }

  //cout<<"Event "<<t.evt<<" with weight "<< evtweight_ <<" is in sr "<<dirname<<endl;
  plot1D("h_mt2bins"+s,       gamma_mt2_temp,   evtweight_, h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
  plot1D("h_htbins"+s,       t.gamma_ht,   evtweight_, h_1d, ";H_{T} [GeV]", n_htbins, htbins);
  plot1D("h_htbins2"+s,       t.gamma_ht,   evtweight_, h_1d, ";H_{T} [GeV]", n_htbins2, htbins2);
  plot1D("h_njbins"+s,       t.gamma_nJet30,   evtweight_, h_1d, ";N(jets)", n_njbins, njbins);
  plot1D("h_nbjbins"+s,       t.gamma_nBJet20,   evtweight_, h_1d, ";N(bjets)", n_nbjbins, nbjbins);

  if ( (dirname=="crgjnocut" || TString(dirname).Contains("crgjbase") || dirname=="crgjL" || dirname=="crgjM" || dirname=="crgjH") 
       && (s=="" || s=="Fake" || s=="FragGJ" || s=="AllIso" || s=="LooseNotTight" || s=="FakeLooseNotTight") )// Don't make these for Loose, NotLoose. SieieSB
  {
    plot1D("h_Events"+s,  1, 1, h_1d, ";Events, Unweighted", 1, 0, 2);
    plot1D("h_Events_w"+s,  1,   evtweight_, h_1d, ";Events, Weighted", 1, 0, 2);
    plot1D("h_mt2"+s,       gamma_mt2_temp,   evtweight_, h_1d, "; M_{T2} [GeV]", 150, 0, 1500);
    plot1D("h_met"+s,       t.gamma_met_pt,   evtweight_, h_1d, ";E_{T}^{miss} [GeV]", 150, 0, 1500);
    plot1D("h_simplemet"+s,       met_pt_,   evtweight_, h_1d, ";E_{T}^{miss} [GeV]", 150, 0, 1500);
    plot1D("h_gammaPt"+s,       t.gamma_pt[0],   evtweight_, h_1d, ";gamma p_{T} [GeV]", 264, 180, 1500);
    if (fabs(t.gamma_eta[0]) < 1.479) plot1D("h_gammaPtEB"+s,       t.gamma_pt[0],   evtweight_, h_1d, ";gamma p_{T} [GeV]", 264, 180, 1500);
    else plot1D("h_gammaPtEE"+s,       t.gamma_pt[0],   evtweight_, h_1d, ";gamma p_{T} [GeV]", 264, 180, 1500);
    plot1D("h_gammaEta"+s,       t.gamma_eta[0],   evtweight_, h_1d, ";gamma #eta [GeV]", 50, -2.5, 2.5);
    if (t.HLT_Photons) plot1D("h_gammaPt_HLT"+s,       t.gamma_pt[0],   evtweight_, h_1d, ";gamma p_{T} [GeV]", 300, 0, 1500);
    plot1D("h_ht"+s,       t.gamma_ht,   evtweight_, h_1d, ";H_{T} [GeV]", 120, 0, 3000);
    plot1D("h_nJet30"+s,       t.gamma_nJet30,   evtweight_, h_1d, ";N(jets)", 15, 0, 15);
    plot1D("h_nBJet20"+s,      t.gamma_nBJet20,   evtweight_, h_1d, ";N(bjets)", 6, 0, 6);
    plot1D("h_deltaPhiMin"+s,  t.gamma_deltaPhiMin,   evtweight_, h_1d, ";#Delta#phi_{min}", 32, 0, 3.2);
    plot1D("h_diffMetMht"+s,   t.gamma_diffMetMht,   evtweight_, h_1d, ";|E_{T}^{miss} - MHT| [GeV]", 120, 0, 300);
    plot1D("h_diffMetMhtOverMet"+s,   t.gamma_diffMetMht/t.gamma_met_pt,   evtweight_, h_1d, ";|E_{T}^{miss} - MHT| / E_{T}^{miss}", 100, 0, 2.);
    plot1D("h_minMTBMet"+s,   t.gamma_minMTBMet,   evtweight_, h_1d, ";min M_{T}(b, E_{T}^{miss}) [GeV]", 150, 0, 1500);
    plot1D("h_nlepveto"+s,     nlepveto_,   evtweight_, h_1d, ";N(leps)", 10, 0, 10);

    plot1D("h_drMinParton"+s,   t.gamma_drMinParton[0],   evtweight_, h_1d, ";DRmin(photon, parton)", 100, 0, 5);
    TString drName = "h_drMinPartonSample";
    drName += TString::Itoa(t.evt_id, 10);
    drName += s;
    plot1D(drName.Data(),   t.gamma_drMinParton[0],   evtweight_, h_1d, ";DRmin(photon, parton)", 100, 0, 5);

    // make drMinParton plot for separate ht regions
    if(t.gamma_ht >= 250 && t.gamma_ht < 450){
      plot1D("h_drMinParton_ht250to450"+s,   t.gamma_drMinParton[0],   evtweight_, h_1d, ";DRmin(photon, parton)", 100, 0, 5);
    }else if(t.gamma_ht >= 450 && t.gamma_ht < 1000){
      plot1D("h_drMinParton_ht450to1000"+s,   t.gamma_drMinParton[0],   evtweight_, h_1d, ";DRmin(photon, parton)", 100, 0, 5);
    }else if(t.gamma_ht >= 1000){
      plot1D("h_drMinParton_ht1000toInf"+s,   t.gamma_drMinParton[0],   evtweight_, h_1d, ";DRmin(photon, parton)", 100, 0, 5);
    }

    if (t.gamma_ht > 250) 
    {
      plot1D("h_bosonptbins"+s,      t.gamma_pt[0],   evtweight_, h_1d, ";p_{T}^{V} [GeV]", n_ptVbins, ptVbins);
      plot1D("h_bosonpt"+s,      t.gamma_pt[0],   evtweight_, h_1d, ";p_{T}^{V} [GeV]",300, 0, 1500);
      if ( fabs(t.gamma_eta[0])<1.4 ) plot1D("h_bosonptbinsCentral"+s,      t.gamma_pt[0],   evtweight_, h_1d, ";p_{T}^{V} [GeV]", n_ptVbins, ptVbins);
    }
  }

  outfile_->cd();
  return;
}

void MT2Looper::fillHistosDY(std::map<std::string, TH1*>& h_1d, int n_mt2bins, float* mt2bins, const std::string& dirname, const std::string& s) {
  TDirectory * dir = (TDirectory*)outfile_->Get(dirname.c_str());
  if (dir == 0) {
    dir = outfile_->mkdir(dirname.c_str());
  } 
  dir->cd();

  // workaround for monojet bins
  float zll_mt2_temp = t.zll_mt2;
  if (nJet30_ == 1) zll_mt2_temp = t.zll_ht;

  //const std::string&  NB = nBJet20_ == 0 ? "_0B" : ( nBJet20_ == 1 ? "_1B" : ( nBJet20_ == 2 ? "_2B" : "_3B") );
  plot1D("h_mt2bins"+s,       zll_mt2_temp,   evtweight_, h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
  //plot1D("h_mt2bins"+NB+s,       zll_mt2_temp,   evtweight_, h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
  
  if (dirname=="crdybaseVL") {
    if (nJet30_ < 4) {
      plot1D("h_mt2bins23J"+s,       zll_mt2_temp,   evtweight_, h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
      //plot1D("h_mt2bins23J"+NB+s,       zll_mt2_temp,   evtweight_, h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
    }
    else {
      plot1D("h_mt2bins4J"+s,       zll_mt2_temp,   evtweight_, h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
      //plot1D("h_mt2bins4J"+NB+s,       zll_mt2_temp,   evtweight_, h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
    }
    if (nJet30_ > 3) {
      plot1D("h_mt2bins3J"+s,       zll_mt2_temp,   evtweight_, h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
      //plot1D("h_mt2bins3J"+NB+s,       zll_mt2_temp,   evtweight_, h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
    }
  }
  if (dirname=="crdybaseL" || dirname=="crdybaseM" || dirname=="crdybaseH") {
    if (nJet30_ < 7) {
      plot1D("h_mt2bins26J"+s,       zll_mt2_temp,   evtweight_, h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);  // used for 26J 3B
      //plot1D("h_mt2bins26J"+NB+s,       zll_mt2_temp,   evtweight_, h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);  // used for 26J 3B
    }
    if (nJet30_ < 4) {
      plot1D("h_mt2bins23J"+s,       zll_mt2_temp,   evtweight_, h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
      //plot1D("h_mt2bins23J"+NB+s,       zll_mt2_temp,   evtweight_, h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
    }
    else if (nJet30_ < 7) {
      plot1D("h_mt2bins46J"+s,       zll_mt2_temp,   evtweight_, h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
      //plot1D("h_mt2bins46J"+NB+s,       zll_mt2_temp,   evtweight_, h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
    }
    else {
      plot1D("h_mt2bins7J"+s,       zll_mt2_temp,   evtweight_, h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
      //plot1D("h_mt2bins7J"+NB+s,       zll_mt2_temp,   evtweight_, h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
    }
    if (nJet30_ > 3 && nJet30_ < 7) {
      plot1D("h_mt2bins36J"+s,       zll_mt2_temp,   evtweight_, h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
      //plot1D("h_mt2bins36J"+NB+s,       zll_mt2_temp,   evtweight_, h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
    }
  }
  if (dirname=="crdy20") { // Special plot to get MT2 template for 3B aggregate region
    if (nJet30_ > 2) {
      plot1D("h_mt2bins3J"+s,       zll_mt2_temp,   evtweight_, h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
    }
  }
  
  if (dirname=="crdynocut" || TString(dirname).Contains("crdybase") || dirname=="crdyL" || dirname=="crdyM" || dirname=="crdyH") {

//    // Count Loose b-tags (and also Medium, just to make sure we do it right)
//    int nlooseb = 0;
//    int nmediumb = 0;
//    int ntightb = 0;
//    for (int ijet = 0; ijet < t.njet; ijet++) {
//      if (t.jet_pt[ijet] > 20. && fabs(t.jet_eta[ijet]) < 2.4) {
//	if ( t.jet_btagCSV[ijet] > 0.460 ) nlooseb++;
//	if ( t.jet_btagCSV[ijet] > 0.800 ) nmediumb++;
//	if ( t.jet_btagCSV[ijet] > 0.935 ) ntightb++;
//      }
//    } 

    plot1D("h_Events"+s,  1, 1, h_1d, ";Events, Unweighted", 1, 0, 2);
    plot1D("h_Events_w"+s,  1,   evtweight_, h_1d, ";Events, Weighted", 1, 0, 2);
    plot1D("h_mt2"+s,       zll_mt2_temp,   evtweight_, h_1d, "; M_{T2} [GeV]", 150, 0, 1500);
    plot1D("h_met"+s,       t.zll_met_pt,   evtweight_, h_1d, ";E_{T}^{miss} [GeV]", 150, 0, 1500);
    plot1D("h_ht"+s,       t.zll_ht,   evtweight_, h_1d, ";H_{T} [GeV]", 120, 0, 3000);
    plot1D("h_nJet30"+s,       nJet30_,   evtweight_, h_1d, ";N(jets)", 15, 0, 15);
    plot1D("h_nBJet20"+s,      nBJet20_,   evtweight_, h_1d, ";N(bjets)", 6, 0, 6);
//    plot1D("h_nBJet20L"+s,      nlooseb,   evtweight_, h_1d, ";N(bjets, L)", 6, 0, 6);
//    plot1D("h_nBJet20M"+s,      nmediumb,   evtweight_, h_1d, ";N(bjets, M)", 6, 0, 6);
//    plot1D("h_nBJet20T"+s,      ntightb,   evtweight_, h_1d, ";N(bjets, T)", 6, 0, 6);
    plot1D("h_deltaPhiMin"+s,  t.zll_deltaPhiMin,   evtweight_, h_1d, ";#Delta#phi_{min}", 32, 0, 3.2);
    plot1D("h_diffMetMht"+s,   t.zll_diffMetMht,   evtweight_, h_1d, ";|E_{T}^{miss} - MHT| [GeV]", 120, 0, 300);
    plot1D("h_diffMetMhtOverMet"+s,   t.zll_diffMetMht/t.zll_met_pt,   evtweight_, h_1d, ";|E_{T}^{miss} - MHT| / E_{T}^{miss}", 100, 0, 2.);
    plot1D("h_minMTBMet"+s,   t.zll_minMTBMet,   evtweight_, h_1d, ";min M_{T}(b, E_{T}^{miss}) [GeV]", 150, 0, 1500);
    plot1D("h_nlepveto"+s,     t.nLepLowMT,   evtweight_, h_1d, ";N(leps)", 10, 0, 10);
    plot1D("h_htbins"+s,       t.zll_ht,   evtweight_, h_1d, ";H_{T} [GeV]", n_htbins, htbins);
    plot1D("h_htbins2"+s,       t.zll_ht,   evtweight_, h_1d, ";H_{T} [GeV]", n_htbins2, htbins2);
    //    if (nJet30_>7) cout<<"event "<<t.evt<<" in run "<<t.run<<" has njets "<<nJet30_<<" and ht "<<t.zll_ht<<endl;
    plot1D("h_njbins"+s,       nJet30_,   evtweight_, h_1d, ";N(jets)", n_njbins, njbins);
    plot1D("h_nbjbins"+s,       nBJet20_,   evtweight_, h_1d, ";N(bjets)", n_nbjbins, nbjbins);
    plot1D("h_leppt1"+s,      t.lep_pt[0],   evtweight_, h_1d, ";p_{T}(lep1) [GeV]", 20, 0, 500);
    plot1D("h_leppt2"+s,      t.lep_pt[1],   evtweight_, h_1d, ";p_{T}(lep2) [GeV]", 20, 0, 500);
    plot1D("h_zllmass"+s,      t.zll_mass,   evtweight_, h_1d, ";m_{ll} [GeV]", 60, 0, 120);
    if (abs(t.lep_pdgId[0])==11) plot1D("h_zllmassEle"+s,      t.zll_mass,   evtweight_, h_1d, ";m_{ll} [GeV]", 60, 0, 120);
    if (abs(t.lep_pdgId[0])==13) plot1D("h_zllmassMu"+s,      t.zll_mass,   evtweight_, h_1d, ";m_{ll} [GeV]", 60, 0, 120);
    if (t.zll_ht > 250) 
    {
      plot1D("h_bosonptbins"+s,      t.zll_pt,   evtweight_, h_1d, ";p_{T}^{V} [GeV]", n_ptVbins, ptVbins);
      TLorentzVector Zll(0,0,0,0);
      Zll.SetPtEtaPhiM(t.zll_pt, t.zll_eta, t.zll_phi, t.zll_mass);
      float Zrapidity = Zll.Rapidity();
      if ( fabs(Zrapidity)<1.4 ) plot1D("h_bosonptbinsCentral"+s,      t.zll_pt,   evtweight_, h_1d, ";p_{T}^{V} [GeV]", n_ptVbins, ptVbins);
    }
  }

  // lepton efficiency variation in control region: smallish uncertainty on leptons which ARE vetoed
  if (!t.isData && doLepEffVars && (applyLeptonSFfromFiles || applyLeptonSFfromBabies)) {
    // lepsf was already applied as a central value, take it back out
    plot1D("h_mt2bins_lepeff_UP"+s,       zll_mt2_temp,   evtweight_ / weight_lepsf_cr_ * weight_lepsf_cr_UP_, h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
    plot1D("h_mt2bins_lepeff_DN"+s,       zll_mt2_temp,   evtweight_ / weight_lepsf_cr_ * weight_lepsf_cr_DN_, h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
    
  }
  
  outfile_->cd();
  return;
}

void MT2Looper::fillHistosRemovedLepton(std::map<std::string, TH1*>& h_1d, int n_mt2bins, float* mt2bins, const std::string& dirname, const std::string& s) {
  TDirectory * dir = (TDirectory*)outfile_->Get(dirname.c_str());
  if (dir == 0) {
    dir = outfile_->mkdir(dirname.c_str());
  } 
  dir->cd();

  // workaround for monojet bins
  float rl_mt2_temp = t.rl_mt2;
  if (nJet30_ == 1) rl_mt2_temp = t.rl_ht;

  plot1D("h_mt2bins"+s,       rl_mt2_temp,   evtweight_, h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);

  if (dirname=="crrlnocut" || TString(dirname).Contains("crrlbase") || dirname=="crrlL" || dirname=="crrlM" || dirname=="crrlH" ||
      dirname=="crrlelnocut" || dirname=="crrlelbase" || dirname=="crrlelL" || dirname=="crrlelM" || dirname=="crrlelH" ||
      dirname=="crrlmunocut" || dirname=="crrlmubase" || dirname=="crrlmuL" || dirname=="crrlmuM" || dirname=="crrlmuH" ) { 

    float mt = MT(t.lep_pt[0],t.lep_phi[0],met_pt_,met_phi_);
    plot1D("h_Events"+s,  1, 1, h_1d, ";Events, Unweighted", 1, 0, 2);
    plot1D("h_Events_w"+s,  1,   evtweight_, h_1d, ";Events, Weighted", 1, 0, 2);
    plot1D("h_mt2"+s,       rl_mt2_temp,   evtweight_, h_1d, "; M_{T2} [GeV]", 150, 0, 1500);
    plot1D("h_met"+s,       t.rl_met_pt,   evtweight_, h_1d, ";E_{T}^{miss} [GeV]", 150, 0, 1500);
    plot1D("h_ht"+s,       t.rl_ht,   evtweight_, h_1d, ";H_{T} [GeV]", 120, 0, 3000);
    plot1D("h_nJet30"+s,       nJet30_,   evtweight_, h_1d, ";N(jets)", 15, 0, 15);
    plot1D("h_nBJet20"+s,      nBJet20_,   evtweight_, h_1d, ";N(bjets)", 6, 0, 6);
    plot1D("h_deltaPhiMin"+s,  t.rl_deltaPhiMin,   evtweight_, h_1d, ";#Delta#phi_{min}", 32, 0, 3.2);
    plot1D("h_diffMetMht"+s,   t.rl_diffMetMht,   evtweight_, h_1d, ";|E_{T}^{miss} - MHT| [GeV]", 120, 0, 300);
    plot1D("h_diffMetMhtOverMet"+s,   t.rl_diffMetMht/t.rl_met_pt,   evtweight_, h_1d, ";|E_{T}^{miss} - MHT| / E_{T}^{miss}", 100, 0, 2.);
    plot1D("h_minMTBMet"+s,   t.rl_minMTBMet,   evtweight_, h_1d, ";min M_{T}(b, E_{T}^{miss}) [GeV]", 150, 0, 1500);
    plot1D("h_nlepveto"+s,     t.nLepLowMT,   evtweight_, h_1d, ";N(leps)", 10, 0, 10);
    plot1D("h_htbins"+s,       t.rl_ht,   evtweight_, h_1d, ";H_{T} [GeV]", n_htbins, htbins);
    plot1D("h_htbins2"+s,       t.rl_ht,   evtweight_, h_1d, ";H_{T} [GeV]", n_htbins2, htbins2);
    plot1D("h_njbins"+s,       nJet30_,   evtweight_, h_1d, ";N(jets)", n_njbins, njbins);
    plot1D("h_nbjbins"+s,       nBJet20_,   evtweight_, h_1d, ";N(bjets)", n_nbjbins, nbjbins);
    plot1D("h_leppt"+s,      t.lep_pt[0],   evtweight_, h_1d, ";p_{T}(lep) [GeV]", 200, 0, 1000);
    plot1D("h_lepeta"+s,      t.lep_eta[0],   evtweight_, h_1d, "#eta(lep) [GeV]", 50, -2.5, 2.5);
    plot1D("h_mt"+s,      mt,   evtweight_, h_1d, ";m_{T}(lep, MET) [GeV]", 200, 0, 400);
    plot1D("h_lepreliso"+s,      t.lep_relIso03[0],   evtweight_, h_1d, ";RelIso03", 50, 0, 1);
    plot1D("h_lepabsiso"+s,      t.lep_relIso03[0]*t.lep_pt[0],   evtweight_, h_1d, ";AbsIso03 [GeV]", 50, 0, 50);
    plot1D("h_simplemet"+s,      met_pt_,   evtweight_, h_1d, ";E_{T}^{miss} [GeV]", 200, 0, 1000);
  }//dirname

  outfile_->cd();
  return;
}

void MT2Looper::fillHistosQCD(std::map<std::string, TH1*>& h_1d, int n_mt2bins, float* mt2bins, const std::string& dirname, const std::string& s) {
  if (print_qcd_event_list && t.isData)
    // if (print_qcd_event_list)    
    fqcdlist << dirname << "\t" << t.run << "\t" << t.lumi << "\t" << t.evt << std::endl;

  TDirectory * dir = (TDirectory*)outfile_->Get(dirname.c_str());
  if (dir == 0) {
    dir = outfile_->mkdir(dirname.c_str());
  } 
  dir->cd();

  // workaround for monojet bins
  float mt2_temp = mt2_;
  TString directoryname(dirname);
  // to include QCD estimate for monojet region
  if (nJet30_ == 1 || directoryname.Contains("J")) mt2_temp = ht_;

  plot1D("h_Events"+s,  1, 1, h_1d, ";Events, Unweighted", 1, 0, 2);
  plot1D("h_Events_w"+s,  1,   evtweight_, h_1d, ";Events, Weighted", 1, 0, 2);
  plot1D("h_mt2bins"+s,       mt2_temp,   evtweight_, h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
  plot1D("h_htbins"+s,       ht_,   evtweight_, h_1d, ";H_{T} [GeV]", n_htbins, htbins);
  plot1D("h_htbins2"+s,       ht_,   evtweight_, h_1d, ";H_{T} [GeV]", n_htbins2, htbins2);
  plot1D("h_njbins"+s,       nJet30_,   evtweight_, h_1d, ";N(jets)", n_njbins, njbins);
  plot1D("h_nbjbins"+s,       nBJet20_,   evtweight_, h_1d, ";N(bjets)", n_nbjbins, nbjbins);
  plot1D("h_mt2"+s,       mt2_temp,   evtweight_, h_1d, "; M_{T2} [GeV]", 150, 0, 1500);
  if (!doMinimalPlots) {
    plot1D("h_met"+s,       met_pt_,   evtweight_, h_1d, ";E_{T}^{miss} [GeV]", 150, 0, 1500);
    plot1D("h_ht"+s,       ht_,   evtweight_, h_1d, ";H_{T} [GeV]", 120, 0, 3000);
    plot1D("h_nJet30"+s,       nJet30_,   evtweight_, h_1d, ";N(jets)", 15, 0, 15);
    plot1D("h_nJet30Eta3"+s,       nJet30Eta3_,   evtweight_, h_1d, ";N(jets, |#eta| > 3.0)", 10, 0, 10);
    plot1D("h_nBJet20"+s,      nBJet20_,   evtweight_, h_1d, ";N(bjets)", 6, 0, 6);
    plot1D("h_deltaPhiMin"+s,  deltaPhiMin_,   evtweight_, h_1d, ";#Delta#phi_{min}", 32, 0, 3.2);
    plot1D("h_diffMetMht"+s,   diffMetMht_,   evtweight_, h_1d, ";|E_{T}^{miss} - MHT| [GeV]", 120, 0, 300);
    plot1D("h_diffMetMhtOverMet"+s,   diffMetMht_/met_pt_,   evtweight_, h_1d, ";|E_{T}^{miss} - MHT| / E_{T}^{miss}", 100, 0, 2.);
    plot1D("h_minMTBMet"+s,   t.minMTBMet,   evtweight_, h_1d, ";min M_{T}(b, E_{T}^{miss}) [GeV]", 150, 0, 1500);
    plot1D("h_nlepveto"+s,     nlepveto_,   evtweight_, h_1d, ";N(leps)", 10, 0, 10);
    plot1D("h_J0pt"+s,       jet1_pt_,   evtweight_, h_1d, ";p_{T}(jet1) [GeV]", 150, 0, 1500);
    plot1D("h_J0eta"+s,      t.jet_eta[0],  evtweight_, h_1d, ";eta(jet1)", 100,-5,5);
    plot1D("h_J0phi"+s,      t.jet_phi[0],  evtweight_, h_1d, ";phi(jet1)", 100,-3.2,3.2);
    plot1D("h_J1pt"+s,       jet2_pt_,   evtweight_, h_1d, ";p_{T}(jet2) [GeV]", 150, 0, 1500);
  }
  
  // perform r_effective calculation
  if(doReffCalculation && t.nJet30>=2 && t.ht>=250 && t.mt2>=200){

    int ht_ind = 0;
    if(t.ht>=450) ht_ind = 1;
    if(t.ht>=575) ht_ind = 2;
    if(t.ht>=1000) ht_ind = 3;
    if(t.ht>=1500) ht_ind = 4;

    vector<TF1*>* fits = &rphi_fits_data;
    TF1 *fit = fits->at(3*ht_ind);
    TF1 *fit_systUp = fits->at(3*ht_ind+1);
    TF1 *fit_systDown = fits->at(3*ht_ind+2);
    // cout << t.ht << " " << t.mt2 << " " << fit->Eval(t.mt2) << " " << fit_systUp->Eval(t.mt2) << " " << fit_systDown->Eval(t.mt2) << endl;
    plot1D("h_sumRphi"+s,          t.mt2, evtweight_ * fit->Eval(t.mt2),          h_1d, ";Sum r_#phi", n_mt2bins, mt2bins);
    plot1D("h_sumRphi_systUp"+s,   t.mt2, evtweight_ * fit_systUp->Eval(t.mt2),   h_1d, ";Sum r_#phi, syst up", n_mt2bins, mt2bins);
    plot1D("h_sumRphi_systDown"+s, t.mt2, evtweight_ * fit_systDown->Eval(t.mt2), h_1d, ";Sum r_#phi, syst down", n_mt2bins, mt2bins);

    fits = &rphi_fits_mc;
    fit = fits->at(3*ht_ind);
    fit_systUp = fits->at(3*ht_ind+1);
    fit_systDown = fits->at(3*ht_ind+2);
    plot1D("h_sumRphi_mc"+s,          t.mt2, evtweight_ * fit->Eval(t.mt2),          h_1d, ";Sum r_#phi", n_mt2bins, mt2bins);
    plot1D("h_sumRphi_mc_systUp"+s,   t.mt2, evtweight_ * fit_systUp->Eval(t.mt2),   h_1d, ";Sum r_#phi, syst up", n_mt2bins, mt2bins);
    plot1D("h_sumRphi_mc_systDown"+s, t.mt2, evtweight_ * fit_systDown->Eval(t.mt2), h_1d, ";Sum r_#phi, syst down", n_mt2bins, mt2bins);

  }

  outfile_->cd();
  return;
}

void MT2Looper::fillLepCorSRfromFile() {

  // lepton efficiency variation in signal region: large uncertainty on leptons NOT vetoed
  cor_lepeff_sr_ = 1.;
  unc_lepeff_sr_UP_ = 1.;
  unc_lepeff_sr_DN_ = 1.;
  // require 0 veto leptons
  if (nlepveto_ != 0) return;
  for (int ilep = 0; ilep < t.ngenLep+t.ngenLepFromTau; ++ilep) {
    float pt,eta;
    int pdgId;
    if (ilep < t.ngenLep) {
      pt = t.genLep_pt[ilep];
      eta = t.genLep_eta[ilep];
      pdgId = t.genLep_pdgId[ilep];
    } else {
      pt = t.genLepFromTau_pt[ilep-t.ngenLep];
      eta = t.genLepFromTau_eta[ilep-t.ngenLep];
      pdgId = t.genLepFromTau_pdgId[ilep-t.ngenLep];
    }
    // check acceptance for veto: pt > 5, |eta| < 2.4
    if (pt < 5.) continue;
    if (fabs(eta) > 2.4) continue;

    // look up SF and vetoeff, by flavor
    weightStruct sf_struct_fullsim = getLepSFFromFile(pt, eta, pdgId);
    float sf = sf_struct_fullsim.cent;
    float vetoeff = getLepVetoEffFromFile_fullsim(pt, eta, pdgId);
    float unc = (sf_struct_fullsim.up - sf_struct_fullsim.cent);
    
    if (isSignal_) {
      weightStruct sf_struct_fastsim = getLepSFFromFile_fastsim(pt, eta, pdgId);
      sf = sf_struct_fullsim.cent * sf_struct_fastsim.cent;
      vetoeff = getLepVetoEffFromFile_fastsim(pt, eta, pdgId);
      unc = (sf_struct_fullsim.up - sf_struct_fullsim.cent) + (sf_struct_fastsim.up - sf_struct_fastsim.cent);
    }

    // apply correction to central value in 0L events
    if (applyLeptonSFtoSR) {
      // apply SF to vetoeff, then correction for 0L will be (1 - vetoeff_cor) / (1 - vetoeff) - 1.
      float vetoeff_cor = vetoeff * sf;
      float cor_0l = ( (1. - vetoeff_cor) / (1. - vetoeff) ) - 1.;
      cor_lepeff_sr_ *= (1. + cor_0l);
      float vetoeff_cor_unc_UP = vetoeff_cor * (1. + unc);
      float unc_UP_0l = ( (1. - vetoeff_cor_unc_UP) / (1. - vetoeff_cor) ) - 1.;
      unc_lepeff_sr_UP_ *= (1. + cor_0l + unc_UP_0l);
      unc_lepeff_sr_DN_ *= (1. + cor_0l - unc_UP_0l);
    }
    // do NOT apply correction to central value in 0L events
    else {
      float vetoeff_unc_UP = vetoeff * (1. + unc);
      float unc_UP_0l = ( (1. - vetoeff_unc_UP) / (1. - vetoeff) ) - 1.;
      unc_lepeff_sr_UP_ *= (1. + unc_UP_0l);
      unc_lepeff_sr_DN_ *= (1. - unc_UP_0l);
    }
	  
  } // loop over gen leptons

  return;
}

void MT2Looper::fillLepSFWeightsFromFile() {

  weight_lepsf_cr_ = 1.;
  weight_lepsf_cr_UP_ = 1.;
  weight_lepsf_cr_DN_ = 1.;

  for (int ilep = 0; ilep < t.nlep; ++ilep) {
    weightStruct weights = getLepSFFromFile(t.lep_pt[ilep], t.lep_eta[ilep], t.lep_pdgId[ilep]);
    weight_lepsf_cr_ *= weights.cent;
    weight_lepsf_cr_UP_ *= weights.up;
    weight_lepsf_cr_DN_ *= weights.dn;
    if (isSignal_) {
      weightStruct weights_fastsim = getLepSFFromFile_fastsim(t.lep_pt[ilep], t.lep_eta[ilep], t.lep_pdgId[ilep]);
      weight_lepsf_cr_ *= weights_fastsim.cent;
      weight_lepsf_cr_UP_ *= weights_fastsim.up;
      weight_lepsf_cr_DN_ *= weights_fastsim.dn;
    }
  } // loop over leps

  // have to also check for leptons only reco'd as PF leptons.. only add on to uncertainty
  for (int itrk = 0; itrk < t.nisoTrack; ++itrk) {
    float pt = t.isoTrack_pt[itrk];
    if (pt < 5.) continue;
    int pdgId = t.isoTrack_pdgId[itrk];
    if ((abs(pdgId) != 11) && (abs(pdgId) != 13)) continue;
    if (t.isoTrack_absIso[itrk]/pt > 0.2) continue;
    float mt = sqrt( 2 * met_pt_ * pt * ( 1 - cos( met_phi_ - t.isoTrack_phi[itrk]) ) );
    if (mt > 100.) continue;

    // check overlap with reco leptons
    bool overlap = false;
    for(int ilep = 0; ilep < t.nlep; ilep++){
      float thisDR = DeltaR(t.isoTrack_eta[itrk], t.lep_eta[ilep], t.isoTrack_phi[itrk], t.lep_phi[ilep]);
      if (thisDR < 0.01) {
	overlap = true;
	break;
      }
    } // loop over reco leps
    if (overlap) continue;

    weightStruct weights = getLepSFFromFile(t.isoTrack_pt[itrk], t.isoTrack_eta[itrk], t.isoTrack_pdgId[itrk]);
    weight_lepsf_cr_UP_ *= (1.0 + (weights.up - weights.cent));
    weight_lepsf_cr_DN_ *= (1.0 - (weights.cent - weights.dn));
    if (isSignal_) {
      weightStruct weights_fastsim = getLepSFFromFile_fastsim(t.isoTrack_pt[itrk], t.isoTrack_eta[itrk], t.lep_pdgId[itrk]);
      weight_lepsf_cr_UP_ *= (1.0 + (weights_fastsim.up - weights_fastsim.cent));
      weight_lepsf_cr_DN_ *= (1.0 - (weights_fastsim.cent - weights_fastsim.dn));
    }

  } // loop over isoTracks
  
  return;
}

void MT2Looper::fillHistosGenMET(std::map<std::string, TH1*>& h_1d, int n_mt2bins, float* mt2bins, const std::string& dirname, const std::string& s) {
  TDirectory * dir = (TDirectory*)outfile_->Get(dirname.c_str());
  if (dir == 0) {
    dir = outfile_->mkdir(dirname.c_str());
  } 
  dir->cd();

  // workaround for monojet bins
  float mt2_temp = t.mt2_genmet;
  if (nJet30_ == 1) mt2_temp = ht_;

  plot1D("h_mt2bins_genmet"+s,       mt2_temp,   evtweight_, h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);

  if (isSignal_) {
    plot3D("h_mt2bins_sigscan_genmet"+s, mt2_temp, t.GenSusyMScan1, t.GenSusyMScan2, evtweight_, h_1d, ";M_{T2} [GeV];mass1 [GeV];mass2 [GeV]", n_mt2bins, mt2bins, n_m1bins, m1bins, n_m2bins, m2bins);
  }
  
  outfile_->cd();
  return;
}

float MT2Looper::getAverageISRWeight(const int evt_id, const int var) {

  // madgraph ttsl, from RunIISummer16MiniAODv2
  if (evt_id == 301 || evt_id == 302) {
    if (var == 0) return 0.909; // nominal
    else if (var == 1) return 0.954; // UP
    else if (var == -1) return 0.863; // DN
  }
  // madgraph ttdl, from RunIISummer16MiniAODv2
  else if (evt_id == 303) {
    if (var == 0) return 0.895; // nominal
    else if (var == 1) return 0.948; // UP
    else if (var == -1) return 0.843; // DN
  }

  std::cout << "WARNING: MT2Looper::getAverageISRWeight: didn't recognize either evt_id: " << evt_id
	    << " or variation: " << var << std::endl;
  return 1.;
}

