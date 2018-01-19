// C++
#include <iostream>
#include <vector>
#include <set>

// ROOT
#include "TDirectory.h"
#include "TTreeCache.h"
#include "Math/VectorUtil.h"
#include "TVector2.h"
#include "TBenchmark.h"
#include "TLorentzVector.h"
// CMS2
//#include "../MT2CORE/CMS2.h"
//#include "../MT2CORE/tools.h"
//#include "../MT2CORE/selections.h"
//#include "../MT2CORE/hemJet.h"
//#include "../MT2CORE/MT2/MT2.h"
//#include "../MT2CORE/IsoTrackVeto.h"

// header
#include "ZllLooper.h"

//MT2
#include "../MT2CORE/Plotting/PlotUtilities.h"


bool printHtCalc = false;
bool makeEffAccWeights = true;
bool useEffAccWeights = true;
const int nptbins = 12;
const int netabins = 10;
const float ptbins[] = {20, 25, 30, 40, 60, 80, 100, 150, 200, 300, 500, 800, 1100};
const float etabins[] = {-2.5, -2, -1.5, -1, -0.5, 0, 0.5, 1, 1.5, 2, 2.5};

using namespace std;
using namespace mt2;

class mt2tree;

ZllLooper::ZllLooper(){
  // set some stuff here

}


ZllLooper::~ZllLooper(){

};

