#include "src/InputManager.cpp"
#include "analyzers/src/aQGCAnalyzer.cpp"

 
void TestMacro() {
  EnableImplicitMT(); // Allow multithreating in RDataFrame
  
  InputManager input_manager;
  input_manager.setInputDirectory( "/nfs/dust/ilc/group/ild/beyerjac/VBS/aQGCAnalysis/rootfiles" );
  input_manager.setFilenameExtension( "root" );
  input_manager.findFiles();
  
  vector<string> root_file_paths {};
  input_manager.getFilePaths( root_file_paths );
  
  float e_beam_polarization = -0.8;
  float p_beam_polarization = 0.3;
  float luminosity = 2000;
  
  aQGCAnalyzer analyzer;
  analyzer.setInputPaths( root_file_paths );
  analyzer.setBeamPolarizations( e_beam_polarization, p_beam_polarization );
  analyzer.setLuminosity( luminosity );
  analyzer.run();
};
