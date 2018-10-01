#ifndef ANALYZER_h 
#define ANALYZER_h 1

#include "../src/SelectionCutTemplates.cpp"

using namespace ROOT;

class Analyzer {
  
  protected:
    vector<string> m_input_file_paths {};
    RDataFrame* m_dataframe{};
    vector<TChain*> m_all_chains {};

    float m_luminosity {};
    float m_e_beam_polarization {};
    float m_p_beam_polarization {};
    

  public:
    void setInputPaths( vector<string> &input_file_paths );
    void setBeamPolarizations( float &e_beam_polarization, float &p_beam_polarization );
    void setLuminosity( float &luminosity );
    
    void run();

  protected:
    void aliasMCColumnsInDataframe();
    void getCombinedDataframe();
    virtual void performAnalysis() = 0;
    void clearMemory();
    
    float getPolarizationWeight( float e_polarization, float p_polarization );
    float getProcessWeight( float polarization_weight, float cross_section, int N_process_events );
    
};

#endif