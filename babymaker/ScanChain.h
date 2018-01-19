#ifndef ScanChain_h
#define ScanChain_h

// C++ includes
//#include <string>
//#include <vector>

// ROOT includes
#include "TROOT.h"
#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH2D.h"
#include "Math/LorentzVector.h"
#include "Math/GenVector/LorentzVector.h"

// TODO: add this file and get everything working
#include "../MT2CORE/RebalSmear/JRTreader.h"

typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > LorentzVector;

class BTagCalibration;
class BTagCalibrationReader;


class babyMaker {

 public:

  babyMaker() : doRecomputeRawPFMET_(false) {};
  ~babyMaker() {
    delete BabyFile_;
    delete BabyTree_;
  };

  void ScanChain(TChain*, std::string = "testSample", bool isFastsim = false, int max_events = -1);

  void MakeBabyNtuple(const char *);
  void InitBabyNtuple();
  void FillBabyNtuple();
  void CloseBabyNtuple();

  void SetRecomputeRawPFMET(bool flag) {doRecomputeRawPFMET_ = flag;};

  static void minuitFunction(int& nDim, double* gout, double& result, double par[], int flg);
  JRTreader rebal_reader;

 private:

  float getBtagEffFromFile(float pt, float eta, int mcFlavour, bool isFastsim = false);
  
  TFile *BabyFile_;
  TTree *BabyTree_;

  TH1D* count_hist_;

  bool isDataFromFileName;
  bool isPromptReco;
  bool doRecomputeRawPFMET_;

  // for btag SFs
  BTagCalibration* calib;
  BTagCalibrationReader* reader_heavy;
  BTagCalibrationReader* reader_heavy_UP;
  BTagCalibrationReader* reader_heavy_DN;
  BTagCalibrationReader* reader_light;
  BTagCalibrationReader* reader_light_UP;
  BTagCalibrationReader* reader_light_DN;

  TH2D* h_btag_eff_b;
  TH2D* h_btag_eff_c;
  TH2D* h_btag_eff_udsg;
  
  BTagCalibration* calib_fastsim;
  BTagCalibrationReader* reader_fastsim;
  BTagCalibrationReader* reader_fastsim_UP;
  BTagCalibrationReader* reader_fastsim_DN;

  TH2D* h_btag_eff_b_fastsim;
  TH2D* h_btag_eff_c_fastsim;
  TH2D* h_btag_eff_udsg_fastsim;
  

  //baby ntuple variables

  Int_t           run;
  Int_t           lumi;
  ULong64_t       evt;
  Int_t           isData;
  Int_t           isGolden;

  Float_t         evt_scale1fb;
  Float_t         evt_xsec;
  Float_t         evt_kfactor;
  Float_t         evt_filter;
  ULong64_t       evt_nEvts;
  Int_t           evt_id;
  Float_t         genWeight;
  Float_t         puWeight;
  Int_t           nVert;
  Int_t           nTrueInt;
  Float_t         rho;

  Int_t           nJet30;
  Int_t           nJet30JECup;
  Int_t           nJet30JECdn;
  Int_t           nJet40;
  Int_t           nBJet20;
  Int_t           nBJet20JECup;
  Int_t           nBJet20JECdn;
  Int_t           nBJet25;
  Int_t           nBJet30;
  Int_t           nBJet40;
  Int_t           nBJet20csv;
  Int_t           nBJet20mva;
  Int_t           nBJet30csv;
  Int_t           nBJet30mva;
  Int_t           nJet30FailId;
  Int_t           nJet100FailId;
  Int_t           nJet20BadFastsim;
  Int_t           nJet200MuFrac50DphiMet;
  Int_t           nMuons10;
  Int_t           nBadMuons20;
  Int_t           nElectrons10;
  Int_t           nLepLowMT;
  Int_t           nTaus20;
  Int_t           nGammas20;
  Int_t           nPFCHCand3;

