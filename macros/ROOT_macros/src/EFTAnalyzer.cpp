#include "../include/EFTAnalyzer.h"

// TODO Create RECOMBINE_ALL_FILES flag and make sure it is considered in all important places

//-------------------------------------------------------------------------------------------------

void EFTAnalyzer::findParameterPoints( string weight_directory ){
  // WARNING: Uses hardcoded file name conventions!
  if (weight_directory == "") {
    cout << "ERROR in findParameterPoints: Empty input path!" << endl;
    return;
  }
  
  InputManager parameter_file_searcher;
  parameter_file_searcher.setInputDirectory( weight_directory );
  parameter_file_searcher.setFilenameExtension( "py" );
  parameter_file_searcher.findFiles();
  
  vector<string> parameter_file_paths {};
  parameter_file_searcher.getFilePaths( parameter_file_paths );
  
  string parameter_path = "";
  if ( parameter_file_paths.size() > 1 ){
    cout  << "ERROR in findParameterPoints: found multiple parameter (root) files in " 
          << weight_directory << endl;
  } else if ( parameter_file_paths.size() == 0 ) {
    cout  << "In findParameterPoints: no parameter files in " << weight_directory << endl;
  } else {
    cout  << "In findParameterPoints: found parameter file in " << weight_directory << endl;
    parameter_path = parameter_file_paths[0];
  }
  
  vector<vector<float>> parameter_matrix = PyHelp::read2DMatrixFromFile( parameter_path );
  // Remove point (0,0) -> Stupid convention made by me...
  vector<float> zero_point {0,0};
  parameter_matrix.erase(remove(parameter_matrix.begin(),parameter_matrix.end(), zero_point),parameter_matrix.end());
  
  map<int, ParameterPoint> parameter_points{};
  for (int i=0;i<parameter_matrix.size();i++) {
    vector<float> parameters = parameter_matrix[i];
    if ( parameters.size() != 2 ) {
      cout << "ERROR in findParameterPoints: not two parameters for setting!!!" << endl;
      exit(1);
    } 
    parameter_points[i+1] = make_pair(parameters[0], parameters[1]);
  }
  
  if (m_parameter_points.empty()) {
    m_parameter_points = parameter_points;
    cout << "Found parameter points!" << endl;
  } else if ( m_parameter_points != parameter_points ) {
    cout << "ERROR in findParameterPoints: inconsistent with previous file!" << endl;
    exit(1);
  }
}

//-------------------------------------------------------------------------------------------------

void EFTAnalyzer::setupDummyWeightsFile( string weight_file_path ){
  if ( m_dummy_weights_file != "" ) { return; }
  
  if (weight_file_path == "") {
    cout << "ERROR in setupDummyWeightsFile: Empty input path!" << endl;
    return;
  }
  
  TFile *weight_file = TFile::Open(weight_file_path.c_str());
  TTree *weight_tree = (TTree*)weight_file->Get("weightsTree");
  
  // TODO Move this to a setting file or somewhere more obvious
  m_dummy_weights_file = "/afs/desy.de/group/flc/pool/beyerjac/tmp/dummy_weights.root";
  
  int N_entries   = weight_tree->GetEntries();
  int N_branches  = weight_tree->GetNbranches(); 
  TObjArray *branch_list = weight_tree->GetListOfBranches();
  string branch_names [N_branches];
  
  for (int branch=0; branch<N_branches; branch++) {
    branch_names[branch] = branch_list->At(branch)->GetName();
  }
  weight_file->Close();
  delete weight_file;
  
  cout << "Creating weight file dummy template." << endl;
  TFile *dummy_file = new TFile(m_dummy_weights_file.c_str(), "recreate");
  TTree* dummy_weight_tree = new TTree("weightsTree", "weightsTree");
  float weight_ptrs [N_branches];
  
  // Create same branches as other file had
  for (int branch=0; branch<N_branches; branch++) {
    string branch_name = branch_names[branch];
    dummy_weight_tree->Branch( branch_name.c_str(), &weight_ptrs[branch], (branch_name + "/F").c_str());
    weight_ptrs[branch] = 1.0;
  }
  
  for (int entry=0; entry<100*N_entries; entry++) {
    dummy_weight_tree->Fill();
  }
  
  dummy_file->cd();
  dummy_weight_tree->Write();
  dummy_file->Close();
  delete dummy_file;
}

