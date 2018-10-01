#include "../include/aQGCAnalyzer.h"

//-------------------------------------------------------------------------------------------------

void aQGCAnalyzer::performAnalysis(){
  // final cuts:
  float V_m_min         = 50;
  float V_m_max         = 110;
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
  
  float leadEtrack_cosTheta_max = 2;
  float leadEtrack_coneE_min    = 0.99;
  
  auto VMassCut                 = Cuts::getMinMaxCutLambda( V_m_min , V_m_max );
  auto VpTCut                   = Cuts::getMinCutLambda( V_pT_min );
  auto VAbsCosThetaCut          = Cuts::getMaxCutLambda( V_cosTheta_max );
  auto VVMassCut                = Cuts::getMinMaxCutLambda( VV_m_min, VV_m_max );
  auto VVpTCut                  = Cuts::getMinCutLambda( VV_pT_min );
  auto VVETCut                  = Cuts::getMinCutLambda( VV_ET_min );
  auto MRecoilCut               = Cuts::getMinCutLambda( m_recoil_min );
  auto Y34Cut                   = Cuts::getMinCutLambda( y_34_min );
  auto JetECut                  = Cuts::getMinCutLambda( min_jetE_min );
  auto JetNParticlesCut         = Cuts::getMinCutLambda( min_jetNparticles_min );
  auto JetNChargedCut           = Cuts::getMinCutLambda( min_jetNcharged_min );
  auto LeadETrackAbsCosThetaCut = Cuts::getMaxCutLambda( leadEtrack_cosTheta_max );
  auto LeadETrackConeECut       = Cuts::getMinCutLambda( leadEtrack_coneE_min );
  auto IsoLepsCut               = Cuts::getBoolCutLambda( false );
  
  auto data_after_cuts = m_dataframe->Filter( VMassCut, {"V1_m"} )
                                    .Filter( VMassCut, {"V2_m"} )
                                    .Filter( VAbsCosThetaCut, {"V1_cosTheta"} )
                                    .Filter( VAbsCosThetaCut, {"V2_cosTheta"} )
                                    .Filter( VVMassCut, {"VV_m"} )
                                    .Filter( VVpTCut, {"VV_pT"} )
                                    .Filter( VVETCut, {"VV_ET"} )
                                    .Filter( MRecoilCut, {"m_recoil"} )
                                    .Filter( Y34Cut, {"y_34"} )
                                    .Filter( JetECut, {"min_jetE"} )
                                    .Filter( JetNParticlesCut, {"min_jetNparticles"} )
                                    .Filter( JetNChargedCut, {"min_jetNcharged"} )
                                    .Filter( LeadETrackAbsCosThetaCut, {"leadEtrack_cosTheta"} )
                                    .Filter( LeadETrackConeECut, {"leadEtrack_coneE"} )
                                    .Filter( IsoLepsCut, {"found_isolep"} );

  // TODO CREATE ACTUAL WEIGHTS BY COMBINING WITH PROCESS WEIGHTS
  

  // // TODO Event weights!!!                              
  // 
  // TODO Test this!
  cout << *data_after_cuts.Filter("mctruth.signal_type > 0").Count() << endl;
  
  // auto h1_VV_m = data_after_cuts.Histo1D({"h1_VV_m", "Di-boson mass after cuts; m_{VV}; Events", 100, 0, 1000}, "VV_m", "event_weights");
}

//-------------------------------------------------------------------------------------------------


