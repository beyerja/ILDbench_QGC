#ifndef ANALYZER_h 
#define ANALYZER_h 1

#include "../src/SelectionCutTemplates.cpp"

using namespace ROOT;

class Analyzer {
  
  protected:
    vector<string> m_input_file_paths {};
    RDataFrame* m_dataframe{};
    vector<TChain*> m_all_chains {};
    
    typedef map<pair<float,float>, int> pol_Nevents_map;
    map<string, pol_Nevents_map> m_final_state_Nevents{};

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
    void findAllFinalStates( TChain *info_chain );
    void getCombinedDataframe();
    virtual void performAnalysis() = 0;
    void clearMemory();
    
    int getNProcessEvents( TString* process_name_ptr, float e_polarization, float p_polarization );
    float getPolarizationWeight( float e_polarization, float p_polarization );
    float getProcessWeight(  TString* process_name_ptr, float e_polarization, float p_polarization, float cross_section );
    
  	function<float (TString, float, float, float)> getProcessWeightLambda(); 

};

#endif
