#ifndef ANALYZER_h 
#define ANALYZER_h 1

#include "../src/SelectionCutTemplates.cpp"
#include "../src/VariableCombinationTemplates.cpp"
#include "../src/SystemHelper.cpp"

using namespace ROOT;

// TODO SOMEHOW REWRITE THIS SO THAT NO EVERY ANALYSIS NEEDS TO SEARCH FOR ALL FINAL STATES AND NEVENTS ETC
// TODO (e.g. create separate class for that part which then can/has to be passed to the analyzer class on construction)
class Analyzer {
  
  protected:
    vector<string> m_input_file_paths {};
    RDataFrame* m_dataframe = nullptr;
    vector<TChain*> m_all_chains {};
    
    set<string> m_final_states{};
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
    
    int getNProcessEvents( TString* process_name_ptr, float e_polarization, float p_polarization ) const;
    float getPolarizationWeight( float e_polarization, float p_polarization ) const;
    float getProcessWeight(  TString* process_name_ptr, float e_polarization, float p_polarization, float cross_section ) const;
    
    // Lambda can be used in dataframe to extract weight
  	function<float (TString, float, float, float)> getProcessWeightLambda() const; 

    string getOutputDirectory() const;
    set<string> getFinalStatesSet() const;
    
    // Convenient typedefs for working with RDF
    typedef ROOT::RDF::RResultPtr<ULong64_t> ResultCount;
    typedef ROOT::RDF::RResultPtr<RDFDetail::SumReturnType_t<float>> ResultSumFloat;
    typedef ROOT::RDF::RResultPtr<RDFDetail::SumReturnType_t<double>> ResultSumDouble;
    typedef ROOT::RDF::RResultPtr<TH1D> ResultTH1D;
    typedef ROOT::RDF::RResultPtr<TH2D> ResultTH2D;
};

#endif
