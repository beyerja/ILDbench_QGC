#ifndef ANALYZER_h 
#define ANALYZER_h 1

#include "../src/SelectionCutTemplates.cpp"

using namespace ROOT;

class Analyzer {
  
  protected:
    vector<string> m_input_file_paths {};
    RDataFrame* m_dataframe;
    vector<TChain*> m_all_chains;

  public:
    void setInputPaths( vector<string> &input_file_paths );
    virtual void performAnalysis() = 0;
    void clearMemory();

  protected:
    void aliasMCColumnsInDataframe();
    void getCombinedDataframe();
    
    
};

#endif