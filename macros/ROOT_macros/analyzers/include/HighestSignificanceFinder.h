#ifndef HighestSignificanceFinder_h 
#define HighestSignificanceFinder_h 1

#include "../../src/Analyzer.cpp"

using namespace Math;

class HighestSignificanceFinder : public Analyzer {
  
  public:
    void performAnalysis();
    
  private:
    vector<float> getSignificance( vector<int> &sig, vector<int> &bkg );
    int findMaxSignificanceIndex( vector<int> &sig, vector<int> &bkg );
    
    template<class Cut, class ParameterType>
      float singleCutSignificance( ParameterType x, Cut cut, initializer_list<string> variable_names ); 
  };


#endif