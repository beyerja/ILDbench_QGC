#include "../include/Analyzer.h"

//-------------------------------------------------------------------------------------------------

void Analyzer::setInputFiles( vector<TFile*> &input_files ){
  m_input_files = input_files;
}

//-------------------------------------------------------------------------------------------------


void Analyzer::aliasMCColumnsInDataframe( RDataFrame &dataframe ) {
  auto col_names = dataframe.GetColumnNames();
  regex mc_like {"mc.*"};
  for (auto &&col_name : col_names) {
    if ( regex_match( col_name, mc_like ) ) {
      string mc_column_alias = "mc_" + col_name.substr(3);
      dataframe.Alias( mc_column_alias , col_name );
    }
  }
}

//-------------------------------------------------------------------------------------------------

void Analyzer::getCombinedDataframes() {
  for ( auto const& file: m_input_files ) {
    TTree* reco_tree  = (TTree*)file->Get("recoObservablesTree");
    TTree* mc_tree    = (TTree*)file->Get("mcObservablesTree");
    reco_tree->AddFriend(mc_tree, "mc");
    RDataFrame befriended_dataframe = RDataFrame(*reco_tree);
    this->aliasMCColumnsInDataframe( befriended_dataframe );
    m_dataframes.push_back( befriended_dataframe );
  }
}

//-------------------------------------------------------------------------------------------------

void Analyzer::clearMemory(){
  for (auto const& tree_ptr : m_all_trees) {
    delete tree_ptr;
  }
  m_all_trees = {};
}

//-------------------------------------------------------------------------------------------------