  Float_t         deltaPhiMin;
  Float_t         deltaPhiMin_genmet;
  Float_t         diffMetMht;
  Float_t         diffMetMht_genmet;
  Float_t         deltaPhiMinJECup;
  Float_t         deltaPhiMinJECdn;
  Float_t         diffMetMhtJECup;
  Float_t         diffMetMhtJECdn;
  Float_t         minMTBMet;
  Float_t         gamma_minMTBMet;
  Float_t         zll_minMTBMet;
  Float_t         rl_minMTBMet;

  Float_t         ht;
  Float_t         htJECup;
  Float_t         htJECdn;
  Float_t         mt2;
  Float_t         mt2JECup;
  Float_t         mt2JECdn;
  Float_t         mt2_gen;
  Float_t         mt2_genmet;

  Float_t         jet1_pt;
  Float_t         jet2_pt;
  Float_t         jet1_ptJECup;
  Float_t         jet2_ptJECup;
  Float_t         jet1_ptJECdn;
  Float_t         jet2_ptJECdn;
  Int_t           jet_failFSveto;
  Float_t         gamma_jet1_pt;
  Float_t         gamma_jet2_pt;

  Float_t         pseudoJet1_pt;
  Float_t         pseudoJet1_eta;
  Float_t         pseudoJet1_phi;
  Float_t         pseudoJet1_mass;
  Float_t         pseudoJet2_pt;
  Float_t         pseudoJet2_eta;
  Float_t         pseudoJet2_phi;
  Float_t         pseudoJet2_mass;

