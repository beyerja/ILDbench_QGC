#include "../include/HighestSignificanceFinder.h"

//-------------------------------------------------------------------------------------------------

vector<float> HighestSignificanceFinder::getSignificance( vector<int> &sig, vector<int> &bkg ){
  vector<float> significance_vector;
  for ( int i=0; i<sig.size(); i++ ) {
    float significance = float(sig[i]) / sqrt(sig[i] + bkg[i]);
    significance_vector.push_back(significance);
  }
  return significance_vector;
}

//-------------------------------------------------------------------------------------------------

int HighestSignificanceFinder::findMaxSignificanceIndex( vector<int> &sig, vector<int> &bkg ){
  vector<float> significance {this->getSignificance(sig, bkg)};
  auto max_signif_element = max_element(significance.begin(), significance.end());
  int max_signif_index = distance(significance.begin(), max_signif_element);
  cout << *max_signif_element << endl;
  return max_signif_index;
}
  
//-------------------------------------------------------------------------------------------------

void HighestSignificanceFinder::performAnalysis(){
  this->getCombinedDataframe();
  
  vector<int>   Nsig_after_cut {};
  vector<int>   Nbkg_after_cut {};
  
  auto sig = m_dataframe->Filter("mc_signal_type > 0");
  auto bkg = m_dataframe->Filter("mc_signal_type == 0");
  
  float min_V_pT_start  = 0;
  float min_V_pT_step   = 1;
  float min_V_pT_end    = 20;
  for ( float min_V_pT=min_V_pT_start; min_V_pT<min_V_pT_end; min_V_pT+=min_V_pT_step ) {
    Nsig_after_cut.push_back( *sig.Filter( CutVpT(min_V_pT).c_str() ).Count() );
    Nbkg_after_cut.push_back( *bkg.Filter( CutVpT(min_V_pT).c_str() ).Count() );
  }
  
  int max_significance_index = findMaxSignificanceIndex( Nsig_after_cut, Nbkg_after_cut );
  float optimal_V_pT_cut = min_V_pT_start + max_significance_index*min_V_pT_step;
  cout << optimal_V_pT_cut << endl;
  
  Nsig_after_cut = {};
  Nbkg_after_cut = {};
  
  float min_m_recoil_start  = 0;
  float min_m_recoil_step   = 10;
  float min_m_recoil_end    = 20;
  for ( float min_m_recoil=min_m_recoil_start; min_m_recoil<min_m_recoil_end; min_m_recoil+=min_m_recoil_step ) {
    Nsig_after_cut.push_back( *sig.Filter( CutVpT(optimal_V_pT_cut).c_str() ).Filter( CutMRecoil(min_m_recoil).c_str() ).Count() );
    Nbkg_after_cut.push_back( *bkg.Filter( CutVpT(optimal_V_pT_cut).c_str() ).Filter( CutMRecoil(min_m_recoil).c_str() ).Count() );
  }
  
  max_significance_index = findMaxSignificanceIndex( Nsig_after_cut, Nbkg_after_cut );
  float optimal_m_recoil_cut = min_m_recoil_start + max_significance_index*min_m_recoil_step;
  cout << optimal_m_recoil_cut << endl;
  
};

//-------------------------------------------------------------------------------------------------