#include "../include/TestAnalyzer.h"

//-------------------------------------------------------------------------------------------------

void TestAnalyzer::performAnalysis(){
  cout << *m_dataframe->Filter( CutVpT(100).c_str() ).Count() << endl;
  auto h_VV_m = m_dataframe->Histo1D("VV_m");
  cout << h_VV_m->GetNbinsX() << endl;
};

//-------------------------------------------------------------------------------------------------