void ZllLooper::loop(TChain* chain, std::string output_name){

  // Benchmark
  TBenchmark *bmark = new TBenchmark();
  bmark->Start("benchmark");

  gROOT->cd();
  cout << "[ZllLooper::loop] creating output file: " << output_name << endl;

  outfile_ = new TFile(output_name.c_str(),"RECREATE") ; 

  cout << "[ZllLooper::loop] setting up histos" << endl;

  std::map<std::string, TH1D*> h_1d_nocut;
  std::map<std::string, TH1D*> h_1d_srH;
  std::map<std::string, TH1D*> h_1d_srM;
  std::map<std::string, TH1D*> h_1d_srL;
  std::map<std::string, TH1D*> h_1d_sr1H;
  std::map<std::string, TH1D*> h_1d_sr2H;
  std::map<std::string, TH1D*> h_1d_sr3H;
  std::map<std::string, TH1D*> h_1d_sr4H;
  std::map<std::string, TH1D*> h_1d_sr5H;
  std::map<std::string, TH1D*> h_1d_sr6H;
  std::map<std::string, TH1D*> h_1d_sr7H;
  std::map<std::string, TH1D*> h_1d_sr8H;
  std::map<std::string, TH1D*> h_1d_sr9H;
  std::map<std::string, TH1D*> h_1d_sr1M;
  std::map<std::string, TH1D*> h_1d_sr2M;
  std::map<std::string, TH1D*> h_1d_sr3M;
  std::map<std::string, TH1D*> h_1d_sr4M;
  std::map<std::string, TH1D*> h_1d_sr5M;
  std::map<std::string, TH1D*> h_1d_sr6M;
  std::map<std::string, TH1D*> h_1d_sr7M;
  std::map<std::string, TH1D*> h_1d_sr8M;
  std::map<std::string, TH1D*> h_1d_sr9M;
  std::map<std::string, TH1D*> h_1d_sr1L;
  std::map<std::string, TH1D*> h_1d_sr2L;
  std::map<std::string, TH1D*> h_1d_sr3L;
  std::map<std::string, TH1D*> h_1d_sr4L;
  std::map<std::string, TH1D*> h_1d_sr5L;
  std::map<std::string, TH1D*> h_1d_sr6L;
  std::map<std::string, TH1D*> h_1d_sr7L;
  std::map<std::string, TH1D*> h_1d_sr8L;
  std::map<std::string, TH1D*> h_1d_sr9L;
  std::map<std::string, TH2D*> h_2d;
  

    // Beginning of Efficiency and Acceptance


  if (makeEffAccWeights) {
    TString file1 =  "output/test/test.root";
    TFile f1(file1, "READ");
    if(!f1.IsOpen()){      
      std::cout<<"!!! BAD !!! File "<<file1<<" can't be found, will not be used "<<std::endl;
      makeEffAccWeights = false;
    }
    if (makeEffAccWeights) {
      cout<<"Opened file "<<file1<<" for EffAccWeights!"<<endl;
      if(f1.GetKey("h_AccDen_genVpt")==0){
	std::cout<<"Histogram h_AccDen_genVpt not found in file "<<file1<<std::endl;
	makeEffAccWeights = false;
      }
      if (makeEffAccWeights) {
	// Get histograms
	TH1D* AccDen= (TH1D*) f1.Get("h_AccDen_genVpt");
	TH1D* AccNum= (TH1D*) f1.Get("h_AccNum_genVpt");
	TH1D* InvMassEffDen= (TH1D*) f1.Get("h_InvMassEffDen_genVpt");
	TH1D* InvMassEffNum= (TH1D*) f1.Get("h_InvMassEffNum_genVpt");
	TH1D* GenInvMassEffDen= (TH1D*) f1.Get("h_GenInvMassEffDen_genVpt");
	TH1D* GenInvMassEffNum= (TH1D*) f1.Get("h_GenInvMassEffNum_genVpt");
	TH1D* GenDilepEffDen= (TH1D*) f1.Get("h_GenDilepEffDen_genVpt");
	TH1D* GenDilepEffNum= (TH1D*) f1.Get("h_GenDilepEffNum_genVpt");
	TH2D* LepEffDen_el= (TH2D*) f1.Get("h_LepEffDen_el");
	TH2D* LepEffNum_el= (TH2D*) f1.Get("h_LepEffNum_el");
	TH2D* LepEffDen_mu= (TH2D*) f1.Get("h_LepEffDen_mu");
	TH2D* LepEffNum_mu= (TH2D*) f1.Get("h_LepEffNum_mu");
	
	outfile_->cd();
	TH1D* Acc= (TH1D*) f1.Get("h_AccNum_genVpt")->Clone("LepAcc");
	Acc->Reset();
	Acc->SetTitle("LepAcc");
	if (!Acc->GetSumw2N()) Acc->Sumw2();
	
	TH1D* InvMassEff= (TH1D*) f1.Get("h_InvMassEffNum_genVpt")->Clone("InvMassEff");
	InvMassEff->Reset();
	InvMassEff->SetTitle("InvMassEff");
	if (!InvMassEff->GetSumw2N()) InvMassEff->Sumw2();

	TH1D* GenInvMassEff= (TH1D*) f1.Get("h_GenInvMassEffNum_genVpt")->Clone("GenInvMassEff");
	GenInvMassEff->Reset();
	GenInvMassEff->SetTitle("GenInvMassEff");
	if (!GenInvMassEff->GetSumw2N()) GenInvMassEff->Sumw2();
	
	TH1D* GenDilepEff= (TH1D*) f1.Get("h_GenDilepEffNum_genVpt")->Clone("GenDilepEff");
	GenDilepEff->Reset();
	GenDilepEff->SetTitle("GenDilepEff");
	if (!GenDilepEff->GetSumw2N()) GenDilepEff->Sumw2();

	TH2D* LepEff_el= (TH2D*) f1.Get("h_LepEffNum_el")->Clone("ElEff");
	LepEff_el->Reset();
	LepEff_el->SetTitle("LepEff_el");
	if (!LepEff_el->GetSumw2N()) LepEff_el->Sumw2();
	
	TH2D* LepEff_mu= (TH2D*) f1.Get("h_LepEffNum_mu")->Clone("MuEff");
	LepEff_mu->Reset();
	LepEff_mu->SetTitle("LepEff_mu");
	if (!LepEff_mu->GetSumw2N()) LepEff_mu->Sumw2();
	// Divide them
	Acc->Divide(AccNum, AccDen, 1.0, 1.0, "B");
	InvMassEff->Divide(InvMassEffNum, InvMassEffDen, 1.0, 1.0, "B");
	GenInvMassEff->Divide(GenInvMassEffNum, GenInvMassEffDen, 1.0, 1.0, "B");
	GenDilepEff->Divide(GenDilepEffNum, GenDilepEffDen, 1.0, 1.0, "B");
	LepEff_el->Divide(LepEffNum_el, LepEffDen_el, 1.0, 1.0, "B");
	LepEff_mu->Divide(LepEffNum_mu, LepEffDen_mu, 1.0, 1.0, "B");
	h_1d_global.insert(std::pair<std::string, TH1D*> ("Acc",Acc));
	h_1d_global.insert(std::pair<std::string, TH1D*> ("InvMassEff",InvMassEff));
	h_1d_global.insert(std::pair<std::string, TH1D*> ("GenInvMassEff",GenInvMassEff));
	h_1d_global.insert(std::pair<std::string, TH1D*> ("GenDilepEff",GenDilepEff));
	h_2d_global.insert(std::pair<std::string, TH2D*> ("LepEff_el",LepEff_el));
	h_2d_global.insert(std::pair<std::string, TH2D*> ("LepEff_mu",LepEff_mu));
      }
    }	
  }
    // End of Efficiency and Acceptance




  
  // File Loop
  int nDuplicates = 0;
  int nEvents = chain->GetEntries();
  unsigned int nEventsChain = nEvents;
  cout << "[ZllLooper::loop] running on " << nEventsChain << " events" << endl;
  unsigned int nEventsTotal = 0;
  TObjArray *listOfFiles = chain->GetListOfFiles();
  TIter fileIter(listOfFiles);
  TFile *currentFile = 0;
  while ( (currentFile = (TFile*)fileIter.Next()) ) {
    cout << "[ZllLooper::loop] running on file: " << currentFile->GetTitle() << endl;

    // Get File Content
    TFile f( currentFile->GetTitle() );
    TTree *tree = (TTree*)f.Get("mt2");
    TTreeCache::SetLearnEntries(10);
    tree->SetCacheSize(128*1024*1024);
    //mt2tree t(tree);
    t.Init(tree);

    // Event Loop
    unsigned int nEventsTree = tree->GetEntriesFast();
    for( unsigned int event = 0; event < nEventsTree; ++event) {
    //    for( unsigned int event = 0; event < nEventsTree/10.; ++event) {
    //for( unsigned int event = 0; event < 100; ++event) { 

      t.GetEntry(event);

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
      }
 
      //---------------------
      // skip duplicates -- will need this eventually
      //---------------------
      //if( isData ) {
      //  DorkyEventIdentifier id = {stopt.run(), stopt.event(), stopt.lumi() };
      //  if (is_duplicate(id, already_seen) ){
      //    continue;
      //  }
      //}

      //---------------------
      // set weights and start making plots
      //---------------------
      outfile_->cd();
      const float lumi = 1.;
      evtweight_ = t.evt_scale1fb * lumi;

      //---------------------
      // Generator level plots to classify samples and find Z, l, g
      //---------------------
      float genVphi;
      float genVeta;
      float posLepPt;
      float posLepEta;
      float posLepPhi;
      float negLepPt;
      float negLepEta;
      float negLepPhi;
      bool ismu = false;
      bool isel = false;
      bool istau = false;
      bool isZ = false;
      bool isG = false;
      bool isZnn = false;
      partonHT_ = 0;
      for (int i = 0; i < t.ngenPart; i++) {
	//	if (t.genPart_pdgId[i]!=23 && t.genPart_pdgId[i]!=22 && t.genPart_status[i]==23) { 
	if ((fabs(t.genPart_pdgId[i])<10 || t.genPart_pdgId[i]==21) && (t.genPart_status[i]==23 || t.genPart_status[i]==22)) { 
	  // sum final state quarks and gluons
	  partonHT_ += t.genPart_pt[i];
	}
	if (  t.evt_id >= 700 && t.evt_id < 800 ) { // dilepton
	  if (t.genPart_pdgId[i]==23 && t.genPart_status[i]==22)  {	    
	    genVpt_  = t.genPart_pt[i];
	    genVphi = t.genPart_phi[i];
	    genVeta = t.genPart_eta[i];
	  }
	  float pdgId = t.genPart_pdgId[i];
	  float momId =  t.genPart_motherId[i];
	  bool thisIsMuOrE = false;
	  if (fabs(pdgId) == 13 && momId==23) { ismu=true; thisIsMuOrE = true; }
	  if (fabs(pdgId) == 11 && momId==23) { isel=true; thisIsMuOrE = true; }
	  if (fabs(pdgId) == 15 && momId==23) istau=true;
	  if (pdgId<0 && thisIsMuOrE) { posLepPt = t.genPart_pt[i]; posLepEta = t.genPart_eta[i]; posLepPhi = t.genPart_phi[i]; }
	  if (pdgId>0 && thisIsMuOrE) { negLepPt = t.genPart_pt[i]; negLepEta = t.genPart_eta[i]; negLepPhi = t.genPart_phi[i]; }
	  
	  isZ=true;
	}
	else if (  t.evt_id >= 200 && t.evt_id < 300 ) { // gamma
	  if (t.genPart_pdgId[i]==22 && t.genPart_status[i]==23) {
	    genVpt_  = t.genPart_pt[i];
	    genVphi = t.genPart_phi[i];
	    genVeta = t.genPart_eta[i];
	  }
	  isG=true;
	}
	else if (  t.evt_id >= 600 && t.evt_id < 700 ) { // Z invisible
	  if (t.genPart_pdgId[i]==23 && t.genPart_status[i]==22)  {	    
	    genVpt_  = t.genPart_pt[i];
	    genVphi = t.genPart_phi[i];
	    genVeta = t.genPart_eta[i];
	  }
	  isZ=true;
	  isZnn=true;
	}	
      }
      if (!isG && !isZ) continue; // this should never happen
      if (genVpt_ < 60) continue; // require this to match gamma+jets samples


      //---------------------
      // Plots for Acceptance and lepton efficiency
      //---------------------
      // Denominator: e+mu, Zpt >60
      // Numerator: Denom, lepPt>20, lepEta<2.5
      if (isZ && !istau && !isZnn) {

	float lep1pt = posLepPt > negLepPt ? posLepPt : negLepPt;
	float lep1eta = posLepPt > negLepPt ? posLepEta : negLepEta;
	float lep2pt = posLepPt > negLepPt ? negLepPt : posLepPt;
	float lep2eta = posLepPt > negLepPt ? negLepEta : posLepEta;
	plot1D("h_Z_lep1pt",   lep1pt, evtweight_, h_1d_global, "", 100, 0, 1000);
	plot1D("h_Z_lep2pt",   lep2pt, evtweight_, h_1d_global, "", 100, 0, 1000);
	plot1D("h_Z_lep1eta",   lep1eta, evtweight_, h_1d_global, "", 50, -5, 5);
	plot1D("h_Z_lep2eta",   lep2eta, evtweight_, h_1d_global, "", 50, -5, 5);
	plot2D("h__Z_lep1ptVeta",  lep1pt, lep1eta, evtweight_, h_2d, "",  100, 0, 1000, 50, -5, 5);
	plot2D("h__Z_lep2ptVeta",  lep2pt, lep1eta, evtweight_, h_2d, "",  100, 0, 1000, 50, -5, 5);

	plot1D("h_AccDen_genVpt",   genVpt_, evtweight_, h_1d_global, "", 200, 0, 2000);
	if ( posLepPt > 20 && negLepPt > 20 && fabs(posLepEta)<2.5 && fabs(negLepEta)<2.5) { // In the fiducial region!
	  plot1D("h_AccNum_genVpt",   genVpt_, evtweight_, h_1d_global, "", 200, 0, 2000);

	  // Lepton efficiency (include charge, since we will request it)
	  std::string type = isel ? "el" : (ismu ? "mu" : "");
	  plot2D("h_LepEffDen_"+type, (posLepPt), posLepEta, evtweight_, h_2d, "", nptbins, ptbins, netabins, etabins);
	  bool foundIt = false;
	  for (int i = 0; i < t.nlep; i++) {
	    if (t.lep_charge[i] < 0) continue; // match with positive
	    float thisDR = DeltaR(t.lep_eta[i], posLepEta, t.lep_phi[i], posLepPhi);
	    if (thisDR<0.1) foundIt = true;
	  }
	  if (foundIt) plot2D("h_LepEffNum_"+type,  (posLepPt), posLepEta, evtweight_, h_2d, "", nptbins, ptbins, netabins, etabins);
	  // again for the other lepton
	  plot2D("h_LepEffDen_"+type, (negLepPt), negLepEta, evtweight_, h_2d, "", nptbins, ptbins, netabins, etabins);
	  foundIt = false;
	  for (int i = 0; i < t.nlep; i++) {
	    if (t.lep_charge[i] > 0) continue; // match with negative
	    float thisDR = DeltaR(t.lep_eta[i], negLepEta, t.lep_phi[i], negLepPhi);
	    if (thisDR<0.1) foundIt = true;
	  }
	  if (foundIt) plot2D("h_LepEffNum_"+type, (negLepPt), negLepEta, evtweight_, h_2d, "", nptbins, ptbins, netabins, etabins);

	  // Gen level efficiency of the invariant mass cut
	  TLorentzVector l0(0,0,0,0);
	  TLorentzVector l1(0,0,0,0);
	  l0.SetPtEtaPhiM(posLepPt, posLepEta, posLepPhi, 0);
	  l1.SetPtEtaPhiM(negLepPt, negLepEta, negLepPhi, 0);
	  TLorentzVector Dilep = l0+l1;
	  float genZmass = Dilep.M();
	  plot1D("h_GenInvMassEffDen_genVpt",   genVpt_, evtweight_, h_1d_global, "", 200, 0, 2000);
	  if ( fabs(genZmass - 90) < 20 ) 
	    plot1D("h_GenInvMassEffNum_genVpt",   genVpt_, evtweight_, h_1d_global, "", 200, 0, 2000);

	  // Dilepton efficiency
	  plot1D("h_GenDilepEffDen_genVpt",   genVpt_, evtweight_, h_1d_global, "", 200, 0, 2000);
	  if (t.nlep==2) {
	    if (t.lep_charge[0] * t.lep_charge[1] == -1 && abs(t.lep_pdgId[0]) == abs(t.lep_pdgId[1])) { // OS SF
	      plot1D("h_GenDilepEffNum_genVpt",   genVpt_, evtweight_, h_1d_global, "", 200, 0, 2000);
	    }
	  }
	  // How does dilepton efficiency compare with the Eff1*Eff2 ?
	  // Basically we want to compare the dilepton efficiency (above) with the efficiency using as "numerator" the Den*EffLep1*EffLep2
	  if (makeEffAccWeights) {
	    int PtEtaBin0 = h_2d_global["LepEff_mu"]->FindBin((lep1pt),  lep1eta);
	    int PtEtaBin1 = h_2d_global["LepEff_mu"]->FindBin((lep2pt),  lep2eta);
	    float lepEff0 = 1;
	    float lepEff1 = 1;
	    if (ismu) lepEff0 = h_2d_global["LepEff_mu"]->GetBinContent(PtEtaBin0);
	    else lepEff0 = h_2d_global["LepEff_el"]->GetBinContent(PtEtaBin0);
	    if (ismu) lepEff1 = h_2d_global["LepEff_mu"]->GetBinContent(PtEtaBin1);
	    else lepEff1 = h_2d_global["LepEff_el"]->GetBinContent(PtEtaBin1);				  
	    float EffEff = lepEff0 * lepEff1;
	    plot1D("h_GenDilepEffNum_FromLepEff1LepEff2_genVpt",   genVpt_, evtweight_ * EffEff, h_1d_global, "", 200, 0, 2000);
	  } 

	  // Efficiency of the invariant mass cut
	  if (t.nlep==2) {
	    if (t.lep_charge[0] * t.lep_charge[1] == -1 && abs(t.lep_pdgId[0]) == abs(t.lep_pdgId[1])) { // OS SF
	      plot1D("h_InvMassEffDen_genVpt",   genVpt_, evtweight_, h_1d_global, "", 200, 0, 2000);
	      if ( fabs(t.zll_invmass - 90) < 20 ) 
		plot1D("h_InvMassEffNum_genVpt",   genVpt_, evtweight_, h_1d_global, "", 200, 0, 2000);
	    }
	  }
	} // Leaving the fiducial region
      } // End denominator


      //---------------------
      // Adapting Zll and Gamma+Jets events to look like Z invisible (Truth Level)
      //---------------------
      float metPt  = t.met_genPt;
      plot1D("h_truth_met_emutau",       metPt,       evtweight_, h_1d_global, "", 200, 0, 1500);	
      if (istau) plot1D("h_truth_met_tau",       metPt,       evtweight_, h_1d_global, "", 200, 0, 1500);	
      else if (!isZnn) { // taus ruin MET. they're also not part of our Z-->ll
	
	// PS This could be an issue when Z-->tau-->mu give rise to real MET with no gamma+jets equivalent
	
	////// Recalculate MET including the vector boson pT //////
	TVector2 newMet;
	TVector2 genV;
	newMet.SetMagPhi(metPt, t.met_genPhi);
	genV.SetMagPhi(genVpt_, genVphi);
	newMet += genV;
	
	plot1D("h_truth_Vpt",       genVpt_,       evtweight_, h_1d_global, "", 400, 0, 2000);
	plot1D("h_truth_Veta",      genVeta,      evtweight_, h_1d_global, "", 100, -4, 4);
	plot1D("h_truth_met",       metPt,        evtweight_, h_1d_global, "", 200, 0, 1500);
	plot1D("h_truth_newmet",    newMet.Mod(), evtweight_, h_1d_global, "", 200, 0, 1500);
	plot1D("h_truth_partonHT",   partonHT_, evtweight_, h_1d_global, "", 120, 0, 3000);
	
	////// Recalculate HT with overlap removal ///////
	HTdrMinV_ = 0;
	HTdr04V_  = 0;
	float stdHt = 0;
	float minDr = 5;
	float minDrP = 5;
	float minDrN = 5;
	float secondClosestDr = 5;
	int minDrIdx = -1;
	int minDrIdxP = -1;
	int minDrIdxN = -1;
	float thisDRP = 6;
	float thisDRN = 6;
	bool recoLepP = false;
	bool recoLepN = false;
	if (t.nlep==2) {recoLepN = true; recoLepP = true;}
	if (t.nlep==1) {
	  if (t.lep_charge[0]<0) recoLepN = true;
	  if (t.lep_charge[0]>0) recoLepP = true;
	}
	
	if (printHtCalc) {
	  cout<<"---- New Event "<<t.evt<<" ---"<<endl;
	  cout<<"Truth leptons have pt/eta/phi: "<<posLepPt<<"/"<<posLepEta<<"/"<<posLepPhi<<", "<<negLepPt<<"/"<<negLepEta<<"/"<<negLepPhi<<endl;
	  cout<<"Reco leptons ("<< t.nlep <<") have pt/eta/phi: ";
	  for (int i = 0; i < t.nlep; i++) cout<<t.lep_pt[i]<<"/"<<t.lep_eta[i]<<"/"<<t.lep_phi[i]<<", ";
	  cout<<endl;
	  cout<<"Reco jets (after o.r. with "<< t.nlep <<" reco leptons): ";
	}
	for (int i = 0; i < t.njet; i++) {
	  //if (t.jet_pt[i] < 40) break; // pt ordered jets, stop looping
	  if (fabs(t.jet_eta[i]) > 2.5 ) continue;
	  
	  float thisDR = DeltaR(t.jet_eta[i], genVeta, t.jet_phi[i], genVphi);
	  if (thisDR < minDr) { secondClosestDr = minDr; minDr = thisDR; minDrIdx = i; }
	  if (thisDR < secondClosestDr && thisDR > minDr) secondClosestDr = thisDR;
	  
	  if (isZ) {
	    thisDRP = DeltaR(t.jet_eta[i], posLepEta, t.jet_phi[i], posLepPhi);
	    if (thisDRP < minDrP) { minDrP = thisDRP; minDrIdxP = i; }
	    thisDRN = DeltaR(t.jet_eta[i], negLepEta, t.jet_phi[i], negLepPhi);
	    if (thisDRN < minDrN) { minDrN = thisDRN; minDrIdxN = i; }
	  }
	  if (t.jet_pt[i] > 40) {
	    if (thisDR > 0.4 && (recoLepP || thisDRP > 0.4 ) && (recoLepN || thisDRN > 0.4)) HTdr04V_ += t.jet_pt[i];
	    stdHt += t.jet_pt[i];
	    if (printHtCalc) cout<<"("<<t.jet_pt[i]<<", "<<t.jet_eta[i]<<", "<<t.jet_phi[i]<<"), ";
	  }
	}
	if (printHtCalc) {
	  cout<<" making up a reco HT of "<<stdHt<<endl;
	  // Subtract closest jets to leptons from standard HT, unless they are reco leptons (in that case jets are already subtracted)
	  cout<<"Closest jet to PosLepton (reco "<< recoLepP <<") has minDrP "<<minDrP<<" and pt/eta/phi "<<t.jet_pt[minDrIdxP]<<", "<<t.jet_eta[minDrIdxP]<<", "<<t.jet_phi[minDrIdxP]<<endl;
	  cout<<"Closest jet to NegLepton (reco "<< recoLepN <<") has minDrN "<<minDrN<<" and pt/eta/phi "<<t.jet_pt[minDrIdxN]<<", "<<t.jet_eta[minDrIdxN]<<", "<<t.jet_phi[minDrIdxN]<<endl;
	}
	int alreadyRemovedP = -1;
	int alreadyRemovedN = -1;
	if (!recoLepP && minDrP<0.4 && t.jet_pt[minDrIdxP]>40) 
	  { stdHt = stdHt - t.jet_pt[minDrIdxP]; alreadyRemovedP = minDrIdxP; }
	if (!recoLepN && minDrN<0.4 && minDrIdxN != alreadyRemovedP && t.jet_pt[minDrIdxN]>40) 
	  { stdHt = stdHt - t.jet_pt[minDrIdxN]; alreadyRemovedN = minDrIdxN; }
	
	if (printHtCalc) cout<<"After gen lepton removal, HT is "<<stdHt<<endl;
	// Subtract closest jets to vector bosons 
	HTdrMinV_ = stdHt;
	if (minDr<0.4 && t.jet_pt[minDrIdx]>40 && minDrIdx != alreadyRemovedP && minDrIdx != alreadyRemovedN)   
	  HTdrMinV_ = HTdrMinV_ - t.jet_pt[minDrIdx];
	if (printHtCalc) cout<<"After removing vector boson, reco HT is "<< HTdrMinV_<<endl;
	
	// DR boson jet
	plot1D("h_DRJetBoson",         minDr,   evtweight_, h_1d_global, "", 100, 0, 5);
	if (genVpt_ < 100)      plot1D("h_DRJetBoson_VptLT100" ,         minDr,   evtweight_,  h_1d_global, "", 100, 0, 5);
	else if (genVpt_ < 200) plot1D("h_DRJetBoson_Vpt100200",         minDr,   evtweight_,  h_1d_global, "", 100, 0, 5);
	else if (genVpt_ < 400) plot1D("h_DRJetBoson_Vpt200400",         minDr,   evtweight_,  h_1d_global, "", 100, 0, 5);
	else if (genVpt_ < 600) plot1D("h_DRJetBoson_Vpt400600",         minDr,   evtweight_,  h_1d_global, "", 100, 0, 5);
	else if (genVpt_ < 800) plot1D("h_DRJetBoson_Vpt600800",         minDr,   evtweight_,  h_1d_global, "", 100, 0, 5);
	else if (genVpt_ <1000) plot1D("h_DRJetBoson_Vpt8001000",         minDr,   evtweight_, h_1d_global, "", 100, 0, 5);
	else                    plot1D("h_DRJetBoson_VptGT1000" ,         minDr,   evtweight_, h_1d_global, "", 100, 0, 5);
	plot1D("h_DRJet2Boson",  secondClosestDr,   evtweight_, h_1d_global, "", 100, 0, 5);
	if (genVpt_ < 100)      plot1D("h_DRJet2Boson_VptLT100" ,   secondClosestDr,   evtweight_,  h_1d_global, "", 100, 0, 5);
	else if (genVpt_ < 200) plot1D("h_DRJet2Boson_Vpt100200",   secondClosestDr,   evtweight_,  h_1d_global, "", 100, 0, 5);
	else if (genVpt_ < 400) plot1D("h_DRJet2Boson_Vpt200400",   secondClosestDr,   evtweight_,  h_1d_global, "", 100, 0, 5);
	else if (genVpt_ < 600) plot1D("h_DRJet2Boson_Vpt400600",   secondClosestDr,   evtweight_,  h_1d_global, "", 100, 0, 5);
	else if (genVpt_ < 800) plot1D("h_DRJet2Boson_Vpt600800",   secondClosestDr,   evtweight_,  h_1d_global, "", 100, 0, 5);
	else if (genVpt_ <1000) plot1D("h_DRJet2Boson_Vpt8001000",   secondClosestDr,   evtweight_, h_1d_global, "", 100, 0, 5);
	else                    plot1D("h_DRJet2Boson_VptGT1000" ,   secondClosestDr,   evtweight_, h_1d_global, "", 100, 0, 5);
	if (minDr < 0.4) { 
	  float deltaPt = (t.jet_pt[minDrIdx] - genVpt_)/genVpt_;
	  plot1D("h_DPtJetBoson",  deltaPt,   evtweight_, h_1d_global, "", 100, 0, 5);
	  if (genVpt_ < 100)      plot1D("h_DPtJetBoson_VptLT100" ,   deltaPt,   evtweight_,  h_1d_global, "", 100, -3, 3);
	  else if (genVpt_ < 200) plot1D("h_DPtJetBoson_Vpt100200",   deltaPt,   evtweight_,  h_1d_global, "", 100, -3, 3);
	  else if (genVpt_ < 400) plot1D("h_DPtJetBoson_Vpt200400",   deltaPt,   evtweight_,  h_1d_global, "", 100, -3, 3);
	  else if (genVpt_ < 600) plot1D("h_DPtJetBoson_Vpt400600",   deltaPt,   evtweight_,  h_1d_global, "", 100, -3, 3);
	  else if (genVpt_ < 800) plot1D("h_DPtJetBoson_Vpt600800",   deltaPt,   evtweight_,  h_1d_global, "", 100, -3, 3);
	  else if (genVpt_ <1000) plot1D("h_DPtJetBoson_Vpt8001000",   deltaPt,   evtweight_, h_1d_global, "", 100, -3, 3);
	  else                    plot1D("h_DPtJetBoson_VptGT1000" ,   deltaPt,   evtweight_, h_1d_global, "", 100, -3, 3);
	}
	plot1D("h_truth_recoHT",    stdHt, evtweight_, h_1d_global, "", 120, 0, 3000); // remove closest jets to lepton ...
	plot1D("h_truth_recoHTdr04V",   HTdr04V_ , evtweight_, h_1d_global, "", 120, 0, 3000); // ... and all in DR04 with leptons/V
	plot1D("h_truth_recoHTdrMinV",  HTdrMinV_, evtweight_, h_1d_global, "", 120, 0, 3000); // ... and only closest (in DR04) to V
	
	//if (fabs(t.ht - stdHt)> 10) cout<<"event "<<t.evt<<" has ht "<<t.ht<<" and stdHt "<<stdHt<<endl;
	//	plot2D("h_DeltaHTvsHT")
	
//	  ////// Calculate Gen HT with overlap removal ///////	  
//	  minDr = 5;
//	  minDrP = 5;
//	  minDrN = 5;
//	  minDrIdx = -1;
//	  minDrIdxP = -1;
//	  minDrIdxN = -1;
//	  float GenHTdr04V_;
//	  float GenHTdrMinV_;
//	  float stdGenHt;
//	  for (int i = 0; i < t.ngenjet; i++) {
//	    if (fabs(t.genjet_eta[i]) > 2.5 ) continue;
//	    
//	    float thisDR = DeltaR(t.genjet_eta[i], genVeta, t.genjet_phi[i], genVphi);
//	    if (thisDR < minDr) { minDr = thisDR; minDrIdx = i; }
//	    
//	    if (isZ) {
//	      thisDRP = DeltaR(t.genjet_eta[i], posLepEta, t.genjet_phi[i], posLepPhi);
//	      if (thisDRP < minDrP) { minDrP = thisDRP; minDrIdxP = i; }
//	      thisDRN = DeltaR(t.genjet_eta[i], negLepEta, t.genjet_phi[i], negLepPhi);
//	      if (thisDRN < minDrN) { minDrN = thisDRN; minDrIdxN = i; }
//	    }
//	    if (t.jet_pt[i] > 40) {
//	      if (thisDR > 0.4 && thisDRP > 0.4 && thisDRN > 0.4) GenHTdr04V_ += t.jet_pt[i];
//	      stdGenHt += t.jet_pt[i];
//	    }
//	  }
//	  // Remove closest genjets to leptons
//	  int alreadyRemovedP = -1;
//	  int alreadyRemovedN = -1;
//	  if ( minDrP<0.4 && t.jet_pt[minDrIdxP]>40) 
//	    { stdGenHt = stdGenHt - t.genjet_pt[minDrIdxP]; alreadyRemovedP = minDrIdxP; }
//	  if ( minDrN<0.4 && minDrIdxN != alreadyRemovedP && t.jet_pt[minDrIdxN]>40) 
//	    { stdGenHt = stdGenHt - t.genjet_pt[minDrIdxN]; alreadyRemovedN = minDrIdxN; }
//	  // Subtract closest genjets to vector bosons 
//	  GenHTdrMinV_ = stdGenHt;
//	  if (minDr<0.4 && t.jet_pt[minDrIdx]>40 && minDrIdx != alreadyRemovedP && minDrIdx != alreadyRemovedN)   
//	    GenHTdrMinV_ = GenHTdrMinV_ - t.genjet_pt[minDrIdx];

	
      }
      
      //      continue; // Exit after truth plots


      //---------------------
      // Move to Reco level
      //---------------------

      if (!PassesEventSelection(t.nVert)) continue;
      
      plot1D("h_nvtx",       t.nVert,       evtweight_, h_1d_global, "", 80, 0, 80);
      plot1D("h_mt2raw",       t.mt2,       evtweight_, h_1d_global, "", 80, 0, 800);
      plot1D("h_zll_mt2",       t.zll_mt2,       evtweight_, h_1d_global, "", 80, 0, 800);
      plot1D("h_gamma_mt2",       t.gamma_mt2,       evtweight_, h_1d_global, "", 80, 0, 800);
      
      nlepveto_ = t.nMuons10 + t.nElectrons10 + t.nTaus20;
      //cout <<"t.met_pt, t.ht, t.nJet40, t.nBJet40, t.deltaPhiMin, t.diffMetMht, nlepveto_, t.jet_pt[0], t.jet_pt[1]"<<endl;
      //cout<<t.met_pt<<" "<<t.ht<<" "<<t.nJet40<<" "<<t.nBJet40<<" "<<t.deltaPhiMin<<" "<<t.diffMetMht<<" "<<nlepveto_<<" "<<t.jet_pt[0]<<" "<<t.jet_pt[1]<<endl;
      
      //---------------------
      // Adapting Zll and Gamma+Jets events to look like Z invisible (Reco Level)
      // Also simply running on Z invisible
      //---------------------
      //______________________________
      // Select Z-->ll or gamma+jets
      if ( t.nlep == 2 && t.evt_id >= 700 && t.evt_id < 800 ) { // dilepton
	//	if (t.ht < 400) continue; // require at least 100 HT apart from the Z
	plot1D("h_lepCharge",    t.lep_charge[0], evtweight_, h_1d_global, "", 3, -1.5, 1.5);
	plot1D("h_lepCharge",    t.lep_charge[1], evtweight_, h_1d_global, "", 3, -1.5, 1.5);
	plot1D("h_lepChargeMult",    t.lep_charge[0] * t.lep_charge[1], evtweight_, h_1d_global, "", 3, -1.5, 1.5);
	plot1D("h_lepPdgId", t.lep_pdgId[0]  , evtweight_, h_1d_global, "", 20, 0, 20);
	plot1D("h_lepPdgId", t.lep_pdgId[1]  , evtweight_, h_1d_global, "", 20, 0, 20);
	if ( t.lep_charge[0] * t.lep_charge[1] != -1 ) continue; // OS
	if ( abs(t.lep_pdgId[0]) != abs(t.lep_pdgId[1])) continue; // SF (will also look at e-mu pairs later)
	plot1D("h_zllInvMass", t.zll_invmass  , evtweight_, h_1d_global, "", 200, 0, 200);
	if ( fabs(t.zll_invmass - 90) > 20 ) continue; // 70-110 GeV mass
	if (printHtCalc) cout<<"!! Event "<<t.evt<<" passes dilepton reco requirements !!"<<endl;
	TLorentzVector l0(0,0,0,0);
	TLorentzVector l1(0,0,0,0);
	l0.SetPtEtaPhiM(t.lep_pt[0], t.lep_eta[0], t.lep_phi[0], t.lep_mass[0]);
	l1.SetPtEtaPhiM(t.lep_pt[1], t.lep_eta[1], t.lep_phi[1], t.lep_mass[1]);
	TLorentzVector Dilep = l0+l1;
	Zinv_pt_ = Dilep.Pt();
	mt2_Zinv_	  =  t.zll_mt2;
	nJet40_Zinv_	  =  t.nJet40;
	nBJet40_Zinv_     =  t.nBJet40;
	ht_Zinv_	  =  t.ht;
	deltaPhiMin_Zinv_ =  t.zll_deltaPhiMin;
	diffMetMht_Zinv_  =  t.zll_diffMetMht;
	mht_pt_Zinv_	  =  t.zll_mht_pt;
	mht_phi_Zinv_     =  t.zll_mht_phi;
	met_pt_Zinv_	  =  t.zll_met_pt;
	met_phi_Zinv_     =  t.zll_met_phi;   
	nlepveto_ = nlepveto_ - 2; // apply veto on third lepton
	jetIdx0_          =  0;
	jetIdx1_          =  1;

	// Modified weight to estimate Z-->nn content
	if (makeEffAccWeights) {
	  evtweightZnn_ = evtweight_;

	  int PtEtaBin0 = h_2d_global["LepEff_mu"]->FindBin((t.lep_pt[0]),  t.lep_eta[0]);
	  int PtEtaBin1 = h_2d_global["LepEff_mu"]->FindBin((t.lep_pt[1]),  t.lep_eta[1]);
	  int GenVptBin = h_1d_global["Acc"]->FindBin(Zinv_pt_);
	  float lepEff0 = 1;
	  float lepEff1 = 1;
	  float lepAcc = 1;
	  float invMassEff = 1;
	  if (fabs(t.lep_pdgId[0])==13) lepEff0 = h_2d_global["LepEff_mu"]->GetBinContent(PtEtaBin0);
	  else lepEff0 = h_2d_global["LepEff_el"]->GetBinContent(PtEtaBin0);
	  if (fabs(t.lep_pdgId[1])==13) lepEff1 = h_2d_global["LepEff_mu"]->GetBinContent(PtEtaBin1);
	  else lepEff1 = h_2d_global["LepEff_el"]->GetBinContent(PtEtaBin1);				  
	  lepAcc = h_1d_global["Acc"]->GetBinContent(GenVptBin);
	  invMassEff =h_1d_global["InvMassEff"]->GetBinContent(GenVptBin);
	  float EffAcc = lepEff0 * lepEff1 * lepAcc * invMassEff;
	  if (EffAcc<0.0001) {
	    evtweightZnn_ = 0; // Watch out for large weights or divisions by 0...
	    //cout<<evtweight_ <<" "<<evtweightZnn_<<endl;	  
	  }
	  else evtweightZnn_ = evtweight_ * 3. / EffAcc ; // *1.5 to include taus, *2 to go from leptons to neutrinos
	} 
      }
      else if ( t.nGammas20 > 0 && t.nlep==0  && t.evt_id >= 200 && t.evt_id < 300 ) { // gamma
	//	if (t.gamma_ht < 400) continue; // require at least 100 HT apart from the photon
	
	if ( DeltaR(genVeta, t.gamma_eta[0], genVphi, t.gamma_phi[0]) >  0.1) continue; // truth-match the photon
	mt2_Zinv_	  =  t.gamma_mt2;
	nJet40_Zinv_	  =  t.gamma_nJet40;
	nBJet40_Zinv_     =  t.gamma_nBJet40;
	ht_Zinv_	  =  t.gamma_ht;
	deltaPhiMin_Zinv_ =  t.gamma_deltaPhiMin;
	diffMetMht_Zinv_  =  t.gamma_diffMetMht;
	mht_pt_Zinv_	  =  t.gamma_mht_pt;
	mht_phi_Zinv_     =  t.gamma_mht_phi;
	met_pt_Zinv_	  =  t.gamma_met_pt;
	met_phi_Zinv_     =  t.gamma_met_phi;   
	Zinv_pt_          =  t.gamma_pt[0];

	// Redefine leading 2 jets after overlap removal with gamma
	float minDR = 0.4;
	int gammaJet = -1;
	for (int i = 0; i < t.njet; i++) {
	  float thisDR = DeltaR(t.jet_eta[i], t.gamma_eta[0], t.jet_phi[i], t.gamma_phi[0]);
          if(thisDR < minDR){
            minDR = thisDR; 
	    gammaJet = i;
          }
	}

	jetIdx0_ = 0;
	jetIdx1_ = 1;
	if (gammaJet==0) { jetIdx0_++; jetIdx1_++;}
	if (gammaJet==1) { jetIdx1_++;}
	
	
	// Still need to require MET < 100 to reduce signal contamination 

      }
      else if ( t.nlep==0  && t.evt_id >= 600 && t.evt_id < 700 ) { // Z invisible
	//	if (t.gamma_ht < 400) continue; // require at least 100 HT apart from the photon
	
	mt2_Zinv_	  =  t.mt2;
	nJet40_Zinv_	  =  t.nJet40;
	nBJet40_Zinv_     =  t.nBJet40;
	ht_Zinv_	  =  t.ht;
	deltaPhiMin_Zinv_ =  t.deltaPhiMin;
	diffMetMht_Zinv_  =  t.diffMetMht;
	mht_pt_Zinv_	  =  t.mht_pt;
	mht_phi_Zinv_     =  t.mht_phi;
	met_pt_Zinv_	  =  t.met_pt;
	met_phi_Zinv_     =  t.met_phi;   
	Zinv_pt_          =  0;
	jetIdx0_          =  0;
	jetIdx1_          =  1;
      }
      else continue; // Skip event if it is not a Z-->ll or Gamma+jets event

      //if (Zinv_pt_ < 70) continue; // Gamma+jets have a Generator cut at 60 GeV GammaPt

      fillHistos(h_1d_nocut, "nocut");

      fillHistosSignalRegion(h_1d_srH, SignalRegionJets::nocut, SignalRegionHtMet::h_ht, "srH");
      fillHistosSignalRegion(h_1d_srM, SignalRegionJets::nocut, SignalRegionHtMet::m_ht, "srM");
      fillHistosSignalRegion(h_1d_srL, SignalRegionJets::nocut, SignalRegionHtMet::l_ht, "srL");
      fillHistosSignalRegion(h_1d_sr1H, SignalRegionJets::sr1, SignalRegionHtMet::h_ht, "sr1H");
      fillHistosSignalRegion(h_1d_sr2H, SignalRegionJets::sr2, SignalRegionHtMet::h_ht, "sr2H");
      fillHistosSignalRegion(h_1d_sr3H, SignalRegionJets::sr3, SignalRegionHtMet::h_ht, "sr3H");
      fillHistosSignalRegion(h_1d_sr4H, SignalRegionJets::sr4, SignalRegionHtMet::h_ht, "sr4H");
      fillHistosSignalRegion(h_1d_sr5H, SignalRegionJets::sr5, SignalRegionHtMet::h_ht, "sr5H");
      fillHistosSignalRegion(h_1d_sr6H, SignalRegionJets::sr6, SignalRegionHtMet::h_ht, "sr6H");
      fillHistosSignalRegion(h_1d_sr7H, SignalRegionJets::sr7, SignalRegionHtMet::h_ht, "sr7H");
      fillHistosSignalRegion(h_1d_sr8H, SignalRegionJets::sr8, SignalRegionHtMet::h_ht, "sr8H");
      fillHistosSignalRegion(h_1d_sr9H, SignalRegionJets::sr9, SignalRegionHtMet::h_ht, "sr9H");
      fillHistosSignalRegion(h_1d_sr1M, SignalRegionJets::sr1, SignalRegionHtMet::m_ht, "sr1M");
      fillHistosSignalRegion(h_1d_sr2M, SignalRegionJets::sr2, SignalRegionHtMet::m_ht, "sr2M");
      fillHistosSignalRegion(h_1d_sr3M, SignalRegionJets::sr3, SignalRegionHtMet::m_ht, "sr3M");
      fillHistosSignalRegion(h_1d_sr4M, SignalRegionJets::sr4, SignalRegionHtMet::m_ht, "sr4M");
      fillHistosSignalRegion(h_1d_sr5M, SignalRegionJets::sr5, SignalRegionHtMet::m_ht, "sr5M");
      fillHistosSignalRegion(h_1d_sr6M, SignalRegionJets::sr6, SignalRegionHtMet::m_ht, "sr6M");
      fillHistosSignalRegion(h_1d_sr7M, SignalRegionJets::sr7, SignalRegionHtMet::m_ht, "sr7M");
      fillHistosSignalRegion(h_1d_sr8M, SignalRegionJets::sr8, SignalRegionHtMet::m_ht, "sr8M");
      fillHistosSignalRegion(h_1d_sr9M, SignalRegionJets::sr9, SignalRegionHtMet::m_ht, "sr9M");
      fillHistosSignalRegion(h_1d_sr1L, SignalRegionJets::sr1, SignalRegionHtMet::l_ht, "sr1L");
      fillHistosSignalRegion(h_1d_sr2L, SignalRegionJets::sr2, SignalRegionHtMet::l_ht, "sr2L");
      fillHistosSignalRegion(h_1d_sr3L, SignalRegionJets::sr3, SignalRegionHtMet::l_ht, "sr3L");
      fillHistosSignalRegion(h_1d_sr4L, SignalRegionJets::sr4, SignalRegionHtMet::l_ht, "sr4L");
      fillHistosSignalRegion(h_1d_sr5L, SignalRegionJets::sr5, SignalRegionHtMet::l_ht, "sr5L");
      fillHistosSignalRegion(h_1d_sr6L, SignalRegionJets::sr6, SignalRegionHtMet::l_ht, "sr6L");
      fillHistosSignalRegion(h_1d_sr7L, SignalRegionJets::sr7, SignalRegionHtMet::l_ht, "sr7L");
      fillHistosSignalRegion(h_1d_sr8L, SignalRegionJets::sr8, SignalRegionHtMet::l_ht, "sr8L");
      fillHistosSignalRegion(h_1d_sr9L, SignalRegionJets::sr9, SignalRegionHtMet::l_ht, "sr9L");

   }//end loop on events in a file
  
    delete tree;
    f.Close();
  }//end loop on files
  
  cout << "[ZllLooper::loop] processed " << nEventsTotal << " events" << endl;
  if ( nEventsChain != nEventsTotal ) {
    std::cout << "ERROR: number of events from files is not equal to total number of events" << std::endl;
  }

  cout << nDuplicates << " duplicate events were skipped." << endl;

  //---------------------
  // Save Plots
  //---------------------

  outfile_->cd();
  savePlotsDir(h_1d_global,outfile_,"");
  savePlots2Dir(h_2d_global,outfile_,"");
  savePlots2Dir(h_2d,outfile_,"");
  savePlotsDir(h_1d_nocut,outfile_,"nocut");
  savePlotsDir(h_1d_srH,outfile_,"srH");
  savePlotsDir(h_1d_srM,outfile_,"srM");
  savePlotsDir(h_1d_srL,outfile_,"srL");
  savePlotsDir(h_1d_sr1H,outfile_,"sr1H");
  savePlotsDir(h_1d_sr2H,outfile_,"sr2H");
  savePlotsDir(h_1d_sr3H,outfile_,"sr3H");
  savePlotsDir(h_1d_sr4H,outfile_,"sr4H");
  savePlotsDir(h_1d_sr5H,outfile_,"sr5H");
  savePlotsDir(h_1d_sr6H,outfile_,"sr6H");
  savePlotsDir(h_1d_sr7H,outfile_,"sr7H");
  savePlotsDir(h_1d_sr8H,outfile_,"sr8H");
  savePlotsDir(h_1d_sr9H,outfile_,"sr9H");
  savePlotsDir(h_1d_sr1M,outfile_,"sr1M");
  savePlotsDir(h_1d_sr2M,outfile_,"sr2M");
  savePlotsDir(h_1d_sr3M,outfile_,"sr3M");
  savePlotsDir(h_1d_sr4M,outfile_,"sr4M");
  savePlotsDir(h_1d_sr5M,outfile_,"sr5M");
  savePlotsDir(h_1d_sr6M,outfile_,"sr6M");
  savePlotsDir(h_1d_sr7M,outfile_,"sr7M");
  savePlotsDir(h_1d_sr8M,outfile_,"sr8M");
  savePlotsDir(h_1d_sr9M,outfile_,"sr9M");
  savePlotsDir(h_1d_sr1L,outfile_,"sr1L");
  savePlotsDir(h_1d_sr2L,outfile_,"sr2L");
  savePlotsDir(h_1d_sr3L,outfile_,"sr3L");
  savePlotsDir(h_1d_sr4L,outfile_,"sr4L");
  savePlotsDir(h_1d_sr5L,outfile_,"sr5L");
  savePlotsDir(h_1d_sr6L,outfile_,"sr6L");
  savePlotsDir(h_1d_sr7L,outfile_,"sr7L");
  savePlotsDir(h_1d_sr8L,outfile_,"sr8L");
  savePlotsDir(h_1d_sr9L,outfile_,"sr9L");


  //---------------------
  // Write and Close file
  //---------------------
  outfile_->Write();
  outfile_->Close();
  delete outfile_;

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

