#include "SignalSelectionCuts.cpp"

void test_macro() {
  TCut recoil_cut = CutMRecoil( 200 );
  
  ROOT::EnableImplicitMT();
  ROOT::RDataFrame d( "recoObservablesTree" , "/afs/desy.de/user/b/beyerjac/flc/VBS/aQGC_analysis/scripts/tests/test.root" );
};