  Float_t         mht_pt;
  Float_t         mht_ptJECup;
  Float_t         mht_ptJECdn;
  Float_t         mht_phi;
  Float_t         mht_phiJECup;
  Float_t         mht_phiJECdn;
  Float_t         met_pt;
  Float_t         met_ptJECup;
  Float_t         met_ptJECdn;
  Float_t         met_phi;
  Float_t         met_phiJECup;
  Float_t         met_phiJECdn;
  Float_t         met_rawPt;
  Float_t         met_rawPhi;
  Float_t         met_caloPt;
  Float_t         met_caloPhi;
  Float_t         met_trkPt;
  Float_t         met_trkPhi;
  Float_t         met_genPt;
  Float_t         met_genPhi;
  Float_t         met_miniaodPt;
  Float_t         met_miniaodPhi;

//----- MET FILTERS
  Int_t           Flag_EcalDeadCellTriggerPrimitiveFilter;
  Int_t           Flag_trkPOG_manystripclus53X;
  Int_t           Flag_ecalLaserCorrFilter;
  Int_t           Flag_trkPOG_toomanystripclus53X;
  Int_t           Flag_hcalLaserEventFilter;
  Int_t           Flag_trkPOG_logErrorTooManyClusters;
  Int_t           Flag_trkPOGFilters;
  Int_t           Flag_trackingFailureFilter;
  Int_t           Flag_CSCTightHaloFilter;
  Int_t           Flag_CSCTightHalo2015Filter;
  Int_t           Flag_globalTightHalo2016Filter;
  Int_t           Flag_globalSuperTightHalo2016Filter;
  Int_t           Flag_HBHENoiseFilter;
  Int_t           Flag_HBHENoiseIsoFilter;
  Int_t           Flag_goodVertices;
  Int_t           Flag_eeBadScFilter;
  Int_t           Flag_badMuonFilter;
  Int_t           Flag_badMuonFilterV2;  
  Int_t           Flag_badMuons;  
  Int_t           Flag_duplicateMuons;  
  Int_t           Flag_noBadMuons;  
  Int_t           Flag_badChargedHadronFilter;
  Int_t           Flag_badChargedHadronFilterV2;  
  Int_t           Flag_METFilters;  

//----- TRIGGER 
  Int_t           HLT_PFHT800;   
  Int_t           HLT_PFHT900;   
  Int_t           HLT_PFMET170;
  Int_t           HLT_PFHT300_PFMET100;   
  Int_t           HLT_PFHT300_PFMET110;   
  Int_t           HLT_PFHT350_PFMET100;   
  Int_t           HLT_PFHT350_PFMET120;   
  Int_t           HLT_PFMETNoMu90_PFMHTNoMu90;
  Int_t           HLT_MonoCentralPFJet80_PFMETNoMu90_PFMHTNoMu90;
  Int_t           HLT_PFMETNoMu100_PFMHTNoMu100;
  Int_t           HLT_PFMETNoMu110_PFMHTNoMu110;
  Int_t           HLT_PFMETNoMu120_PFMHTNoMu120;
  Int_t           HLT_PFMET90_PFMHT90;
  Int_t           HLT_PFMET100_PFMHT100;
  Int_t           HLT_PFMET110_PFMHT110;
  Int_t           HLT_PFMET120_PFMHT120;
  Int_t           HLT_PFJet450;   
  Int_t           HLT_PFJet500;   
  Int_t           HLT_ECALHT800;   
  Int_t           HLT_SingleMu;   
  Int_t           HLT_SingleMu_NonIso;   
  Int_t           HLT_SingleEl;   
  Int_t           HLT_SingleEl_NonIso;   
  Int_t           HLT_DoubleEl;   
  Int_t           HLT_DoubleEl33;   
  Int_t           HLT_MuX_Ele12;   
  Int_t           HLT_Mu8_EleX;   
  Int_t           HLT_Mu30_Ele30_NonIso;   
  Int_t           HLT_Mu33_Ele33_NonIso;   
  Int_t           HLT_DoubleMu;   
  Int_t           HLT_DoubleMu_NonIso;   
  Int_t           HLT_Photon120;   
  Int_t           HLT_Photon165_HE10;   
  Int_t           HLT_Photon250_NoHE;   
  Int_t           HLT_PFHT125_Prescale;   
  Int_t           HLT_PFHT200_Prescale;   
  Int_t           HLT_PFHT300_Prescale;   
  Int_t           HLT_PFHT350_Prescale;   
  Int_t           HLT_PFHT475_Prescale;   
  Int_t           HLT_PFHT600_Prescale;   
  Int_t           HLT_DiCentralPFJet70_PFMET120;
  Int_t           HLT_DiCentralPFJet55_PFMET110;

//----- LEPTONS
  static const int max_nlep = 50;
  Int_t           nlep;
  Float_t         lep_pt[max_nlep];   //[nlep]
  Float_t         lep_eta[max_nlep];   //[nlep]
  Float_t         lep_phi[max_nlep];   //[nlep]
  Float_t         lep_mass[max_nlep];   //[nlep]
  Int_t           lep_charge[max_nlep];   //[nlep]
  Int_t           lep_pdgId[max_nlep];   //[nlep]
  Float_t         lep_dxy[max_nlep];   //[nlep]
  Float_t         lep_dz[max_nlep];   //[nlep]
  Int_t           lep_tightId[max_nlep];   //[nlep]
  Int_t           lep_heepId[max_nlep];   //[nlep]
  Float_t         lep_highPtFit_pt[max_nlep];   //[nlep]
  Float_t         lep_highPtFit_eta[max_nlep];   //[nlep]
  Float_t         lep_highPtFit_phi[max_nlep];   //[nlep]
  Float_t         lep_relIso03[max_nlep];   //[nlep]
  Float_t         lep_relIso04[max_nlep];   //[nlep]
  Float_t         lep_miniRelIso[max_nlep];   //[nlep]
  Float_t         lep_relIsoAn04[max_nlep];   //[nlep]
  Int_t           lep_mcMatchId[max_nlep];   //[nlep]
  Int_t           lep_lostHits[max_nlep];   //[nlep]
  Int_t           lep_convVeto[max_nlep];   //[nlep]
  Int_t           lep_tightCharge[max_nlep];   //[nlep]

//----- ISOLATED TRACK
  static const int max_nisoTrack = 50;
  Int_t             nisoTrack;
  Float_t           isoTrack_pt[max_nisoTrack];
  Float_t           isoTrack_eta[max_nisoTrack];
  Float_t           isoTrack_phi[max_nisoTrack];
  Float_t           isoTrack_mass[max_nisoTrack];
  Float_t           isoTrack_absIso[max_nisoTrack];
  Float_t           isoTrack_relIsoAn04[max_nisoTrack];
  Float_t           isoTrack_dz[max_nisoTrack];
  Int_t             isoTrack_pdgId[max_nisoTrack];
  Int_t             isoTrack_mcMatchId[max_nisoTrack];

//----- ALL TRACKS
  static const int max_AllTrack = 1000;
  Int_t             nAllTrack;
  Float_t           AllTrack_pt[max_AllTrack];
  Float_t           AllTrack_eta[max_AllTrack];
  Float_t           AllTrack_phi[max_AllTrack];
  Float_t           AllTrack_mass[max_AllTrack];
  Float_t           AllTrack_absIso[max_AllTrack];
  Float_t           AllTrack_relIsoAn04[max_AllTrack];
  Float_t           AllTrack_dz[max_AllTrack];
  Int_t             AllTrack_pdgId[max_AllTrack];
  Int_t             AllTrack_mcMatchId[max_AllTrack];

