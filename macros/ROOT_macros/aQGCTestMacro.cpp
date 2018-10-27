#include "analyzers/src/aQGCSampleSMAnalyzer.cpp"

void aQGCTestMacro() {
  EnableImplicitMT(20); // Allow multithreating in RDataFrame
  // EnableThreadSafety(); 
  
  // TODO Create clean-up option for temporary files
  
  // First find all aQGC files 
  // -> Run InputManager over several directories and collect all root files
  string base_dir {"/afs/desy.de/group/flc/pool/beyerjac/WHIZARD/vvqqqq"};
  vector<string> main_dirs {"signal", "bkg"};
  
  InputManager input_manager;
  input_manager.setFilenameExtension( "root" );
  
  for ( auto &main_dir: main_dirs ) {
    string search_dir = base_dir + "/" + main_dir;
    DIR *dir;    
    if((dir  = opendir(search_dir.c_str())) == NULL) {
      cout << "Error(" << errno << ") opening " << search_dir << endl;
      return errno;
    }

    struct dirent *content;
    while ((content = readdir(dir)) != NULL) {
      string content_name = content->d_name;
      if ( content_name == "." || content_name == ".." ) {continue;}
      input_manager.setInputDirectory( search_dir + "/" + content_name ); 
      input_manager.findFiles();
    }
    closedir(dir);
  }
    
  vector<string> root_file_paths {};
  input_manager.getFilePaths( root_file_paths );
  
  // Now to the analysis!
  float e_beam_polarization = -0.8;
  float p_beam_polarization = 0.3;
  float luminosity = 2000;
  
  // string output_directory = "/afs/desy.de/group/flc/pool/beyerjac/VBS/aQGCAnalysis/analyzer_plots/EFTTester";
  // EFTTester analyzer;  
  // analyzer.setInputPaths( root_file_paths );
  // analyzer.setBeamPolarizations( e_beam_polarization, p_beam_polarization );
  // analyzer.setLuminosity( luminosity );
  // analyzer.setOutputDirectory( output_directory );
  // analyzer.run();
  
  string output_directory = "/afs/desy.de/group/flc/pool/beyerjac/VBS/aQGCAnalysis/analyzer_plots/aQGCSampleSMAnalyzer";
  aQGCSampleSMAnalyzer analyzer;  
  analyzer.setInputPaths( root_file_paths );
  analyzer.setBeamPolarizations( e_beam_polarization, p_beam_polarization );
  analyzer.setLuminosity( luminosity );
  analyzer.setOutputDirectory( output_directory );
  analyzer.run();
};
