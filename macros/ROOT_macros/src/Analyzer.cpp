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

//-------------------------------------------------------------------------------------------------

void Analyzer::findAllFinalStates( TTree *info_chain ) {
  if ( !m_final_state_Nevents.empty() ){
    cout << "ERROR in Analyzer::findAllFinalStates : Searching final states, but they are already found!"  << endl;
    return;
  }
  
  int N_total = info_chain->GetEntries();
  TString* process_name_ptr {};
  float e_polarization, p_polarization {};
  info_chain->SetBranchAddress("process_name", &process_name_ptr);
  info_chain->SetBranchAddress("e_polarization", &e_polarization);
  info_chain->SetBranchAddress("p_polarization", &p_polarization);
  
  // For each event determine if final state (incl. pol.) already captured.
  // If so increment specific event counter, else create new one as 1.
  // -> Check only every 100th event to speed up (no file should have less...)
  for (int i=0; i<N_total; i+=100) {
    info_chain->GetEvent(i);
    string process_name = process_name_ptr->Data();
    
    // Test if final state already found
    if ( m_final_state_Nevents.find(process_name) == m_final_state_Nevents.end() ) {
      cout  << "Found final state: " <<  process_name << endl;
      m_final_state_Nevents[process_name] = pol_Nevents_map{};
    }
    
    // Test if polarization already found for this final state
    pol_Nevents_map* fs_pol_map = &m_final_state_Nevents[process_name]; 
    auto pol_pair = make_pair(e_polarization, p_polarization);
    if ( fs_pol_map->find(pol_pair) == fs_pol_map->end() ) {
      cout  << "Found final state - polarization pairing: " << process_name 
            << " " << e_polarization << " " << p_polarization << endl;
      m_final_state_Nevents[process_name][pol_pair] = 1;
    } else {
      m_final_state_Nevents[process_name][pol_pair]++;
    }
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
  
  this->findAllFinalStates( info_chain );
  
  info_chain->AddFriend(reco_chain, "reco");
  info_chain->AddFriend(mc_chain, "mcobs");
  info_chain->AddFriend(truth_chain, "mctruth");
  RDataFrame* befriended_dataframe = new RDataFrame(*info_chain);
  m_dataframe = befriended_dataframe;
  
  m_all_chains = {info_chain, reco_chain, mc_chain, truth_chain};
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

int Analyzer::getNProcessEvents( TString* process_name_ptr, float e_polarization, float p_polarization ) {
  string process_name = process_name_ptr->Data();
  
  // Test if final state is registered
  if ( m_final_state_Nevents.find(process_name) == m_final_state_Nevents.end() ) {
    cout  << "ERROR in Analyzer::getNProcessEvents: Unknown process "
          << process_name << endl;
    return -1;
  }
  
  // Test if polarization is registered
  pol_Nevents_map* fs_pol_map = &m_final_state_Nevents[process_name]; 
  auto pol_pair = make_pair(e_polarization, p_polarization);
  if ( fs_pol_map->find(pol_pair) == fs_pol_map->end() ) {
    cout  << "ERROR in Analyzer::getNProcessEvents: Unknown process - pol pairing "
    << process_name << " " << e_polarization << " " << p_polarization << endl;
    return -1;
  } 
  
  return m_final_state_Nevents[process_name][pol_pair];
}

//-------------------------------------------------------------------------------------------------

float Analyzer::getPolarizationWeight( float e_polarization, float p_polarization ) {
  return 0.5*( 1 + e_polarization * m_e_beam_polarization ) * 0.5*( 1 + p_polarization * m_p_beam_polarization );
}

//-------------------------------------------------------------------------------------------------

float Analyzer::getProcessWeight( TString* process_name_ptr, float e_polarization, float p_polarization, float cross_section ){
  float polarization_weight = this->getPolarizationWeight( e_polarization, p_polarization );
  int N_process_events = getNProcessEvents( process_name_ptr, e_polarization, p_polarization );
  return polarization_weight * ( cross_section * m_luminosity ) / float(N_process_events);
}

//-------------------------------------------------------------------------------------------------

function<float (TString, float, float, float)> Analyzer::getProcessWeightLambda() {
  return[this](TString process_name_ptr, float e_polarization, float p_polarization, float cross_section) {
    return this->getProcessWeight( &process_name_ptr, e_polarization, p_polarization, cross_section ); 
  };
}

//-------------------------------------------------------------------------------------------------
