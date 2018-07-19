#include "../include/TestAnalyzer.h"

//-------------------------------------------------------------------------------------------------

void TestAnalyzer::performAnalysis(){
  this->getCombinedDataframes();
  
  for ( auto & dataframe: m_dataframes ) {
    cout << *dataframe.Filter( CutVpT(100).c_str() ).Count() << endl;
  }
};

//-------------------------------------------------------------------------------------------------

void TestAnalyzer::interpretResults(){
  return;
};

//-------------------------------------------------------------------------------------------------

  
  // RDataFrame reco( "recoObservablesTree" , "/nfs/dust/ilc/group/ild/beyerjac/VBS/aQGCAnalysis/rootfiles/*.root" );
  // 
  // auto VV_m_histo = reco.Histo1D("VV_m");
  // TCanvas* can = new TCanvas("VV_m_can", "", 0, 0, 600, 600);
  // VV_m_histo->Draw();
  // can->Print("test_VV_m.pdf");
  // delete can;
