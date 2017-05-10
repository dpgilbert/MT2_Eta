#include <fstream>
#include <sstream>
#include <iostream>
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>

#include "../MT2CORE/mt2tree.h"
#include "Skimmer.h"

using namespace std;

void skim_low_met (TChain* chain, const string& sample)
{
  TFile *outfile = TFile::Open( (sample+"_LowMETEvents.root").c_str() , "RECREATE");

  // events with MET < 1 GeV
  TTree* LowMETEvents = chain->CopyTree( "met_pt < 1.0" );
  
  LowMETEvents->Write();
  outfile->Close();
}

void skim_SUSY_signal (TChain* chain, const string& sample )
{  
  string header = "SkimmedSUSYSignal/"+sample;

  TFile *outfile1 = TFile::Open( (header+"_SUSYskimmed1.root").c_str() , "RECREATE");
  TFile *outfile2 = TFile::Open( (header+"_SUSYskimmed2.root").c_str() , "RECREATE");
  TFile *outfile3 = TFile::Open( (header+"_SUSYskimmed3.root").c_str(), "RECREATE");

  TTree* susy1 = chain->CopyTree( "( (GenSusyMScan1 == 1000) && ( GenSusyMScan2 == 900) ) " );
  TTree* susy2 = chain->CopyTree( "( (GenSusyMScan1 == 2200) && ( GenSusyMScan2 == 100) ) " );
  TTree* susy3 = chain->CopyTree( "( (GenSusyMScan1 == 1400) && ( GenSusyMScan2 == 1200) ) " );

  outfile1->cd();
  susy1->Write();
  outfile1->Close();

  outfile2->cd();
  susy2->Write();
  outfile2->Close();

  outfile3->cd();
  susy3->Write();
  outfile3->Close();
}

void skim ()
{
  cout << "skimming" << endl;
  TChain* chain = new TChain("mt2");
  string next_line;
  string output_file_name = "skimmed.root";
  bool input_given = false;
  while (getline(cin, next_line))
    {
      if (next_line.find(".root") == string::npos) // output file name
	{	  
	  output_file_name = next_line;
	  cout << "Using " << output_file_name << " as output file name." << endl;
	}
      else
	{
	  input_given = true;
	  chain->Add(next_line.c_str());
	  cout << "Added input file " << next_line << " to the TChain." << endl;
	}
    }
  if (input_given)
    {
      SeekMassPoints(chain, output_file_name);
      //skim_low_met(chain,output_file_name);
      //skim_SUSY_signal(chain,output_file_name);
      return;
    }
  else
    {
      cout << "List output file names without .root along with input file names including .root" << endl;
      return;
    }
}

void SeekMassPoints (TChain * chain, const string& sample)
{
  for (int m1 = 200; m1 < 3000; m1+=100)
    {
      for (int m2 = 100; m2 < m1; m2+=100)
	{
	  string selection = "( (GenSusyMScan1 == " + to_string(m1) + ") && ( GenSusyMScan2 == " + to_string(m2) + ") ) ";
	  if (chain->GetEntries(selection.c_str()) > 0)
	    {
	      cout << sample << " is positive for m1 = " << m1 << " and m2 = " << m2 << endl;
	    }
	}
    }
  return;
}
