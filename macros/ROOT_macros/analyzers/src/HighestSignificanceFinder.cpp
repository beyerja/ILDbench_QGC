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

template<class Cut, class ParameterType>
float HighestSignificanceFinder::singleCutSignificance( ParameterType x, Cut cut, initializer_list<string> variable_names ){
  
  auto cut_lambda = [cut,x]( float value ) { return cut( value, x ); }; 
  
  auto is_sig_lambda = [] ( int mc_signal_type ) { return mc_signal_type>0; };
  auto is_bkg_lambda = [] ( int mc_signal_type ) { return mc_signal_type==0; };
  
  auto after_cuts = m_dataframe->Filter( cut_lambda, variable_names ); //.Filter( old_cuts );
  
  auto sig_after_cuts = after_cuts.Filter( is_sig_lambda, {"mc.signal_type"} );
  auto bkg_after_cuts = after_cuts.Filter( is_bkg_lambda, {"mc.signal_type"} );
  
  float N_sig = *sig_after_cuts.Count();
  float N_bkg = *bkg_after_cuts.Count();
  
  float significance = float( N_sig ) / sqrt( N_sig + N_bkg );
  return significance;
}

//-------------------------------------------------------------------------------------------------




//-------------------------------------------------------------------------------------------------

void HighestSignificanceFinder::performAnalysis(){
  initializer_list<string> variable_names = {"reco.VV_pT"};
  function<float (float)> neg_significance_lambda 
    = [this,variable_names](float x) { return this->singleCutSignificance( x, PassesCutVVpT, variable_names ); };
  Functor1D         neg_cut_significance (neg_significance_lambda);
  BrentMinimizer1D  cut_optimizer;
  cut_optimizer.SetFunction(neg_cut_significance, 0, 250);
  cut_optimizer.SetNpx( 5 );
  // cut_optimizer.Minimize(2, 5, 5);
  
  
  // optimized_cuts += "(" + CutVpT(cut_optimizer.XMinimum()) + ")";
  // 
  // neg_significance_lambda = [this,optimized_cuts](float x) { return this->singleCutSignificance( x, CutVAbsCosTheta, optimized_cuts ); };
  // cut_optimizer.SetFunction(neg_cut_significance, 0, 1.0);
  // cut_optimizer.Minimize(2, 0.1, 0.1);
  
  
  // vector<int>   Nsig_after_cut {};
  // vector<int>   Nbkg_after_cut {};
  // 
  // auto sig = m_dataframe->Filter("mc_signal_type > 0");
  // auto bkg = m_dataframe->Filter("mc_signal_type == 0");
  // 
  // float min_V_pT_start  = 0;
  // float min_V_pT_step   = 1;
  // float min_V_pT_end    = 20;
  // for ( float min_V_pT=min_V_pT_start; min_V_pT<min_V_pT_end; min_V_pT+=min_V_pT_step ) {
  //   Nsig_after_cut.push_back( *sig.Filter( CutVpT(min_V_pT).c_str() ).Count() );
  //   Nbkg_after_cut.push_back( *bkg.Filter( CutVpT(min_V_pT).c_str() ).Count() );
  // }
  // 
  // int max_significance_index = findMaxSignificanceIndex( Nsig_after_cut, Nbkg_after_cut );
  // float optimal_V_pT_cut = min_V_pT_start + max_significance_index*min_V_pT_step;
  // cout << optimal_V_pT_cut << endl;
  // 
  // Nsig_after_cut = {};
  // Nbkg_after_cut = {};
  // 
  // float min_m_recoil_start  = 0;
  // float min_m_recoil_step   = 10;
  // float min_m_recoil_end    = 20;
  // for ( float min_m_recoil=min_m_recoil_start; min_m_recoil<min_m_recoil_end; min_m_recoil+=min_m_recoil_step ) {
  //   Nsig_after_cut.push_back( *sig.Filter( CutVpT(optimal_V_pT_cut).c_str() ).Filter( CutMRecoil(min_m_recoil).c_str() ).Count() );
  //   Nbkg_after_cut.push_back( *bkg.Filter( CutVpT(optimal_V_pT_cut).c_str() ).Filter( CutMRecoil(min_m_recoil).c_str() ).Count() );
  // }
  // 
  // max_significance_index = findMaxSignificanceIndex( Nsig_after_cut, Nbkg_after_cut );
  // float optimal_m_recoil_cut = min_m_recoil_start + max_significance_index*min_m_recoil_step;
  // cout << optimal_m_recoil_cut << endl;
  // 
};

//-------------------------------------------------------------------------------------------------