//-------------------------------------------------------------------------------------------------

void EFTAnalyzer::searchForWeightFile( string file_path ){
  // Uses weight file convention to search for weight file to event file
  // WARNING: Has convention hardcoded!
  string file_directory = SysHelp::extract_directory(file_path);
  string weight_directory = file_directory + "/rescan_output";
  
  InputManager weight_file_searcher;
  weight_file_searcher.setInputDirectory( weight_directory );
  weight_file_searcher.setFilenameExtension( "root" );
  weight_file_searcher.beSilent(true);
  weight_file_searcher.findFiles();
  
  vector<string> weight_file_paths {};
  weight_file_searcher.getFilePaths( weight_file_paths );
  
  string weight_path = "";
  if ( weight_file_paths.size() > 1 ){
    cout  << "ERROR in searchForWeightFile: found multiple weight (root) files in " 
          << weight_directory << endl;
  } else if ( weight_file_paths.size() == 0 ) {
    cout  << "In searchForWeightFile: no weight files for " << file_path << endl;
  } else {
    cout  << "In searchForWeightFile: found weight file for " << file_path << endl;
    weight_path = weight_file_paths[0];
    
    this->findParameterPoints(weight_directory);
    this->setupDummyWeightsFile(weight_path);
  }
  
  FilePair events_weights_pair = make_pair( file_path, weight_path );
  m_events_weights_pairs.push_back(events_weights_pair);
}

//-------------------------------------------------------------------------------------------------

void EFTAnalyzer::searchForWeightFiles() {
  for ( auto const& file_path: m_input_file_paths ) {
    // Must be done separately first separately to create dummy weight file
    this->searchForWeightFile(file_path);
  }
}

//-------------------------------------------------------------------------------------------------

string EFTAnalyzer::makeDummyCopy( string file_path) {
  /** Creates copy of the dummy tree and returns the copys path.
  */
  string dummy_directory =  SysHelp::extract_directory(m_dummy_weights_file);
  string file_name = SysHelp::extract_filename(file_path);
  string dummy_copy = dummy_directory + "/dummy_copy_" + file_name;
  
  if ( SysHelp::file_exists(dummy_copy) ) {
    cout << "Already have weight dummy, not overwriting: " << dummy_copy << endl;
  
  } else {
    // Determine number of events the copy need
    TFile *file = TFile::Open(file_path.c_str());
    TTree *tree = (TTree*)file->Get("processInfoTree");
    int N_entries = tree->GetEntries();
    file->Close();
    delete file;
    
    cout << "Creating dummy weight file: " << dummy_copy << endl;
    SysHelp::make_Nevents_treecopy(m_dummy_weights_file, "weightsTree", N_entries, dummy_copy);
  }
  return dummy_copy;
  
  //TODO In clean-up clean dummies -> NOPE! Instead: Give option to clean them up with new run!
}

//-------------------------------------------------------------------------------------------------

// TODO Find way to combine with previous function or at least create common functions
string EFTAnalyzer::combineFileAndWeight( string file_path, string weight_path ){
  
  string dummy_directory = SysHelp::extract_directory(m_dummy_weights_file);
  string file_name = SysHelp::extract_filename(file_path);
  
  string combined_path = dummy_directory + "/combined_" + file_name;

  if ( SysHelp::file_exists(combined_path) ) {
    cout << "Already have combined file, not overwriting: " << combined_path << endl;
  } else {
    // Determine number of events the copy need
    cout << "Creating combined file: " << combined_path << endl;
    // ~/flc/VBS/aQGC_analysis/macros/ROOT_macros/run_in_old_root.sh
    
    string paths_to_combine = file_path + " " + weight_path;
    SysHelp::hadd_rootfiles( combined_path , paths_to_combine, "-f" );
  }
  
  return combined_path;

}

//-------------------------------------------------------------------------------------------------

