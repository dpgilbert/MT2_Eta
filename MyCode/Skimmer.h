#ifndef SKIMMER_h
#define SKIMMER_h

using namespace std;

void skim_low_met (TChain* chain, const string& sample);
void skim_SUSY_signal (TChain* chain, const string& sample);
void skim ();
void SeekMassPoints(TChain* chain, const string& sample);

#endif
