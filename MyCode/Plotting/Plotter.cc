// C++
#include <iostream>
#include <vector>
#include <set>
#include <cmath>
#include <sstream>
#include <stdexcept>
#include <algorithm>

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
#include "TCanvas.h"
#include "TFile.h"
#include "TLegend.h"
#include "THStack.h"

//#include "RooRealVar.h"
//#include "RooDataSet.h"

// Tools
#include "../../CORE/Tools/utils.h"

using namespace std;

bool compare_by_integral (const pair<string,TH1* >& a, const pair<string,TH1*>& b)
{
  float a_int = a.second->Integral();
  float b_int = b.second->Integral();

  return a_int < b_int;
}

int main ()
{
  vector<string> file_list;
  file_list.push_back("dyjets");
  file_list.push_back("qcd"); 
  file_list.push_back("singletop");
  file_list.push_back("ttdl");
  file_list.push_back("ttg");
  file_list.push_back("tth");
  file_list.push_back("ttsl");
  file_list.push_back("tttt");
  file_list.push_back("ttw");
  file_list.push_back("ttz");
  file_list.push_back("twz");
  file_list.push_back("wjets");
  file_list.push_back("ww");
  file_list.push_back("zinv");

  TCanvas * c1 = new TCanvas();
  c1->SetLogy();
  c1->cd();

  vector< pair<string,TH1*> > hist_list;

  for (vector<string>::iterator it = file_list.begin(); it != file_list.end(); ++it)
    {
      string file_name = *it;
      TFile * current_file = TFile::Open( ("../output/Unskimmed/"+file_name+".root").c_str() );

      TIter next(current_file->GetList()); 
      TObject *obj; 
      string name;
      while ((obj=next())) 
	{ 
	  if (obj->InheritsFrom("TH1")) 
	    { 
	      TH1 *h = (TH1*)obj; 
	      hist_list.push_back(pair<string,TH1*>(file_name,h));
	      h->SetFillColor(kRed);
	      h->Draw("hist");
	      h->Draw("Esame");
	      name = h->GetName();
	      c1->Print((file_name+"_"+name+".pdf").c_str());
	    } 
	}
    }

  sort(hist_list.begin(), hist_list.end(), compare_by_integral );

  THStack *hs_mt2_nocut = new THStack("hs_mt2_nocut", "Stacked Initial M_{T2}");
  THStack *hs_mt2_nm1 = new THStack("hs_mt2_nm1", "Stacked M_{T2}, All Other Cuts");
  THStack *hs_mt2_allcut = new THStack("hs_mt2_allcut", "Stacked Final M_{T2}");

  THStack *hs_ht_nocut = new THStack("hs_ht_nocut", "Stacked Initial H_{T}");
  THStack *hs_ht_nm1 = new THStack("hs_ht_nm1", "Stacked Initial H_{T}");
  THStack *hs_ht_allcut = new THStack("hs_ht_allcut", "Stacked Initial H_{T}");

  Double_t x1 = 0.9, x2 = x1+0.1, y1 = 0.75, y2 = y1+0.2;
  TLegend *mt2_nc_leg = new TLegend(x1, x2, y1, y2);
  TLegend *mt2_nm1_leg = new TLegend(x1, x2, y1, y2);
  TLegend *mt2_ac_leg = new TLegend(x1, x2, y1, y2);
  TLegend *ht_nc_leg = new TLegend(x1, x2, y1, y2);
  TLegend *ht_nm1_leg = new TLegend(x1, x2, y1, y2);
  TLegend *ht_ac_leg = new TLegend(x1, x2, y1, y2);
  

  for (vector<pair<string, TH1* > >::iterator it = hist_list.begin(); it < hist_list.end(); ++it)
    {
      TH1* h = (*it).second;
      const string name = h->GetName();
      const char * source_file = (*it).first.c_str();
      if ( name.compare("h_mt2_nocut") == 0)
	{
	  hs_mt2_nocut->Add(h);
	  mt2_nc_leg->AddEntry(h,source_file);
	}
      else if ( name.compare("h_mt2_nm1") == 0)
	{
	  hs_mt2_nm1->Add(h);
	  mt2_nm1_leg->AddEntry(h,source_file);
	}
      else if ( name.compare("h_mt2_allcut") == 0)
	{
	  hs_mt2_allcut->Add(h);
	  mt2_ac_leg->AddEntry(h, source_file);
	}

    } 
  hs_mt2_nocut->Draw("hist");
  mt2_nc_leg->Draw();
  c1->Print("stacked_mt2_nocut.pdf");
  
  hs_mt2_nm1->Draw("hist");
  mt2_nm1_leg->Draw();
  c1->Print("stacked_mt2_nm1.pdf");

  hs_mt2_allcut->Draw("hist");
  mt2_ac_leg->Draw();
  c1->Print("stacked_mt2_allcut.pdf");
}

