#include "../include/Analyzer.h"

//-------------------------------------------------------------------------------------------------

void Analyzer::setInputPaths( vector<string> &input_file_paths ){
  m_input_file_paths = input_file_paths;
}

void Analyzer::setBeamPolarizations( float &e_beam_polarization, float &p_beam_polarization ){
  m_e_beam_polarization = e_beam_polarization;
  m_p_beam_polarization = p_beam_polarization;
}

void Analyzer::setLuminosity( float &luminosity ){
  m_luminosity = luminosity;
}

void Analyzer::setOutputDirectory( string &output_directory ){
  m_output_directory = output_directory;
  if ( ! SysHelp::pathExists(m_output_directory) ) {
    cout << "Given output directory does not exist. Trying to create it..." << endl;
    SysHelp::createDirectory(m_output_directory);
    if ( SysHelp::pathExists(m_output_directory) ) {
      cout << "Successfully created output directory!" << endl;
    } else {
      cout << "Something went wrong in creating output directory: " << m_output_directory << endl;
      cout << "Exiting now." << endl;
      exit(1);
    }
  }
}

//-------------------------------------------------------------------------------------------------

string Analyzer::getOutputDirectory() const {
  if ( ! SysHelp::pathExists(m_output_directory) ) {
    cout << "Something went wrong, trying to use non-existent output path! Exiting..." << endl;
    exit(1);
  } 
  return m_output_directory;
}

set<string> Analyzer::getFinalStatesSet() const {
  return m_final_states;
}

//-------------------------------------------------------------------------------------------------

void Analyzer::findAllFinalStates( TTree *info_chain ) {
  /* Structure of this function: 
      - First find all final states and polarizations by rough search (assumes that no combination has less than 100 events!)
      - Then makes RDataFrame count Nevents for each combination
  */
  // TODO THIS WORKS BUT CREATES HUGE OVERHEAD (RDF IS VERY SLOW FOR SOME REASON)
  // TODO 1. Work out why RDF does not multithread
  // TODO 2. Do this function based entirely on RDF!
  
  if ( !m_final_state_Nevents.empty() ){
    cout << "WARNING in Analyzer::findAllFinalStates : Searching final states, but they are already found!"  << endl;
    return;
  }
  
  if ( nullptr == m_dataframe ) {
    cout << "ERROR in Analyzer::findAllFinalStates : No RDataFrame set!" << endl;
    exit(1);
  }
  
  int N_total = info_chain->GetEntries();
  TString* process_name_ptr {};
  float e_polarization, p_polarization {};
  info_chain->SetBranchAddress("process_name", &process_name_ptr);
  info_chain->SetBranchAddress("e_polarization", &e_polarization);
  info_chain->SetBranchAddress("p_polarization", &p_polarization);
  
  // Structure similar to final mapping but with ResultCount (weird RDF thing)
  vector<pair<string,pair<pair<float,float>, ResultCount>>> final_state_Nevents {};
   
  // Find all final state - polarisation combinations, then ask dataframe for N
  // -> Check only every 100th event to speed up (no file should have less...)
  for (int i=0; i<N_total; i+=100) {
    info_chain->GetEvent(i);
    string process_name = process_name_ptr->Data();
  
    // Test if final state already found
    if ( m_final_states.find(process_name) == m_final_states.end() ) {
      cout  << "Found final state: " <<  process_name << endl;
      m_final_states.insert(process_name);
      m_final_state_Nevents[process_name] = pol_Nevents_map{}; // Prepare map for process entry
    } 
    
    // For letting RDF test for this process name
    auto ProcessNameLambda = Cuts::getStringTestLambda(process_name);

    // Test if polarization already found for this final state
    auto pol_pair = make_pair(e_polarization, p_polarization);
    
    // Check if process - pol combination is already in vector using a lambda function
    auto GotProcessPol = [process_name, pol_pair](const pair<string,pair<pair<float,float>, ResultCount>>& element) {
      return (element.first == process_name) && (element.second.first == pol_pair);
    };
    
    if ( find_if(final_state_Nevents.begin(), final_state_Nevents.end(), GotProcessPol) == final_state_Nevents.end() ) {
      cout  << "Found final state - polarization pairing: " << process_name 
            << " " << e_polarization << " " << p_polarization << endl;
      
      auto ElectronPolTestLambda = Cuts::getFloatCutLambda(e_polarization);
      auto PositronPolTestLambda = Cuts::getFloatCutLambda(p_polarization);

      // This is the combination of process and polarisation -> push back object that tells RDF to count exactly that
      auto Nevents_count = m_dataframe->Filter(ProcessNameLambda, {"process_name"}).Filter(ElectronPolTestLambda, {"e_polarization"}).Filter(PositronPolTestLambda, {"p_polarization"}).Count();
      final_state_Nevents.push_back( make_pair(process_name, make_pair(pol_pair, Nevents_count)) );
    }
  }
  
  // Now fill actually used map by making rdf count Nevents for each combination
  for ( auto & fs_pol_count: final_state_Nevents) {
    auto process_name   = fs_pol_count.first;
    auto pol_count_pair = fs_pol_count.second;      
    auto pol_pair       = pol_count_pair.first;   
    // Trigger RDF to count the number of events for this process - pol combination
    int Nevents         = *pol_count_pair.second;
    m_final_state_Nevents[process_name][pol_pair] = Nevents;
  }
}

