#include "sigSelections.h"

namespace mt2 {

  std::vector<SR> getSignalRegions2012(){

    std::vector<SR> temp_SR_vec;
    std::vector<SR> SRVec;
    SR sr;
    SR baseSR;

    //first set binning in njet-nbjet plane
    sr.SetName("1");
    sr.SetVar("njets", 2, 3);
    sr.SetVar("nbjets", 0, 1);
    sr.SetVar("mt2", 200, -1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("2");
    sr.SetVar("njets", 2, 3);
    sr.SetVar("nbjets", 1, 3);
    sr.SetVar("mt2", 200, -1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("3");
    sr.SetVar("njets", 3, 6);
    sr.SetVar("nbjets", 0, 1);
    sr.SetVar("mt2", 200, 400);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("4");
    sr.SetVar("njets", 3, 6);
    sr.SetVar("nbjets", 1, 2);
    sr.SetVar("mt2", 200, -1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("5");
    sr.SetVar("njets", 3, 6);
    sr.SetVar("nbjets", 2, 3);
    sr.SetVar("mt2", 200, -1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("6");
    sr.SetVar("njets", 6, -1);
    sr.SetVar("nbjets", 0, 1);
    sr.SetVar("mt2", 200, -1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("7");
    sr.SetVar("njets", 6, -1);
    sr.SetVar("nbjets", 1, 2);
    sr.SetVar("mt2", 200, 400);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("8");
    sr.SetVar("njets", 6, -1);
    sr.SetVar("nbjets", 2, 3);
    sr.SetVar("mt2", 200, -1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("9");
    sr.SetVar("njets", 3, -1);
    sr.SetVar("nbjets", 3, -1);
    sr.SetVar("mt2", 200, 400);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    //add HT and MET requirements
    for(unsigned int iSR = 0; iSR < temp_SR_vec.size(); iSR++){
      SR fullSR = temp_SR_vec.at(iSR);  
      fullSR.SetName(fullSR.GetName() + "L");
      fullSR.SetVar("ht", 450, 750);
      fullSR.SetVar("met", 200, -1);
      SRVec.push_back(fullSR);
    }
    for(unsigned int iSR = 0; iSR < temp_SR_vec.size(); iSR++){
      SR fullSR = temp_SR_vec.at(iSR);  
      fullSR.SetName(fullSR.GetName() + "M");
      fullSR.SetVar("ht", 750, 1200);
      fullSR.SetVar("met", 30, -1);
      SRVec.push_back(fullSR);
    }
    for(unsigned int iSR = 0; iSR < temp_SR_vec.size(); iSR++){
      SR fullSR = temp_SR_vec.at(iSR);  
      fullSR.SetName(fullSR.GetName() + "H");
      fullSR.SetVar("ht", 1200, -1);
      fullSR.SetVar("met", 30, -1);
      SRVec.push_back(fullSR);
    }

    //define baseline selections commmon to all signal regions 
    //baseSR.SetVar("mt2", 200, -1);
    baseSR.SetVar("j1pt", 100, -1);
    baseSR.SetVar("j2pt", 100, -1);
    baseSR.SetVar("deltaPhiMin", 0.3, -1);
    baseSR.SetVar("diffMetMht", 0, 70);
    baseSR.SetVar("nlep", 0, 1);
    //baseSR.SetVar("passesHtMet", 1, 1);

    //add baseline selections to all signal regions 
    std::vector<std::string> vars = baseSR.GetListOfVariables();
    for(unsigned int i = 0; i < SRVec.size(); i++){
      for(unsigned int j = 0; j < vars.size(); j++){
        SRVec.at(i).SetVar(vars.at(j), baseSR.GetLowerBound(vars.at(j)), baseSR.GetUpperBound(vars.at(j)));
      }
    }

    return SRVec;

  }

  std::vector<SR> getSignalRegions2015LowLumi(){//used for AN-15-009

    std::vector<SR> temp_SR_vec;
    std::vector<SR> SRVec;
    SR sr;
    SR baseSR;

    //first set binning in njet-nbjet plane
    sr.SetName("1");
    sr.SetVar("njets", 2, 4);
    sr.SetVar("nbjets", 0, 1);
    sr.SetVar("lowMT", 0, -1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("2");
    sr.SetVar("njets", 2, 4);
    sr.SetVar("nbjets", 1, 2);
    sr.SetVar("lowMT", 0, -1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("3");
    sr.SetVar("njets", 2, 4);
    sr.SetVar("nbjets", 2, 3);
    sr.SetVar("lowMT", 1, 2);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("4");
    sr.SetVar("njets", 2, 4);
    sr.SetVar("nbjets", 2, 3);
    sr.SetVar("lowMT", 0, 1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("5");
    sr.SetVar("njets", 4, -1);
    sr.SetVar("nbjets", 0, 1);
    sr.SetVar("lowMT", 0, -1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("6");
    sr.SetVar("njets", 4, -1);
    sr.SetVar("nbjets", 1, 2);
    sr.SetVar("lowMT", 0, -1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("7");
    sr.SetVar("njets", 4, -1);
    sr.SetVar("nbjets", 2, 3);
    sr.SetVar("lowMT", 1, 2);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("8");
    sr.SetVar("njets", 4, -1);
    sr.SetVar("nbjets", 2, 3);
    sr.SetVar("lowMT", 0, 1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("9");
    sr.SetVar("njets", 2, -1);
    sr.SetVar("nbjets", 3, -1);
    sr.SetVar("lowMT", 1, 2);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("10");
    sr.SetVar("njets", 2, -1);
    sr.SetVar("nbjets", 3, -1);
    sr.SetVar("lowMT", 0, 1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    //add HT and MET requirements
    for(unsigned int iSR = 0; iSR < temp_SR_vec.size(); iSR++){
      SR fullSR = temp_SR_vec.at(iSR);  
      fullSR.SetName(fullSR.GetName() + "L");
      fullSR.SetVar("ht", 450, 575);
      fullSR.SetVar("met", 200, -1);
      SRVec.push_back(fullSR);
    }
    for(unsigned int iSR = 0; iSR < temp_SR_vec.size(); iSR++){
      SR fullSR = temp_SR_vec.at(iSR);  
      fullSR.SetName(fullSR.GetName() + "M");
      fullSR.SetVar("ht", 575, 1000);
      fullSR.SetVar("met", 200, -1);
      SRVec.push_back(fullSR);
    }
    for(unsigned int iSR = 0; iSR < temp_SR_vec.size(); iSR++){
      SR fullSR = temp_SR_vec.at(iSR);  
      fullSR.SetName(fullSR.GetName() + "H");
      fullSR.SetVar("ht", 1000, -1);
      fullSR.SetVar("met", 30, -1);
      SRVec.push_back(fullSR);
    }

    //define baseline selections commmon to all signal regions 
    baseSR.SetVar("mt2", 200, -1);
    baseSR.SetVar("j1pt", 100, -1);
    baseSR.SetVar("j2pt", 100, -1);
    baseSR.SetVar("deltaPhiMin", 0.3, -1);
    baseSR.SetVar("diffMetMhtOverMet", 0, 0.5);
    baseSR.SetVar("nlep", 0, 1);
    //baseSR.SetVar("passesHtMet", 1, 1);

    //add baseline selections to all signal regions 
    std::vector<std::string> vars = baseSR.GetListOfVariables();
    for(unsigned int i = 0; i < SRVec.size(); i++){
      for(unsigned int j = 0; j < vars.size(); j++){
        SRVec.at(i).SetVar(vars.at(j), baseSR.GetLowerBound(vars.at(j)), baseSR.GetUpperBound(vars.at(j)));
      }
    }

    return SRVec;

  }


  std::vector<SR> getSignalRegions2015ExtendedNJets(){

    std::vector<SR> temp_SR_vec;
    std::vector<SR> SRVec;
    SR sr;
    SR baseSR;

    //first set binning in njet-nbjet plane
    sr.SetName("1");
    sr.SetVar("njets", 2, 4);
    sr.SetVar("nbjets", 0, 1);
    sr.SetVar("lowMT", 0, -1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("2");
    sr.SetVar("njets", 2, 4);
    sr.SetVar("nbjets", 1, 2);
    sr.SetVar("lowMT", 0, -1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("3");
    sr.SetVar("njets", 2, 4);
    sr.SetVar("nbjets", 2, 3);
    sr.SetVar("lowMT", 1, 2);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("4");
    sr.SetVar("njets", 2, 4);
    sr.SetVar("nbjets", 2, 3);
    sr.SetVar("lowMT", 0, 1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("5");
    sr.SetVar("njets", 4, 7);
    sr.SetVar("nbjets", 0, 1);
    sr.SetVar("lowMT", 0, -1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("6");
    sr.SetVar("njets", 4, 7);
    sr.SetVar("nbjets", 1, 2);
    sr.SetVar("lowMT", 0, -1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("7");
    sr.SetVar("njets", 4, 7);
    sr.SetVar("nbjets", 2, 3);
    sr.SetVar("lowMT", 1, 2);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("8");
    sr.SetVar("njets", 4, 7);
    sr.SetVar("nbjets", 2, 3);
    sr.SetVar("lowMT", 0, 1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("9");
    sr.SetVar("njets", 7, 9);
    sr.SetVar("nbjets", 0, 1);
    sr.SetVar("lowMT", 0, -1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("10");
    sr.SetVar("njets", 7, 9);
    sr.SetVar("nbjets", 1, 2);
    sr.SetVar("lowMT", 0, -1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("11");
    sr.SetVar("njets", 7, 9);
    sr.SetVar("nbjets", 2, 3);
    sr.SetVar("lowMT", 1, 2);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("12");
    sr.SetVar("njets", 7, 9);
    sr.SetVar("nbjets", 2, 3);
    sr.SetVar("lowMT", 0, 1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("13");
    sr.SetVar("njets", 9, -1);
    sr.SetVar("nbjets", 0, 1);
    sr.SetVar("lowMT", 0, -1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("14");
    sr.SetVar("njets", 9, -1);
    sr.SetVar("nbjets", 1, 2);
    sr.SetVar("lowMT", 0, -1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("15");
    sr.SetVar("njets", 9, -1);
    sr.SetVar("nbjets", 2, 3);
    sr.SetVar("lowMT", 1, 2);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("16");
    sr.SetVar("njets", 9, -1);
    sr.SetVar("nbjets", 2, 3);
    sr.SetVar("lowMT", 0, 1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("17");
    sr.SetVar("njets", 2, 7);
    sr.SetVar("nbjets", 3, -1);
    sr.SetVar("lowMT", 1, 2);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("18");
    sr.SetVar("njets", 2, 7);
    sr.SetVar("nbjets", 3, -1);
    sr.SetVar("lowMT", 0, 1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("19");
    sr.SetVar("njets", 7, -1);
    sr.SetVar("nbjets", 3, -1);
    sr.SetVar("lowMT", 1, 2);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("20");
    sr.SetVar("njets", 7, -1);
    sr.SetVar("nbjets", 3, -1);
    sr.SetVar("lowMT", 0, 1);
    temp_SR_vec.push_back(sr);
    sr.Clear();


    //add HT and MET requirements
    for(unsigned int iSR = 0; iSR < temp_SR_vec.size(); iSR++){
      SR fullSR = temp_SR_vec.at(iSR);  
      fullSR.SetName(fullSR.GetName() + "L");
      fullSR.SetVar("ht", 450, 575);
      fullSR.SetVar("met", 200, -1);
      SRVec.push_back(fullSR);
    }
    for(unsigned int iSR = 0; iSR < temp_SR_vec.size(); iSR++){
      SR fullSR = temp_SR_vec.at(iSR);  
      fullSR.SetName(fullSR.GetName() + "M");
      fullSR.SetVar("ht", 575, 1000);
      fullSR.SetVar("met", 200, -1);
      SRVec.push_back(fullSR);
    }
    for(unsigned int iSR = 0; iSR < temp_SR_vec.size(); iSR++){
      SR fullSR = temp_SR_vec.at(iSR);  
      fullSR.SetName(fullSR.GetName() + "H");
      fullSR.SetVar("ht", 1000, -1);
      fullSR.SetVar("met", 30, -1);
      SRVec.push_back(fullSR);
    }

    //define baseline selections commmon to all signal regions 
    baseSR.SetVar("mt2", 200, -1);
    baseSR.SetVar("j1pt", 100, -1);
    baseSR.SetVar("j2pt", 100, -1);
    baseSR.SetVar("deltaPhiMin", 0.3, -1);
    baseSR.SetVar("diffMetMhtOverMet", 0, 0.5);
    baseSR.SetVar("nlep", 0, 1);
    //baseSR.SetVar("passesHtMet", 1, 1);

    //add baseline selections to all signal regions 
    std::vector<std::string> vars = baseSR.GetListOfVariables();
    for(unsigned int i = 0; i < SRVec.size(); i++){
      for(unsigned int j = 0; j < vars.size(); j++){
        SRVec.at(i).SetVar(vars.at(j), baseSR.GetLowerBound(vars.at(j)), baseSR.GetUpperBound(vars.at(j)));
      }
    }

    return SRVec;

  }

  std::vector<SR> getSignalRegions2015ExtendedNJets_UltraHighHT(){

    std::vector<SR> temp_SR_vec;
    std::vector<SR> SRVec;
    SR sr;
    SR baseSR;

    //first set binning in njet-nbjet plane
    sr.SetName("1");
    sr.SetVar("njets", 2, 4);
    sr.SetVar("nbjets", 0, 1);
    sr.SetVar("lowMT", 0, -1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("2");
    sr.SetVar("njets", 2, 4);
    sr.SetVar("nbjets", 1, 2);
    sr.SetVar("lowMT", 0, -1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("3");
    sr.SetVar("njets", 2, 4);
    sr.SetVar("nbjets", 2, 3);
    sr.SetVar("lowMT", 1, 2);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("4");
    sr.SetVar("njets", 2, 4);
    sr.SetVar("nbjets", 2, 3);
    sr.SetVar("lowMT", 0, 1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("5");
    sr.SetVar("njets", 4, 7);
    sr.SetVar("nbjets", 0, 1);
    sr.SetVar("lowMT", 0, -1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("6");
    sr.SetVar("njets", 4, 7);
    sr.SetVar("nbjets", 1, 2);
    sr.SetVar("lowMT", 0, -1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("7");
    sr.SetVar("njets", 4, 7);
    sr.SetVar("nbjets", 2, 3);
    sr.SetVar("lowMT", 1, 2);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("8");
    sr.SetVar("njets", 4, 7);
    sr.SetVar("nbjets", 2, 3);
    sr.SetVar("lowMT", 0, 1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("9");
    sr.SetVar("njets", 7, 9);
    sr.SetVar("nbjets", 0, 1);
    sr.SetVar("lowMT", 0, -1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("10");
    sr.SetVar("njets", 7, 9);
    sr.SetVar("nbjets", 1, 2);
    sr.SetVar("lowMT", 0, -1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("11");
    sr.SetVar("njets", 7, 9);
    sr.SetVar("nbjets", 2, 3);
    sr.SetVar("lowMT", 1, 2);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("12");
    sr.SetVar("njets", 7, 9);
    sr.SetVar("nbjets", 2, 3);
    sr.SetVar("lowMT", 0, 1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("13");
    sr.SetVar("njets", 9, -1);
    sr.SetVar("nbjets", 0, 1);
    sr.SetVar("lowMT", 0, -1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("14");
    sr.SetVar("njets", 9, -1);
    sr.SetVar("nbjets", 1, 2);
    sr.SetVar("lowMT", 0, -1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("15");
    sr.SetVar("njets", 9, -1);
    sr.SetVar("nbjets", 2, 3);
    sr.SetVar("lowMT", 1, 2);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("16");
    sr.SetVar("njets", 9, -1);
    sr.SetVar("nbjets", 2, 3);
    sr.SetVar("lowMT", 0, 1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("17");
    sr.SetVar("njets", 2, 7);
    sr.SetVar("nbjets", 3, -1);
    sr.SetVar("lowMT", 1, 2);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("18");
    sr.SetVar("njets", 2, 7);
    sr.SetVar("nbjets", 3, -1);
    sr.SetVar("lowMT", 0, 1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("19");
    sr.SetVar("njets", 7, -1);
    sr.SetVar("nbjets", 3, -1);
    sr.SetVar("lowMT", 1, 2);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("20");
    sr.SetVar("njets", 7, -1);
    sr.SetVar("nbjets", 3, -1);
    sr.SetVar("lowMT", 0, 1);
    temp_SR_vec.push_back(sr);
    sr.Clear();


    //add HT and MET requirements
    for(unsigned int iSR = 0; iSR < temp_SR_vec.size(); iSR++){
      SR fullSR = temp_SR_vec.at(iSR);  
      fullSR.SetName(fullSR.GetName() + "L");
      fullSR.SetVar("ht", 450, 575);
      fullSR.SetVar("met", 200, -1);
      SRVec.push_back(fullSR);
    }
    for(unsigned int iSR = 0; iSR < temp_SR_vec.size(); iSR++){
      SR fullSR = temp_SR_vec.at(iSR);  
      fullSR.SetName(fullSR.GetName() + "M");
      fullSR.SetVar("ht", 575, 1000);
      fullSR.SetVar("met", 200, -1);
      SRVec.push_back(fullSR);
    }
    for(unsigned int iSR = 0; iSR < temp_SR_vec.size(); iSR++){
      SR fullSR = temp_SR_vec.at(iSR);  
      fullSR.SetName(fullSR.GetName() + "H");
      fullSR.SetVar("ht", 1000, 1500);
      fullSR.SetVar("met", 30, -1);
      SRVec.push_back(fullSR);
    }
    for(unsigned int iSR = 0; iSR < temp_SR_vec.size(); iSR++){
      SR fullSR = temp_SR_vec.at(iSR);  
      fullSR.SetName(fullSR.GetName() + "UH");
      fullSR.SetVar("ht", 1500, -1);
      fullSR.SetVar("met", 30, -1);
      SRVec.push_back(fullSR);
    }

    //define baseline selections commmon to all signal regions 
    baseSR.SetVar("mt2", 200, -1);
    baseSR.SetVar("j1pt", 100, -1);
    baseSR.SetVar("j2pt", 100, -1);
    baseSR.SetVar("deltaPhiMin", 0.3, -1);
    baseSR.SetVar("diffMetMhtOverMet", 0, 0.5);
    baseSR.SetVar("nlep", 0, 1);
    //baseSR.SetVar("passesHtMet", 1, 1);

    //add baseline selections to all signal regions 
    std::vector<std::string> vars = baseSR.GetListOfVariables();
    for(unsigned int i = 0; i < SRVec.size(); i++){
      for(unsigned int j = 0; j < vars.size(); j++){
        SRVec.at(i).SetVar(vars.at(j), baseSR.GetLowerBound(vars.at(j)), baseSR.GetUpperBound(vars.at(j)));
      }
    }

    return SRVec;

  }


  std::vector<SR> getSignalRegions2015SevenJets_UltraHighHT(){

    std::vector<SR> temp_SR_vec;
    std::vector<SR> SRVec;
    SR sr;
    SR baseSR;

    //first set binning in njet-nbjet plane
    sr.SetName("1");
    sr.SetVar("njets", 2, 4);
    sr.SetVar("nbjets", 0, 1);
    sr.SetVar("lowMT", 0, -1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("2");
    sr.SetVar("njets", 2, 4);
    sr.SetVar("nbjets", 1, 2);
    sr.SetVar("lowMT", 0, -1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("3");
    sr.SetVar("njets", 2, 4);
    sr.SetVar("nbjets", 2, 3);
    sr.SetVar("lowMT", 1, 2);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("4");
    sr.SetVar("njets", 2, 4);
    sr.SetVar("nbjets", 2, 3);
    sr.SetVar("lowMT", 0, 1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("5");
    sr.SetVar("njets", 4, 7);
    sr.SetVar("nbjets", 0, 1);
    sr.SetVar("lowMT", 0, -1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("6");
    sr.SetVar("njets", 4, 7);
    sr.SetVar("nbjets", 1, 2);
    sr.SetVar("lowMT", 0, -1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("7");
    sr.SetVar("njets", 4, 7);
    sr.SetVar("nbjets", 2, 3);
    sr.SetVar("lowMT", 1, 2);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("8");
    sr.SetVar("njets", 4, 7);
    sr.SetVar("nbjets", 2, 3);
    sr.SetVar("lowMT", 0, 1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("9");
    sr.SetVar("njets", 7, -1);
    sr.SetVar("nbjets", 0, 1);
    sr.SetVar("lowMT", 0, -1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("10");
    sr.SetVar("njets", 7, -1);
    sr.SetVar("nbjets", 1, 2);
    sr.SetVar("lowMT", 0, -1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("11");
    sr.SetVar("njets", 7, -1);
    sr.SetVar("nbjets", 2, 3);
    sr.SetVar("lowMT", 1, 2);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("12");
    sr.SetVar("njets", 7, -1);
    sr.SetVar("nbjets", 2, 3);
    sr.SetVar("lowMT", 0, 1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("13");
    sr.SetVar("njets", 2, 7);
    sr.SetVar("nbjets", 3, -1);
    sr.SetVar("lowMT", 1, 2);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("14");
    sr.SetVar("njets", 2, 7);
    sr.SetVar("nbjets", 3, -1);
    sr.SetVar("lowMT", 0, 1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("15");
    sr.SetVar("njets", 7, -1);
    sr.SetVar("nbjets", 3, -1);
    sr.SetVar("lowMT", 1, 2);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("16");
    sr.SetVar("njets", 7, -1);
    sr.SetVar("nbjets", 3, -1);
    sr.SetVar("lowMT", 0, 1);
    temp_SR_vec.push_back(sr);
    sr.Clear();


    //add HT and MET requirements
    for(unsigned int iSR = 0; iSR < temp_SR_vec.size(); iSR++){
      SR fullSR = temp_SR_vec.at(iSR);  
      fullSR.SetName(fullSR.GetName() + "L");
      fullSR.SetVar("ht", 450, 575);
      fullSR.SetVar("met", 200, -1);
      SRVec.push_back(fullSR);
    }
    for(unsigned int iSR = 0; iSR < temp_SR_vec.size(); iSR++){
      SR fullSR = temp_SR_vec.at(iSR);  
      fullSR.SetName(fullSR.GetName() + "M");
      fullSR.SetVar("ht", 575, 1000);
      fullSR.SetVar("met", 200, -1);
      SRVec.push_back(fullSR);
    }
    for(unsigned int iSR = 0; iSR < temp_SR_vec.size(); iSR++){
      SR fullSR = temp_SR_vec.at(iSR);  
      fullSR.SetName(fullSR.GetName() + "H");
      fullSR.SetVar("ht", 1000, 1500);
      fullSR.SetVar("met", 30, -1);
      SRVec.push_back(fullSR);
    }
    for(unsigned int iSR = 0; iSR < temp_SR_vec.size(); iSR++){
      SR fullSR = temp_SR_vec.at(iSR);  
      fullSR.SetName(fullSR.GetName() + "UH");
      fullSR.SetVar("ht", 1500, -1);
      fullSR.SetVar("met", 30, -1);
      SRVec.push_back(fullSR);
    }

    //define baseline selections commmon to all signal regions 
    baseSR.SetVar("mt2", 200, -1);
    baseSR.SetVar("j1pt", 100, -1);
    baseSR.SetVar("j2pt", 100, -1);
    baseSR.SetVar("deltaPhiMin", 0.3, -1);
    baseSR.SetVar("diffMetMhtOverMet", 0, 0.5);
    baseSR.SetVar("nlep", 0, 1);
    //baseSR.SetVar("passesHtMet", 1, 1);

    //add baseline selections to all signal regions 
    std::vector<std::string> vars = baseSR.GetListOfVariables();
    for(unsigned int i = 0; i < SRVec.size(); i++){
      for(unsigned int j = 0; j < vars.size(); j++){
        SRVec.at(i).SetVar(vars.at(j), baseSR.GetLowerBound(vars.at(j)), baseSR.GetUpperBound(vars.at(j)));
      }
    }

    return SRVec;

  }

  std::vector<SR> getSignalRegionsZurich(){

    std::vector<SR> temp_SR_vec;
    std::vector<SR> SRVec;
    SR sr;
    SR baseSR;

    //first set binning in njet-nbjet plane
    sr.SetName("1");
    sr.SetVar("njets", 2, 4);
    sr.SetVar("nbjets", 0, 1);
    sr.SetVarCRSL("njets", 2, 4);
    sr.SetVarCRSL("nbjets", 0, 1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("2");
    sr.SetVar("njets", 2, 4);
    sr.SetVar("nbjets", 1, 2);
    sr.SetVarCRSL("njets", 2, 4);
    sr.SetVarCRSL("nbjets", 1, 2);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("3");
    sr.SetVar("njets", 2, 4);
    sr.SetVar("nbjets", 2, 3);
    sr.SetVarCRSL("njets", 2, 4);
    sr.SetVarCRSL("nbjets", 2, 3);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("4");
    sr.SetVar("njets", 4, 7);
    sr.SetVar("nbjets", 0, 1);
    sr.SetVarCRSL("njets", 4, 7);
    sr.SetVarCRSL("nbjets", 0, 1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("5");
    sr.SetVar("njets", 4, 7);
    sr.SetVar("nbjets", 1, 2);
    sr.SetVarCRSL("njets", 4, 7);
    sr.SetVarCRSL("nbjets", 1, 2);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("6");
    sr.SetVar("njets", 4, 7);
    sr.SetVar("nbjets", 2, 3);
    sr.SetVarCRSL("njets", 4, 7);
    sr.SetVarCRSL("nbjets", 2, 3);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("7");
    sr.SetVar("njets", 7, -1);
    sr.SetVar("nbjets", 0, 1);
    sr.SetVarCRSL("njets", 7, -1);
    sr.SetVarCRSL("nbjets", 0, 1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    // shared CR: 7j1-2b
    sr.SetName("8");
    sr.SetVar("njets", 7, -1);
    sr.SetVar("nbjets", 1, 2);
    sr.SetVarCRSL("njets", 7, -1);
    sr.SetVarCRSL("nbjets", 1, 3);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    // shared CR: 7j1-2b
    sr.SetName("9");
    sr.SetVar("njets", 7, -1);
    sr.SetVar("nbjets", 2, 3);
    sr.SetVarCRSL("njets", 7, -1);
    sr.SetVarCRSL("nbjets", 1, 3);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("10");
    sr.SetVar("njets", 2, 7);
    sr.SetVar("nbjets", 3, -1);
    sr.SetVarCRSL("njets", 2, 7);
    sr.SetVarCRSL("nbjets", 3, -1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    // shared CR: 7j1-2b
    sr.SetName("11");
    sr.SetVar("njets", 7, -1);
    sr.SetVar("nbjets", 3, -1);
    sr.SetVarCRSL("njets", 7, -1);
    sr.SetVarCRSL("nbjets", 1, 3);
    temp_SR_vec.push_back(sr);
    sr.Clear();


    //add HT and MET requirements
    for(unsigned int iSR = 0; iSR < temp_SR_vec.size(); iSR++){
      SR fullSR = temp_SR_vec.at(iSR);  
      fullSR.SetName(fullSR.GetName() + "L");
      fullSR.SetVar("ht", 450, 575);
      fullSR.SetVar("met", 200, -1);
      fullSR.SetVarCRSL("ht", 450, 575);
      fullSR.SetVarCRSL("met", 200, -1);
      int njets_lo = fullSR.GetLowerBound("njets");
      int nbjets_lo = fullSR.GetLowerBound("nbjets");
      if     (njets_lo == 2 && nbjets_lo == 0){float mt2bins[5] = {200, 300, 400, 500, 1500}; fullSR.SetMT2Bins(4, mt2bins);}
      else if(njets_lo == 2 && nbjets_lo == 1){float mt2bins[5] = {200, 300, 400, 500, 1500}; fullSR.SetMT2Bins(4, mt2bins);}
      else if(njets_lo == 2 && nbjets_lo == 2){float mt2bins[5] = {200, 300, 400, 500, 1500}; fullSR.SetMT2Bins(4, mt2bins);}
      else if(njets_lo == 4 && nbjets_lo == 0){float mt2bins[5] = {200, 300, 400, 500, 1500}; fullSR.SetMT2Bins(4, mt2bins);}
      else if(njets_lo == 4 && nbjets_lo == 1){float mt2bins[5] = {200, 300, 400, 500, 1500}; fullSR.SetMT2Bins(4, mt2bins);}
      else if(njets_lo == 4 && nbjets_lo == 2){float mt2bins[5] = {200, 300, 400, 500, 1500}; fullSR.SetMT2Bins(4, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 0){float mt2bins[2] = {200, 1500};                fullSR.SetMT2Bins(1, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 1){float mt2bins[3] = {200, 300, 1500};           fullSR.SetMT2Bins(2, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 2){float mt2bins[2] = {200, 1500};                fullSR.SetMT2Bins(1, mt2bins);}
      else if(njets_lo == 2 && nbjets_lo == 3){float mt2bins[3] = {200, 300, 1500};           fullSR.SetMT2Bins(2, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 3){float mt2bins[2] = {200, 1500};                fullSR.SetMT2Bins(1, mt2bins);}
      SRVec.push_back(fullSR);
    }
    for(unsigned int iSR = 0; iSR < temp_SR_vec.size(); iSR++){
      SR fullSR = temp_SR_vec.at(iSR);  
      fullSR.SetName(fullSR.GetName() + "M");
      fullSR.SetVar("ht", 575, 1000);
      fullSR.SetVar("met", 200, -1);
      fullSR.SetVarCRSL("ht", 575, 1000);
      fullSR.SetVarCRSL("met", 200, -1);
      int njets_lo = fullSR.GetLowerBound("njets");
      int nbjets_lo = fullSR.GetLowerBound("nbjets");
      if     (njets_lo == 2 && nbjets_lo == 0){float mt2bins[6] = {200, 300, 400, 600, 800, 1500}; fullSR.SetMT2Bins(5, mt2bins);}
      else if(njets_lo == 2 && nbjets_lo == 1){float mt2bins[6] = {200, 300, 400, 600, 800, 1500}; fullSR.SetMT2Bins(5, mt2bins);}
      else if(njets_lo == 2 && nbjets_lo == 2){float mt2bins[5] = {200, 300, 400, 600, 1500};      fullSR.SetMT2Bins(4, mt2bins);}
      else if(njets_lo == 4 && nbjets_lo == 0){float mt2bins[6] = {200, 300, 400, 600, 800, 1500}; fullSR.SetMT2Bins(5, mt2bins);}
      else if(njets_lo == 4 && nbjets_lo == 1){float mt2bins[5] = {200, 300, 400, 600, 1500};      fullSR.SetMT2Bins(4, mt2bins);}
      else if(njets_lo == 4 && nbjets_lo == 2){float mt2bins[5] = {200, 300, 400, 600, 1500};      fullSR.SetMT2Bins(4, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 0){float mt2bins[4] = {200, 300, 400, 1500};           fullSR.SetMT2Bins(3, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 1){float mt2bins[4] = {200, 300, 400, 1500};           fullSR.SetMT2Bins(3, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 2){float mt2bins[4] = {200, 300, 400, 1500};           fullSR.SetMT2Bins(3, mt2bins);}
      else if(njets_lo == 2 && nbjets_lo == 3){float mt2bins[4] = {200, 300, 400, 1500};           fullSR.SetMT2Bins(3, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 3){float mt2bins[4] = {200, 300, 400, 1500};           fullSR.SetMT2Bins(3, mt2bins);}
      SRVec.push_back(fullSR);
    }
    for(unsigned int iSR = 0; iSR < temp_SR_vec.size(); iSR++){
      SR fullSR = temp_SR_vec.at(iSR);  
      fullSR.SetName(fullSR.GetName() + "H");
      fullSR.SetVar("ht", 1000, 1500);
      fullSR.SetVar("met", 30, -1);
      fullSR.SetVarCRSL("ht", 1000, 1500);
      fullSR.SetVarCRSL("met", 30, -1);
      int njets_lo = fullSR.GetLowerBound("njets");
      int nbjets_lo = fullSR.GetLowerBound("nbjets");
      if     (njets_lo == 2 && nbjets_lo == 0){float mt2bins[6] = {200, 400, 600, 800, 1000, 1500}; fullSR.SetMT2Bins(5, mt2bins);}
      else if(njets_lo == 2 && nbjets_lo == 1){float mt2bins[5] = {200, 400, 600, 800, 1500};       fullSR.SetMT2Bins(4, mt2bins);}
      else if(njets_lo == 2 && nbjets_lo == 2){float mt2bins[3] = {200, 400, 1500};                 fullSR.SetMT2Bins(2, mt2bins);}
      else if(njets_lo == 4 && nbjets_lo == 0){float mt2bins[6] = {200, 400, 600, 800, 1000, 1500}; fullSR.SetMT2Bins(5, mt2bins);}
      else if(njets_lo == 4 && nbjets_lo == 1){float mt2bins[5] = {200, 400, 600, 800, 1500};       fullSR.SetMT2Bins(4, mt2bins);}
      else if(njets_lo == 4 && nbjets_lo == 2){float mt2bins[4] = {200, 400, 600, 1500};            fullSR.SetMT2Bins(3, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 0){float mt2bins[4] = {200, 400, 600, 1500};            fullSR.SetMT2Bins(3, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 1){float mt2bins[4] = {200, 400, 600, 1500};            fullSR.SetMT2Bins(3, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 2){float mt2bins[3] = {200, 400, 1500};                 fullSR.SetMT2Bins(2, mt2bins);}
      else if(njets_lo == 2 && nbjets_lo == 3){float mt2bins[3] = {200, 400, 1500};                 fullSR.SetMT2Bins(2, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 3){float mt2bins[3] = {200, 400, 1500};                 fullSR.SetMT2Bins(2, mt2bins);}
      SRVec.push_back(fullSR);
    }
    for(unsigned int iSR = 0; iSR < temp_SR_vec.size(); iSR++){
      SR fullSR = temp_SR_vec.at(iSR);  
      fullSR.SetName(fullSR.GetName() + "UH");
      fullSR.SetVar("ht", 1500, -1);
      fullSR.SetVar("met", 30, -1);
      fullSR.SetVarCRSL("ht", 1500, -1);
      fullSR.SetVarCRSL("met", 30, -1);
      int njets_lo = fullSR.GetLowerBound("njets");
      int nbjets_lo = fullSR.GetLowerBound("nbjets");
      if     (njets_lo == 2 && nbjets_lo == 0){float mt2bins[6] = {200, 400, 600, 800, 1000, 1500}; fullSR.SetMT2Bins(5, mt2bins);}
      else if(njets_lo == 2 && nbjets_lo == 1){float mt2bins[4] = {200, 400, 600, 1500};            fullSR.SetMT2Bins(3, mt2bins);}
      else if(njets_lo == 2 && nbjets_lo == 2){float mt2bins[2] = {200, 1500};                      fullSR.SetMT2Bins(1, mt2bins);}
      else if(njets_lo == 4 && nbjets_lo == 0){float mt2bins[6] = {200, 400, 600, 800, 1000, 1500}; fullSR.SetMT2Bins(5, mt2bins);}
      else if(njets_lo == 4 && nbjets_lo == 1){float mt2bins[4] = {200, 400, 600, 1500};            fullSR.SetMT2Bins(3, mt2bins);}
      else if(njets_lo == 4 && nbjets_lo == 2){float mt2bins[4] = {200, 400, 600, 1500};            fullSR.SetMT2Bins(3, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 0){float mt2bins[3] = {200, 400, 1500};                 fullSR.SetMT2Bins(2, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 1){float mt2bins[3] = {200, 400, 1500};                 fullSR.SetMT2Bins(2, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 2){float mt2bins[3] = {200, 400, 1500};                 fullSR.SetMT2Bins(2, mt2bins);}
      else if(njets_lo == 2 && nbjets_lo == 3){float mt2bins[2] = {200, 1500};                      fullSR.SetMT2Bins(1, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 3){float mt2bins[2] = {200, 1500};                      fullSR.SetMT2Bins(1, mt2bins);}
      SRVec.push_back(fullSR);
    }

    //define baseline selections commmon to all signal regions 
    baseSR.SetVar("mt2", 200, -1);
    baseSR.SetVar("j1pt", 100, -1);
    baseSR.SetVar("j2pt", 100, -1);
    baseSR.SetVar("deltaPhiMin", 0.3, -1);
    baseSR.SetVar("diffMetMhtOverMet", 0, 0.5);
    baseSR.SetVar("nlep", 0, 1);
    //baseSR.SetVar("passesHtMet", 1, 1);

    // common selections for CRSL
    baseSR.SetVarCRSL("mt2", 200, -1);
    baseSR.SetVarCRSL("j1pt", 100, -1);
    baseSR.SetVarCRSL("j2pt", 100, -1);
    baseSR.SetVarCRSL("deltaPhiMin", 0.3, -1);
    baseSR.SetVarCRSL("diffMetMhtOverMet", 0, 0.5);
    baseSR.SetVarCRSL("nlep", 1, 2);

    //add baseline selections to all signal regions 
    std::vector<std::string> vars = baseSR.GetListOfVariables();
    for(unsigned int i = 0; i < SRVec.size(); i++){
      for(unsigned int j = 0; j < vars.size(); j++){
        SRVec.at(i).SetVar(vars.at(j), baseSR.GetLowerBound(vars.at(j)), baseSR.GetUpperBound(vars.at(j)));
      }
    }

    //add baseline selections to all CRSL regions 
    std::vector<std::string> varsCRSL = baseSR.GetListOfVariablesCRSL();
    for(unsigned int i = 0; i < SRVec.size(); i++){
      for(unsigned int j = 0; j < varsCRSL.size(); j++){
        SRVec.at(i).SetVarCRSL(varsCRSL.at(j), baseSR.GetLowerBoundCRSL(varsCRSL.at(j)), baseSR.GetUpperBoundCRSL(varsCRSL.at(j)));
      }
    }

    return SRVec;

  }

  std::vector<SR> getSignalRegionsZurich_jetpt40(){

    std::vector<SR> SRVec = getSignalRegionsZurich();

    //change j1pt and j2pt cuts to 40 GeV
    for(unsigned int i = 0; i < SRVec.size(); i++){
      SRVec.at(i).RemoveVar("j1pt");
      SRVec.at(i).RemoveVar("j2pt");
      SRVec.at(i).SetVar("j1pt", 40, -1);
      SRVec.at(i).SetVar("j2pt", 40, -1);

      SRVec.at(i).RemoveVarCRSL("j1pt");
      SRVec.at(i).RemoveVarCRSL("j2pt");
      SRVec.at(i).SetVarCRSL("j1pt", 40, -1);
      SRVec.at(i).SetVarCRSL("j2pt", 40, -1);
    }

    return SRVec;

  }

  std::vector<SR> getSignalRegionsZurich_jetpt30(){

    std::vector<SR> SRVec = getSignalRegionsZurich();

    //change j1pt and j2pt cuts to 30 GeV
    for(unsigned int i = 0; i < SRVec.size(); i++){
      SRVec.at(i).RemoveVar("j1pt");
      SRVec.at(i).RemoveVar("j2pt");
      SRVec.at(i).SetVar("j1pt", 30, -1);
      SRVec.at(i).SetVar("j2pt", 30, -1);

      SRVec.at(i).RemoveVarCRSL("j1pt");
      SRVec.at(i).RemoveVarCRSL("j2pt");
      SRVec.at(i).SetVarCRSL("j1pt", 30, -1);
      SRVec.at(i).SetVarCRSL("j2pt", 30, -1);
    }

    return SRVec;

  }

  std::vector<SR> getSignalRegionsJamboree(){

    std::vector<SR> temp_SR_vec;
    std::vector<SR> SRVec;
    SR sr;
    SR baseSR;

    //first set binning in njet-nbjet plane
    sr.SetName("1");
    sr.SetVar("njets", 2, 4);
    sr.SetVar("nbjets", 0, 1);
    sr.SetVarCRSL("njets", 2, 4);
    sr.SetVarCRSL("nbjets", 0, 1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("2");
    sr.SetVar("njets", 2, 4);
    sr.SetVar("nbjets", 1, 2);
    sr.SetVarCRSL("njets", 2, 4);
    sr.SetVarCRSL("nbjets", 1, 2);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("3");
    sr.SetVar("njets", 2, 4);
    sr.SetVar("nbjets", 2, 3);
    sr.SetVarCRSL("njets", 2, 4);
    sr.SetVarCRSL("nbjets", 2, 3);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("4");
    sr.SetVar("njets", 4, 7);
    sr.SetVar("nbjets", 0, 1);
    sr.SetVarCRSL("njets", 4, 7);
    sr.SetVarCRSL("nbjets", 0, 1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("5");
    sr.SetVar("njets", 4, 7);
    sr.SetVar("nbjets", 1, 2);
    sr.SetVarCRSL("njets", 4, 7);
    sr.SetVarCRSL("nbjets", 1, 2);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("6");
    sr.SetVar("njets", 4, 7);
    sr.SetVar("nbjets", 2, 3);
    sr.SetVarCRSL("njets", 4, 7);
    sr.SetVarCRSL("nbjets", 2, 3);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("7");
    sr.SetVar("njets", 7, -1);
    sr.SetVar("nbjets", 0, 1);
    sr.SetVarCRSL("njets", 7, -1);
    sr.SetVarCRSL("nbjets", 0, 1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    // shared CR: 7j1-2b
    sr.SetName("8");
    sr.SetVar("njets", 7, -1);
    sr.SetVar("nbjets", 1, 2);
    sr.SetVarCRSL("njets", 7, -1);
    sr.SetVarCRSL("nbjets", 1, 3);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    // shared CR: 7j1-2b
    sr.SetName("9");
    sr.SetVar("njets", 7, -1);
    sr.SetVar("nbjets", 2, 3);
    sr.SetVarCRSL("njets", 7, -1);
    sr.SetVarCRSL("nbjets", 1, 3);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("10");
    sr.SetVar("njets", 2, 7);
    sr.SetVar("nbjets", 3, -1);
    sr.SetVarCRSL("njets", 2, 7);
    sr.SetVarCRSL("nbjets", 3, -1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    // shared CR: 7j1-2b
    sr.SetName("11");
    sr.SetVar("njets", 7, -1);
    sr.SetVar("nbjets", 3, -1);
    sr.SetVarCRSL("njets", 7, -1);
    sr.SetVarCRSL("nbjets", 1, 3);
    temp_SR_vec.push_back(sr);
    sr.Clear();


    //add HT and MET requirements
    for(unsigned int iSR = 0; iSR < temp_SR_vec.size(); iSR++){
      SR fullSR = temp_SR_vec.at(iSR);  
      fullSR.SetName(fullSR.GetName() + "VL");
      fullSR.SetVar("ht", 200, 450);
      fullSR.SetVar("met", 200, -1);
      fullSR.SetVarCRSL("ht", 200, 450);
      fullSR.SetVarCRSL("met", 200, -1);
      fullSR.SetVarCRQCD("ht", 200, 450);
      fullSR.SetVarCRQCD("met", 200, -1);
      int njets_lo = fullSR.GetLowerBound("njets");
      int nbjets_lo = fullSR.GetLowerBound("nbjets");
      if     (njets_lo == 2 && nbjets_lo == 0){float mt2bins[4] = {200, 300, 400, 1500}; fullSR.SetMT2Bins(3, mt2bins);}
      else if(njets_lo == 2 && nbjets_lo == 1){float mt2bins[4] = {200, 300, 400, 1500}; fullSR.SetMT2Bins(3, mt2bins);}
      else if(njets_lo == 2 && nbjets_lo == 2){float mt2bins[4] = {200, 300, 400, 1500}; fullSR.SetMT2Bins(3, mt2bins);}
      else if(njets_lo == 4 && nbjets_lo == 0){float mt2bins[4] = {200, 300, 400, 1500}; fullSR.SetMT2Bins(3, mt2bins);}
      else if(njets_lo == 4 && nbjets_lo == 1){float mt2bins[4] = {200, 300, 400, 1500}; fullSR.SetMT2Bins(3, mt2bins);}
      else if(njets_lo == 4 && nbjets_lo == 2){float mt2bins[4] = {200, 300, 400, 1500}; fullSR.SetMT2Bins(3, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 0){float mt2bins[2] = {200, 1500};           fullSR.SetMT2Bins(1, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 1){float mt2bins[2] = {200, 1500};           fullSR.SetMT2Bins(1, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 2){float mt2bins[2] = {200, 1500};           fullSR.SetMT2Bins(1, mt2bins);}
      else if(njets_lo == 2 && nbjets_lo == 3){float mt2bins[3] = {200, 300, 1500};      fullSR.SetMT2Bins(2, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 3){float mt2bins[2] = {200, 1500};           fullSR.SetMT2Bins(1, mt2bins);}
      SRVec.push_back(fullSR);
    }
    for(unsigned int iSR = 0; iSR < temp_SR_vec.size(); iSR++){
      SR fullSR = temp_SR_vec.at(iSR);  
      fullSR.SetName(fullSR.GetName() + "L");
      fullSR.SetVar("ht", 450, 575);
      fullSR.SetVar("met", 200, -1);
      fullSR.SetVarCRSL("ht", 450, 575);
      fullSR.SetVarCRSL("met", 200, -1);
      fullSR.SetVarCRQCD("ht", 450, 575);
      fullSR.SetVarCRQCD("met", 200, -1);
      int njets_lo = fullSR.GetLowerBound("njets");
      int nbjets_lo = fullSR.GetLowerBound("nbjets");
      if     (njets_lo == 2 && nbjets_lo == 0){float mt2bins[5] = {200, 300, 400, 500, 1500}; fullSR.SetMT2Bins(4, mt2bins);}
      else if(njets_lo == 2 && nbjets_lo == 1){float mt2bins[5] = {200, 300, 400, 500, 1500}; fullSR.SetMT2Bins(4, mt2bins);}
      else if(njets_lo == 2 && nbjets_lo == 2){float mt2bins[5] = {200, 300, 400, 500, 1500}; fullSR.SetMT2Bins(4, mt2bins);}
      else if(njets_lo == 4 && nbjets_lo == 0){float mt2bins[5] = {200, 300, 400, 500, 1500}; fullSR.SetMT2Bins(4, mt2bins);}
      else if(njets_lo == 4 && nbjets_lo == 1){float mt2bins[5] = {200, 300, 400, 500, 1500}; fullSR.SetMT2Bins(4, mt2bins);}
      else if(njets_lo == 4 && nbjets_lo == 2){float mt2bins[5] = {200, 300, 400, 500, 1500}; fullSR.SetMT2Bins(4, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 0){float mt2bins[2] = {200, 1500};                fullSR.SetMT2Bins(1, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 1){float mt2bins[3] = {200, 300, 1500};           fullSR.SetMT2Bins(2, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 2){float mt2bins[2] = {200, 1500};                fullSR.SetMT2Bins(1, mt2bins);}
      else if(njets_lo == 2 && nbjets_lo == 3){float mt2bins[3] = {200, 300, 1500};           fullSR.SetMT2Bins(2, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 3){float mt2bins[2] = {200, 1500};                fullSR.SetMT2Bins(1, mt2bins);}
      SRVec.push_back(fullSR);
    }
    for(unsigned int iSR = 0; iSR < temp_SR_vec.size(); iSR++){
      SR fullSR = temp_SR_vec.at(iSR);  
      fullSR.SetName(fullSR.GetName() + "M");
      fullSR.SetVar("ht", 575, 1000);
      fullSR.SetVar("met", 200, -1);
      fullSR.SetVarCRSL("ht", 575, 1000);
      fullSR.SetVarCRSL("met", 200, -1);
      fullSR.SetVarCRQCD("ht", 575, 1000);
      fullSR.SetVarCRQCD("met", 200, -1);
      int njets_lo = fullSR.GetLowerBound("njets");
      int nbjets_lo = fullSR.GetLowerBound("nbjets");
      if     (njets_lo == 2 && nbjets_lo == 0){float mt2bins[6] = {200, 300, 400, 600, 800, 1500}; fullSR.SetMT2Bins(5, mt2bins);}
      else if(njets_lo == 2 && nbjets_lo == 1){float mt2bins[6] = {200, 300, 400, 600, 800, 1500}; fullSR.SetMT2Bins(5, mt2bins);}
      else if(njets_lo == 2 && nbjets_lo == 2){float mt2bins[5] = {200, 300, 400, 600, 1500};      fullSR.SetMT2Bins(4, mt2bins);}
      else if(njets_lo == 4 && nbjets_lo == 0){float mt2bins[6] = {200, 300, 400, 600, 800, 1500}; fullSR.SetMT2Bins(5, mt2bins);}
      else if(njets_lo == 4 && nbjets_lo == 1){float mt2bins[5] = {200, 300, 400, 600, 1500};      fullSR.SetMT2Bins(4, mt2bins);}
      else if(njets_lo == 4 && nbjets_lo == 2){float mt2bins[5] = {200, 300, 400, 600, 1500};      fullSR.SetMT2Bins(4, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 0){float mt2bins[4] = {200, 300, 400, 1500};           fullSR.SetMT2Bins(3, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 1){float mt2bins[4] = {200, 300, 400, 1500};           fullSR.SetMT2Bins(3, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 2){float mt2bins[4] = {200, 300, 400, 1500};           fullSR.SetMT2Bins(3, mt2bins);}
      else if(njets_lo == 2 && nbjets_lo == 3){float mt2bins[4] = {200, 300, 400, 1500};           fullSR.SetMT2Bins(3, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 3){float mt2bins[4] = {200, 300, 400, 1500};           fullSR.SetMT2Bins(3, mt2bins);}
      SRVec.push_back(fullSR);
    }
    for(unsigned int iSR = 0; iSR < temp_SR_vec.size(); iSR++){
      SR fullSR = temp_SR_vec.at(iSR);  
      fullSR.SetName(fullSR.GetName() + "H");
      fullSR.SetVar("ht", 1000, 1500);
      fullSR.SetVar("met", 30, -1);
      fullSR.SetVarCRSL("ht", 1000, 1500);
      fullSR.SetVarCRSL("met", 30, -1);
      fullSR.SetVarCRQCD("ht", 1000, 1500);
      fullSR.SetVarCRQCD("met", 30, -1);
      int njets_lo = fullSR.GetLowerBound("njets");
      int nbjets_lo = fullSR.GetLowerBound("nbjets");
      if     (njets_lo == 2 && nbjets_lo == 0){float mt2bins[6] = {200, 400, 600, 800, 1000, 1500}; fullSR.SetMT2Bins(5, mt2bins);}
      else if(njets_lo == 2 && nbjets_lo == 1){float mt2bins[5] = {200, 400, 600, 800, 1500};       fullSR.SetMT2Bins(4, mt2bins);}
      else if(njets_lo == 2 && nbjets_lo == 2){float mt2bins[3] = {200, 400, 1500};                 fullSR.SetMT2Bins(2, mt2bins);}
      else if(njets_lo == 4 && nbjets_lo == 0){float mt2bins[6] = {200, 400, 600, 800, 1000, 1500}; fullSR.SetMT2Bins(5, mt2bins);}
      else if(njets_lo == 4 && nbjets_lo == 1){float mt2bins[5] = {200, 400, 600, 800, 1500};       fullSR.SetMT2Bins(4, mt2bins);}
      else if(njets_lo == 4 && nbjets_lo == 2){float mt2bins[4] = {200, 400, 600, 1500};            fullSR.SetMT2Bins(3, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 0){float mt2bins[4] = {200, 400, 600, 1500};            fullSR.SetMT2Bins(3, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 1){float mt2bins[4] = {200, 400, 600, 1500};            fullSR.SetMT2Bins(3, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 2){float mt2bins[3] = {200, 400, 1500};                 fullSR.SetMT2Bins(2, mt2bins);}
      else if(njets_lo == 2 && nbjets_lo == 3){float mt2bins[3] = {200, 400, 1500};                 fullSR.SetMT2Bins(2, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 3){float mt2bins[3] = {200, 400, 1500};                 fullSR.SetMT2Bins(2, mt2bins);}
      SRVec.push_back(fullSR);
    }
    for(unsigned int iSR = 0; iSR < temp_SR_vec.size(); iSR++){
      SR fullSR = temp_SR_vec.at(iSR);  
      fullSR.SetName(fullSR.GetName() + "UH");
      fullSR.SetVar("ht", 1500, -1);
      fullSR.SetVar("met", 30, -1);
      fullSR.SetVarCRSL("ht", 1500, -1);
      fullSR.SetVarCRSL("met", 30, -1);
      fullSR.SetVarCRQCD("ht", 1500, -1);
      fullSR.SetVarCRQCD("met", 30, -1);
      int njets_lo = fullSR.GetLowerBound("njets");
      int nbjets_lo = fullSR.GetLowerBound("nbjets");
      if     (njets_lo == 2 && nbjets_lo == 0){float mt2bins[6] = {200, 400, 600, 800, 1000, 1500}; fullSR.SetMT2Bins(5, mt2bins);}
      else if(njets_lo == 2 && nbjets_lo == 1){float mt2bins[4] = {200, 400, 600, 1500};            fullSR.SetMT2Bins(3, mt2bins);}
      else if(njets_lo == 2 && nbjets_lo == 2){float mt2bins[2] = {200, 1500};                      fullSR.SetMT2Bins(1, mt2bins);}
      else if(njets_lo == 4 && nbjets_lo == 0){float mt2bins[6] = {200, 400, 600, 800, 1000, 1500}; fullSR.SetMT2Bins(5, mt2bins);}
      else if(njets_lo == 4 && nbjets_lo == 1){float mt2bins[4] = {200, 400, 600, 1500};            fullSR.SetMT2Bins(3, mt2bins);}
      else if(njets_lo == 4 && nbjets_lo == 2){float mt2bins[4] = {200, 400, 600, 1500};            fullSR.SetMT2Bins(3, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 0){float mt2bins[3] = {200, 400, 1500};                 fullSR.SetMT2Bins(2, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 1){float mt2bins[3] = {200, 400, 1500};                 fullSR.SetMT2Bins(2, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 2){float mt2bins[3] = {200, 400, 1500};                 fullSR.SetMT2Bins(2, mt2bins);}
      else if(njets_lo == 2 && nbjets_lo == 3){float mt2bins[2] = {200, 1500};                      fullSR.SetMT2Bins(1, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 3){float mt2bins[2] = {200, 1500};                      fullSR.SetMT2Bins(1, mt2bins);}
      SRVec.push_back(fullSR);
    }

    //define baseline selections commmon to all signal regions 
    baseSR.SetVar("mt2", 200, -1);
    baseSR.SetVar("j1pt", 30, -1);
    baseSR.SetVar("j2pt", 30, -1);
    baseSR.SetVar("deltaPhiMin", 0.3, -1);
    baseSR.SetVar("diffMetMhtOverMet", 0, 0.5);
    baseSR.SetVar("nlep", 0, 1);
    //baseSR.SetVar("passesHtMet", 1, 1);

    // common selections for CRSL
    baseSR.SetVarCRSL("mt2", 200, -1);
    baseSR.SetVarCRSL("j1pt", 30, -1);
    baseSR.SetVarCRSL("j2pt", 30, -1);
    baseSR.SetVarCRSL("deltaPhiMin", 0.3, -1);
    baseSR.SetVarCRSL("diffMetMhtOverMet", 0, 0.5);
    baseSR.SetVarCRSL("nlep", 1, 2);

    // common selections for QCD
    baseSR.SetVarCRQCD("mt2", 200, -1);
    baseSR.SetVarCRQCD("j1pt", 30, -1);
    baseSR.SetVarCRQCD("j2pt", 30, -1);
    baseSR.SetVarCRQCD("deltaPhiMin", 0., 0.3);
    baseSR.SetVarCRQCD("diffMetMhtOverMet", 0, 0.5);
    baseSR.SetVarCRQCD("nlep", 0, 1);

    //add baseline selections to all signal regions 
    std::vector<std::string> vars = baseSR.GetListOfVariables();
    for(unsigned int i = 0; i < SRVec.size(); i++){
      for(unsigned int j = 0; j < vars.size(); j++){
        SRVec.at(i).SetVar(vars.at(j), baseSR.GetLowerBound(vars.at(j)), baseSR.GetUpperBound(vars.at(j)));
      }
    }

    //add baseline selections to all CRSL regions 
    std::vector<std::string> varsCRSL = baseSR.GetListOfVariablesCRSL();
    for(unsigned int i = 0; i < SRVec.size(); i++){
      for(unsigned int j = 0; j < varsCRSL.size(); j++){
        SRVec.at(i).SetVarCRSL(varsCRSL.at(j), baseSR.GetLowerBoundCRSL(varsCRSL.at(j)), baseSR.GetUpperBoundCRSL(varsCRSL.at(j)));
      }
    }

    //add baseline selections to all QCD regions 
    std::vector<std::string> varsCRQCD = baseSR.GetListOfVariablesCRQCD();
    for(unsigned int i = 0; i < SRVec.size(); i++){
      for(unsigned int j = 0; j < varsCRQCD.size(); j++){
        SRVec.at(i).SetVarCRQCD(varsCRQCD.at(j), baseSR.GetLowerBoundCRQCD(varsCRQCD.at(j)), baseSR.GetUpperBoundCRQCD(varsCRQCD.at(j)));
      }
    }

    return SRVec;

  }
  

  std::vector<SR> getSignalRegionsMonojet(){

    std::vector<SR> temp_SR_vec;
    std::vector<SR> SRVec;
    SR baseSR;

    // define baseline selections commmon to all monojet regions
    //  ETH doesn't explictly cut on jet1_pt for SR or CRSL, just ht, so we won't either for this iteration
    float mt2bins_monojet[2] = {0, 1500};
    //baseSR.SetVar("j1pt", 200, -1);
    baseSR.SetVar("nlep", 0, 1);
    baseSR.SetVar("njets", 1, 2);
    baseSR.SetVar("met", 200, -1);
    baseSR.SetVar("deltaPhiMin", 0.3, -1);
    baseSR.SetVar("diffMetMhtOverMet", 0, 0.5);
    //baseSR.SetVarCRSL("j1pt", 200, -1);
    baseSR.SetVarCRSL("nlep", 1, 2);
    baseSR.SetVarCRSL("njets", 1, 2);
    baseSR.SetVarCRSL("met", 200, -1);
    baseSR.SetVarCRSL("deltaPhiMin", 0.3, -1);
    baseSR.SetVarCRSL("diffMetMhtOverMet", 0, 0.5);
    // QCD region: 2 jets, low deltaPhiMin, pt subleading between 30 and 60 GeV
    baseSR.SetVarCRQCD("j1pt", 200, -1);
    baseSR.SetVarCRQCD("j2pt", 30, 60);
    baseSR.SetVarCRQCD("nlep", 0, 1);
    baseSR.SetVarCRQCD("njets", 2, 3);
    baseSR.SetVarCRQCD("met", 200, -1);
    baseSR.SetVarCRQCD("deltaPhiMin", 0., 0.3);
    baseSR.SetVarCRQCD("diffMetMhtOverMet", 0, 0.5);
    baseSR.SetMT2Bins(1, mt2bins_monojet);

    // fine binning in HT
    const unsigned int nbins_monojet_0b = 7;
    float htbins_0b[nbins_monojet_0b+1] = {200, 250, 350, 450, 575, 700, 1000, -1};
    float htbins_0b_forplot[nbins_monojet_0b+1] = {200, 250, 350, 450, 575, 700, 1000, 1500};
    const unsigned int nbins_monojet_1b = 5;
    float htbins_1b[nbins_monojet_1b+1] = {200, 250, 350, 450, 575, -1};
    float htbins_1b_forplot[nbins_monojet_1b+1] = {200, 250, 350, 450, 575, 1500};

    temp_SR_vec.clear();
    for(unsigned int iSR = 0; iSR < nbins_monojet_0b; iSR++){
      SR fullSR0b = baseSR;  
      fullSR0b.SetName(std::to_string(iSR+1) + "J");
      fullSR0b.SetVar("ht", htbins_0b[iSR], htbins_0b[iSR+1]);
      fullSR0b.SetVar("nbjets", 0, 1);
      fullSR0b.SetVarCRSL("ht", htbins_0b[iSR], htbins_0b[iSR+1]);
      fullSR0b.SetVarCRSL("nbjets", 0, 1);
      fullSR0b.SetVarCRQCD("ht", htbins_0b[iSR], htbins_0b[iSR+1]);
      fullSR0b.SetVarCRQCD("nbjets", 0, 1);
      SRVec.push_back(fullSR0b);
    }
    for(unsigned int iSR = 0; iSR < nbins_monojet_1b; iSR++){
      SR fullSR1b = baseSR;  
      fullSR1b.SetName(std::to_string(iSR+11) + "J");
      fullSR1b.SetVar("ht", htbins_1b[iSR], htbins_1b[iSR+1]);
      fullSR1b.SetVar("nbjets", 1, -1);
      fullSR1b.SetVarCRSL("ht", htbins_1b[iSR], htbins_1b[iSR+1]);
      fullSR1b.SetVarCRSL("nbjets", 1, -1);
      fullSR1b.SetVarCRQCD("ht", htbins_1b[iSR], htbins_1b[iSR+1]);
      fullSR1b.SetVarCRQCD("nbjets", 1, -1);
      SRVec.push_back(fullSR1b);
    }


    // Also put in some inclusive regions: 0b, 1b
    SR fullSR0b = baseSR;  
    fullSR0b.SetName("baseJ0B");
    fullSR0b.SetVar("nbjets", 0, 1);
    fullSR0b.SetVarCRSL("nbjets", 0, 1);
    fullSR0b.SetVarCRQCD("nbjets", 0, 1);
    fullSR0b.SetVar("ht", 200, -1);
    fullSR0b.SetVarCRSL("ht", 200, -1);
    fullSR0b.SetVarCRQCD("ht", 200, -1);
    fullSR0b.SetMT2Bins(nbins_monojet_0b, htbins_0b_forplot);
    SRVec.push_back(fullSR0b);

    SR fullSR1b = baseSR;  
    fullSR1b.SetName("baseJ1B");
    fullSR1b.SetVar("nbjets", 1, -1);
    fullSR1b.SetVarCRSL("nbjets", 1, -1);
    fullSR1b.SetVarCRQCD("nbjets", 1, -1);
    fullSR1b.SetVar("ht", 200, -1);
    fullSR1b.SetVarCRSL("ht", 200, -1);
    fullSR1b.SetVarCRQCD("ht", 200, -1);
    fullSR1b.SetMT2Bins(nbins_monojet_1b, htbins_1b_forplot);
    SRVec.push_back(fullSR1b);

    return SRVec;

  }


  std::vector<SR> getSignalRegionsICHEP(){

    std::vector<SR> temp_SR_vec;
    std::vector<SR> SRVec;
    SR sr;
    SR baseSR;

    //first set binning in njet-nbjet plane
    sr.SetName("1");
    sr.SetVar("njets", 2, 4);
    sr.SetVar("nbjets", 0, 1);
    sr.SetVarCRSL("njets", 2, 4);
    sr.SetVarCRSL("nbjets", 0, 1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("2");
    sr.SetVar("njets", 2, 4);
    sr.SetVar("nbjets", 1, 2);
    sr.SetVarCRSL("njets", 2, 4);
    sr.SetVarCRSL("nbjets", 1, 2);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("3");
    sr.SetVar("njets", 2, 4);
    sr.SetVar("nbjets", 2, 3);
    sr.SetVarCRSL("njets", 2, 4);
    sr.SetVarCRSL("nbjets", 2, 3);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("4");
    sr.SetVar("njets", 4, 7);
    sr.SetVar("nbjets", 0, 1);
    sr.SetVarCRSL("njets", 4, 7);
    sr.SetVarCRSL("nbjets", 0, 1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("5");
    sr.SetVar("njets", 4, 7);
    sr.SetVar("nbjets", 1, 2);
    sr.SetVarCRSL("njets", 4, 7);
    sr.SetVarCRSL("nbjets", 1, 2);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("6");
    sr.SetVar("njets", 4, 7);
    sr.SetVar("nbjets", 2, 3);
    sr.SetVarCRSL("njets", 4, 7);
    sr.SetVarCRSL("nbjets", 2, 3);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("7");
    sr.SetVar("njets", 7, -1);
    sr.SetVar("nbjets", 0, 1);
    sr.SetVarCRSL("njets", 7, -1);
    sr.SetVarCRSL("nbjets", 0, 1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    // shared CR: 7j1-2b
    sr.SetName("8");
    sr.SetVar("njets", 7, -1);
    sr.SetVar("nbjets", 1, 2);
    sr.SetVarCRSL("njets", 7, -1);
    sr.SetVarCRSL("nbjets", 1, 3);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    // shared CR: 7j1-2b
    sr.SetName("9");
    sr.SetVar("njets", 7, -1);
    sr.SetVar("nbjets", 2, 3);
    sr.SetVarCRSL("njets", 7, -1);
    sr.SetVarCRSL("nbjets", 1, 3);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("10");
    sr.SetVar("njets", 2, 7);
    sr.SetVar("nbjets", 3, -1);
    sr.SetVarCRSL("njets", 2, 7);
    sr.SetVarCRSL("nbjets", 3, -1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    // shared CR: 7j1-2b
    sr.SetName("11");
    sr.SetVar("njets", 7, -1);
    sr.SetVar("nbjets", 3, -1);
    sr.SetVarCRSL("njets", 7, -1);
    sr.SetVarCRSL("nbjets", 1, 3);
    temp_SR_vec.push_back(sr);
    sr.Clear();


    //add HT and MET requirements
    for(unsigned int iSR = 0; iSR < temp_SR_vec.size(); iSR++){
      SR fullSR = temp_SR_vec.at(iSR);  
      fullSR.SetName(fullSR.GetName() + "VL");
      fullSR.SetVar("ht", 200, 450);
      fullSR.SetVar("met", 200, -1);
      fullSR.SetVarCRSL("ht", 200, 450);
      fullSR.SetVarCRSL("met", 200, -1);
      fullSR.SetVarCRQCD("ht", 200, 450);
      fullSR.SetVarCRQCD("met", 200, -1);
      int njets_lo = fullSR.GetLowerBound("njets");
      int nbjets_lo = fullSR.GetLowerBound("nbjets");
      if     (njets_lo == 2 && nbjets_lo == 0){float mt2bins[4] = {200, 300, 400, 1500}; fullSR.SetMT2Bins(3, mt2bins);}
      else if(njets_lo == 2 && nbjets_lo == 1){float mt2bins[4] = {200, 300, 400, 1500}; fullSR.SetMT2Bins(3, mt2bins);}
      else if(njets_lo == 2 && nbjets_lo == 2){float mt2bins[4] = {200, 300, 400, 1500}; fullSR.SetMT2Bins(3, mt2bins);}
      else if(njets_lo == 4 && nbjets_lo == 0){float mt2bins[4] = {200, 300, 400, 1500}; fullSR.SetMT2Bins(3, mt2bins);}
      else if(njets_lo == 4 && nbjets_lo == 1){float mt2bins[4] = {200, 300, 400, 1500}; fullSR.SetMT2Bins(3, mt2bins);}
      else if(njets_lo == 4 && nbjets_lo == 2){float mt2bins[4] = {200, 300, 400, 1500}; fullSR.SetMT2Bins(3, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 0){float mt2bins[2] = {200, 1500};           fullSR.SetMT2Bins(1, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 1){float mt2bins[2] = {200, 1500};           fullSR.SetMT2Bins(1, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 2){float mt2bins[2] = {200, 1500};           fullSR.SetMT2Bins(1, mt2bins);}
      else if(njets_lo == 2 && nbjets_lo == 3){float mt2bins[3] = {200, 300, 1500};      fullSR.SetMT2Bins(2, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 3){float mt2bins[2] = {200, 1500};           fullSR.SetMT2Bins(1, mt2bins);}
      SRVec.push_back(fullSR);
    }
    for(unsigned int iSR = 0; iSR < temp_SR_vec.size(); iSR++){
      SR fullSR = temp_SR_vec.at(iSR);  
      fullSR.SetName(fullSR.GetName() + "L");
      fullSR.SetVar("ht", 450, 575);
      fullSR.SetVar("met", 200, -1);
      fullSR.SetVarCRSL("ht", 450, 575);
      fullSR.SetVarCRSL("met", 200, -1);
      fullSR.SetVarCRQCD("ht", 450, 575);
      fullSR.SetVarCRQCD("met", 200, -1);
      int njets_lo = fullSR.GetLowerBound("njets");
      int nbjets_lo = fullSR.GetLowerBound("nbjets");
      if     (njets_lo == 2 && nbjets_lo == 0){float mt2bins[5] = {200, 300, 400, 500, 1500}; fullSR.SetMT2Bins(4, mt2bins);}
      else if(njets_lo == 2 && nbjets_lo == 1){float mt2bins[5] = {200, 300, 400, 500, 1500}; fullSR.SetMT2Bins(4, mt2bins);}
      else if(njets_lo == 2 && nbjets_lo == 2){float mt2bins[5] = {200, 300, 400, 500, 1500}; fullSR.SetMT2Bins(4, mt2bins);}
      else if(njets_lo == 4 && nbjets_lo == 0){float mt2bins[5] = {200, 300, 400, 500, 1500}; fullSR.SetMT2Bins(4, mt2bins);}
      else if(njets_lo == 4 && nbjets_lo == 1){float mt2bins[5] = {200, 300, 400, 500, 1500}; fullSR.SetMT2Bins(4, mt2bins);}
      else if(njets_lo == 4 && nbjets_lo == 2){float mt2bins[5] = {200, 300, 400, 500, 1500}; fullSR.SetMT2Bins(4, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 0){float mt2bins[2] = {200, 1500};                fullSR.SetMT2Bins(1, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 1){float mt2bins[3] = {200, 300, 1500};           fullSR.SetMT2Bins(2, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 2){float mt2bins[2] = {200, 1500};                fullSR.SetMT2Bins(1, mt2bins);}
      else if(njets_lo == 2 && nbjets_lo == 3){float mt2bins[3] = {200, 300, 1500};           fullSR.SetMT2Bins(2, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 3){float mt2bins[2] = {200, 1500};                fullSR.SetMT2Bins(1, mt2bins);}
      SRVec.push_back(fullSR);
    }
    for(unsigned int iSR = 0; iSR < temp_SR_vec.size(); iSR++){
      SR fullSR = temp_SR_vec.at(iSR);  
      fullSR.SetName(fullSR.GetName() + "M");
      fullSR.SetVar("ht", 575, 1000);
      fullSR.SetVar("met", 200, -1);
      fullSR.SetVarCRSL("ht", 575, 1000);
      fullSR.SetVarCRSL("met", 200, -1);
      fullSR.SetVarCRQCD("ht", 575, 1000);
      fullSR.SetVarCRQCD("met", 200, -1);
      int njets_lo = fullSR.GetLowerBound("njets");
      int nbjets_lo = fullSR.GetLowerBound("nbjets");
      if     (njets_lo == 2 && nbjets_lo == 0){float mt2bins[6] = {200, 300, 400, 600, 800, 1500}; fullSR.SetMT2Bins(5, mt2bins);}
      else if(njets_lo == 2 && nbjets_lo == 1){float mt2bins[6] = {200, 300, 400, 600, 800, 1500}; fullSR.SetMT2Bins(5, mt2bins);}
      else if(njets_lo == 2 && nbjets_lo == 2){float mt2bins[5] = {200, 300, 400, 600, 1500};      fullSR.SetMT2Bins(4, mt2bins);}
      else if(njets_lo == 4 && nbjets_lo == 0){float mt2bins[6] = {200, 300, 400, 600, 800, 1500}; fullSR.SetMT2Bins(5, mt2bins);}
      else if(njets_lo == 4 && nbjets_lo == 1){float mt2bins[5] = {200, 300, 400, 600, 1500};      fullSR.SetMT2Bins(4, mt2bins);}
      else if(njets_lo == 4 && nbjets_lo == 2){float mt2bins[5] = {200, 300, 400, 600, 1500};      fullSR.SetMT2Bins(4, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 0){float mt2bins[4] = {200, 300, 400, 1500};           fullSR.SetMT2Bins(3, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 1){float mt2bins[4] = {200, 300, 400, 1500};           fullSR.SetMT2Bins(3, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 2){float mt2bins[4] = {200, 300, 400, 1500};           fullSR.SetMT2Bins(3, mt2bins);}
      else if(njets_lo == 2 && nbjets_lo == 3){float mt2bins[4] = {200, 300, 400, 1500};           fullSR.SetMT2Bins(3, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 3){float mt2bins[4] = {200, 300, 400, 1500};           fullSR.SetMT2Bins(3, mt2bins);}
      SRVec.push_back(fullSR);
    }
    for(unsigned int iSR = 0; iSR < temp_SR_vec.size(); iSR++){
      SR fullSR = temp_SR_vec.at(iSR);  
      fullSR.SetName(fullSR.GetName() + "H");
      fullSR.SetVar("ht", 1000, 1500);
      fullSR.SetVar("met", 30, -1);
      fullSR.SetVarCRSL("ht", 1000, 1500);
      fullSR.SetVarCRSL("met", 30, -1);
      fullSR.SetVarCRQCD("ht", 1000, 1500);
      fullSR.SetVarCRQCD("met", 30, -1);
      int njets_lo = fullSR.GetLowerBound("njets");
      int nbjets_lo = fullSR.GetLowerBound("nbjets");
      if     (njets_lo == 2 && nbjets_lo == 0){float mt2bins[6] = {200, 400, 600, 800, 1000, 1500}; fullSR.SetMT2Bins(5, mt2bins);}
      else if(njets_lo == 2 && nbjets_lo == 1){float mt2bins[5] = {200, 400, 600, 800, 1500};       fullSR.SetMT2Bins(4, mt2bins);}
      else if(njets_lo == 2 && nbjets_lo == 2){float mt2bins[3] = {200, 400, 1500};                 fullSR.SetMT2Bins(2, mt2bins);}
      else if(njets_lo == 4 && nbjets_lo == 0){float mt2bins[6] = {200, 400, 600, 800, 1000, 1500}; fullSR.SetMT2Bins(5, mt2bins);}
      else if(njets_lo == 4 && nbjets_lo == 1){float mt2bins[5] = {200, 400, 600, 800, 1500};       fullSR.SetMT2Bins(4, mt2bins);}
      else if(njets_lo == 4 && nbjets_lo == 2){float mt2bins[4] = {200, 400, 600, 1500};            fullSR.SetMT2Bins(3, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 0){float mt2bins[4] = {200, 400, 600, 1500};            fullSR.SetMT2Bins(3, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 1){float mt2bins[4] = {200, 400, 600, 1500};            fullSR.SetMT2Bins(3, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 2){float mt2bins[3] = {200, 400, 1500};                 fullSR.SetMT2Bins(2, mt2bins);}
      else if(njets_lo == 2 && nbjets_lo == 3){float mt2bins[3] = {200, 400, 1500};                 fullSR.SetMT2Bins(2, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 3){float mt2bins[3] = {200, 400, 1500};                 fullSR.SetMT2Bins(2, mt2bins);}
      SRVec.push_back(fullSR);
    }
    for(unsigned int iSR = 0; iSR < temp_SR_vec.size(); iSR++){
      SR fullSR = temp_SR_vec.at(iSR);  
      fullSR.SetName(fullSR.GetName() + "UH");
      fullSR.SetVar("ht", 1500, -1);
      fullSR.SetVar("met", 30, -1);
      fullSR.SetVarCRSL("ht", 1500, -1);
      fullSR.SetVarCRSL("met", 30, -1);
      fullSR.SetVarCRQCD("ht", 1500, -1);
      fullSR.SetVarCRQCD("met", 30, -1);
      int njets_lo = fullSR.GetLowerBound("njets");
      int nbjets_lo = fullSR.GetLowerBound("nbjets");
      if     (njets_lo == 2 && nbjets_lo == 0){float mt2bins[6] = {200, 400, 600, 800, 1000, 1500}; fullSR.SetMT2Bins(5, mt2bins);}
      else if(njets_lo == 2 && nbjets_lo == 1){float mt2bins[4] = {200, 400, 600, 1500};            fullSR.SetMT2Bins(3, mt2bins);}
      else if(njets_lo == 2 && nbjets_lo == 2){float mt2bins[2] = {200, 1500};                      fullSR.SetMT2Bins(1, mt2bins);}
      else if(njets_lo == 4 && nbjets_lo == 0){float mt2bins[6] = {200, 400, 600, 800, 1000, 1500}; fullSR.SetMT2Bins(5, mt2bins);}
      else if(njets_lo == 4 && nbjets_lo == 1){float mt2bins[4] = {200, 400, 600, 1500};            fullSR.SetMT2Bins(3, mt2bins);}
      else if(njets_lo == 4 && nbjets_lo == 2){float mt2bins[4] = {200, 400, 600, 1500};            fullSR.SetMT2Bins(3, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 0){float mt2bins[3] = {200, 400, 1500};                 fullSR.SetMT2Bins(2, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 1){float mt2bins[3] = {200, 400, 1500};                 fullSR.SetMT2Bins(2, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 2){float mt2bins[3] = {200, 400, 1500};                 fullSR.SetMT2Bins(2, mt2bins);}
      else if(njets_lo == 2 && nbjets_lo == 3){float mt2bins[3] = {200, 400, 1500};                 fullSR.SetMT2Bins(2, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 3){float mt2bins[3] = {200, 400, 1500};                 fullSR.SetMT2Bins(2, mt2bins);}
      SRVec.push_back(fullSR);
    }

    //define baseline selections commmon to all signal regions 
    baseSR.SetVar("mt2", 200, -1);
    baseSR.SetVar("j1pt", 30, -1);
    baseSR.SetVar("j2pt", 30, -1);
    baseSR.SetVar("deltaPhiMin", 0.3, -1);
    baseSR.SetVar("diffMetMhtOverMet", 0, 0.5);
    baseSR.SetVar("nlep", 0, 1);
    //baseSR.SetVar("passesHtMet", 1, 1);

    // common selections for CRSL
    baseSR.SetVarCRSL("mt2", 200, -1);
    baseSR.SetVarCRSL("j1pt", 30, -1);
    baseSR.SetVarCRSL("j2pt", 30, -1);
    baseSR.SetVarCRSL("deltaPhiMin", 0.3, -1);
    baseSR.SetVarCRSL("diffMetMhtOverMet", 0, 0.5);
    baseSR.SetVarCRSL("nlep", 1, 2);

    // common selections for QCD
    baseSR.SetVarCRQCD("mt2", 200, -1);
    baseSR.SetVarCRQCD("j1pt", 30, -1);
    baseSR.SetVarCRQCD("j2pt", 30, -1);
    baseSR.SetVarCRQCD("deltaPhiMin", 0., 0.3);
    baseSR.SetVarCRQCD("diffMetMhtOverMet", 0, 0.5);
    baseSR.SetVarCRQCD("nlep", 0, 1);

    //add baseline selections to all signal regions 
    std::vector<std::string> vars = baseSR.GetListOfVariables();
    for(unsigned int i = 0; i < SRVec.size(); i++){
      for(unsigned int j = 0; j < vars.size(); j++){
        SRVec.at(i).SetVar(vars.at(j), baseSR.GetLowerBound(vars.at(j)), baseSR.GetUpperBound(vars.at(j)));
      }
    }

    //add baseline selections to all CRSL regions 
    std::vector<std::string> varsCRSL = baseSR.GetListOfVariablesCRSL();
    for(unsigned int i = 0; i < SRVec.size(); i++){
      for(unsigned int j = 0; j < varsCRSL.size(); j++){
        SRVec.at(i).SetVarCRSL(varsCRSL.at(j), baseSR.GetLowerBoundCRSL(varsCRSL.at(j)), baseSR.GetUpperBoundCRSL(varsCRSL.at(j)));
      }
    }

    //add baseline selections to all QCD regions 
    std::vector<std::string> varsCRQCD = baseSR.GetListOfVariablesCRQCD();
    for(unsigned int i = 0; i < SRVec.size(); i++){
      for(unsigned int j = 0; j < varsCRQCD.size(); j++){
        SRVec.at(i).SetVarCRQCD(varsCRQCD.at(j), baseSR.GetLowerBoundCRQCD(varsCRQCD.at(j)), baseSR.GetUpperBoundCRQCD(varsCRQCD.at(j)));
      }
    }

    return SRVec;

  }
  
  std::vector<SR> getSignalRegionsMonojet2016(){

    std::vector<SR> temp_SR_vec;
    std::vector<SR> SRVec;
    SR baseSR;

    // define baseline selections commmon to all monojet regions
    //  ETH doesn't explictly cut on jet1_pt for SR or CRSL, just ht, so we won't either for this iteration
    float mt2bins_monojet[2] = {0, 1500};
    //baseSR.SetVar("j1pt", 200, -1);
    baseSR.SetVar("nlep", 0, 1);
    baseSR.SetVar("njets", 1, 2);
    baseSR.SetVar("met", 250, -1);
    baseSR.SetVar("deltaPhiMin", 0.3, -1);
    baseSR.SetVar("diffMetMhtOverMet", 0, 0.5);
    //baseSR.SetVarCRSL("j1pt", 200, -1);
    baseSR.SetVarCRSL("nlep", 1, 2);
    baseSR.SetVarCRSL("njets", 1, 2);
    baseSR.SetVarCRSL("met", 250, -1);
    baseSR.SetVarCRSL("deltaPhiMin", 0.3, -1);
    baseSR.SetVarCRSL("diffMetMhtOverMet", 0, 0.5);
    baseSR.SetVarCRDY("nlep", 0, 1);
    baseSR.SetVarCRDY("njets", 1, 2);
    baseSR.SetVarCRDY("met", 250, -1);
    baseSR.SetVarCRDY("deltaPhiMin", 0.3, -1);
    baseSR.SetVarCRDY("diffMetMhtOverMet", 0, 0.5);
    // QCD region: 2 jets, low deltaPhiMin, pt subleading between 30 and 60 GeV
    baseSR.SetVarCRQCD("j1pt", 250, -1);
    baseSR.SetVarCRQCD("j2pt", 30, 60);
    baseSR.SetVarCRQCD("nlep", 0, 1);
    baseSR.SetVarCRQCD("njets", 2, 3);
    baseSR.SetVarCRQCD("met", 250, -1);
    baseSR.SetVarCRQCD("deltaPhiMin", 0., 0.3);
    baseSR.SetVarCRQCD("diffMetMhtOverMet", 0, 0.5);
    baseSR.SetMT2Bins(1, mt2bins_monojet);

    // fine binning in HT
    const unsigned int nbins_monojet_0b = 7;
    float htbins_0b[nbins_monojet_0b+1] = {250, 350, 450, 575, 700, 1000, 1200, -1};
    float htbins_0b_forplot[nbins_monojet_0b+1] = {250, 350, 450, 575, 700, 1000, 1200, 1500};
    const unsigned int nbins_monojet_1b = 5;
    float htbins_1b[nbins_monojet_1b+1] = {250, 350, 450, 575, 700, -1};
    float htbins_1b_forplot[nbins_monojet_1b+1] = {250, 350, 450, 575, 700, 1500};

    temp_SR_vec.clear();
    for(unsigned int iSR = 0; iSR < nbins_monojet_0b; iSR++){
      SR fullSR0b = baseSR;  
      fullSR0b.SetName(std::to_string(iSR+1) + "J");
      fullSR0b.SetVar("ht", htbins_0b[iSR], htbins_0b[iSR+1]);
      fullSR0b.SetVar("nbjets", 0, 1);
      fullSR0b.SetVarCRSL("ht", htbins_0b[iSR], htbins_0b[iSR+1]);
      fullSR0b.SetVarCRSL("nbjets", 0, 1);
      fullSR0b.SetVarCRDY("ht", htbins_0b[iSR], htbins_0b[iSR+1]);
      fullSR0b.SetVarCRDY("nbjets", 0, 1);
      fullSR0b.SetVarCRQCD("ht", htbins_0b[iSR], htbins_0b[iSR+1]);
      fullSR0b.SetVarCRQCD("nbjets", 0, 1);
      SRVec.push_back(fullSR0b);
    }
    for(unsigned int iSR = 0; iSR < nbins_monojet_1b; iSR++){
      SR fullSR1b = baseSR;  
      fullSR1b.SetName(std::to_string(iSR+11) + "J");
      fullSR1b.SetVar("ht", htbins_1b[iSR], htbins_1b[iSR+1]);
      fullSR1b.SetVar("nbjets", 1, -1);
      fullSR1b.SetVarCRSL("ht", htbins_1b[iSR], htbins_1b[iSR+1]);
      fullSR1b.SetVarCRSL("nbjets", 1, -1);
      fullSR1b.SetVarCRDY("ht", htbins_1b[iSR], htbins_1b[iSR+1]);
      fullSR1b.SetVarCRDY("nbjets", 1, -1);
      fullSR1b.SetVarCRQCD("ht", htbins_1b[iSR], htbins_1b[iSR+1]);
      fullSR1b.SetVarCRQCD("nbjets", 1, -1);
      SRVec.push_back(fullSR1b);
    }


    // Also put in some inclusive regions: 0b, 1b
    SR fullSR0b = baseSR;  
    fullSR0b.SetName("baseJ0B");
    fullSR0b.SetVar("nbjets", 0, 1);
    fullSR0b.SetVarCRSL("nbjets", 0, 1);
    fullSR0b.SetVarCRDY("nbjets", 0, 1);
    fullSR0b.SetVarCRQCD("nbjets", 0, 1);
    fullSR0b.SetVar("ht", 250, -1);
    fullSR0b.SetVarCRSL("ht", 250, -1);
    fullSR0b.SetVarCRDY("ht", 250, -1);
    fullSR0b.SetVarCRQCD("ht", 250, -1);
    fullSR0b.SetMT2Bins(nbins_monojet_0b, htbins_0b_forplot);
    SRVec.push_back(fullSR0b);

    SR fullSR1b = baseSR;  
    fullSR1b.SetName("baseJ1B");
    fullSR1b.SetVar("nbjets", 1, -1);
    fullSR1b.SetVarCRSL("nbjets", 1, -1);
    fullSR1b.SetVarCRDY("nbjets", 1, -1);
    fullSR1b.SetVarCRQCD("nbjets", 1, -1);
    fullSR1b.SetVar("ht", 250, -1);
    fullSR1b.SetVarCRSL("ht", 250, -1);
    fullSR1b.SetVarCRDY("ht", 250, -1);
    fullSR1b.SetVarCRQCD("ht", 250, -1);
    fullSR1b.SetMT2Bins(nbins_monojet_1b, htbins_1b_forplot);
    SRVec.push_back(fullSR1b);

    return SRVec;

  }  

  std::vector<SR> getSignalRegions2016(){

    std::vector<SR> temp_SR_vec;
    std::vector<SR> SRVec;
    SR sr;
    SR baseSR;

    //first set binning in njet-nbjet plane
    sr.SetName("1");
    sr.SetVar("njets", 2, 4);
    sr.SetVar("nbjets", 0, 1);
    sr.SetVarCRSL("njets", 2, 4);
    sr.SetVarCRSL("nbjets", 0, 1);
    sr.SetVarCRDY("njets", 2, 4);
    sr.SetVarCRDY("nbjets", 0, 1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("2");
    sr.SetVar("njets", 2, 4);
    sr.SetVar("nbjets", 1, 2);
    sr.SetVarCRSL("njets", 2, 4);
    sr.SetVarCRSL("nbjets", 1, 2);
    sr.SetVarCRDY("njets", 2, 4);
    sr.SetVarCRDY("nbjets", 1, 2);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("3");
    sr.SetVar("njets", 2, 4);
    sr.SetVar("nbjets", 2, 3);
    sr.SetVarCRSL("njets", 2, 4);
    sr.SetVarCRSL("nbjets", 2, 3);
    sr.SetVarCRDY("njets", 2, 4);
    sr.SetVarCRDY("nbjets", 2, 3);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("4");
    sr.SetVar("njets", 4, 7);
    sr.SetVar("nbjets", 0, 1);
    sr.SetVarCRSL("njets", 4, 7);
    sr.SetVarCRSL("nbjets", 0, 1);
    sr.SetVarCRDY("njets", 4, 7);
    sr.SetVarCRDY("nbjets", 0, 1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("5");
    sr.SetVar("njets", 4, 7);
    sr.SetVar("nbjets", 1, 2);
    sr.SetVarCRSL("njets", 4, 7);
    sr.SetVarCRSL("nbjets", 1, 2);
    sr.SetVarCRDY("njets", 4, 7);
    sr.SetVarCRDY("nbjets", 1, 2);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("6");
    sr.SetVar("njets", 4, 7);
    sr.SetVar("nbjets", 2, 3);
    sr.SetVarCRSL("njets", 4, 7);
    sr.SetVarCRSL("nbjets", 2, 3);
    sr.SetVarCRDY("njets", 4, 7);
    sr.SetVarCRDY("nbjets", 2, 3);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("7");
    sr.SetVar("njets", 7, -1);
    sr.SetVar("nbjets", 0, 1);
    sr.SetVarCRSL("njets", 7, -1);
    sr.SetVarCRSL("nbjets", 0, 1);
    sr.SetVarCRDY("njets", 7, -1);
    sr.SetVarCRDY("nbjets", 0, 1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    // shared CR: 7j1-2b
    sr.SetName("8");
    sr.SetVar("njets", 7, -1);
    sr.SetVar("nbjets", 1, 2);
    sr.SetVarCRSL("njets", 7, -1);
    sr.SetVarCRSL("nbjets", 1, 3);
    sr.SetVarCRDY("njets", 7, -1);
    sr.SetVarCRDY("nbjets", 1, -1); // DY: NB>0
    temp_SR_vec.push_back(sr);
    sr.Clear();

    // shared CR: 7j1-2b
    sr.SetName("9");
    sr.SetVar("njets", 7, -1);
    sr.SetVar("nbjets", 2, 3);
    sr.SetVarCRSL("njets", 7, -1);
    sr.SetVarCRSL("nbjets", 1, 3);
    sr.SetVarCRDY("njets", 7, -1);
    sr.SetVarCRDY("nbjets", 1, -1); // DY: NB>0
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("10");
    sr.SetVar("njets", 2, 7);
    sr.SetVar("nbjets", 3, -1);
    sr.SetVarCRSL("njets", 2, 7);
    sr.SetVarCRSL("nbjets", 3, -1);
    sr.SetVarCRDY("njets", 2, 7);
    sr.SetVarCRDY("nbjets", 3, -1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    // shared CR: 7j1-2b
    sr.SetName("11");
    sr.SetVar("njets", 7, -1);
    sr.SetVar("nbjets", 3, -1);
    sr.SetVarCRSL("njets", 7, -1);
    sr.SetVarCRSL("nbjets", 1, 3);
    sr.SetVarCRDY("njets", 7, -1);
    sr.SetVarCRDY("nbjets", 1, -1); // DY: NB>0
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("12");
    sr.SetVar("njets", 4, -1);
    sr.SetVar("nbjets", 0, 1);
    sr.SetVarCRSL("njets", 4, -1);
    sr.SetVarCRSL("nbjets", 0, 1);
    sr.SetVarCRDY("njets", 4, -1);
    sr.SetVarCRDY("nbjets", 0, 1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("13");
    sr.SetVar("njets", 4, -1);
    sr.SetVar("nbjets", 1, 2);
    sr.SetVarCRSL("njets", 4, -1);
    sr.SetVarCRSL("nbjets", 1, 2);
    sr.SetVarCRDY("njets", 4, -1);
    sr.SetVarCRDY("nbjets", 1, 2);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("14");
    sr.SetVar("njets", 4, -1);
    sr.SetVar("nbjets", 2, 3);
    sr.SetVarCRSL("njets", 4, -1);
    sr.SetVarCRSL("nbjets", 2, 3);
    sr.SetVarCRDY("njets", 4, -1);
    sr.SetVarCRDY("nbjets", 2, 3);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("15");
    sr.SetVar("njets", 2, -1);
    sr.SetVar("nbjets", 3, -1);
    sr.SetVarCRSL("njets", 2, -1);
    sr.SetVarCRSL("nbjets", 3, -1);
    sr.SetVarCRDY("njets", 2, -1);
    sr.SetVarCRDY("nbjets", 3, -1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("16");
    sr.SetVar("njets", 2, -1);
    sr.SetVar("nbjets", 0, -1);
    sr.SetVarCRSL("njets", 2, -1);
    sr.SetVarCRSL("nbjets", 0, -1);
    sr.SetVarCRDY("njets", 2, -1);
    sr.SetVarCRDY("nbjets", 0, -1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("17");
    sr.SetVar("njets", 4, -1);
    sr.SetVar("nbjets", 0, -1);
    sr.SetVarCRSL("njets", 4, -1);
    sr.SetVarCRSL("nbjets", 0, -1);
    sr.SetVarCRDY("njets", 4, -1);
    sr.SetVarCRDY("nbjets", 0, -1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("18");
    sr.SetVar("njets", 7, -1);
    sr.SetVar("nbjets", 0, -1);
    sr.SetVarCRSL("njets", 7, -1);
    sr.SetVarCRSL("nbjets", 0, -1);
    sr.SetVarCRDY("njets", 7, -1);
    sr.SetVarCRDY("nbjets", 0, -1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("19");
    sr.SetVar("njets", 2, -1);
    sr.SetVar("nbjets", 2, -1);
    sr.SetVarCRSL("njets", 2, -1);
    sr.SetVarCRSL("nbjets", 2, -1);
    sr.SetVarCRDY("njets", 2, -1);
    sr.SetVarCRDY("nbjets", 2, -1);
    temp_SR_vec.push_back(sr);
    sr.Clear();
    
    //add HT and MET requirements
    for(unsigned int iSR = 0; iSR < temp_SR_vec.size(); iSR++){
      if (iSR >= 3 && iSR <= 10) continue;
      if (iSR >= 15) continue;
      SR fullSR = temp_SR_vec.at(iSR); 
      fullSR.SetName(fullSR.GetName() + "VL");
      fullSR.SetVar("ht", 250, 450);
      fullSR.SetVar("met", 250, -1);
      fullSR.SetVarCRSL("ht", 250, 450);
      fullSR.SetVarCRSL("met", 250, -1);
      fullSR.SetVarCRDY("ht", 250, 450);
      fullSR.SetVarCRDY("met", 250, -1);
      fullSR.SetVarCRQCD("ht", 250, 450);
      fullSR.SetVarCRQCD("met", 250, -1);
      int njets_lo = fullSR.GetLowerBound("njets");
      int nbjets_lo = fullSR.GetLowerBound("nbjets");
      if     (njets_lo == 2 && nbjets_lo == 0){float mt2bins[4] = {200, 300, 400, 1500}; fullSR.SetMT2Bins(3, mt2bins);}
      else if(njets_lo == 2 && nbjets_lo == 1){float mt2bins[4] = {200, 300, 400, 1500}; fullSR.SetMT2Bins(3, mt2bins);}
      else if(njets_lo == 2 && nbjets_lo == 2){float mt2bins[4] = {200, 300, 400, 1500}; fullSR.SetMT2Bins(3, mt2bins);}
      else if(njets_lo == 4 && nbjets_lo == 0){float mt2bins[4] = {200, 300, 400, 1500}; fullSR.SetMT2Bins(3, mt2bins);}
      else if(njets_lo == 4 && nbjets_lo == 1){float mt2bins[4] = {200, 300, 400, 1500}; fullSR.SetMT2Bins(3, mt2bins);}
      else if(njets_lo == 4 && nbjets_lo == 2){float mt2bins[4] = {200, 300, 400, 1500}; fullSR.SetMT2Bins(3, mt2bins);}
      else if(njets_lo == 2 && nbjets_lo == 3){float mt2bins[4] = {200, 300, 400, 1500}; fullSR.SetMT2Bins(3, mt2bins);}
      SRVec.push_back(fullSR);
    }
    for(unsigned int iSR = 0; iSR < temp_SR_vec.size(); iSR++){
      if (iSR >= 11) continue;
      SR fullSR = temp_SR_vec.at(iSR);      
      fullSR.SetName(fullSR.GetName() + "L");
      fullSR.SetVar("ht", 450, 575);
      fullSR.SetVar("met", 250, -1);
      fullSR.SetVarCRSL("ht", 450, 575);
      fullSR.SetVarCRSL("met", 250, -1);
      fullSR.SetVarCRDY("ht", 450, 575);
      fullSR.SetVarCRDY("met", 250, -1);
      fullSR.SetVarCRQCD("ht", 450, 575);
      fullSR.SetVarCRQCD("met", 250, -1);
      int njets_lo = fullSR.GetLowerBound("njets");
      int nbjets_lo = fullSR.GetLowerBound("nbjets");
      if     (njets_lo == 2 && nbjets_lo == 0){float mt2bins[5] = {200, 300, 400, 500, 1500}; fullSR.SetMT2Bins(4, mt2bins);}
      else if(njets_lo == 2 && nbjets_lo == 1){float mt2bins[5] = {200, 300, 400, 500, 1500}; fullSR.SetMT2Bins(4, mt2bins);}
      else if(njets_lo == 2 && nbjets_lo == 2){float mt2bins[5] = {200, 300, 400, 500, 1500}; fullSR.SetMT2Bins(4, mt2bins);}
      else if(njets_lo == 4 && nbjets_lo == 0){float mt2bins[5] = {200, 300, 400, 500, 1500}; fullSR.SetMT2Bins(4, mt2bins);}
      else if(njets_lo == 4 && nbjets_lo == 1){float mt2bins[5] = {200, 300, 400, 500, 1500}; fullSR.SetMT2Bins(4, mt2bins);}
      else if(njets_lo == 4 && nbjets_lo == 2){float mt2bins[5] = {200, 300, 400, 500, 1500}; fullSR.SetMT2Bins(4, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 0){float mt2bins[4] = {200, 300, 400, 1500};      fullSR.SetMT2Bins(3, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 1){float mt2bins[4] = {200, 300, 400, 1500};      fullSR.SetMT2Bins(3, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 2){float mt2bins[4] = {200, 300, 400, 1500};      fullSR.SetMT2Bins(3, mt2bins);}
      else if(njets_lo == 2 && nbjets_lo == 3){float mt2bins[5] = {200, 300, 400, 500, 1500}; fullSR.SetMT2Bins(4, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 3){float mt2bins[4] = {200, 300, 400, 1500};      fullSR.SetMT2Bins(3, mt2bins);}
      SRVec.push_back(fullSR);
    }
    for(unsigned int iSR = 0; iSR < temp_SR_vec.size(); iSR++){
      if (iSR >= 11) continue;
      SR fullSR = temp_SR_vec.at(iSR);  
      fullSR.SetName(fullSR.GetName() + "M");
      fullSR.SetVar("ht", 575, 1000);
      fullSR.SetVar("met", 250, -1);
      fullSR.SetVarCRSL("ht", 575, 1000);
      fullSR.SetVarCRSL("met", 250, -1);
      fullSR.SetVarCRDY("ht", 575, 1000);
      fullSR.SetVarCRDY("met", 250, -1);
      fullSR.SetVarCRQCD("ht", 575, 1000);
      fullSR.SetVarCRQCD("met", 250, -1);
      int njets_lo = fullSR.GetLowerBound("njets");
      int nbjets_lo = fullSR.GetLowerBound("nbjets");
      if     (njets_lo == 2 && nbjets_lo == 0){float mt2bins[6] = {200, 300, 400, 600, 800, 1500}; fullSR.SetMT2Bins(5, mt2bins);}
      else if(njets_lo == 2 && nbjets_lo == 1){float mt2bins[6] = {200, 300, 400, 600, 800, 1500}; fullSR.SetMT2Bins(5, mt2bins);}
      else if(njets_lo == 2 && nbjets_lo == 2){float mt2bins[6] = {200, 300, 400, 600, 800, 1500}; fullSR.SetMT2Bins(5, mt2bins);}
      else if(njets_lo == 4 && nbjets_lo == 0){float mt2bins[6] = {200, 300, 400, 600, 800, 1500}; fullSR.SetMT2Bins(5, mt2bins);}
      else if(njets_lo == 4 && nbjets_lo == 1){float mt2bins[6] = {200, 300, 400, 600, 800, 1500}; fullSR.SetMT2Bins(5, mt2bins);}
      else if(njets_lo == 4 && nbjets_lo == 2){float mt2bins[6] = {200, 300, 400, 600, 800, 1500}; fullSR.SetMT2Bins(5, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 0){float mt2bins[6] = {200, 300, 400, 600, 800, 1500}; fullSR.SetMT2Bins(5, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 1){float mt2bins[5] = {200, 300, 400, 600, 1500};      fullSR.SetMT2Bins(4, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 2){float mt2bins[5] = {200, 300, 400, 600, 1500};      fullSR.SetMT2Bins(4, mt2bins);}
      else if(njets_lo == 2 && nbjets_lo == 3){float mt2bins[5] = {200, 300, 400, 600, 1500};      fullSR.SetMT2Bins(4, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 3){float mt2bins[5] = {200, 300, 400, 600, 1500};      fullSR.SetMT2Bins(4, mt2bins);}
      SRVec.push_back(fullSR);
    }
    for(unsigned int iSR = 0; iSR < temp_SR_vec.size(); iSR++){
      if (iSR >= 11) continue;      
      SR fullSR = temp_SR_vec.at(iSR);  
      fullSR.SetName(fullSR.GetName() + "H");
      fullSR.SetVar("ht", 1000, 1500);
      fullSR.SetVar("met", 30, -1);
      fullSR.SetVarCRSL("ht", 1000, 1500);
      fullSR.SetVarCRSL("met", 30, -1);
      fullSR.SetVarCRDY("ht", 1000, 1500);
      fullSR.SetVarCRDY("met", 30, -1);
      fullSR.SetVarCRQCD("ht", 1000, 1500);
      fullSR.SetVarCRQCD("met", 30, -1);
      int njets_lo = fullSR.GetLowerBound("njets");
      int nbjets_lo = fullSR.GetLowerBound("nbjets");
      if     (njets_lo == 2 && nbjets_lo == 0){float mt2bins[7] = {200, 400, 600, 800, 1000, 1200, 1500}; fullSR.SetMT2Bins(6, mt2bins);}
      else if(njets_lo == 2 && nbjets_lo == 1){float mt2bins[7] = {200, 400, 600, 800, 1000, 1200, 1500}; fullSR.SetMT2Bins(6, mt2bins);}
      else if(njets_lo == 2 && nbjets_lo == 2){float mt2bins[6] = {200, 400, 600, 800, 1000, 1500};       fullSR.SetMT2Bins(5, mt2bins);}
      else if(njets_lo == 4 && nbjets_lo == 0){float mt2bins[7] = {200, 400, 600, 800, 1000, 1200, 1500}; fullSR.SetMT2Bins(6, mt2bins);}
      else if(njets_lo == 4 && nbjets_lo == 1){float mt2bins[7] = {200, 400, 600, 800, 1000, 1200, 1500}; fullSR.SetMT2Bins(6, mt2bins);}
      else if(njets_lo == 4 && nbjets_lo == 2){float mt2bins[6] = {200, 400, 600, 800, 1000, 1500};       fullSR.SetMT2Bins(5, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 0){float mt2bins[6] = {200, 400, 600, 800, 1000, 1500};       fullSR.SetMT2Bins(5, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 1){float mt2bins[5] = {200, 400, 600, 800, 1500};             fullSR.SetMT2Bins(4, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 2){float mt2bins[5] = {200, 400, 600, 800, 1500};             fullSR.SetMT2Bins(4, mt2bins);}
      else if(njets_lo == 2 && nbjets_lo == 3){float mt2bins[4] = {200, 400, 600, 1500};                  fullSR.SetMT2Bins(3, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 3){float mt2bins[4] = {200, 400, 600, 1500};                  fullSR.SetMT2Bins(3, mt2bins);}
      SRVec.push_back(fullSR);
    }
    for(unsigned int iSR = 0; iSR < temp_SR_vec.size(); iSR++){
      if (iSR >= 11) continue;
      SR fullSR = temp_SR_vec.at(iSR);  
      fullSR.SetName(fullSR.GetName() + "UH");
      fullSR.SetVar("ht", 1500, -1);
      fullSR.SetVar("met", 30, -1);
      fullSR.SetVarCRSL("ht", 1500, -1);
      fullSR.SetVarCRSL("met", 30, -1);
      fullSR.SetVarCRDY("ht", 1500, -1);
      fullSR.SetVarCRDY("met", 30, -1);
      fullSR.SetVarCRQCD("ht", 1500, -1);
      fullSR.SetVarCRQCD("met", 30, -1);
      int njets_lo = fullSR.GetLowerBound("njets");
      int nbjets_lo = fullSR.GetLowerBound("nbjets");
      int njets_hi = fullSR.GetUpperBound("njets");
      if     (njets_lo == 2 && nbjets_lo == 0){float mt2bins[7] = {200, 400, 600, 800, 1000, 1400, 1800}; fullSR.SetMT2Bins(6, mt2bins);}
      else if(njets_lo == 2 && nbjets_lo == 1){float mt2bins[6] = {200, 400, 600, 800, 1000, 1800};       fullSR.SetMT2Bins(5, mt2bins);}
      else if(njets_lo == 2 && nbjets_lo == 2){
        float mt2bins[3] = {200, 400, 1800};
        float mt2bins_ssr[4] = {200, 400, 600, 1800};
        if (njets_hi == -1)
          fullSR.SetMT2Bins(3, mt2bins_ssr);
        else
          fullSR.SetMT2Bins(2, mt2bins);          
      }      
      else if(njets_lo == 4 && nbjets_lo == 0){float mt2bins[7] = {200, 400, 600, 800, 1000, 1400, 1800}; fullSR.SetMT2Bins(6, mt2bins);}
      else if(njets_lo == 4 && nbjets_lo == 1){float mt2bins[7] = {200, 400, 600, 800, 1000, 1400, 1800}; fullSR.SetMT2Bins(6, mt2bins);}
      else if(njets_lo == 4 && nbjets_lo == 2){float mt2bins[5] = {200, 400, 600, 800, 1800};             fullSR.SetMT2Bins(4, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 0){float mt2bins[6] = {200, 400, 600, 800, 1000, 1800};       fullSR.SetMT2Bins(5, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 1){float mt2bins[5] = {200, 400, 600, 800, 1800};             fullSR.SetMT2Bins(4, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 2){float mt2bins[5] = {200, 400, 600, 800, 1800};             fullSR.SetMT2Bins(4, mt2bins);}
      else if(njets_lo == 2 && nbjets_lo == 3){float mt2bins[4] = {200, 400, 600, 1800};                  fullSR.SetMT2Bins(3, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 3){float mt2bins[3] = {200, 400, 1800};                       fullSR.SetMT2Bins(2, mt2bins);}
      SRVec.push_back(fullSR);
    }

    // --- super signal regions, numbered 20 and up
    //     inclusive in HT, NJ, NB
    //     MT2 binning matches the most granular TR that would be included
    //     MT2 cut will be applied later by summing appropriate datacards

    // 2j loose
    sr.SetName("20");
    sr.SetVar("njets", 2, -1);
    sr.SetVar("nbjets", 0, -1);
    sr.SetVar("ht", 1000, -1);
    sr.SetVar("met", 30, -1);
    sr.SetVarCRSL("njets", 2, -1);
    sr.SetVarCRSL("nbjets", 0, -1);
    sr.SetVarCRSL("ht", 1000, -1);
    sr.SetVarCRSL("met", 30, -1);
    sr.SetVarCRDY("njets", 2, -1);
    sr.SetVarCRDY("nbjets", 0, -1);
    sr.SetVarCRDY("ht", 1000, -1);
    sr.SetVarCRDY("met", 30, -1);
    sr.SetVarCRQCD("ht", 1000, -1);
    sr.SetVarCRQCD("met", 30, -1);
    float mt2bins_sr20[] = {200, 400, 600, 800, 1000, 1200, 1800};
    sr.SetMT2Bins(6, mt2bins_sr20);
    SRVec.push_back(sr);
    sr.Clear();
    
    // 2j tight
    sr.SetName("21");
    sr.SetVar("njets", 2, -1);
    sr.SetVar("nbjets", 0, -1);
    sr.SetVar("ht", 1500, -1);
    sr.SetVar("met", 30, -1);
    sr.SetVarCRSL("njets", 2, -1);
    sr.SetVarCRSL("nbjets", 0, -1);
    sr.SetVarCRSL("ht", 1500, -1);
    sr.SetVarCRSL("met", 30, -1);
    sr.SetVarCRDY("njets", 2, -1);
    sr.SetVarCRDY("nbjets", 0, -1);
    sr.SetVarCRDY("ht", 1500, -1);
    sr.SetVarCRDY("met", 30, -1);
    sr.SetVarCRQCD("ht", 1500, -1);
    sr.SetVarCRQCD("met", 30, -1);
    float mt2bins_sr21[] = {200, 400, 600, 800, 1000, 1400, 1800};
    sr.SetMT2Bins(6, mt2bins_sr21);
    SRVec.push_back(sr);
    sr.Clear();
    
    // 4j loose
    sr.SetName("22");
    sr.SetVar("njets", 4, -1);
    sr.SetVar("nbjets", 0, -1);
    sr.SetVar("ht", 1000, -1);
    sr.SetVar("met", 30, -1);
    sr.SetVarCRSL("njets", 4, -1);
    sr.SetVarCRSL("nbjets", 0, -1);
    sr.SetVarCRSL("ht", 1000, -1);
    sr.SetVarCRSL("met", 30, -1);
    sr.SetVarCRDY("njets", 4, -1);
    sr.SetVarCRDY("nbjets", 0, -1);
    sr.SetVarCRDY("ht", 1000, -1);
    sr.SetVarCRDY("met", 30, -1);
    sr.SetVarCRQCD("ht", 1000, -1);
    sr.SetVarCRQCD("met", 30, -1);
    float mt2bins_sr22[] = {200, 400, 600, 800, 1000, 1200, 1800};
    sr.SetMT2Bins(6, mt2bins_sr22);
    SRVec.push_back(sr);
    sr.Clear();
    
    // 4j tight
    sr.SetName("23");
    sr.SetVar("njets", 4, -1);
    sr.SetVar("nbjets", 0, -1);
    sr.SetVar("ht", 1500, -1);
    sr.SetVar("met", 30, -1);
    sr.SetVarCRSL("njets", 4, -1);
    sr.SetVarCRSL("nbjets", 0, -1);
    sr.SetVarCRSL("ht", 1500, -1);
    sr.SetVarCRSL("met", 30, -1);
    sr.SetVarCRDY("njets", 4, -1);
    sr.SetVarCRDY("nbjets", 0, -1);
    sr.SetVarCRDY("ht", 1500, -1);
    sr.SetVarCRDY("met", 30, -1);
    sr.SetVarCRQCD("ht", 1500, -1);
    sr.SetVarCRQCD("met", 30, -1);
    float mt2bins_sr23[] = {200, 400, 600, 800, 1000, 1400, 1800};
    sr.SetMT2Bins(6, mt2bins_sr23);
    SRVec.push_back(sr);
    sr.Clear();
    
    // 7j loose
    sr.SetName("24");
    sr.SetVar("njets", 7, -1);
    sr.SetVar("nbjets", 0, -1);
    sr.SetVar("ht", 1000, -1);
    sr.SetVar("met", 30, -1);
    sr.SetVarCRSL("njets", 7, -1);
    sr.SetVarCRSL("nbjets", 0, -1);
    sr.SetVarCRSL("ht", 1000, -1);
    sr.SetVarCRSL("met", 30, -1);
    sr.SetVarCRDY("njets", 7, -1);
    sr.SetVarCRDY("nbjets", 0, -1);
    sr.SetVarCRDY("ht", 1000, -1);
    sr.SetVarCRDY("met", 30, -1);
    sr.SetVarCRQCD("ht", 1000, -1);
    sr.SetVarCRQCD("met", 30, -1);
    float mt2bins_sr24[] = {200, 400, 600, 800, 1000, 1800};
    sr.SetMT2Bins(5, mt2bins_sr24);
    SRVec.push_back(sr);
    sr.Clear();
    
    // 7j tight
    sr.SetName("25");
    sr.SetVar("njets", 7, -1);
    sr.SetVar("nbjets", 0, -1);
    sr.SetVar("ht", 1500, -1);
    sr.SetVar("met", 30, -1);
    sr.SetVarCRSL("njets", 7, -1);
    sr.SetVarCRSL("nbjets", 0, -1);
    sr.SetVarCRSL("ht", 1500, -1);
    sr.SetVarCRSL("met", 30, -1);
    sr.SetVarCRDY("njets", 7, -1);
    sr.SetVarCRDY("nbjets", 0, -1);
    sr.SetVarCRDY("ht", 1500, -1);
    sr.SetVarCRDY("met", 30, -1);
    sr.SetVarCRQCD("ht", 1500, -1);
    sr.SetVarCRQCD("met", 30, -1);
    float mt2bins_sr25[] = {200, 400, 600, 800, 1000, 1800};
    sr.SetMT2Bins(5, mt2bins_sr25);
    SRVec.push_back(sr);
    sr.Clear();
    

    // 2b loose
    sr.SetName("26");
    sr.SetVar("njets", 2, -1);
    sr.SetVar("nbjets", 2, -1);
    sr.SetVar("ht", 1000, -1);
    sr.SetVar("met", 30, -1);
    sr.SetVarCRSL("njets", 2, -1);
    sr.SetVarCRSL("nbjets", 2, -1);
    sr.SetVarCRSL("ht", 1000, -1);
    sr.SetVarCRSL("met", 30, -1);
    sr.SetVarCRDY("njets", 2, -1);
    sr.SetVarCRDY("nbjets", 2, -1);
    sr.SetVarCRDY("ht", 1000, -1);
    sr.SetVarCRDY("met", 30, -1);
    sr.SetVarCRQCD("ht", 1000, -1);
    sr.SetVarCRQCD("met", 30, -1);
    float mt2bins_sr26[] = {200, 400, 600, 800, 1000, 1800};
    sr.SetMT2Bins(5, mt2bins_sr26);
    SRVec.push_back(sr);
    sr.Clear();
    
    // 2b tight
    sr.SetName("27");
    sr.SetVar("njets", 2, -1);
    sr.SetVar("nbjets", 2, -1);
    sr.SetVar("ht", 1500, -1);
    sr.SetVar("met", 30, -1);
    sr.SetVarCRSL("njets", 2, -1);
    sr.SetVarCRSL("nbjets", 2, -1);
    sr.SetVarCRSL("ht", 1500, -1);
    sr.SetVarCRSL("met", 30, -1);
    sr.SetVarCRDY("njets", 2, -1);
    sr.SetVarCRDY("nbjets", 2, -1);
    sr.SetVarCRDY("ht", 1500, -1);
    sr.SetVarCRDY("met", 30, -1);
    sr.SetVarCRQCD("ht", 1500, -1);
    sr.SetVarCRQCD("met", 30, -1);
    float mt2bins_sr27[] = {200, 400, 600, 800, 1800};
    sr.SetMT2Bins(4, mt2bins_sr27);
    SRVec.push_back(sr);
    sr.Clear();
    
    // 3b loose
    sr.SetName("28");
    sr.SetVar("njets", 2, -1);
    sr.SetVar("nbjets", 3, -1);
    sr.SetVar("ht", 1000, -1);
    sr.SetVar("met", 30, -1);
    sr.SetVarCRSL("njets", 2, -1);
    sr.SetVarCRSL("nbjets", 3, -1);
    sr.SetVarCRSL("ht", 1000, -1);
    sr.SetVarCRSL("met", 30, -1);
    sr.SetVarCRDY("njets", 2, -1);
    sr.SetVarCRDY("nbjets", 1, -1); // DY: NB>0
    sr.SetVarCRDY("ht", 1000, -1);
    sr.SetVarCRDY("met", 30, -1);
    sr.SetVarCRQCD("ht", 1000, -1);
    sr.SetVarCRQCD("met", 30, -1);
    float mt2bins_sr28[] = {200, 400, 600, 1800};
    sr.SetMT2Bins(3, mt2bins_sr28);
    SRVec.push_back(sr);
    sr.Clear();
    
    // 3b tight
    sr.SetName("29");
    sr.SetVar("njets", 2, -1);
    sr.SetVar("nbjets", 3, -1);
    sr.SetVar("ht", 1500, -1);
    sr.SetVar("met", 30, -1);
    sr.SetVarCRSL("njets", 2, -1);
    sr.SetVarCRSL("nbjets", 3, -1);
    sr.SetVarCRSL("ht", 1500, -1);
    sr.SetVarCRSL("met", 30, -1);
    sr.SetVarCRDY("njets", 2, -1);
    sr.SetVarCRDY("nbjets", 1, -1); // DY: NB>0
    sr.SetVarCRDY("ht", 1500, -1);
    sr.SetVarCRDY("met", 30, -1);
    sr.SetVarCRQCD("ht", 1500, -1);
    sr.SetVarCRQCD("met", 30, -1);
    float mt2bins_sr29[] = {200, 400, 600, 1800};
    sr.SetMT2Bins(3, mt2bins_sr29);
    SRVec.push_back(sr);
    sr.Clear();
    
    // 7j3b loose
    sr.SetName("30");
    sr.SetVar("njets", 7, -1);
    sr.SetVar("nbjets", 3, -1);
    sr.SetVar("ht", 1000, -1);
    sr.SetVar("met", 30, -1);
    sr.SetVarCRSL("njets", 7, -1);
    sr.SetVarCRSL("nbjets", 3, -1);
    sr.SetVarCRSL("ht", 1000, -1);
    sr.SetVarCRSL("met", 30, -1);
    sr.SetVarCRDY("njets", 7, -1);
    sr.SetVarCRDY("nbjets", 1, -1); // DY: NB>0
    sr.SetVarCRDY("ht", 1000, -1);
    sr.SetVarCRDY("met", 30, -1);
    sr.SetVarCRQCD("ht", 1000, -1);
    sr.SetVarCRQCD("met", 30, -1);
    float mt2bins_sr30[] = {200, 400, 600, 1800};
    sr.SetMT2Bins(3, mt2bins_sr30);
    SRVec.push_back(sr);
    sr.Clear();
    
    // 7j3b tight
    sr.SetName("31");
    sr.SetVar("njets", 7, -1);
    sr.SetVar("nbjets", 3, -1);
    sr.SetVar("ht", 1500, -1);
    sr.SetVar("met", 30, -1);
    sr.SetVarCRSL("njets", 7, -1);
    sr.SetVarCRSL("nbjets", 3, -1);
    sr.SetVarCRSL("ht", 1500, -1);
    sr.SetVarCRSL("met", 30, -1);
    sr.SetVarCRDY("njets", 7, -1);
    sr.SetVarCRDY("nbjets", 1, -1); // DY: NB>0
    sr.SetVarCRDY("ht", 1500, -1);
    sr.SetVarCRDY("met", 30, -1);
    sr.SetVarCRQCD("ht", 1500, -1);
    sr.SetVarCRQCD("met", 30, -1);
    float mt2bins_sr31[] = {200, 400, 1800};
    sr.SetMT2Bins(2, mt2bins_sr31);
    SRVec.push_back(sr);
    sr.Clear();
    
    //define baseline selections commmon to all signal regions 
    baseSR.SetVar("mt2", 200, -1);
    baseSR.SetVar("j1pt", 30, -1);
    baseSR.SetVar("j2pt", 30, -1);
    baseSR.SetVar("deltaPhiMin", 0.3, -1);
    baseSR.SetVar("diffMetMhtOverMet", 0, 0.5);
    baseSR.SetVar("nlep", 0, 1);
    //baseSR.SetVar("passesHtMet", 1, 1);

    // common selections for CRSL
    baseSR.SetVarCRSL("mt2", 200, -1);
    baseSR.SetVarCRSL("j1pt", 30, -1);
    baseSR.SetVarCRSL("j2pt", 30, -1);
    baseSR.SetVarCRSL("deltaPhiMin", 0.3, -1);
    baseSR.SetVarCRSL("diffMetMhtOverMet", 0, 0.5);
    baseSR.SetVarCRSL("nlep", 1, 2);

    // common selections for CRDY
    baseSR.SetVarCRDY("mt2", 200, -1);
    baseSR.SetVarCRDY("j1pt", 30, -1);
    baseSR.SetVarCRDY("j2pt", 30, -1);
    baseSR.SetVarCRDY("deltaPhiMin", 0.3, -1);
    baseSR.SetVarCRDY("diffMetMhtOverMet", 0, 0.5);
    baseSR.SetVarCRDY("nlep", 0, 1);

    // common selections for QCD
    baseSR.SetVarCRQCD("mt2", 200, -1);
    baseSR.SetVarCRQCD("j1pt", 30, -1);
    baseSR.SetVarCRQCD("j2pt", 30, -1);
    baseSR.SetVarCRQCD("deltaPhiMin", 0., 0.3);
    baseSR.SetVarCRQCD("diffMetMhtOverMet", 0, 0.5);
    baseSR.SetVarCRQCD("nlep", 0, 1);

    //add baseline selections to all signal regions 
    std::vector<std::string> vars = baseSR.GetListOfVariables();
    for(unsigned int i = 0; i < SRVec.size(); i++){
      for(unsigned int j = 0; j < vars.size(); j++){
        SRVec.at(i).SetVar(vars.at(j), baseSR.GetLowerBound(vars.at(j)), baseSR.GetUpperBound(vars.at(j)));
      }
    }

    //add baseline selections to all CRSL regions 
    std::vector<std::string> varsCRSL = baseSR.GetListOfVariablesCRSL();
    for(unsigned int i = 0; i < SRVec.size(); i++){
      for(unsigned int j = 0; j < varsCRSL.size(); j++){
        SRVec.at(i).SetVarCRSL(varsCRSL.at(j), baseSR.GetLowerBoundCRSL(varsCRSL.at(j)), baseSR.GetUpperBoundCRSL(varsCRSL.at(j)));
      }
    }

    //add baseline selections to all CRDY regions 
    std::vector<std::string> varsCRDY = baseSR.GetListOfVariablesCRDY();
    for(unsigned int i = 0; i < SRVec.size(); i++){
      for(unsigned int j = 0; j < varsCRDY.size(); j++){
        SRVec.at(i).SetVarCRDY(varsCRDY.at(j), baseSR.GetLowerBoundCRDY(varsCRDY.at(j)), baseSR.GetUpperBoundCRDY(varsCRDY.at(j)));
      }
    }

    //add baseline selections to all QCD regions 
    std::vector<std::string> varsCRQCD = baseSR.GetListOfVariablesCRQCD();
    for(unsigned int i = 0; i < SRVec.size(); i++){
      for(unsigned int j = 0; j < varsCRQCD.size(); j++){
        SRVec.at(i).SetVarCRQCD(varsCRQCD.at(j), baseSR.GetLowerBoundCRQCD(varsCRQCD.at(j)), baseSR.GetUpperBoundCRQCD(varsCRQCD.at(j)));
      }
    }

    return SRVec;

  }

  std::vector<SR> getSignalRegions2016vs2017 () {

    std::vector<SR> SRVec, temp_SR_vec;
    SR sr, baseSR;

    // set a few very inclusive regions for comparison between 2016 and 2017 data
    
    // first define njet x nbtag binning
    sr.SetName("LowJetLowTag");
    sr.SetVar("njets", 2, 4); // 2 or 3 jets
    sr.SetVar("nbjets", 0, 1); // 0 b tags
    sr.SetVarCRSL("njets", 2, 4); 
    sr.SetVarCRSL("nbjets", 0, 1);
    sr.SetVarCRDY("njets", 2, 4);
    sr.SetVarCRDY("nbjets", 0, 1);
    sr.SetVarCRQCD("njets", 2, 4);
    sr.SetVarCRQCD("nbjets", 0, 1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("HighJetLowTag");
    sr.SetVar("njets", 4, -1); // 4 or more jets
    sr.SetVar("nbjets", 0, 1); // 0 b tags
    sr.SetVarCRSL("njets", 4, -1); 
    sr.SetVarCRSL("nbjets", 0, 1);
    sr.SetVarCRDY("njets", 4, -1);
    sr.SetVarCRDY("nbjets", 0, 1);
    sr.SetVarCRQCD("njets", 4, -1);
    sr.SetVarCRQCD("nbjets", 0, 1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("LowJetHighTag");
    sr.SetVar("njets", 2, 4); // 2 or 3 jets
    sr.SetVar("nbjets", 1, -1); // nonzero b tags
    sr.SetVarCRSL("njets", 2, 4); 
    sr.SetVarCRSL("nbjets", 1, -1);
    sr.SetVarCRDY("njets", 2, 4);
    sr.SetVarCRDY("nbjets", 1, -1);
    sr.SetVarCRQCD("njets", 2, 4);
    sr.SetVarCRQCD("nbjets", 1, -1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("HighJetHighTag");
    sr.SetVar("njets", 4, -1); // 4 or more jets
    sr.SetVar("nbjets", 1, -1); // nonzero b tags
    sr.SetVarCRSL("njets", 4, -1); 
    sr.SetVarCRSL("nbjets", 1, -1);
    sr.SetVarCRDY("njets", 4, -1);
    sr.SetVarCRDY("nbjets", 1, -1);
    sr.SetVarCRQCD("njets", 4, -1);
    sr.SetVarCRQCD("nbjets", 1, -1);
    temp_SR_vec.push_back(sr);
    sr.Clear();
    
    // now divide into high and low HT
    for(unsigned int iSR = 0; iSR < temp_SR_vec.size(); iSR++){
      SR fullSR = temp_SR_vec.at(iSR);  
      fullSR.SetName(fullSR.GetName() + "HighHT");
      fullSR.SetVar("ht", 1200, -1); // ht at least 1200
      fullSR.SetVar("met", 30, -1); // met at least 30
      fullSR.SetVarCRSL("ht", 1200, -1);
      fullSR.SetVarCRSL("met", 30, -1);
      fullSR.SetVarCRDY("ht", 1200, -1);
      fullSR.SetVarCRDY("met", 30, -1);
      fullSR.SetVarCRQCD("ht", 1200, -1);
      fullSR.SetVarCRQCD("met", 30, -1);
      float mt2bins[4] = {200, 400, 600, 1800};
      fullSR.SetMT2Bins(3,mt2bins);
      SRVec.push_back(fullSR);
    }
    for(unsigned int iSR = 0; iSR < temp_SR_vec.size(); iSR++){
      SR fullSR = temp_SR_vec.at(iSR);  
      fullSR.SetName(fullSR.GetName() + "LowHT");
      fullSR.SetVar("ht", 250, 1200); // ht less than 1200, greater than 250
      fullSR.SetVar("met", 200, -1); // met at least 200
      fullSR.SetVarCRSL("ht", 250, 1200);
      fullSR.SetVarCRSL("met", 250, -1);
      fullSR.SetVarCRDY("ht", 250, 1200);
      fullSR.SetVarCRDY("met", 250, -1);
      fullSR.SetVarCRQCD("ht", 250, 1200);
      fullSR.SetVarCRQCD("met", 250, -1);
      float mt2bins[4] = {200, 400, 600, 1800};
      fullSR.SetMT2Bins(3,mt2bins);
      SRVec.push_back(fullSR);
    }
    
    //define baseline selections commmon to all signal regions 
    baseSR.SetVar("mt2", 200, -1);
    baseSR.SetVar("j1pt", 30, -1);
    baseSR.SetVar("j2pt", 30, -1);
    baseSR.SetVar("deltaPhiMin", 0.3, -1);
    baseSR.SetVar("diffMetMhtOverMet", 0, 0.5);
    baseSR.SetVar("nlep", 0, 1);

    // common selections for CRSL
    baseSR.SetVarCRSL("mt2", 200, -1);
    baseSR.SetVarCRSL("j1pt", 30, -1);
    baseSR.SetVarCRSL("j2pt", 30, -1);
    baseSR.SetVarCRSL("deltaPhiMin", 0.3, -1);
    baseSR.SetVarCRSL("diffMetMhtOverMet", 0, 0.5);
    baseSR.SetVarCRSL("nlep", 1, 2);

    // common selections for CRDY
    baseSR.SetVarCRDY("mt2", 200, -1);
    baseSR.SetVarCRDY("j1pt", 30, -1);
    baseSR.SetVarCRDY("j2pt", 30, -1);
    baseSR.SetVarCRDY("deltaPhiMin", 0.3, -1);
    baseSR.SetVarCRDY("diffMetMhtOverMet", 0, 0.5);
    baseSR.SetVarCRDY("nlep", 0, 1);

    // common selections for QCD
    baseSR.SetVarCRQCD("mt2", 200, -1);
    baseSR.SetVarCRQCD("j1pt", 30, -1);
    baseSR.SetVarCRQCD("j2pt", 30, -1);
    baseSR.SetVarCRQCD("deltaPhiMin", 0., 0.3);
    baseSR.SetVarCRQCD("diffMetMhtOverMet", 0, 0.5);
    baseSR.SetVarCRQCD("nlep", 0, 1);

    //add baseline selections to all signal regions 
    std::vector<std::string> vars = baseSR.GetListOfVariables();
    for(unsigned int i = 0; i < SRVec.size(); i++){
      for(unsigned int j = 0; j < vars.size(); j++){
        SRVec.at(i).SetVar(vars.at(j), baseSR.GetLowerBound(vars.at(j)), baseSR.GetUpperBound(vars.at(j)));
      }
    }

    //add baseline selections to all CRSL regions 
    std::vector<std::string> varsCRSL = baseSR.GetListOfVariablesCRSL();
    for(unsigned int i = 0; i < SRVec.size(); i++){
      for(unsigned int j = 0; j < varsCRSL.size(); j++){
        SRVec.at(i).SetVarCRSL(varsCRSL.at(j), baseSR.GetLowerBoundCRSL(varsCRSL.at(j)), baseSR.GetUpperBoundCRSL(varsCRSL.at(j)));
      }
    }

    //add baseline selections to all CRDY regions 
    std::vector<std::string> varsCRDY = baseSR.GetListOfVariablesCRDY();
    for(unsigned int i = 0; i < SRVec.size(); i++){
      for(unsigned int j = 0; j < varsCRDY.size(); j++){
        SRVec.at(i).SetVarCRDY(varsCRDY.at(j), baseSR.GetLowerBoundCRDY(varsCRDY.at(j)), baseSR.GetUpperBoundCRDY(varsCRDY.at(j)));
      }
    }

    //add baseline selections to all QCD regions 
    std::vector<std::string> varsCRQCD = baseSR.GetListOfVariablesCRQCD();
    for(unsigned int i = 0; i < SRVec.size(); i++){
      for(unsigned int j = 0; j < varsCRQCD.size(); j++){
        SRVec.at(i).SetVarCRQCD(varsCRQCD.at(j), baseSR.GetLowerBoundCRQCD(varsCRQCD.at(j)), baseSR.GetUpperBoundCRQCD(varsCRQCD.at(j)));
      }
    }

    // add the 1VL SR exactly for testing purposes
    SR test1VL; 

    test1VL.SetName("1VL");
    test1VL.SetVar("njets", 2, 4);
    test1VL.SetVar("nbjets", 0, 1);
    test1VL.SetVarCRSL("njets", 2, 4);
    test1VL.SetVarCRSL("nbjets", 0, 1);
    test1VL.SetVarCRDY("njets", 2, 4);
    test1VL.SetVarCRDY("nbjets", 0, 1);
    test1VL.SetVarCRQCD("njets", 0, -1);
    test1VL.SetVarCRQCD("nbjets", 0, -1);
    test1VL.SetVar("ht", 250, 450);
    test1VL.SetVar("met", 250, -1);
    test1VL.SetVarCRSL("ht", 250, 450);
    test1VL.SetVarCRSL("met", 250, -1);
    test1VL.SetVarCRDY("ht", 250, 450);
    test1VL.SetVarCRDY("met", 250, -1);
    test1VL.SetVarCRQCD("ht", 250, 450);
    test1VL.SetVarCRQCD("met", 250, -1);
    float mt2bins[4] = {200, 300, 400, 1500}; 
    test1VL.SetMT2Bins(3, mt2bins);
  
    // SR
    test1VL.SetVar("mt2", 200, -1);
    test1VL.SetVar("j1pt", 30, -1);
    test1VL.SetVar("j2pt", 30, -1);
    test1VL.SetVar("deltaPhiMin", 0.3, -1);
    test1VL.SetVar("diffMetMhtOverMet", 0, 0.5);
    test1VL.SetVar("nlep", 0, 1);

    // CRSL
    test1VL.SetVarCRSL("mt2", 200, -1);
    test1VL.SetVarCRSL("j1pt", 30, -1);
    test1VL.SetVarCRSL("j2pt", 30, -1);
    test1VL.SetVarCRSL("deltaPhiMin", 0.3, -1);
    test1VL.SetVarCRSL("diffMetMhtOverMet", 0, 0.5);
    test1VL.SetVarCRSL("nlep", 1, 2);

    // CRDY
    test1VL.SetVarCRDY("mt2", 200, -1);
    test1VL.SetVarCRDY("j1pt", 30, -1);
    test1VL.SetVarCRDY("j2pt", 30, -1);
    test1VL.SetVarCRDY("deltaPhiMin", 0.3, -1);
    test1VL.SetVarCRDY("diffMetMhtOverMet", 0, 0.5);
    test1VL.SetVarCRDY("nlep", 0, 1);

    // QCD
    test1VL.SetVarCRQCD("mt2", 200, -1);
    test1VL.SetVarCRQCD("j1pt", 30, -1);
    test1VL.SetVarCRQCD("j2pt", 30, -1);
    test1VL.SetVarCRQCD("deltaPhiMin", 0., 0.3);
    test1VL.SetVarCRQCD("diffMetMhtOverMet", 0, 0.5);
    test1VL.SetVarCRQCD("nlep", 0, 1);

    SRVec.push_back(test1VL);

    return SRVec;
  }

  std::vector<SR> getSignalRegionsMonojet2016vs2017(){

    std::vector<SR> temp_SR_vec;
    std::vector<SR> SRVec;
    SR baseSR;

    // define baseline selections commmon to all monojet regions, same as standard 2016 selections
    float mt2bins_monojet[2] = {0, 1500};
    baseSR.SetVar("nlep", 0, 1);
    baseSR.SetVar("njets", 1, 2);
    baseSR.SetVar("met", 250, -1);
    baseSR.SetVar("deltaPhiMin", 0.3, -1);
    baseSR.SetVar("diffMetMhtOverMet", 0, 0.5);
    baseSR.SetVarCRSL("nlep", 1, 2);
    baseSR.SetVarCRSL("njets", 1, 2);
    baseSR.SetVarCRSL("met", 250, -1);
    baseSR.SetVarCRSL("deltaPhiMin", 0.3, -1);
    baseSR.SetVarCRSL("diffMetMhtOverMet", 0, 0.5);
    baseSR.SetVarCRDY("nlep", 0, 1);
    baseSR.SetVarCRDY("njets", 1, 2);
    baseSR.SetVarCRDY("met", 250, -1);
    baseSR.SetVarCRDY("deltaPhiMin", 0.3, -1);
    baseSR.SetVarCRDY("diffMetMhtOverMet", 0, 0.5);
    // QCD region: 2 jets, low deltaPhiMin, pt subleading between 30 and 60 GeV
    baseSR.SetVarCRQCD("j1pt", 250, -1);
    baseSR.SetVarCRQCD("j2pt", 30, 60);
    baseSR.SetVarCRQCD("nlep", 0, 1);
    baseSR.SetVarCRQCD("njets", 2, 3);
    baseSR.SetVarCRQCD("met", 250, -1);
    baseSR.SetVarCRQCD("deltaPhiMin", 0., 0.3);
    baseSR.SetVarCRQCD("diffMetMhtOverMet", 0, 0.5);
    baseSR.SetMT2Bins(1, mt2bins_monojet);

    // broader HT binning than 2016 analysis, just hi and low
    const unsigned int nbins_monojet_0b = 2;
    float htbins_0b[nbins_monojet_0b+1] = {250, 750, -1};
    float htbins_0b_forplot[nbins_monojet_0b+1] = {250, 750, 1500};
    const unsigned int nbins_monojet_1b = 2;
    float htbins_1b[nbins_monojet_1b+1] = {250, 750, -1};
    float htbins_1b_forplot[nbins_monojet_1b+1] = {250, 750, 1500};

    temp_SR_vec.clear();
    for(unsigned int iSR = 0; iSR < nbins_monojet_0b; iSR++){
      SR fullSR0b = baseSR;  
      fullSR0b.SetName(std::to_string(iSR+1) + "J");
      fullSR0b.SetVar("ht", htbins_0b[iSR], htbins_0b[iSR+1]);
      fullSR0b.SetVar("nbjets", 0, 1);
      fullSR0b.SetVarCRSL("ht", htbins_0b[iSR], htbins_0b[iSR+1]);
      fullSR0b.SetVarCRSL("nbjets", 0, 1);
      fullSR0b.SetVarCRDY("ht", htbins_0b[iSR], htbins_0b[iSR+1]);
      fullSR0b.SetVarCRDY("nbjets", 0, 1);
      fullSR0b.SetVarCRQCD("ht", htbins_0b[iSR], htbins_0b[iSR+1]);
      fullSR0b.SetVarCRQCD("nbjets", 0, 1);
      SRVec.push_back(fullSR0b);
    }
    for(unsigned int iSR = 0; iSR < nbins_monojet_1b; iSR++){
      SR fullSR1b = baseSR;  
      fullSR1b.SetName(std::to_string(iSR+11) + "J");
      fullSR1b.SetVar("ht", htbins_1b[iSR], htbins_1b[iSR+1]);
      fullSR1b.SetVar("nbjets", 1, -1);
      fullSR1b.SetVarCRSL("ht", htbins_1b[iSR], htbins_1b[iSR+1]);
      fullSR1b.SetVarCRSL("nbjets", 1, -1);
      fullSR1b.SetVarCRDY("ht", htbins_1b[iSR], htbins_1b[iSR+1]);
      fullSR1b.SetVarCRDY("nbjets", 1, -1);
      fullSR1b.SetVarCRQCD("ht", htbins_1b[iSR], htbins_1b[iSR+1]);
      fullSR1b.SetVarCRQCD("nbjets", 1, -1);
      SRVec.push_back(fullSR1b);
    }

    return SRVec;

  }  


  std::vector<SR> getSignalRegions2016Edit(){

    std::vector<SR> temp_SR_vec;
    std::vector<SR> temp_SR_vec2;
    std::vector<SR> SRVec;
    SR sr;
    SR baseSR;

    //first set binning in njet-nbjet plane
    sr.SetName("1");
    sr.SetVar("njets", 2, 4);
    sr.SetVar("nbjets", 0, 1);
    sr.SetVarCRSL("njets", 2, 4);
    sr.SetVarCRSL("nbjets", 0, 1);
    sr.SetVarCRDY("njets", 2, 4);
    sr.SetVarCRDY("nbjets", 0, 1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("2");
    sr.SetVar("njets", 2, 4);
    sr.SetVar("nbjets", 1, 2);
    sr.SetVarCRSL("njets", 2, 4);
    sr.SetVarCRSL("nbjets", 1, 2);
    sr.SetVarCRDY("njets", 2, 4);
    sr.SetVarCRDY("nbjets", 1, 2);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("3");
    sr.SetVar("njets", 2, 4);
    sr.SetVar("nbjets", 2, 3);
    sr.SetVarCRSL("njets", 2, 4);
    sr.SetVarCRSL("nbjets", 2, 3);
    sr.SetVarCRDY("njets", 2, 4);
    sr.SetVarCRDY("nbjets", 2, 3);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("4");
    sr.SetVar("njets", 4, 7);
    sr.SetVar("nbjets", 0, 1);
    sr.SetVarCRSL("njets", 4, 7);
    sr.SetVarCRSL("nbjets", 0, 1);
    sr.SetVarCRDY("njets", 4, 7);
    sr.SetVarCRDY("nbjets", 0, 1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("5");
    sr.SetVar("njets", 4, 7);
    sr.SetVar("nbjets", 1, 2);
    sr.SetVarCRSL("njets", 4, 7);
    sr.SetVarCRSL("nbjets", 1, 2);
    sr.SetVarCRDY("njets", 4, 7);
    sr.SetVarCRDY("nbjets", 1, 2);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("6");
    sr.SetVar("njets", 4, 7);
    sr.SetVar("nbjets", 2, 3);
    sr.SetVarCRSL("njets", 4, 7);
    sr.SetVarCRSL("nbjets", 2, 3);
    sr.SetVarCRDY("njets", 4, 7);
    sr.SetVarCRDY("nbjets", 2, 3);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("7");
    sr.SetVar("njets", 7, -1);
    sr.SetVar("nbjets", 0, 1);
    sr.SetVarCRSL("njets", 7, -1);
    sr.SetVarCRSL("nbjets", 0, 1);
    sr.SetVarCRDY("njets", 7, -1);
    sr.SetVarCRDY("nbjets", 0, 1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    // shared CR: 7j1-2b
    sr.SetName("8");
    sr.SetVar("njets", 7, -1);
    sr.SetVar("nbjets", 1, 2);
    sr.SetVarCRSL("njets", 7, -1);
    sr.SetVarCRSL("nbjets", 1, 3);
    sr.SetVarCRDY("njets", 7, -1);
    sr.SetVarCRDY("nbjets", 1, -1); // DY: NB>0
    temp_SR_vec.push_back(sr);
    sr.Clear();

    // shared CR: 7j1-2b
    sr.SetName("9");
    sr.SetVar("njets", 7, -1);
    sr.SetVar("nbjets", 2, 3);
    sr.SetVarCRSL("njets", 7, -1);
    sr.SetVarCRSL("nbjets", 1, 3);
    sr.SetVarCRDY("njets", 7, -1);
    sr.SetVarCRDY("nbjets", 1, -1); // DY: NB>0
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("10");
    sr.SetVar("njets", 2, 7);
    sr.SetVar("nbjets", 3, -1);
    sr.SetVarCRSL("njets", 2, 7);
    sr.SetVarCRSL("nbjets", 3, -1);
    sr.SetVarCRDY("njets", 2, 7);
    sr.SetVarCRDY("nbjets", 3, -1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    // shared CR: 7j1-2b
    sr.SetName("11");
    sr.SetVar("njets", 7, -1);
    sr.SetVar("nbjets", 3, -1);
    sr.SetVarCRSL("njets", 7, -1);
    sr.SetVarCRSL("nbjets", 1, 3);
    sr.SetVarCRDY("njets", 7, -1);
    sr.SetVarCRDY("nbjets", 1, -1); // DY: NB>0
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("12");
    sr.SetVar("njets", 4, -1);
    sr.SetVar("nbjets", 0, 1);
    sr.SetVarCRSL("njets", 4, -1);
    sr.SetVarCRSL("nbjets", 0, 1);
    sr.SetVarCRDY("njets", 4, -1);
    sr.SetVarCRDY("nbjets", 0, 1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("13");
    sr.SetVar("njets", 4, -1);
    sr.SetVar("nbjets", 1, 2);
    sr.SetVarCRSL("njets", 4, -1);
    sr.SetVarCRSL("nbjets", 1, 2);
    sr.SetVarCRDY("njets", 4, -1);
    sr.SetVarCRDY("nbjets", 1, 2);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("14");
    sr.SetVar("njets", 4, -1);
    sr.SetVar("nbjets", 2, 3);
    sr.SetVarCRSL("njets", 4, -1);
    sr.SetVarCRSL("nbjets", 2, 3);
    sr.SetVarCRDY("njets", 4, -1);
    sr.SetVarCRDY("nbjets", 2, 3);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("15");
    sr.SetVar("njets", 2, -1);
    sr.SetVar("nbjets", 3, -1);
    sr.SetVarCRSL("njets", 2, -1);
    sr.SetVarCRSL("nbjets", 3, -1);
    sr.SetVarCRDY("njets", 2, -1);
    sr.SetVarCRDY("nbjets", 3, -1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("16");
    sr.SetVar("njets", 2, -1);
    sr.SetVar("nbjets", 0, -1);
    sr.SetVarCRSL("njets", 2, -1);
    sr.SetVarCRSL("nbjets", 0, -1);
    sr.SetVarCRDY("njets", 2, -1);
    sr.SetVarCRDY("nbjets", 0, -1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("17");
    sr.SetVar("njets", 4, -1);
    sr.SetVar("nbjets", 0, -1);
    sr.SetVarCRSL("njets", 4, -1);
    sr.SetVarCRSL("nbjets", 0, -1);
    sr.SetVarCRDY("njets", 4, -1);
    sr.SetVarCRDY("nbjets", 0, -1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("18");
    sr.SetVar("njets", 7, -1);
    sr.SetVar("nbjets", 0, -1);
    sr.SetVarCRSL("njets", 7, -1);
    sr.SetVarCRSL("nbjets", 0, -1);
    sr.SetVarCRDY("njets", 7, -1);
    sr.SetVarCRDY("nbjets", 0, -1);
    temp_SR_vec.push_back(sr);
    sr.Clear();

    sr.SetName("19");
    sr.SetVar("njets", 2, -1);
    sr.SetVar("nbjets", 2, -1);
    sr.SetVarCRSL("njets", 2, -1);
    sr.SetVarCRSL("nbjets", 2, -1);
    sr.SetVarCRDY("njets", 2, -1);
    sr.SetVarCRDY("nbjets", 2, -1);
    temp_SR_vec.push_back(sr);
    sr.Clear();
    
    //add HT and MET requirements
    for(unsigned int iSR = 0; iSR < temp_SR_vec.size(); iSR++){
      if (iSR >= 3 && iSR <= 10) continue;
      if (iSR >= 15) continue;
      SR fullSR = temp_SR_vec.at(iSR); 
      fullSR.SetName(fullSR.GetName() + "VL");
      fullSR.SetVar("ht", 250, 450);
      fullSR.SetVar("met", 250, -1);
      fullSR.SetVarCRSL("ht", 250, 450);
      fullSR.SetVarCRSL("met", 250, -1);
      fullSR.SetVarCRDY("ht", 250, 450);
      fullSR.SetVarCRDY("met", 250, -1);
      fullSR.SetVarCRQCD("ht", 250, 450);
      fullSR.SetVarCRQCD("met", 250, -1);
      int njets_lo = fullSR.GetLowerBound("njets");
      int nbjets_lo = fullSR.GetLowerBound("nbjets");
      if     (njets_lo == 2 && nbjets_lo == 0){float mt2bins[4] = {200, 300, 400, 1500}; fullSR.SetMT2Bins(3, mt2bins);}
      else if(njets_lo == 2 && nbjets_lo == 1){float mt2bins[4] = {200, 300, 400, 1500}; fullSR.SetMT2Bins(3, mt2bins);}
      else if(njets_lo == 2 && nbjets_lo == 2){float mt2bins[4] = {200, 300, 400, 1500}; fullSR.SetMT2Bins(3, mt2bins);}
      else if(njets_lo == 4 && nbjets_lo == 0){float mt2bins[4] = {200, 300, 400, 1500}; fullSR.SetMT2Bins(3, mt2bins);}
      else if(njets_lo == 4 && nbjets_lo == 1){float mt2bins[4] = {200, 300, 400, 1500}; fullSR.SetMT2Bins(3, mt2bins);}
      else if(njets_lo == 4 && nbjets_lo == 2){float mt2bins[4] = {200, 300, 400, 1500}; fullSR.SetMT2Bins(3, mt2bins);}
      else if(njets_lo == 2 && nbjets_lo == 3){float mt2bins[4] = {200, 300, 400, 1500}; fullSR.SetMT2Bins(3, mt2bins);}
      temp_SR_vec2.push_back(fullSR);
    }
    for(unsigned int iSR = 0; iSR < temp_SR_vec.size(); iSR++){
      if (iSR >= 11) continue;
      SR fullSR = temp_SR_vec.at(iSR);      
      fullSR.SetName(fullSR.GetName() + "L");
      fullSR.SetVar("ht", 450, 575);
      fullSR.SetVar("met", 250, -1);
      fullSR.SetVarCRSL("ht", 450, 575);
      fullSR.SetVarCRSL("met", 250, -1);
      fullSR.SetVarCRDY("ht", 450, 575);
      fullSR.SetVarCRDY("met", 250, -1);
      fullSR.SetVarCRQCD("ht", 450, 575);
      fullSR.SetVarCRQCD("met", 250, -1);
      int njets_lo = fullSR.GetLowerBound("njets");
      int nbjets_lo = fullSR.GetLowerBound("nbjets");
      if     (njets_lo == 2 && nbjets_lo == 0){float mt2bins[5] = {200, 300, 400, 500, 1500}; fullSR.SetMT2Bins(4, mt2bins);}
      else if(njets_lo == 2 && nbjets_lo == 1){float mt2bins[5] = {200, 300, 400, 500, 1500}; fullSR.SetMT2Bins(4, mt2bins);}
      else if(njets_lo == 2 && nbjets_lo == 2){float mt2bins[5] = {200, 300, 400, 500, 1500}; fullSR.SetMT2Bins(4, mt2bins);}
      else if(njets_lo == 4 && nbjets_lo == 0){float mt2bins[5] = {200, 300, 400, 500, 1500}; fullSR.SetMT2Bins(4, mt2bins);}
      else if(njets_lo == 4 && nbjets_lo == 1){float mt2bins[5] = {200, 300, 400, 500, 1500}; fullSR.SetMT2Bins(4, mt2bins);}
      else if(njets_lo == 4 && nbjets_lo == 2){float mt2bins[5] = {200, 300, 400, 500, 1500}; fullSR.SetMT2Bins(4, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 0){float mt2bins[4] = {200, 300, 400, 1500};      fullSR.SetMT2Bins(3, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 1){float mt2bins[4] = {200, 300, 400, 1500};      fullSR.SetMT2Bins(3, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 2){float mt2bins[4] = {200, 300, 400, 1500};      fullSR.SetMT2Bins(3, mt2bins);}
      else if(njets_lo == 2 && nbjets_lo == 3){float mt2bins[5] = {200, 300, 400, 500, 1500}; fullSR.SetMT2Bins(4, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 3){float mt2bins[4] = {200, 300, 400, 1500};      fullSR.SetMT2Bins(3, mt2bins);}
      temp_SR_vec2.push_back(fullSR);
    }
    for(unsigned int iSR = 0; iSR < temp_SR_vec.size(); iSR++){
      if (iSR >= 11) continue;
      SR fullSR = temp_SR_vec.at(iSR);  
      fullSR.SetName(fullSR.GetName() + "M");
      fullSR.SetVar("ht", 575, 1000);
      fullSR.SetVar("met", 250, -1);
      fullSR.SetVarCRSL("ht", 575, 1000);
      fullSR.SetVarCRSL("met", 250, -1);
      fullSR.SetVarCRDY("ht", 575, 1000);
      fullSR.SetVarCRDY("met", 250, -1);
      fullSR.SetVarCRQCD("ht", 575, 1000);
      fullSR.SetVarCRQCD("met", 250, -1);
      int njets_lo = fullSR.GetLowerBound("njets");
      int nbjets_lo = fullSR.GetLowerBound("nbjets");
      if     (njets_lo == 2 && nbjets_lo == 0){float mt2bins[6] = {200, 300, 400, 600, 800, 1500}; fullSR.SetMT2Bins(5, mt2bins);}
      else if(njets_lo == 2 && nbjets_lo == 1){float mt2bins[6] = {200, 300, 400, 600, 800, 1500}; fullSR.SetMT2Bins(5, mt2bins);}
      else if(njets_lo == 2 && nbjets_lo == 2){float mt2bins[6] = {200, 300, 400, 600, 800, 1500}; fullSR.SetMT2Bins(5, mt2bins);}
      else if(njets_lo == 4 && nbjets_lo == 0){float mt2bins[6] = {200, 300, 400, 600, 800, 1500}; fullSR.SetMT2Bins(5, mt2bins);}
      else if(njets_lo == 4 && nbjets_lo == 1){float mt2bins[6] = {200, 300, 400, 600, 800, 1500}; fullSR.SetMT2Bins(5, mt2bins);}
      else if(njets_lo == 4 && nbjets_lo == 2){float mt2bins[6] = {200, 300, 400, 600, 800, 1500}; fullSR.SetMT2Bins(5, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 0){float mt2bins[6] = {200, 300, 400, 600, 800, 1500}; fullSR.SetMT2Bins(5, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 1){float mt2bins[5] = {200, 300, 400, 600, 1500};      fullSR.SetMT2Bins(4, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 2){float mt2bins[5] = {200, 300, 400, 600, 1500};      fullSR.SetMT2Bins(4, mt2bins);}
      else if(njets_lo == 2 && nbjets_lo == 3){float mt2bins[5] = {200, 300, 400, 600, 1500};      fullSR.SetMT2Bins(4, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 3){float mt2bins[5] = {200, 300, 400, 600, 1500};      fullSR.SetMT2Bins(4, mt2bins);}
      temp_SR_vec2.push_back(fullSR);
    }
    for(unsigned int iSR = 0; iSR < temp_SR_vec.size(); iSR++){
      if (iSR >= 11) continue;      
      SR fullSR = temp_SR_vec.at(iSR);  
      fullSR.SetName(fullSR.GetName() + "H");
      fullSR.SetVar("ht", 1000, 1500);
      fullSR.SetVar("met", 30, -1);
      fullSR.SetVarCRSL("ht", 1000, 1500);
      fullSR.SetVarCRSL("met", 30, -1);
      fullSR.SetVarCRDY("ht", 1000, 1500);
      fullSR.SetVarCRDY("met", 30, -1);
      fullSR.SetVarCRQCD("ht", 1000, 1500);
      fullSR.SetVarCRQCD("met", 30, -1);
      int njets_lo = fullSR.GetLowerBound("njets");
      int nbjets_lo = fullSR.GetLowerBound("nbjets");
      if     (njets_lo == 2 && nbjets_lo == 0){float mt2bins[7] = {200, 400, 600, 800, 1000, 1200, 1500}; fullSR.SetMT2Bins(6, mt2bins);}
      else if(njets_lo == 2 && nbjets_lo == 1){float mt2bins[7] = {200, 400, 600, 800, 1000, 1200, 1500}; fullSR.SetMT2Bins(6, mt2bins);}
      else if(njets_lo == 2 && nbjets_lo == 2){float mt2bins[6] = {200, 400, 600, 800, 1000, 1500};       fullSR.SetMT2Bins(5, mt2bins);}
      else if(njets_lo == 4 && nbjets_lo == 0){float mt2bins[7] = {200, 400, 600, 800, 1000, 1200, 1500}; fullSR.SetMT2Bins(6, mt2bins);}
      else if(njets_lo == 4 && nbjets_lo == 1){float mt2bins[7] = {200, 400, 600, 800, 1000, 1200, 1500}; fullSR.SetMT2Bins(6, mt2bins);}
      else if(njets_lo == 4 && nbjets_lo == 2){float mt2bins[6] = {200, 400, 600, 800, 1000, 1500};       fullSR.SetMT2Bins(5, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 0){float mt2bins[6] = {200, 400, 600, 800, 1000, 1500};       fullSR.SetMT2Bins(5, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 1){float mt2bins[5] = {200, 400, 600, 800, 1500};             fullSR.SetMT2Bins(4, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 2){float mt2bins[5] = {200, 400, 600, 800, 1500};             fullSR.SetMT2Bins(4, mt2bins);}
      else if(njets_lo == 2 && nbjets_lo == 3){float mt2bins[4] = {200, 400, 600, 1500};                  fullSR.SetMT2Bins(3, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 3){float mt2bins[4] = {200, 400, 600, 1500};                  fullSR.SetMT2Bins(3, mt2bins);}
      temp_SR_vec2.push_back(fullSR);
    }
    for(unsigned int iSR = 0; iSR < temp_SR_vec.size(); iSR++){
      if (iSR >= 11) continue;
      SR fullSR = temp_SR_vec.at(iSR);  
      fullSR.SetName(fullSR.GetName() + "UH");
      fullSR.SetVar("ht", 1500, -1);
      fullSR.SetVar("met", 30, -1);
      fullSR.SetVarCRSL("ht", 1500, -1);
      fullSR.SetVarCRSL("met", 30, -1);
      fullSR.SetVarCRDY("ht", 1500, -1);
      fullSR.SetVarCRDY("met", 30, -1);
      fullSR.SetVarCRQCD("ht", 1500, -1);
      fullSR.SetVarCRQCD("met", 30, -1);
      int njets_lo = fullSR.GetLowerBound("njets");
      int nbjets_lo = fullSR.GetLowerBound("nbjets");
      int njets_hi = fullSR.GetUpperBound("njets");
      if     (njets_lo == 2 && nbjets_lo == 0){float mt2bins[7] = {200, 400, 600, 800, 1000, 1400, 1800}; fullSR.SetMT2Bins(6, mt2bins);}
      else if(njets_lo == 2 && nbjets_lo == 1){float mt2bins[6] = {200, 400, 600, 800, 1000, 1800};       fullSR.SetMT2Bins(5, mt2bins);}
      else if(njets_lo == 2 && nbjets_lo == 2){
        float mt2bins[3] = {200, 400, 1800};
        float mt2bins_ssr[4] = {200, 400, 600, 1800};
        if (njets_hi == -1)
          fullSR.SetMT2Bins(3, mt2bins_ssr);
        else
          fullSR.SetMT2Bins(2, mt2bins);          
      }      
      else if(njets_lo == 4 && nbjets_lo == 0){float mt2bins[7] = {200, 400, 600, 800, 1000, 1400, 1800}; fullSR.SetMT2Bins(6, mt2bins);}
      else if(njets_lo == 4 && nbjets_lo == 1){float mt2bins[7] = {200, 400, 600, 800, 1000, 1400, 1800}; fullSR.SetMT2Bins(6, mt2bins);}
      else if(njets_lo == 4 && nbjets_lo == 2){float mt2bins[5] = {200, 400, 600, 800, 1800};             fullSR.SetMT2Bins(4, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 0){float mt2bins[6] = {200, 400, 600, 800, 1000, 1800};       fullSR.SetMT2Bins(5, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 1){float mt2bins[5] = {200, 400, 600, 800, 1800};             fullSR.SetMT2Bins(4, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 2){float mt2bins[5] = {200, 400, 600, 800, 1800};             fullSR.SetMT2Bins(4, mt2bins);}
      else if(njets_lo == 2 && nbjets_lo == 3){float mt2bins[4] = {200, 400, 600, 1800};                  fullSR.SetMT2Bins(3, mt2bins);}
      else if(njets_lo == 7 && nbjets_lo == 3){float mt2bins[3] = {200, 400, 1800};                       fullSR.SetMT2Bins(2, mt2bins);}
      temp_SR_vec2.push_back(fullSR);
    }

    // Add Eta binning
    for(unsigned int iSR = 0; iSR < temp_SR_vec2.size(); iSR++){
      SR fullSRcent = temp_SR_vec2.at(iSR);  
      fullSRcent.SetName(fullSRcent.GetName() + "cent"); // barrel / central
      fullSRcent.SetVar("PseudoJet1_eta", 0, 1.4);      
      fullSRcent.SetVarCRQCD("PseudoJet1_eta", 0, 1.4);      
      fullSRcent.SetVarCRDY("PseudoJet1_eta", 0, 1.4);      
      fullSRcent.SetVarCRSL("PseudoJet1_eta", 0, 1.4);      
      SRVec.push_back(fullSRcent);
      SR fullSRforw = temp_SR_vec2.at(iSR);  
      fullSRforw.SetName(fullSRforw.GetName() + "forw"); // forward / cap
      fullSRforw.SetVar("PseudoJet1_eta", 1.4, -1);      
      fullSRforw.SetVarCRQCD("PseudoJet1_eta", 1.4, -1);      
      fullSRforw.SetVarCRDY("PseudoJet1_eta", 1.4, -1);      
      fullSRforw.SetVarCRSL("PseudoJet1_eta", 1.4, -1);      
      SRVec.push_back(fullSRforw);
    }

    // --- super signal regions, numbered 20 and up
    //     inclusive in HT, NJ, NB
    //     MT2 binning matches the most granular TR that would be included
    //     MT2 cut will be applied later by summing appropriate datacards

    // Note: Added inclusive PseudoJet1_eta cut so the list of variables is the correct length

    // 2j loose
    sr.SetName("20");
    sr.SetVar("njets", 2, -1);
    sr.SetVar("nbjets", 0, -1);
    sr.SetVar("ht", 1000, -1);
    sr.SetVar("met", 30, -1);
    sr.SetVarCRSL("njets", 2, -1);
    sr.SetVarCRSL("nbjets", 0, -1);
    sr.SetVarCRSL("ht", 1000, -1);
    sr.SetVarCRSL("met", 30, -1);
    sr.SetVarCRDY("njets", 2, -1);
    sr.SetVarCRDY("nbjets", 0, -1);
    sr.SetVarCRDY("ht", 1000, -1);
    sr.SetVarCRDY("met", 30, -1);
    sr.SetVarCRQCD("ht", 1000, -1);
    sr.SetVarCRQCD("met", 30, -1);
    sr.SetVar("PseudoJet1_eta", 0, -1);
    sr.SetVarCRSL("PseudoJet1_eta", 0, -1);
    sr.SetVarCRQCD("PseudoJet1_eta", 0, -1);
    sr.SetVarCRDY("PseudoJet1_eta", 0, -1);
    float mt2bins_sr20[] = {200, 400, 600, 800, 1000, 1200, 1800};
    sr.SetMT2Bins(6, mt2bins_sr20);
    SRVec.push_back(sr);
    sr.Clear();
    
    // 2j tight
    sr.SetName("21");
    sr.SetVar("njets", 2, -1);
    sr.SetVar("nbjets", 0, -1);
    sr.SetVar("ht", 1500, -1);
    sr.SetVar("met", 30, -1);
    sr.SetVarCRSL("njets", 2, -1);
    sr.SetVarCRSL("nbjets", 0, -1);
    sr.SetVarCRSL("ht", 1500, -1);
    sr.SetVarCRSL("met", 30, -1);
    sr.SetVarCRDY("njets", 2, -1);
    sr.SetVarCRDY("nbjets", 0, -1);
    sr.SetVarCRDY("ht", 1500, -1);
    sr.SetVarCRDY("met", 30, -1);
    sr.SetVarCRQCD("ht", 1500, -1);
    sr.SetVarCRQCD("met", 30, -1);
    sr.SetVar("PseudoJet1_eta", 0, -1);
    sr.SetVarCRSL("PseudoJet1_eta", 0, -1);
    sr.SetVarCRQCD("PseudoJet1_eta", 0, -1);
    sr.SetVarCRDY("PseudoJet1_eta", 0, -1);
    float mt2bins_sr21[] = {200, 400, 600, 800, 1000, 1400, 1800};
    sr.SetMT2Bins(6, mt2bins_sr21);
    SRVec.push_back(sr);
    sr.Clear();
    
    // 4j loose
    sr.SetName("22");
    sr.SetVar("njets", 4, -1);
    sr.SetVar("nbjets", 0, -1);
    sr.SetVar("ht", 1000, -1);
    sr.SetVar("met", 30, -1);
    sr.SetVarCRSL("njets", 4, -1);
    sr.SetVarCRSL("nbjets", 0, -1);
    sr.SetVarCRSL("ht", 1000, -1);
    sr.SetVarCRSL("met", 30, -1);
    sr.SetVarCRDY("njets", 4, -1);
    sr.SetVarCRDY("nbjets", 0, -1);
    sr.SetVarCRDY("ht", 1000, -1);
    sr.SetVarCRDY("met", 30, -1);
    sr.SetVarCRQCD("ht", 1000, -1);
    sr.SetVarCRQCD("met", 30, -1);
    sr.SetVar("PseudoJet1_eta", 0, -1);
    sr.SetVarCRSL("PseudoJet1_eta", 0, -1);
    sr.SetVarCRQCD("PseudoJet1_eta", 0, -1);
    sr.SetVarCRDY("PseudoJet1_eta", 0, -1);
    float mt2bins_sr22[] = {200, 400, 600, 800, 1000, 1200, 1800};
    sr.SetMT2Bins(6, mt2bins_sr22);
    SRVec.push_back(sr);
    sr.Clear();
    
    // 4j tight
    sr.SetName("23");
    sr.SetVar("njets", 4, -1);
    sr.SetVar("nbjets", 0, -1);
    sr.SetVar("ht", 1500, -1);
    sr.SetVar("met", 30, -1);
    sr.SetVarCRSL("njets", 4, -1);
    sr.SetVarCRSL("nbjets", 0, -1);
    sr.SetVarCRSL("ht", 1500, -1);
    sr.SetVarCRSL("met", 30, -1);
    sr.SetVarCRDY("njets", 4, -1);
    sr.SetVarCRDY("nbjets", 0, -1);
    sr.SetVarCRDY("ht", 1500, -1);
    sr.SetVarCRDY("met", 30, -1);
    sr.SetVarCRQCD("ht", 1500, -1);
    sr.SetVarCRQCD("met", 30, -1);
    sr.SetVar("PseudoJet1_eta", 0, -1);
    sr.SetVarCRSL("PseudoJet1_eta", 0, -1);
    sr.SetVarCRQCD("PseudoJet1_eta", 0, -1);
    sr.SetVarCRDY("PseudoJet1_eta", 0, -1);
    float mt2bins_sr23[] = {200, 400, 600, 800, 1000, 1400, 1800};
    sr.SetMT2Bins(6, mt2bins_sr23);
    SRVec.push_back(sr);
    sr.Clear();
    
    // 7j loose
    sr.SetName("24");
    sr.SetVar("njets", 7, -1);
    sr.SetVar("nbjets", 0, -1);
    sr.SetVar("ht", 1000, -1);
    sr.SetVar("met", 30, -1);
    sr.SetVarCRSL("njets", 7, -1);
    sr.SetVarCRSL("nbjets", 0, -1);
    sr.SetVarCRSL("ht", 1000, -1);
    sr.SetVarCRSL("met", 30, -1);
    sr.SetVarCRDY("njets", 7, -1);
    sr.SetVarCRDY("nbjets", 0, -1);
    sr.SetVarCRDY("ht", 1000, -1);
    sr.SetVarCRDY("met", 30, -1);
    sr.SetVarCRQCD("ht", 1000, -1);
    sr.SetVarCRQCD("met", 30, -1);
    sr.SetVar("PseudoJet1_eta", 0, -1);
    sr.SetVarCRSL("PseudoJet1_eta", 0, -1);
    sr.SetVarCRQCD("PseudoJet1_eta", 0, -1);
    sr.SetVarCRDY("PseudoJet1_eta", 0, -1);
    float mt2bins_sr24[] = {200, 400, 600, 800, 1000, 1800};
    sr.SetMT2Bins(5, mt2bins_sr24);
    SRVec.push_back(sr);
    sr.Clear();
    
    // 7j tight
    sr.SetName("25");
    sr.SetVar("njets", 7, -1);
    sr.SetVar("nbjets", 0, -1);
    sr.SetVar("ht", 1500, -1);
    sr.SetVar("met", 30, -1);
    sr.SetVarCRSL("njets", 7, -1);
    sr.SetVarCRSL("nbjets", 0, -1);
    sr.SetVarCRSL("ht", 1500, -1);
    sr.SetVarCRSL("met", 30, -1);
    sr.SetVarCRDY("njets", 7, -1);
    sr.SetVarCRDY("nbjets", 0, -1);
    sr.SetVarCRDY("ht", 1500, -1);
    sr.SetVarCRDY("met", 30, -1);
    sr.SetVarCRQCD("ht", 1500, -1);
    sr.SetVarCRQCD("met", 30, -1);
    sr.SetVar("PseudoJet1_eta", 0, -1);
    sr.SetVarCRSL("PseudoJet1_eta", 0, -1);
    sr.SetVarCRQCD("PseudoJet1_eta", 0, -1);
    sr.SetVarCRDY("PseudoJet1_eta", 0, -1);
    float mt2bins_sr25[] = {200, 400, 600, 800, 1000, 1800};
    sr.SetMT2Bins(5, mt2bins_sr25);
    SRVec.push_back(sr);
    sr.Clear();
    

    // 2b loose
    sr.SetName("26");
    sr.SetVar("njets", 2, -1);
    sr.SetVar("nbjets", 2, -1);
    sr.SetVar("ht", 1000, -1);
    sr.SetVar("met", 30, -1);
    sr.SetVarCRSL("njets", 2, -1);
    sr.SetVarCRSL("nbjets", 2, -1);
    sr.SetVarCRSL("ht", 1000, -1);
    sr.SetVarCRSL("met", 30, -1);
    sr.SetVarCRDY("njets", 2, -1);
    sr.SetVarCRDY("nbjets", 2, -1);
    sr.SetVarCRDY("ht", 1000, -1);
    sr.SetVarCRDY("met", 30, -1);
    sr.SetVarCRQCD("ht", 1000, -1);
    sr.SetVarCRQCD("met", 30, -1);
    sr.SetVar("PseudoJet1_eta", 0, -1);
    sr.SetVarCRSL("PseudoJet1_eta", 0, -1);
    sr.SetVarCRQCD("PseudoJet1_eta", 0, -1);
    sr.SetVarCRDY("PseudoJet1_eta", 0, -1);
    float mt2bins_sr26[] = {200, 400, 600, 800, 1000, 1800};
    sr.SetMT2Bins(5, mt2bins_sr26);
    SRVec.push_back(sr);
    sr.Clear();
    
    // 2b tight
    sr.SetName("27");
    sr.SetVar("njets", 2, -1);
    sr.SetVar("nbjets", 2, -1);
    sr.SetVar("ht", 1500, -1);
    sr.SetVar("met", 30, -1);
    sr.SetVarCRSL("njets", 2, -1);
    sr.SetVarCRSL("nbjets", 2, -1);
    sr.SetVarCRSL("ht", 1500, -1);
    sr.SetVarCRSL("met", 30, -1);
    sr.SetVarCRDY("njets", 2, -1);
    sr.SetVarCRDY("nbjets", 2, -1);
    sr.SetVarCRDY("ht", 1500, -1);
    sr.SetVarCRDY("met", 30, -1);
    sr.SetVarCRQCD("ht", 1500, -1);
    sr.SetVarCRQCD("met", 30, -1);
    sr.SetVar("PseudoJet1_eta", 0, -1);
    sr.SetVarCRSL("PseudoJet1_eta", 0, -1);
    sr.SetVarCRQCD("PseudoJet1_eta", 0, -1);
    sr.SetVarCRDY("PseudoJet1_eta", 0, -1);
    float mt2bins_sr27[] = {200, 400, 600, 800, 1800};
    sr.SetMT2Bins(4, mt2bins_sr27);
    SRVec.push_back(sr);
    sr.Clear();
    
    // 3b loose
    sr.SetName("28");
    sr.SetVar("njets", 2, -1);
    sr.SetVar("nbjets", 3, -1);
    sr.SetVar("ht", 1000, -1);
    sr.SetVar("met", 30, -1);
    sr.SetVarCRSL("njets", 2, -1);
    sr.SetVarCRSL("nbjets", 3, -1);
    sr.SetVarCRSL("ht", 1000, -1);
    sr.SetVarCRSL("met", 30, -1);
    sr.SetVarCRDY("njets", 2, -1);
    sr.SetVarCRDY("nbjets", 1, -1); // DY: NB>0
    sr.SetVarCRDY("ht", 1000, -1);
    sr.SetVarCRDY("met", 30, -1);
    sr.SetVarCRQCD("ht", 1000, -1);
    sr.SetVarCRQCD("met", 30, -1);
    sr.SetVar("PseudoJet1_eta", 0, -1);
    sr.SetVarCRSL("PseudoJet1_eta", 0, -1);
    sr.SetVarCRQCD("PseudoJet1_eta", 0, -1);
    sr.SetVarCRDY("PseudoJet1_eta", 0, -1);
    float mt2bins_sr28[] = {200, 400, 600, 1800};
    sr.SetMT2Bins(3, mt2bins_sr28);
    SRVec.push_back(sr);
    sr.Clear();
    
    // 3b tight
    sr.SetName("29");
    sr.SetVar("njets", 2, -1);
    sr.SetVar("nbjets", 3, -1);
    sr.SetVar("ht", 1500, -1);
    sr.SetVar("met", 30, -1);
    sr.SetVarCRSL("njets", 2, -1);
    sr.SetVarCRSL("nbjets", 3, -1);
    sr.SetVarCRSL("ht", 1500, -1);
    sr.SetVarCRSL("met", 30, -1);
    sr.SetVarCRDY("njets", 2, -1);
    sr.SetVarCRDY("nbjets", 1, -1); // DY: NB>0
    sr.SetVarCRDY("ht", 1500, -1);
    sr.SetVarCRDY("met", 30, -1);
    sr.SetVarCRQCD("ht", 1500, -1);
    sr.SetVarCRQCD("met", 30, -1);
    sr.SetVar("PseudoJet1_eta", 0, -1);
    sr.SetVarCRSL("PseudoJet1_eta", 0, -1);
    sr.SetVarCRQCD("PseudoJet1_eta", 0, -1);
    sr.SetVarCRDY("PseudoJet1_eta", 0, -1);
    float mt2bins_sr29[] = {200, 400, 600, 1800};
    sr.SetMT2Bins(3, mt2bins_sr29);
    SRVec.push_back(sr);
    sr.Clear();
    
    // 7j3b loose
    sr.SetName("30");
    sr.SetVar("njets", 7, -1);
    sr.SetVar("nbjets", 3, -1);
    sr.SetVar("ht", 1000, -1);
    sr.SetVar("met", 30, -1);
    sr.SetVarCRSL("njets", 7, -1);
    sr.SetVarCRSL("nbjets", 3, -1);
    sr.SetVarCRSL("ht", 1000, -1);
    sr.SetVarCRSL("met", 30, -1);
    sr.SetVarCRDY("njets", 7, -1);
    sr.SetVarCRDY("nbjets", 1, -1); // DY: NB>0
    sr.SetVarCRDY("ht", 1000, -1);
    sr.SetVarCRDY("met", 30, -1);
    sr.SetVarCRQCD("ht", 1000, -1);
    sr.SetVarCRQCD("met", 30, -1);
    sr.SetVar("PseudoJet1_eta", 0, -1);
    sr.SetVarCRSL("PseudoJet1_eta", 0, -1);
    sr.SetVarCRQCD("PseudoJet1_eta", 0, -1);
    sr.SetVarCRDY("PseudoJet1_eta", 0, -1);
    float mt2bins_sr30[] = {200, 400, 600, 1800};
    sr.SetMT2Bins(3, mt2bins_sr30);
    SRVec.push_back(sr);
    sr.Clear();
    
    // 7j3b tight
    sr.SetName("31");
    sr.SetVar("njets", 7, -1);
    sr.SetVar("nbjets", 3, -1);
    sr.SetVar("ht", 1500, -1);
    sr.SetVar("met", 30, -1);
    sr.SetVarCRSL("njets", 7, -1);
    sr.SetVarCRSL("nbjets", 3, -1);
    sr.SetVarCRSL("ht", 1500, -1);
    sr.SetVarCRSL("met", 30, -1);
    sr.SetVarCRDY("njets", 7, -1);
    sr.SetVarCRDY("nbjets", 1, -1); // DY: NB>0
    sr.SetVarCRDY("ht", 1500, -1);
    sr.SetVarCRDY("met", 30, -1);
    sr.SetVarCRQCD("ht", 1500, -1);
    sr.SetVarCRQCD("met", 30, -1);
    sr.SetVar("PseudoJet1_eta", 0, -1);
    sr.SetVarCRSL("PseudoJet1_eta", 0, -1);
    sr.SetVarCRQCD("PseudoJet1_eta", 0, -1);
    sr.SetVarCRDY("PseudoJet1_eta", 0, -1);
    float mt2bins_sr31[] = {200, 400, 1800};
    sr.SetMT2Bins(2, mt2bins_sr31);
    SRVec.push_back(sr);
    sr.Clear();
    
    //define baseline selections commmon to all signal regions 
    baseSR.SetVar("mt2", 200, -1);
    baseSR.SetVar("j1pt", 30, -1);
    baseSR.SetVar("j2pt", 30, -1);
    baseSR.SetVar("deltaPhiMin", 0.3, -1);
    baseSR.SetVar("diffMetMhtOverMet", 0, 0.5);
    baseSR.SetVar("nlep", 0, 1);
    //baseSR.SetVar("passesHtMet", 1, 1);

    // common selections for CRSL
    baseSR.SetVarCRSL("mt2", 200, -1);
    baseSR.SetVarCRSL("j1pt", 30, -1);
    baseSR.SetVarCRSL("j2pt", 30, -1);
    baseSR.SetVarCRSL("deltaPhiMin", 0.3, -1);
    baseSR.SetVarCRSL("diffMetMhtOverMet", 0, 0.5);
    baseSR.SetVarCRSL("nlep", 1, 2);

    // common selections for CRDY
    baseSR.SetVarCRDY("mt2", 200, -1);
    baseSR.SetVarCRDY("j1pt", 30, -1);
    baseSR.SetVarCRDY("j2pt", 30, -1);
    baseSR.SetVarCRDY("deltaPhiMin", 0.3, -1);
    baseSR.SetVarCRDY("diffMetMhtOverMet", 0, 0.5);
    baseSR.SetVarCRDY("nlep", 0, 1);

    // common selections for QCD
    baseSR.SetVarCRQCD("mt2", 200, -1);
    baseSR.SetVarCRQCD("j1pt", 30, -1);
    baseSR.SetVarCRQCD("j2pt", 30, -1);
    baseSR.SetVarCRQCD("deltaPhiMin", 0., 0.3);
    baseSR.SetVarCRQCD("diffMetMhtOverMet", 0, 0.5);
    baseSR.SetVarCRQCD("nlep", 0, 1);

    //add baseline selections to all signal regions 
    std::vector<std::string> vars = baseSR.GetListOfVariables();
    for(unsigned int i = 0; i < SRVec.size(); i++){
      for(unsigned int j = 0; j < vars.size(); j++){
        SRVec.at(i).SetVar(vars.at(j), baseSR.GetLowerBound(vars.at(j)), baseSR.GetUpperBound(vars.at(j)));
      }
    }

    //add baseline selections to all CRSL regions 
    std::vector<std::string> varsCRSL = baseSR.GetListOfVariablesCRSL();
    for(unsigned int i = 0; i < SRVec.size(); i++){
      for(unsigned int j = 0; j < varsCRSL.size(); j++){
        SRVec.at(i).SetVarCRSL(varsCRSL.at(j), baseSR.GetLowerBoundCRSL(varsCRSL.at(j)), baseSR.GetUpperBoundCRSL(varsCRSL.at(j)));
      }
    }

    //add baseline selections to all CRDY regions 
    std::vector<std::string> varsCRDY = baseSR.GetListOfVariablesCRDY();
    for(unsigned int i = 0; i < SRVec.size(); i++){
      for(unsigned int j = 0; j < varsCRDY.size(); j++){
        SRVec.at(i).SetVarCRDY(varsCRDY.at(j), baseSR.GetLowerBoundCRDY(varsCRDY.at(j)), baseSR.GetUpperBoundCRDY(varsCRDY.at(j)));
      }
    }

    //add baseline selections to all QCD regions 
    std::vector<std::string> varsCRQCD = baseSR.GetListOfVariablesCRQCD();
    for(unsigned int i = 0; i < SRVec.size(); i++){
      for(unsigned int j = 0; j < varsCRQCD.size(); j++){
        SRVec.at(i).SetVarCRQCD(varsCRQCD.at(j), baseSR.GetLowerBoundCRQCD(varsCRQCD.at(j)), baseSR.GetUpperBoundCRQCD(varsCRQCD.at(j)));
      }
    }

    return SRVec;

  }

  
} // namespace mt2
