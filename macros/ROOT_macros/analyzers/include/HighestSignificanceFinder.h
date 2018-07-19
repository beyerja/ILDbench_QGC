#ifndef HighestSignificanceFinder_h 
#define HighestSignificanceFinder_h 1

#include "../../src/Analyzer.cpp"

class HighestSignificanceFinder : public Analyzer {
  
  public:
    void performAnalysis();
    
  private:
    vector<float> getSignificance( vector<int> &sig, vector<int> &bkg );
    int findMaxSignificanceIndex( vector<int> &sig, vector<int> &bkg );
};


#endif