//-------------------------------------------------------------------------------------------------

void Analyzer::getCombinedDataframe() {
  TChain* info_chain  = new TChain("processInfoTree");
  TChain* reco_chain  = new TChain("recoObservablesTree");
  TChain* mc_chain    = new TChain("mcObservablesTree");
  TChain* truth_chain = new TChain("mcTruthTree");
  
  for ( auto const& file_path: m_input_file_paths ) {
    info_chain ->Add( file_path.c_str() );
    reco_chain ->Add( file_path.c_str() );
    mc_chain   ->Add( file_path.c_str() );
    truth_chain->Add( file_path.c_str() );
  }
  
  
  info_chain->AddFriend(reco_chain, "reco");
  info_chain->AddFriend(mc_chain, "mcobs");
  info_chain->AddFriend(truth_chain, "mctruth");
  RDataFrame* befriended_dataframe = new RDataFrame(*info_chain);
  m_dataframe = befriended_dataframe;
  m_all_chains = {info_chain, reco_chain, mc_chain, truth_chain};
  
  this->findAllFinalStates( info_chain );
}

//-------------------------------------------------------------------------------------------------

void Analyzer::run(){
  cout << "Reading in trees from files to create dataframe." << endl;
  this->getCombinedDataframe();
  cout << "Starting analysis." << endl;
  this->performAnalysis();
  cout << "Analysis finished - cleaning up." << endl;
  this->clearMemory();
  cout << "Done!" << endl;
}

//-------------------------------------------------------------------------------------------------

void Analyzer::clearMemory(){
  delete m_dataframe;
  
  for (auto const& ptr : m_all_chains) {
    delete ptr;
  }
  m_all_chains = {};
  
}

//-------------------------------------------------------------------------------------------------

int Analyzer::getNProcessEvents( TString* process_name_ptr, float e_polarization, float p_polarization ) const {
  string process_name = process_name_ptr->Data();
  
  // Test if final state is registered
  if ( m_final_state_Nevents.find(process_name) == m_final_state_Nevents.end() ) {
    cout  << "ERROR in Analyzer::getNProcessEvents: Unknown process "
          << process_name << endl;
    return -1;
  }
  
  // Test if polarization is registered
  const pol_Nevents_map* fs_pol_map = &m_final_state_Nevents.at(process_name); 
  auto pol_pair = make_pair(e_polarization, p_polarization);
  if ( fs_pol_map->find(pol_pair) == fs_pol_map->end() ) {
    cout  << "ERROR in Analyzer::getNProcessEvents: Unknown process - pol pairing "
    << process_name << " " << e_polarization << " " << p_polarization << endl;
    return -1;
  } 
  
  return fs_pol_map->at(pol_pair);
}

//-------------------------------------------------------------------------------------------------

float Analyzer::getPolarizationWeight( float e_polarization, float p_polarization ) const {
  return 0.5*( 1. + e_polarization * m_e_beam_polarization ) * 0.5*( 1. + p_polarization * m_p_beam_polarization );
}

//-------------------------------------------------------------------------------------------------

float Analyzer::getProcessWeight( TString* process_name_ptr, float e_polarization, float p_polarization, float cross_section ) const{
  float polarization_weight = this->getPolarizationWeight( e_polarization, p_polarization );
  int N_process_events = this->getNProcessEvents( process_name_ptr, e_polarization, p_polarization );
  return polarization_weight * ( cross_section * m_luminosity ) / float(N_process_events);
}

//-------------------------------------------------------------------------------------------------

function<float (TString, float, float, float)> Analyzer::getProcessWeightLambda() const {
  return[this](TString process_name_ptr, float e_polarization, float p_polarization, float cross_section) {
    return this->getProcessWeight( &process_name_ptr, e_polarization, p_polarization, cross_section ); 
  };
}

//-------------------------------------------------------------------------------------------------