  Int_t             nPFLep5LowMT;
  Int_t             nPFHad10LowMT;

//----- HIGH PT PF CANDS
  static const int max_nhighPtPFcands = 50;
  Int_t             nhighPtPFcands;
  Float_t           highPtPFcands_pt[max_nhighPtPFcands];
  Float_t           highPtPFcands_eta[max_nhighPtPFcands];
  Float_t           highPtPFcands_phi[max_nhighPtPFcands];
  Float_t           highPtPFcands_mass[max_nhighPtPFcands];
  Float_t           highPtPFcands_absIso[max_nhighPtPFcands];
  Float_t           highPtPFcands_relIsoAn04[max_nhighPtPFcands];
  Float_t           highPtPFcands_dz[max_nhighPtPFcands];
  Int_t             highPtPFcands_pdgId[max_nhighPtPFcands];
  Int_t             highPtPFcands_mcMatchId[max_nhighPtPFcands];

//----- TAUS
  static const int max_ntau = 50;
  Int_t           ntau;
  Float_t         tau_pt[max_ntau];   //[ntau]
  Float_t         tau_eta[max_ntau];   //[ntau]
  Float_t         tau_phi[max_ntau];   //[ntau]
  Float_t         tau_mass[max_ntau];   //[ntau]
  Int_t           tau_charge[max_ntau];   //[ntau]
  Float_t         tau_dxy[max_ntau];   //[ntau]
  Float_t         tau_dz[max_ntau];   //[ntau]
  Int_t           tau_idCI3hit[max_ntau];   //[ntau]
  Float_t         tau_isoCI3hit[max_ntau];   //[ntau]
  Int_t           tau_mcMatchId[max_ntau];   //[ntau]

//----- PHOTONS
  static const int max_ngamma = 50;
  Int_t           ngamma;
  Float_t         gamma_pt[max_ngamma];   //[ngamma]
  Float_t         gamma_eta[max_ngamma];   //[ngamma]
  Float_t         gamma_phi[max_ngamma];   //[ngamma]
  Float_t         gamma_mass[max_ngamma];   //[ngamma]
  Int_t           gamma_mcMatchId[max_ngamma];   //[ngamma]
  Float_t         gamma_genIso04[max_ngamma];   //[ngamma]
  Float_t         gamma_drMinParton[max_ngamma];   //[ngamma]
  Float_t         gamma_chHadIso[max_ngamma];   //[ngamma]
  Float_t         gamma_neuHadIso[max_ngamma];   //[ngamma]
  Float_t         gamma_phIso[max_ngamma];   //[ngamma]
  Float_t         gamma_sigmaIetaIeta[max_ngamma];   //[ngamma]
  Float_t         gamma_r9[max_ngamma];   //[ngamma]
  Float_t         gamma_hOverE[max_ngamma];   //[ngamma]
  Float_t         gamma_hOverE015[max_ngamma];   //[ngamma]
  Int_t           gamma_idCutBased[max_ngamma];   //[ngamma]

