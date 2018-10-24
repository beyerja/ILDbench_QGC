#ifndef ANALYZER_h 
#define ANALYZER_h 1

#include "../src/SelectionCutTemplates.cpp"
#include "../src/VariableCombinationTemplates.cpp"
#include "../src/SystemHelper.cpp"

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

    string m_output_directory {"."};

  public:
    void setInputPaths( vector<string> &input_file_paths );
    void setBeamPolarizations( float &e_beam_polarization, float &p_beam_polarization );
    void setLuminosity( float &luminosity );
    void setOutputDirectory( string &output_directory );
    
    void run();

  protected:
    void findAllFinalStates( TTree *info_chain );
    void getCombinedDataframe();
    virtual void performAnalysis() = 0;
    void clearMemory();
    
    int getNProcessEvents( TString* process_name_ptr, float e_polarization, float p_polarization );
    float getPolarizationWeight( float e_polarization, float p_polarization );
    float getProcessWeight(  TString* process_name_ptr, float e_polarization, float p_polarization, float cross_section );
    
    // Lambda can be used in dataframe to extract weight
  	function<float (TString, float, float, float)> getProcessWeightLambda(); 

    string getOutputDirectory();
};

#endif
