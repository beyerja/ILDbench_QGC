#include "src/InputManager.cpp"
#include "analyzers/src/aQGCAnalyzer.cpp"

 
void TestMacro() {
  EnableImplicitMT(5); // Allow multithreating in RDataFrame
  
  InputManager input_manager;
  input_manager.setInputDirectory( "/nfs/dust/ilc/group/ild/beyerjac/VBS/aQGCAnalysis/test_combined" );
  input_manager.setFilenameExtension( "root" );
  input_manager.findFiles();
  
  vector<string> root_file_paths {};
  input_manager.getFilePaths( root_file_paths );
  
  float e_beam_polarization = -0.8;
  float p_beam_polarization = 0.3;
  float luminosity = 1000;
  
  string output_directory = "/afs/desy.de/group/flc/pool/beyerjac/VBS/aQGCAnalysis/analyzer_plots/aQGCAnalyzer";
  aQGCAnalyzer analyzer;
  analyzer.setInputPaths( root_file_paths );
  analyzer.setBeamPolarizations( e_beam_polarization, p_beam_polarization );
  analyzer.setLuminosity( luminosity );
  analyzer.setOutputDirectory( output_directory );
  analyzer.run();
};
