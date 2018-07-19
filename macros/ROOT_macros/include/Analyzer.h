#ifndef ANALYZER_h 
#define ANALYZER_h 1

#include "../src/SelectionCutTemplates.cpp"

using namespace ROOT;

class Analyzer {
  
  protected:
    vector<TFile*> m_input_files {};
    vector<RDataFrame> m_dataframes {};
    vector<TTree*> m_all_trees {};
  
  public:
    void setInputFiles( vector<TFile*> &input_files );
    virtual void performAnalysis() = 0;
    virtual void interpretResults() = 0;
    void clearMemory();

  protected:
    void aliasMCColumnsInDataframe( RDataFrame &rdf );
    void getCombinedDataframes();
    
    
};

#endif