  // event level vars recalculated for photon+jets control region
  Float_t         gamma_mt2;
  Int_t           gamma_nJet30;
  Int_t           gamma_nJet40;
  Int_t           gamma_nJet30FailId;
  Int_t           gamma_nJet100FailId;
  Int_t           gamma_nBJet20;
  Int_t           gamma_nBJet20csv;
  Int_t           gamma_nBJet20mva;
  Int_t           gamma_nBJet25;
  Int_t           gamma_nBJet30;
  Int_t           gamma_nBJet40;
  Float_t         gamma_ht;
  Float_t         gamma_deltaPhiMin;
  Float_t         gamma_diffMetMht;
  Float_t         gamma_mht_pt;
  Float_t         gamma_mht_phi;
  Float_t         gamma_met_pt;
  Float_t         gamma_met_phi;

  // event level vars recalculated for Z-->ll control region
  Float_t         zll_mt2;
  Float_t         zll_deltaPhiMin;
  Float_t         zll_diffMetMht;
  Float_t         zll_met_pt;
  Float_t         zll_met_phi;
  Float_t         zll_mht_pt;
  Float_t         zll_mht_phi;
  Float_t         zll_mass;
  Float_t         zll_pt;
  Float_t         zll_eta;
  Float_t         zll_phi;
  Float_t         zll_ht;

  Float_t         zll_mt2JECup;
  Float_t         zll_deltaPhiMinJECup;
  Float_t         zll_diffMetMhtJECup;
  Float_t         zll_met_ptJECup;
  Float_t         zll_met_phiJECup;
  Float_t         zll_mht_ptJECup;
  Float_t         zll_mht_phiJECup;
  Float_t         zll_htJECup;

  Float_t         zll_mt2JECdn;
  Float_t         zll_deltaPhiMinJECdn;
  Float_t         zll_diffMetMhtJECdn;
  Float_t         zll_met_ptJECdn;
  Float_t         zll_met_phiJECdn;
  Float_t         zll_mht_ptJECdn;
  Float_t         zll_mht_phiJECdn;
  Float_t         zll_htJECdn;

  // event level vars recalculated for Z-->ll MT control region
  Float_t         zllmt_mt2;
  Float_t         zllmt_deltaPhiMin;
  Float_t         zllmt_diffMetMht;
  Float_t         zllmt_met_pt;
  Float_t         zllmt_met_phi;
  Float_t         zllmt_mht_pt;
  Float_t         zllmt_mht_phi;
  Float_t         zllmt_ht;
  Float_t         zllmt_mt;

