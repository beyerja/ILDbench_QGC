#include "../include/Analyzer.h"

//-------------------------------------------------------------------------------------------------

void Analyzer::setInputPaths( vector<string> &input_file_paths ){
  m_input_file_paths = input_file_paths;
}

//-------------------------------------------------------------------------------------------------


void Analyzer::aliasMCColumnsInDataframe() {
  auto col_names = m_dataframe->GetColumnNames();
  regex mc_like {"mc.*"};
  for (auto &&col_name : col_names) {
    if ( regex_match( col_name, mc_like ) ) {
      string mc_column_alias = "mc_" + col_name.substr(3);
      m_dataframe->Alias( mc_column_alias , col_name );
    }
  }
}

//-------------------------------------------------------------------------------------------------

void Analyzer::getCombinedDataframe() {
  TChain* reco_chain = new TChain("recoObservablesTree");
  TChain* mc_chain   = new TChain("mcObservablesTree");
  
  for ( auto const& file_path: m_input_file_paths ) {
    reco_chain ->Add( file_path.c_str() );
    mc_chain   ->Add( file_path.c_str() );
  }
  
  reco_chain->AddFriend(mc_chain, "mc");
  RDataFrame* befriended_dataframe = new RDataFrame(*reco_chain);
  m_dataframe = befriended_dataframe;
  this->aliasMCColumnsInDataframe();
  
  m_all_chains = {reco_chain, mc_chain};
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
