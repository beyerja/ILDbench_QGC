#include "SignalSelectionCuts.cpp"

using namespace ROOT;

void test_macro() {
  string VV_m_cut = CutVVMass( 200, 300 );
  
  EnableImplicitMT(); // Allow multithreating in RDataFrame
  RDataFrame reco( "recoObservablesTree" , "/nfs/dust/ilc/group/ild/beyerjac/VBS/aQGCAnalysis/rootfiles/rv02-00-01_sv02-00-01_mILD_l5_o1_v02_E500_vvxyyx_eL_pR.root" );
  auto VV_m_histo = reco.Filter(VV_m_cut).Histo1D("VV_m");
  TCanvas* can = new TCanvas("VV_m_can", "", 0, 0, 600, 600);
  VV_m_histo->Draw();
  can->Print("test_VV_m.pdf");
  delete can;
};