void ZllLooper::fillHistosSignalRegion(std::map<std::string, TH1D*>& h_1d, const SignalRegionJets::value_type& sr_jets, const SignalRegionHtMet::value_type& sr_htmet, const std::string& dir, const std::string& suffix) {
  
 if ( PassesSignalRegion(met_pt_Zinv_, ht_Zinv_, nJet40_Zinv_, nBJet40_Zinv_, deltaPhiMin_Zinv_, diffMetMht_Zinv_, 
			 nlepveto_, t.jet_pt[jetIdx0_], t.jet_pt[jetIdx1_], sr_jets, sr_htmet) ) 
   {
     plot1D("h_SignalRegion",  sr_jets+sr_htmet,   evtweight_, h_1d_global, "", 100, 0, 100);
     plot1D("h_SignalRegion_wZll",  sr_jets+sr_htmet,   evtweightZnn_, h_1d_global, "", 100, 0, 100);

     fillHistos( h_1d, dir, suffix);
   }
 return;
}

void ZllLooper::fillHistos(std::map<std::string, TH1D*>& h_1d, const std::string& dirname, const std::string& s) {
  TDirectory * dir = (TDirectory*)outfile_->Get(dirname.c_str());
  if (dir == 0) {
    dir = outfile_->mkdir(dirname.c_str());
  } 
  dir->cd();

  plot1D("h_Events"+s,  1, 1, h_1d, "", 1, 0, 2);
  plot1D("h_Events_w"+s,  1,   evtweight_, h_1d, "", 1, 0, 2);
  plot1D("h_Events_wZll"+s,  1,   evtweightZnn_, h_1d, "", 1, 0, 2);
  plot1D("h_mt2raw"+s,    t.mt2,   evtweight_, h_1d, "", 150, 0, 1500);
  plot1D("h_mt2"+s,       mt2_Zinv_,   evtweight_, h_1d, "", 150, 0, 1500);
  plot1D("h_mt2_wZll"+s,       mt2_Zinv_,   evtweightZnn_, h_1d, "", 150, 0, 1500);
  plot1D("h_metraw"+s,      t.met_pt,   evtweight_, h_1d, "", 150, 0, 1500);
  plot1D("h_met"+s,       met_pt_Zinv_,   evtweight_, h_1d, "", 200, 0, 1500);
  plot1D("h_met_wZll"+s,       met_pt_Zinv_,   evtweightZnn_, h_1d, "", 200, 0, 1500);
  plot1D("h_ht"+s,        ht_Zinv_,   evtweight_, h_1d, "", 120, 0, 3000);
  plot1D("h_ht_wZll"+s,        ht_Zinv_,   evtweightZnn_, h_1d, "", 120, 0, 3000);
  plot1D("h_htraw"+s,     t.ht,   evtweight_, h_1d, "", 120, 0, 3000);
  plot1D("h_partonHT"+s,     partonHT_,   evtweight_, h_1d, "", 120, 0, 3000);
  plot1D("h_HTdr04V",   HTdr04V_ , evtweight_, h_1d, "", 120, 0, 3000);
  plot1D("h_HTdrMinV",  HTdrMinV_, evtweight_, h_1d, "", 120, 0, 3000);
  plot1D("h_nJet40"+s,       nJet40_Zinv_,   evtweight_, h_1d, "", 15, 0, 15);
  plot1D("h_nBJet40"+s,      nBJet40_Zinv_,   evtweight_, h_1d, "", 6, 0, 6);
  plot1D("h_deltaPhiMin"+s,  deltaPhiMin_Zinv_,   evtweight_, h_1d, "", 32, 0, 3.2);
  plot1D("h_diffMetMht"+s,   diffMetMht_Zinv_,   evtweight_, h_1d, "", 120, 0, 300);
  plot1D("h_nlepveto"+s,     nlepveto_,   evtweight_, h_1d, "", 10, 0, 10);
  plot1D("h_J0pt"+s,         t.jet_pt[jetIdx0_],   evtweight_, h_1d, "", 200, 0, 500);
  plot1D("h_J1pt"+s,         t.jet_pt[jetIdx1_],   evtweight_, h_1d, "", 200, 0, 500);
  plot1D("h_Vpt"+s,          Zinv_pt_,   evtweight_, h_1d, "", 400, 0, 2000);
  plot1D("h_truth_Vpt",       genVpt_,   evtweight_, h_1d, "", 400, 0, 2000);
  plot1D("h_truth_Vpt_wZll",       genVpt_,   evtweightZnn_, h_1d, "", 400, 0, 2000);
  
  outfile_->cd();
  return;
}
