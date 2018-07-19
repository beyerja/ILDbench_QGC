#include "../include/aQGCAnalyzer.h"

//-------------------------------------------------------------------------------------------------

void aQGCAnalyzer::performAnalysis(){
  this->getCombinedDataframe();
  
  // final cuts:
  float V_m_min         = 70;
  float V_m_max         = 90;
  float V_pT_min        = 10;
  float V_cosTheta_max  = 0.8;
  
  float VV_m_min        = 200;
  float VV_m_max        = 400;
  float VV_pT_min       = 10;
  float VV_ET_min       = 10;
  
  float m_recoil_min    = 100;
  
  float y_34_min        = 0.0001;
  
  float min_jetE_min            = 10;
  float min_jetNparticles_min   = 2;
  float min_jetNcharged_min     = 3;
  template<class T>
T getFilter( RDataFrame* rdf ) {
  return rdf->Filter( CutIsoLeps() );
}

  float leadEtrack_cosTheta_max = 2;
  float leadEtrack_coneE_min    = 0.99;
  
  auto data_after_cuts = m_dataframe
                         ->Filter( CutIsoLeps() )
                          .Filter( CutVMass(V_m_min, V_m_max) )
                          .Filter( CutVpT(V_pT_min) )
                          .Filter( CutVAbsCosTheta(V_cosTheta_max) )
                          .Filter( CutVVMass( VV_m_min, VV_m_max ) )
                          .Filter( CutVVpT( VV_pT_min ) )
                          .Filter( CutVVET( VV_ET_min ) )
                          .Filter( CutMRecoil( m_recoil_min ) )
                          .Filter( CutY34( y_34_min ) )
                          .Filter( CutJetE( min_jetE_min ) )
                          .Filter( CutJetNParticles( min_jetNparticles_min ) )
                          .Filter( CutJetNCharged( min_jetNcharged_min ) )
                          .Filter( CutLeadETrackAbsCosTheta( leadEtrack_cosTheta_max ) )
                          .Filter( CutLeadETrackConeE( leadEtrack_coneE_min ) );
                          
  // TODO Event weights!!!                              
                          
  cout << *data_after_cuts.Filter("mc_signal_type > 0").Count() << endl;
  
}

//-------------------------------------------------------------------------------------------------

