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
#include "TH1.h"
#include "TH2.h"

//#include "RooRealVar.h"
//#include "RooDataSet.h"

// Tools
#include "../CORE/Tools/utils.h"
#include "../CORE/Tools/goodrun.h"
#include "../CORE/Tools/dorky/dorky.h"
#include "../CORE/Tools/badEventFilter.h"

#include "../MT2CORE/mt2tree.h"

#include "Skimmer.h"

using namespace std;
//using namespace mt2;
using namespace duplicate_removal;

void PlotLoop (const TChain& chain, const string& output_file_name)
{
  // first make absolutely sure we're not overwriting an input file like an idiot
  if (output_file_name.find("userdata") != string::npos) 
    {
      cout << "Output file looks dangerous." << endl;
      return;
    }
  
  // Prepare input files and tree for loop
  int nEvents = chain.GetEntries();
  unsigned int nMax = nEvents;
  
  /*
   * Book Histograms
   */

  TH1::SetDefaultSumw2(true);

  int n_mt2_bins = 50;
  float mt2_min = 0;
  float mt2_max = 1000;
  int n_ht_bins = 200;
  float ht_min = 0;
  float ht_max = 4000;
  int n_dphi_bins = 25;
  float dphi_min = 0;
  float dphi_max = 3.2;
  int n_diff_bins = 150;
  float diff_min = 0;
  float diff_max = 1000;
  int n_dom_bins = 10;
  float dom_min = 0;
  float dom_max_nc = 600;
  float dom_max_nm1 = 1;
  float dom_max_ac = 1;
  int n_njet_bins = 12;
  float njet_min = 0;
  float njet_max = 12;
  int n_met_bins = 100;
  float met_min = 0;
  float met_max = 2000;
  int n_bjet_bins = 7;
  float bjet_min = 0;
  float bjet_max = 7;
  int n_nlep_bins = 6;
  float nlep_min = 0;
  float nlep_max = 6;

  stringstream mt2_bin_stream; 
  mt2_bin_stream << (mt2_max / (float) n_mt2_bins) << " GeV";
  string mt2_bin = mt2_bin_stream.str();
  stringstream ht_bin_stream;
  ht_bin_stream << (ht_max / (float) n_ht_bins) << " GeV";
  string ht_bin = ht_bin_stream.str();
  stringstream dphi_bin_stream;
  dphi_bin_stream << (dphi_max / (float) n_dphi_bins) << " rad";
  string dphi_bin = dphi_bin_stream.str();
  stringstream diff_bin_stream;
  diff_bin_stream << (diff_max / (float) n_diff_bins) << " GeV";
  string diff_bin = diff_bin_stream.str();
  stringstream dom_bin_nc_stream;
  dom_bin_nc_stream << (dom_max_nc / (float) n_dom_bins);
  string dom_bin_nc = dom_bin_nc_stream.str();
  stringstream dom_bin_nm1_stream;
  dom_bin_nm1_stream << (dom_max_nm1 / (float) n_dom_bins);
  string dom_bin_nm1 = dom_bin_nm1_stream.str();
  stringstream dom_bin_ac_stream;
  dom_bin_ac_stream << (dom_max_ac / (float) n_dom_bins);
  string dom_bin_ac = dom_bin_ac_stream.str();

  stringstream met_ht_bin_stream;
  met_ht_bin_stream << (met_max / (float) n_met_bins) << " GeV x " << ht_bin;
  string met_ht_bin = met_ht_bin_stream.str();
  stringstream diff_dom_bin_nc_stream;
  diff_dom_bin_nc_stream << dom_bin_nc << " x " << diff_bin;
  string diff_dom_bin_nc = diff_dom_bin_nc_stream.str();
  stringstream diff_dom_bin_nm1_stream;
  diff_dom_bin_nm1_stream << dom_bin_nm1 << " x " << diff_bin;
  string diff_dom_bin_nm1 = diff_dom_bin_nm1_stream.str();
  stringstream diff_dom_bin_ac_stream;
  diff_dom_bin_ac_stream << dom_bin_ac << " x " << diff_bin;
  string diff_dom_bin_ac = diff_dom_bin_ac_stream.str();

  // 1D Histograms

  // NO CUTS
  // MT2
  TH1F h_mt2_nocut("h_mt2_nocut", ("Initial M_{T2} Distribution;M_{T2} (GeV);Count / " + mt2_bin).c_str(), n_mt2_bins, mt2_min, mt2_max);
  // ht
  TH1F h_ht_nocut("h_ht_nocut", ("Initial H_{T} Distribution;H_{T} (GeV);Count / " + ht_bin).c_str(), n_ht_bins, ht_min, ht_max);
  // deltaPhiMin
  TH1F h_dphi_nocut("h_dphi_nocut", ("Initial #Delta#phi_{min} Distribution;#Delta#phi_{min} (rad);Count / " + dphi_bin).c_str(), n_dphi_bins, dphi_min, dphi_max);
  // |MHT-MET|
  TH1F h_diff_nocut("h_diff_nocut", ("Initial |MHT-MET| Distribution;|MHT-MET| (GeV);Count / " + diff_bin).c_str(), n_diff_bins, diff_min, diff_max);
  // |MHT-MET|/MHT
  TH1F h_dom_nocut("h_dom_nocut", ("Initial |MHT-MET|/MET Distribution;|MHT-MET|/MET;Count / " + dom_bin_nc).c_str(), n_dom_bins, dom_min, dom_max_nc);
  // njet
  TH1F h_njet_nocut("h_njet_nocut", "Initial N-Jet Distribution;N_{jet};Count", n_njet_bins, njet_min, njet_max);
  // bjet
  TH1F h_bjet_nocut("h_bjet_nocut", "Initial B-Jet Distribution;N_{bjet};Count", n_bjet_bins, bjet_min, bjet_max);
  // Nlep
  TH1F h_nlep_nocut("h_nlep_nocut", "Initial Lepton Veto Index Distribution;Lepton Veto Index;Count", n_nlep_bins, nlep_min, nlep_max);
  // N light lep
  TH1F h_nll_nocut("h_nll_nocut", "Initial Light Leptons Distribution;Number of Light Leptons;Count", n_nlep_bins, nlep_min, nlep_max);
  // N lowMT lep
  TH1F h_lowmt_nocut("h_lowmt_nocut", "Initial nPFLep5LowMT Distribution;nPFLep5LowMT;Count", n_nlep_bins, nlep_min, nlep_max);

  // Trigger Only
  TH1F h_mt2_trigger("h_mt2_trigger", ("Post-Trigger M_{T2} Distribution;M_{T2} (GeV);Count / " + mt2_bin).c_str(), n_mt2_bins, mt2_min, mt2_max);
  TH1F h_ht_trigger("h_ht_trigger", ("Post-Trigger H_{T} Distribution;H_{T} (GeV);Count / " + ht_bin).c_str(), n_ht_bins, ht_min, ht_max);
  TH1F h_dphi_trigger("h_dphi_trigger", ("Post-Trigger #Delta#phi_{min} Distribution;#Delta#phi_{min} (rad);Count / " + dphi_bin).c_str(), n_dphi_bins, dphi_min, dphi_max);
  TH1F h_diff_trigger("h_diff_trigger", ("Post-Trigger |MHT-MET| Distribution;|MHT-MET| (GeV);Count / " + diff_bin).c_str(), n_diff_bins, diff_min, diff_max);
  TH1F h_dom_trigger("h_dom_trigger", ("Post-Trigger |MHT-MET|/MET Distribution;|MHT-MET|/MET;Count / " + dom_bin_nc).c_str(), n_dom_bins, dom_min, dom_max_nc);
  TH1F h_njet_trigger("h_njet_trigger", "Post-Trigger N-Jet Distribution;N_{jet};Count", n_njet_bins, njet_min, njet_max);
  TH1F h_bjet_trigger("h_bjet_trigger", "Post-Trigger B-Jet Distribution;N_{bjet};Count", n_bjet_bins, bjet_min, bjet_max);
  TH1F h_nlep_trigger("h_nlep_trigger", "Post-Trigger Lepton Veto Index Distribution;Lepton Veto Index;Count", n_nlep_bins, nlep_min, nlep_max);
  TH1F h_nll_trigger("h_nll_trigger", "Post-Trigger Light Leptons Distribution;Number of Light Leptons;Count", n_nlep_bins, nlep_min, nlep_max);
  TH1F h_lowmt_trigger("h_lowmt_trigger", "Post-Trigger nPFLep5LowMT Distribution;nPFLep5LowMT;Count", n_nlep_bins, nlep_min, nlep_max);

  // N-1 Cuts
  TH1F h_mt2_nm1("h_mt2_nm1", ("M_{T2} Distribution, All Other Cuts Applied;M_{T2} (GeV);Count / " + mt2_bin).c_str(), n_mt2_bins, mt2_min, mt2_max);
  TH1F h_dphi_nm1("h_dphi_nm1", ("#Delta#phi_{min} Distribution, All Other Cuts Applied;#Delta#phi_{min} (rad);Count / " + dphi_bin).c_str(), n_dphi_bins, dphi_min, dphi_max);
  TH1F h_diff_nm1("h_diff_nm1", ("|MHT-MET| Distribution, All Other Cuts Applied;|MHT-MET| (GeV);Count / " + diff_bin).c_str(), n_diff_bins, diff_min, diff_max);
  TH1F h_dom_nm1("h_dom_nm1", ("|MHT-MET|/MET Distribution All Other Cuts Applied;|MHT-MET|/MET;Count / " + dom_bin_nm1).c_str(), n_dom_bins, dom_min, dom_max_nm1);
  TH1F h_njet_nm1("h_njet_nm1", "N_{J} Distribution, All Other Cuts Applied;N_{jet};Count", n_njet_bins, njet_min, njet_max);
  TH1F h_nlep_nm1("h_nlep_nm1", "Lepton Veto Index Distribution, All Other Cuts Applied;Lepton Veto Index;Count", n_nlep_bins, nlep_min, nlep_max);
  TH1F h_nll_nm1("h_nll_nm1", "Light Lepton Distribution, All Other Cuts Applied;Number of Light Lepton;Count", n_nlep_bins, nlep_min, nlep_max);
  TH1F h_lowmt_nm1("h_lowmt_nm1", "nPFLep5LowMT Distribution, All Other Cuts Applied;nPFLep5LowMT;Count", n_nlep_bins, nlep_min, nlep_max);

  // All Cuts
  TH1F h_mt2_allcut("h_mt2_allcut", ("Final M_{T2} Distribution;M_{T2} (GeV);Count / " + mt2_bin).c_str(), n_mt2_bins, mt2_min, mt2_max);
  TH1F h_ht_allcut("h_ht_allcut", ("Final H_{T} Distribution;H_{T} (GeV);Count / " + ht_bin).c_str(), n_ht_bins, ht_min, ht_max);
  TH1F h_dphi_allcut("h_dphi_allcut", ("Final #Delta#phi_{min} Distribution;#Delta#phi_{min} (rad);Count / " + dphi_bin).c_str(), n_dphi_bins, dphi_min, dphi_max);
  TH1F h_diff_allcut("h_diff_allcut", ("Final |MHT-MET| Distribution;|MHT-MET| (GeV);Count / " + diff_bin).c_str(), n_diff_bins, diff_min, diff_max);
  TH1F h_dom_allcut("h_dom_allcut", ("Final |MHT-MET|/MET Distribution;|MHT-MET|/MET;Count / " + dom_bin_ac).c_str(), n_dom_bins, dom_min, dom_max_ac);
  TH1F h_njet_allcut("h_njet_allcut", "Final N-Jet Distribution;N_{jet};Count", n_njet_bins, njet_min, njet_max);
  TH1F h_bjet_allcut("h_bjet_allcut", "Final B-Jet Distribution;N_{bjet};Count", n_bjet_bins, bjet_min, bjet_max);  



  // 2D Histograms

  // No Cuts
  // HT x MET
  TH2F h_htmet_nocut("h_htmet_nocut", ("Initial METxH_{T} Distribution;H_{T} (GeV);MET (GeV);Count / " + met_ht_bin).c_str(), n_ht_bins, ht_min, ht_max, n_met_bins, met_min, met_max);
  // nj x bj
  TH2F h_njbj_nocut("h_njbj_nocut", "Initial N_{b}xN_{J} Distribution;N_{jet};N_{bjet};Count", n_njet_bins, njet_min, njet_max, n_bjet_bins, bjet_min, bjet_max);
  // |MHT-MET| x MET
  TH2F h_diffdom_nocut("h_diffdom_nocut", ("Initial METx|MHT-MET| Distribution;|MHT-MET| (GeV);|MHT-MET|/MET; Counts / " + diff_dom_bin_nc).c_str(), n_diff_bins, diff_min, diff_max, n_dom_bins, dom_min, dom_max_nc);
  // MT2 x NJET
  TH2F h_njmt2_nocut("h_njmt2_nocut", ("Initial M_{T2}xN_{J} Distribution;N_{jet};M_{T2} (GeV);Count / " + mt2_bin).c_str(), n_njet_bins, njet_min, njet_max, n_mt2_bins, mt2_min, mt2_max);

  // Trigger
  TH2F h_htmet_trigger("h_htmet_trigger", ("Post-Trigger METxH_{T} Distribution;H_{T} (GeV);MET (GeV);Count / " + met_ht_bin).c_str(), n_ht_bins, ht_min, ht_max, n_met_bins, met_min, met_max);
  TH2F h_njbj_trigger("h_njbj_trigger", "Post-Trigger N_{b}xN_{J} Distribution;N_{jet};N_{bjet};Count", n_njet_bins, njet_min, njet_max, n_bjet_bins, bjet_min, bjet_max);
  TH2F h_diffdom_trigger("h_diffdom_trigger", ("Post-Trigger METx|MHT-MET| Distribution;|MHT-MET| (GeV);|MHT-MET|/MET; Counts / " + diff_dom_bin_nc).c_str(), n_diff_bins, diff_min, diff_max, n_dom_bins, dom_min, dom_max_nc);
  TH2F h_njmt2_trigger("h_njmt2_trigger", ("Post-Trigger M_{T2}xN_{J} Distribution;N_{jet};M_{T2} (GeV);Count / " + mt2_bin).c_str(), n_njet_bins, njet_min, njet_max, n_mt2_bins, mt2_min, mt2_max);

  // N-1 Cuts
  TH2F h_htmet_nm1("h_htmet_nm1", ("METxH_{T} Distribution, All Other Cuts Applied;H_{T} (GeV);MET (GeV);Count / " + met_ht_bin).c_str(), n_ht_bins, ht_min, ht_max, n_met_bins, met_min, met_max);
  TH2F h_njbj_nm1("h_njbj_nm1", "N_{b}xN_{J} Distribution, All Other Cuts Applied;N_{jet};N_{bjet};Count", n_njet_bins, njet_min, njet_max, n_bjet_bins, bjet_min, bjet_max);
  TH2F h_diffdom_nm1("h_diffdom_nm1", ("METx|MHT-MET| Distribution, All Other Cuts Applied;|MHT-MET| (GeV);|MHT-MET|/MET;Count / " + diff_dom_bin_nm1).c_str(), n_diff_bins, diff_min, diff_max, n_dom_bins, dom_min, dom_max_nc);
  TH2F h_njmt2_nm1("h_njmt2_nm1", ("M_{T2}xN_{J} Distribution, All Other Cuts Applied;N_{jet};M_{T2} (GeV);Count / " + mt2_bin).c_str(), n_njet_bins, njet_min, njet_max, n_mt2_bins, mt2_min, mt2_max);

  // All cuts
  TH2F h_htmet_allcut("h_htmet_allcut", ("Final METxH_{T} Distribution;H_{T} (GeV);MET (GeV);Count / " + met_ht_bin).c_str(), n_ht_bins, ht_min, ht_max, n_met_bins, met_min, met_max);
  TH2F h_njbj_allcut("h_njbj_allcut", "Final N_{b}xN_{J} Distribution;N_{jet};N_{bjet};Count", n_njet_bins, njet_min, njet_max, n_bjet_bins, bjet_min, bjet_max);
  TH2F h_diffdom_allcut("h_diffdom_allcut", ("Final METx|MHT-MET| Distribution;|MHT-MET| (GeV);|MHT-MET|/MET;Count / " + diff_dom_bin_ac).c_str(), n_diff_bins, diff_min, diff_max, n_dom_bins, dom_min, dom_max_nc);
  TH2F h_njmt2_allcut("h_njmt2_allcut", ("Final M_{T2}xN_{J} Distribution;N_{jet};M_{T2} (GeV);Count / " + mt2_bin).c_str(), n_njet_bins, njet_min, njet_max, n_mt2_bins, mt2_min, mt2_max);

  TTree *tree = (TTree*)chain.Clone("mt2");
  mt2tree mt2_tree;
  mt2_tree.Init(tree); // Branch boilerplate etc.

  cout << "Running on " << nMax << " events" << endl;
  
  for( unsigned int event = 0; event < nMax; event++)
    {
      if ( (event+1) % 10000 == 0) cout << "Processing event " << event+1 << ", " << (100. * event) / nMax << "% complete" << endl;
      
      mt2_tree.GetEntry(event);
      
      float w_ = 1000. * (mt2_tree.evt_xsec / mt2_tree.fChain->GetEntries()); // manual evt_scale1fb since signal scale1fb is broken
      float mt2_ = mt2_tree.mt2;
      float ht_ =  mt2_tree.ht;
      float dphi_ = mt2_tree.deltaPhiMin;
      float diff_ = mt2_tree.diffMetMht;
      float met_ = mt2_tree.met_pt;
      float dom_ = diff_/met_;
      int nj_ = mt2_tree.nJet30;
      int lep_veto_index_ = mt2_tree.nMuons10 + mt2_tree.nElectrons10 + mt2_tree.nPFLep5LowMT + /*tau*/mt2_tree.nPFHad10LowMT;
      int nleplowMT_ = mt2_tree.nLepLowMT;
      int nleplight_ = mt2_tree.nMuons10 + mt2_tree.nElectrons10;
      int nb_ = mt2_tree.nBJet20;

      // IMPLEMENT DATA LOGIC

      // start making plots

      // No Cuts
      h_mt2_nocut.Fill(mt2_, w_);
      h_ht_nocut.Fill(ht_, w_);
      h_dphi_nocut.Fill(dphi_, w_);
      h_diff_nocut.Fill(diff_, w_);
      h_dom_nocut.Fill(dom_, w_);
      h_njet_nocut.Fill(nj_, w_);
      h_bjet_nocut.Fill(nb_, w_);
      h_nlep_nocut.Fill(lep_veto_index_, w_);
      h_nll_nocut.Fill(nleplight_, w_);
      h_lowmt_nocut.Fill(nleplowMT_, w_);
      
      h_htmet_nocut.Fill(ht_, met_, w_);
      h_njbj_nocut.Fill(nj_, nb_, w_);
      h_diffdom_nocut.Fill(diff_, dom_, w_);
      h_njmt2_nocut.Fill(nj_, mt2_, w_);

      // Apply cuts
      bool PassTrigger = ( (ht_ > 1000 && met_ > 30) || (ht_ > 250 && met_ > 250) );
      bool PassNJ = ( nj_ > 0 );
      bool PassDphi = ( dphi_ > 0.3 );
      bool PassDom = ( dom_ < 0.5 );
      bool PassLep = ( lep_veto_index_ < 1 );
      bool PassMT2 = ( nj_ < 2 || mt2_ > 400 || (mt2_ > 200 && ht_ < 1500) );

      // All Cuts
      if (PassTrigger && PassNJ && PassDphi && PassDom && PassLep && PassMT2)
	{
	  h_mt2_allcut.Fill(mt2_, w_);
	  h_ht_allcut.Fill(ht_, w_);
	  h_dphi_allcut.Fill(dphi_, w_);
	  h_diff_allcut.Fill(diff_, w_);
	  h_dom_allcut.Fill(dom_, w_);
	  h_njet_allcut.Fill(nj_, w_);
	  h_bjet_allcut.Fill(nb_, w_);
	  
	  h_htmet_allcut.Fill(ht_, met_, w_);
	  h_njbj_allcut.Fill(nj_, nb_, w_);
	  h_diffdom_allcut.Fill(diff_, dom_, w_);
	  h_njmt2_allcut.Fill(nj_, mt2_, w_);	  
	}

      // Just Trigger
      if (PassTrigger)
	{
	  h_mt2_trigger.Fill(mt2_, w_);
	  h_ht_trigger.Fill(ht_, w_);
	  h_dphi_trigger.Fill(dphi_, w_);
	  h_diff_trigger.Fill(diff_, w_);
	  h_dom_trigger.Fill(dom_, w_);
	  h_njet_trigger.Fill(nj_, w_);
	  h_bjet_trigger.Fill(nb_, w_);
	  h_nlep_trigger.Fill(lep_veto_index_, w_);
	  h_nll_trigger.Fill(nleplight_, w_);
	  h_lowmt_trigger.Fill(nleplowMT_, w_);
	  
	  h_htmet_trigger.Fill(ht_, met_, w_);
	  h_njbj_trigger.Fill(nj_, nb_, w_);
	  h_diffdom_trigger.Fill(diff_, dom_, w_);
	  h_njmt2_trigger.Fill(nj_, mt2_, w_);
	}

      // N-1 Cuts
      
      // All but Trigger "cut"
      if (PassNJ && PassDphi && PassDom && PassLep && PassMT2)
	{
	  h_htmet_nm1.Fill(ht_, met_, w_);
	}
      
      // njet > 0
      if (PassTrigger && PassDphi && PassDom && PassLep && PassMT2)
	{
	  h_njet_nm1.Fill(nj_, w_);
	  h_njbj_nm1.Fill(nj_, nb_, w_);
	}
      
      // dphi > 0.3
      if (PassTrigger && PassNJ && PassDom && PassLep && PassMT2)
	{
	  h_dphi_nm1.Fill(dphi_, w_);
	}

      // |MHT-MET|/MET < 0.5
      if (PassTrigger && PassNJ && PassDphi && PassLep && PassMT2)
	{
	  h_diff_nm1.Fill(diff_, w_);
	  h_dom_nm1.Fill(dom_, w_);
	}


      // lepton veto
      if (PassTrigger && PassNJ && PassDphi && PassDom && PassMT2)
	{
	  h_nlep_nm1.Fill(lep_veto_index_, w_);
	  h_nll_nm1.Fill(nleplight_, w_);
	  h_lowmt_nm1.Fill(nleplowMT_, w_);
	}

      // mt2 cut for njet >=2 events
      if (PassTrigger && PassNJ && PassDphi && PassDom && PassLep)
	{
	  h_mt2_nm1.Fill(mt2_, w_);
	  h_njmt2_nm1.Fill(nj_, mt2_, w_);
	}
    }
  
  // save plots
  TFile outfile (("output/Unskimmed/"+output_file_name).c_str(),"RECREATE");

  h_mt2_nocut.Write();
  h_ht_nocut.Write();
  h_dphi_nocut.Write();
  h_diff_nocut.Write();
  h_dom_nocut.Write();
  h_njet_nocut.Write();
  h_bjet_nocut.Write();
  h_nlep_nocut.Write();
  h_njmt2_nocut.Write();
  h_diffdom_nocut.Write();
  h_htmet_nocut.Write();
  h_njbj_nocut.Write();
  h_nll_nocut.Write();
  h_lowmt_nocut.Write();

  h_mt2_trigger.Write();
  h_ht_trigger.Write();
  h_dphi_trigger.Write();
  h_diff_trigger.Write();
  h_dom_trigger.Write();
  h_njet_trigger.Write();
  h_bjet_trigger.Write();
  h_nlep_trigger.Write();
  h_njmt2_trigger.Write();
  h_diffdom_trigger.Write();
  h_htmet_trigger.Write();
  h_njbj_trigger.Write();
  h_nll_trigger.Write();
  h_lowmt_trigger.Write();

  h_mt2_nm1.Write();
  h_nlep_nm1.Write();
  h_dphi_nm1.Write();
  h_dom_nm1.Write();
  h_njet_nm1.Write();
  h_nlep_nm1.Write();
  h_diff_nm1.Write();
  h_njmt2_nm1.Write();
  h_diffdom_nm1.Write();
  h_htmet_nm1.Write();
  h_njbj_nm1.Write();
  h_nll_nm1.Write();
  h_lowmt_nm1.Write();

  h_mt2_allcut.Write();
  h_ht_allcut.Write();
  h_dphi_allcut.Write();
  h_diff_allcut.Write();
  h_dom_allcut.Write();
  h_njet_allcut.Write();
  h_bjet_allcut.Write();
  h_njmt2_allcut.Write();
  h_diffdom_allcut.Write();
  h_htmet_allcut.Write();
  h_njbj_allcut.Write();

  outfile.Close();
}
    

int main (const int argc, const char ** run_params) // first file is output, others are input
{
  if (argc > 1)
    {
      skim();
      return 1;
    }
  else
    { 
      TChain chain("mt2");
      string next_line;
      string output_file_name = "histos.root";
      bool input_given = false;
      while (getline(cin, next_line))
	{
	  if (next_line.find(".root") == string::npos) // output file name
	    {	  
	      output_file_name = next_line+".root";
	      cout << "Using " << output_file_name << " as output file name." << endl;
	    }
	  else
	    {
	      input_given = true;
	      chain.Add(next_line.c_str());
	      cout << "Added input file " << next_line << " to the TChain." << endl;
	  }
      }
      if (input_given)
	{
	  PlotLoop(chain,output_file_name);
	  return 1;
	}
      else
	{
	  cout << "List output file names without .root along with input file names including .root" << endl;
	  return 0;
	}
    }


}
