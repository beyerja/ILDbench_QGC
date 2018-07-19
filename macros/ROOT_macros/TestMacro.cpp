#include "src/InputManager.cpp"
#include "analyzers/src/TestAnalyzer.cpp"

 
void TestMacro() {
  EnableImplicitMT(); // Allow multithreating in RDataFrame
  
  InputManager input_manager;
  input_manager.setInputDirectory( "/nfs/dust/ilc/group/ild/beyerjac/VBS/aQGCAnalysis/rootfiles" );
  input_manager.setFilenameExtension( "root" );
  input_manager.openFiles();
  
  vector<TFile*> root_files {};
  input_manager.getFiles( root_files );
  
  TestAnalyzer analyzer;
  analyzer.setInputFiles( root_files );
  analyzer.performAnalysis();
  analyzer.interpretResults();
  analyzer.clearMemory();
  
  input_manager.closeFiles();
};