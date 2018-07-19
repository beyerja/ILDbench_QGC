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
  
  aQGCAnalyzer analyzer;
  analyzer.setInputPaths( root_file_paths );
  analyzer.performAnalysis();
  analyzer.clearMemory();
};