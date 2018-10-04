#ifndef EFTAnalyzer_h 
#define EFTAnalyzer_h 1

#include "../../src/Analyzer.cpp"
#include "../../src/InputManager.cpp"
#include "../../src/PythonHelper.cpp"

class EFTAnalyzer : public Analyzer {

  protected:
    typedef pair<string,string> FilePair;
    vector<FilePair> m_events_weights_pairs {};
    typedef pair<float,float> ParameterPoint;
    map<int, ParameterPoint> m_parameter_points {};
    string m_dummy_weights_file {};

  public:  
    void run();

  protected:
    void findParameterPoints( string weight_file_path );
    void setupDummyWeightsFile( string weight_file_path );
    void searchForWeightFile( string file_path );
    void searchForWeightFiles();
    void getCombinedDataframe();
    

    ParameterPoint getParameterPoint( int setting_index ) const;
    float getEventWeight( float event_weight, float process_weight );
    function<float (float, float)> getEventWeightLambda();
    
    virtual void performAnalysis() = 0;
};


#endif
