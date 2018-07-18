#include "ReadDirectory.cpp"
#include "SelectionCutTemplates.cpp"

using namespace ROOT;

inline void performSelection ( RDataFrame &dataframe ) {
  dataframe.Filter(CutVVMass( 200, 300 ));
}

void TestMacro() {
  string VV_m_cut = CutVVMass( 200, 300 );
  
  string file_directory = "/nfs/dust/ilc/group/ild/beyerjac/VBS/aQGCAnalysis/rootfiles/";
  
  vector<string> root_files {searchDirectoryForFileType( file_directory, "root" )};
  
  
  
  
  EnableImplicitMT(); // Allow multithreating in RDataFrame
  RDataFrame reco( "recoObservablesTree" , "/nfs/dust/ilc/group/ild/beyerjac/VBS/aQGCAnalysis/rootfiles/*.root" );
  performSelection( reco );
  auto VV_m_histo = reco.Histo1D("VV_m");
  TCanvas* can = new TCanvas("VV_m_can", "", 0, 0, 600, 600);
  VV_m_histo->Draw();
  can->Print("test_VV_m.pdf");
  delete can;
};