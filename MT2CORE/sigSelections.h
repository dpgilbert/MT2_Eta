#ifndef SIGNALREGIONS_H
#define SIGNALREGIONS_H

#include <string>
#include "TTree.h"
#include "SR.h"

namespace mt2
{

  std::vector<SR> getSignalRegions2012();
  std::vector<SR> getSignalRegions2015LowLumi();
  std::vector<SR> getSignalRegions2015ExtendedNJets();
  std::vector<SR> getSignalRegions2015ExtendedNJets_UltraHighHT();
  std::vector<SR> getSignalRegions2015SevenJets_UltraHighHT();
  std::vector<SR> getSignalRegionsZurich();
  std::vector<SR> getSignalRegionsZurich_jetpt40();
  std::vector<SR> getSignalRegionsZurich_jetpt30();
  std::vector<SR> getSignalRegionsJamboree();
  std::vector<SR> getSignalRegionsMonojet();
  std::vector<SR> getSignalRegionsICHEP();
  std::vector<SR> getSignalRegionsMonojet2016();
  std::vector<SR> getSignalRegions2016();
  
  std::vector<SR> getSignalRegions2016vs2017();
  std::vector<SR> getSignalRegionsMonojet2016vs2017();

  std::vector<SR> getSignalRegions2016Edit();

} // namespace mt2

#endif // SIGNALREGIONS_H