  // event level vars recalculated for removed lepton control region
  Float_t         rl_mt2;
  Float_t         rl_deltaPhiMin;
  Float_t         rl_diffMetMht;
  Float_t         rl_met_pt;
  Float_t         rl_met_phi;
  Float_t         rl_mht_pt;
  Float_t         rl_mht_phi;
  Float_t         rl_mass;
  Float_t         rl_pt;
  Float_t         rl_eta;
  Float_t         rl_phi;
  Float_t         rl_ht;

//----- GEN PARTICLES
  static const int max_ngenPart = 300;
  Int_t           ngenPart;
  Float_t         genPart_pt[max_ngenPart];   //[ngenPart]
  Float_t         genPart_eta[max_ngenPart];   //[ngenPart]
  Float_t         genPart_phi[max_ngenPart];   //[ngenPart]
  Float_t         genPart_mass[max_ngenPart];   //[ngenPart]
  Int_t           genPart_pdgId[max_ngenPart];   //[ngenPart]
  Int_t           genPart_status[max_ngenPart];   //[ngenPart]
  Float_t         genPart_charge[max_ngenPart];   //[ngenPart]
  Int_t           genPart_motherId[max_ngenPart];   //[ngenPart]
  Int_t           genPart_grandmotherId[max_ngenPart];   //[ngenPart]

//----- GEN LEPTONS (ELECTRONS/MUONS)
  static const int max_ngenLep = 10;
  Int_t           ngenLep;
  Float_t         genLep_pt[max_ngenLep];   //[ngenLep]
  Float_t         genLep_eta[max_ngenLep];   //[ngenLep]
  Float_t         genLep_phi[max_ngenLep];   //[ngenLep]
  Float_t         genLep_mass[max_ngenLep];   //[ngenLep]
  Int_t           genLep_pdgId[max_ngenLep];   //[ngenLep]
  Int_t           genLep_status[max_ngenLep];   //[ngenLep]
  Float_t         genLep_charge[max_ngenLep];   //[ngenLep]
  Int_t           genLep_sourceId[max_ngenLep];   //[ngenLep]

//----- GEN STATUS 23
  static const int max_ngenStat23 = 30;
  Int_t           ngenStat23;
  Float_t         genStat23_pt[max_ngenStat23];   //[ngenStat23]
  Float_t         genStat23_eta[max_ngenStat23];   //[ngenStat23]
  Float_t         genStat23_phi[max_ngenStat23];   //[ngenStat23]
  Float_t         genStat23_mass[max_ngenStat23];   //[ngenStat23]
  Int_t           genStat23_pdgId[max_ngenStat23];   //[ngenStat23]
  Int_t           genStat23_status[max_ngenStat23];   //[ngenStat23]
  Float_t         genStat23_charge[max_ngenStat23];   //[ngenStat23]
  Int_t           genStat23_sourceId[max_ngenStat23];   //[ngenStat23]
  
//----- GEN GAMMAS
  static const int max_ngenGamma = 10;
  Int_t           ngenGamma;
  Float_t         genGamma_pt[max_ngenGamma];   //[ngenGamma]
  Float_t         genGamma_eta[max_ngenGamma];   //[ngenGamma]
  Float_t         genGamma_phi[max_ngenGamma];   //[ngenGamma]
  Int_t           genGamma_motherId[max_ngenGamma];   //[ngenGamma]
  
//----- GEN TAUS
  static const int max_ngenTau = 10;
  Int_t           ngenTau;
  Int_t           ngenTau1Prong;
  Int_t           ngenTau3Prong;
  Float_t         genTau_pt[max_ngenTau];   //[ngenTau]
  Float_t         genTau_eta[max_ngenTau];   //[ngenTau]
  Float_t         genTau_phi[max_ngenTau];   //[ngenTau]
  Float_t         genTau_mass[max_ngenTau];   //[ngenTau]
  Int_t           genTau_pdgId[max_ngenTau];   //[ngenTau]
  Int_t           genTau_status[max_ngenTau];   //[ngenTau]
  Float_t         genTau_charge[max_ngenTau];   //[ngenTau]
  Int_t           genTau_sourceId[max_ngenTau];   //[ngenTau]
  Int_t           genTau_decayMode[max_ngenTau];   //[ngenTau]
  Float_t         genTau_leadTrackPt[max_ngenTau];   //[ngenTau]
  Int_t           genTau_neutralDaughters[max_ngenTau];   //[ngenTau]

//----- GEN LEPTONS FROM TAUS
  static const int max_ngenLepFromTau = 10;
  Int_t           ngenLepFromTau;
  Float_t         genLepFromTau_pt[max_ngenLepFromTau];   //[ngenLepFromTau]
  Float_t         genLepFromTau_eta[max_ngenLepFromTau];   //[ngenLepFromTau]
  Float_t         genLepFromTau_phi[max_ngenLepFromTau];   //[ngenLepFromTau]
  Float_t         genLepFromTau_mass[max_ngenLepFromTau];   //[ngenLepFromTau]
  Int_t           genLepFromTau_pdgId[max_ngenLepFromTau];   //[ngenLepFromTau]
  Int_t           genLepFromTau_status[max_ngenLepFromTau];   //[ngenLepFromTau]
  Float_t         genLepFromTau_charge[max_ngenLepFromTau];   //[ngenLepFromTau]
  Int_t           genLepFromTau_sourceId[max_ngenLepFromTau];   //[ngenLepFromTau]

//----- JETS
  static const int max_njet = 100;
  Int_t           njet;
  Float_t         jet_pt[max_njet];   //[njet]
  Float_t         jet_eta[max_njet];   //[njet]
  Float_t         jet_phi[max_njet];   //[njet]
  Float_t         jet_mass[max_njet];   //[njet]
  Float_t         jet_btagCSV[max_njet];   //[njet]
  Float_t         jet_btagMVA[max_njet];   //[njet]
  Float_t         jet_rawPt[max_njet];   //[njet]
  Float_t         jet_mcPt[max_njet];   //[njet]
  Int_t           jet_mcFlavour[max_njet];   //[njet]
  Int_t           jet_hadronFlavour[max_njet];   //[njet]
  Float_t         jet_qgl[max_njet];   //[njet]
  Float_t         jet_area[max_njet];   //[njet]
  Int_t           jet_id[max_njet];   //[njet]
  Int_t           jet_puId[max_njet];   //[njet]
  Float_t         jet_muf[max_njet];   //[njet]

//----- SUSY SIGNALS
  Int_t           GenSusyMScan1;
  Int_t           GenSusyMScan2;
  Int_t           GenSusyMScan3;
  Int_t           GenSusyMScan4;

//----- WEIGHTS AND VARIATIONS
  Float_t         weight_lepsf;
  Float_t         weight_lepsf_UP;
  Float_t         weight_lepsf_DN;
  Float_t         weight_lepsf_0l;
  Float_t         weight_lepsf_0l_UP;
  Float_t         weight_lepsf_0l_DN;
  Float_t         weight_btagsf;
  Float_t         weight_btagsf_heavy_UP;
  Float_t         weight_btagsf_light_UP;
  Float_t         weight_btagsf_heavy_DN;
  Float_t         weight_btagsf_light_DN;
  Float_t         weight_sigtrigsf;
  Float_t         weight_dileptrigsf;
  Float_t         weight_phottrigsf;
  Float_t         weight_pu;
  Float_t         weight_isr;
  Float_t         weight_isr_UP;
  Float_t         weight_isr_DN;
  Float_t         weight_scales_UP;
  Float_t         weight_scales_DN;
  Float_t         weight_pdfs_UP;
  Float_t         weight_pdfs_DN;
  Float_t         weight_toppt;
  
  Float_t         genRecoil_pt;
  Float_t         genTop_pt;
  Float_t         genTbar_pt;
  Int_t           genProd_pdgId;
  Float_t         weight_pol_L;
  Float_t         weight_pol_R;
  Int_t           nisrMatch;
  Int_t           ngenLepFromZ;
  Int_t           ngenNuFromZ;

//----- MC SCALE AND PDF WEIGHTS
  static const int max_nLHEweight = 500;
  Int_t           nLHEweight;
  Float_t         LHEweight_wgt[max_nLHEweight];   //[nmcweights]

//----- REBALANCING VARIABLES
  Int_t           rebal_status;
  Int_t           nRebalJets;
  Int_t           rebal_useJet[max_njet];
  Float_t         rebal_jetpt[max_njet];
  Float_t         rebal_jeteta[max_njet];
  Float_t         rebal_jetphi[max_njet];
  Float_t         rebal_jetbtagcsv[max_njet];
  Float_t         rebal_factors[max_njet];
  Float_t         rebal_met_pt;
  Float_t         rebal_met_phi;
  Float_t         rebal_pt_soft_x;
  Float_t         rebal_pt_soft_y;
};

#endif