void EFTAnalyzer::combineAllFiles() {
  /** Combines all the aQGC files into one big file.
      This is unfortunately necessary right now to make RDataFrame work.
      Some checks are implemented in the subfunctions to make sure that
      this combining is only done once to because it causes a huge overhead.
  */
  
  // TODO Directory path should be settable option
  m_total_combined_path = "/afs/desy.de/group/flc/pool/beyerjac/tmp/total.root";
  
  if ( SysHelp::file_exists(m_total_combined_path) ) {
    cout << "Already have combined total file, not overwriting: " << m_total_combined_path << endl;
  } else {

    
    cout << "Start combining files." << endl;
    string all_combined_paths {};
    for ( auto const& events_weights_pair: m_events_weights_pairs ) {
      string file_path = events_weights_pair.first;
      string weight_path = events_weights_pair.second;
      
      // Check if weight file found, if not use Dummy weights (all 1)
      if (weight_path == "") {
        weight_path = this->makeDummyCopy(file_path);;
      }
      
      string combined_path = this->combineFileAndWeight( file_path, weight_path );
      all_combined_paths += " " + combined_path;
    }
    
    cout << "Combining all files to final file: " << m_total_combined_path << endl;
    SysHelp::hadd_rootfiles( m_total_combined_path, all_combined_paths, "-f" );
  }
  
}

//-------------------------------------------------------------------------------------------------

void EFTAnalyzer::getCombinedDataframe() {
  this->combineAllFiles();
  
  m_combined_file = TFile::Open(m_total_combined_path.c_str());
  
  TTree* info_tree    = (TTree*)m_combined_file->Get("processInfoTree");
  TTree* reco_tree    = (TTree*)m_combined_file->Get("recoObservablesTree");
  TTree* mc_tree      = (TTree*)m_combined_file->Get("mcObservablesTree");
  TTree* truth_tree   = (TTree*)m_combined_file->Get("mcTruthTree");
  TTree* weight_tree  = (TTree*)m_combined_file->Get("weightsTree");
  
  this->findAllFinalStates( info_tree );
  
  info_tree->AddFriend(reco_tree, "reco");
  info_tree->AddFriend(mc_tree, "mcobs");
  info_tree->AddFriend(truth_tree, "mctruth");
  info_tree->AddFriend(weight_tree, "weights");
  
  cout << "Creating RDataFrame." << endl;
  m_dataframe = new RDataFrame(*info_tree);
}

//-------------------------------------------------------------------------------------------------

void EFTAnalyzer::clearMemory(){
  delete m_dataframe;
  m_combined_file->Close();
  delete m_combined_file;
}

//-------------------------------------------------------------------------------------------------

void EFTAnalyzer::run(){
  cout << "Searching for weight files." << endl;
  this->searchForWeightFiles(); // Includes parameter point finding -> Needs to be done here (or think of better way)
  cout << "Reading in trees from files to create dataframe." << endl;
  this->getCombinedDataframe();
  cout << "Starting analysis." << endl;
  this->performAnalysis();
  cout << "Analysis finished - cleaning up." << endl;
  this->clearMemory();
  cout << "Done!" << endl;
}

//-------------------------------------------------------------------------------------------------

 pair<float, float> EFTAnalyzer::getParameterPoint( int setting_index ) const {
  return m_parameter_points.at(setting_index);
}

//-------------------------------------------------------------------------------------------------

int EFTAnalyzer::getNParameterPoints() const{
  return m_parameter_points.size();
}

//-------------------------------------------------------------------------------------------------

map<int, pair<float, float>> EFTAnalyzer::getParameterPointMap() const{
  return m_parameter_points;
}


//-------------------------------------------------------------------------------------------------

float EFTAnalyzer::getEventWeight( float event_weight, float process_weight ){
  return event_weight * process_weight;
}

//-------------------------------------------------------------------------------------------------

function<float (float, float)> EFTAnalyzer::getEventWeightLambda(){
  return[this](float event_weight, float process_weight) {
    return this->getEventWeight( event_weight, process_weight ); 
  };
}

//-------------------------------------------------------------------------------------------------
