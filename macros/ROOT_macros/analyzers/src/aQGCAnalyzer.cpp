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
  
  // TODO MAYBE IN COMPLETELY SEPARATE FUNCTION!
  // TODO THINK ABOUT WHAT TO DO TO GET TOTAL NUMBER OF EVENTS FOR ONE FINAL STATE!
  // TODO CREATE ACTUAL WEIGHTS BY COMBINING WITH PROCESS WEIGHTS
  // auto pol_weight_lambda = this->getPolarizationWeightLambda();
  
  
  // auto rdf_with_pol_weight = m_dataframe->Define("pol_weight", pol_weight_lambda, {"e_polarization","p_polarization"});  
  auto process_weight_lambda = this->getProcessWeightLambda();
  auto rdf_with_process_weight = m_dataframe->Define("process_weight", process_weight_lambda, {"process_name", "e_polarization", "p_polarization", "cross_section"});  
  
  auto rdf_data_after_cuts = rdf_with_process_weight.Filter( VMassCut, {"reco.V1_m"} )
                                    .Filter( VMassCut, {"reco.V2_m"} )
                                    .Filter( VAbsCosThetaCut, {"reco.V1_cosTheta"} )
                                    .Filter( VAbsCosThetaCut, {"reco.V2_cosTheta"} )
                                    .Filter( VVMassCut, {"reco.VV_m"} )
                                    .Filter( VVpTCut, {"reco.VV_pT"} )
                                    .Filter( VVETCut, {"reco.VV_ET"} )
                                    .Filter( MRecoilCut, {"reco.m_recoil"} )
                                    .Filter( Y34Cut, {"reco.y_34"} )
                                    .Filter( JetECut, {"reco.min_jetE"} )
                                    .Filter( JetNParticlesCut, {"reco.min_jetNparticles"} )
                                    .Filter( JetNChargedCut, {"reco.min_jetNcharged"} )
                                    .Filter( LeadETrackAbsCosThetaCut, {"reco.leadEtrack_cosTheta"} )
                                    .Filter( LeadETrackConeECut, {"reco.leadEtrack_coneE"} )
                                    .Filter( IsoLepsCut, {"reco.found_isolep"} );


  // // TODO Event weights!!!                              
  // 
  cout << *rdf_with_process_weight.Filter("mctruth.signal_type > 0").Count() << endl;
  cout << *rdf_with_process_weight.Filter("mctruth.signal_type == 0").Count() << endl;
  cout << *rdf_data_after_cuts.Filter("mctruth.signal_type > 0").Count() << endl;
  cout << *rdf_data_after_cuts.Filter("mctruth.signal_type == 0").Count() << endl;
  // 
  TCanvas *canvas_h1 = new TCanvas("test", "", 0, 0, 600, 600);
  auto h1_VV_m = rdf_data_after_cuts.Histo1D({"h1_VV_m", "Di-boson mass after cuts; m_{VV}; Events", 100, 0, 1000}, "reco.VV_m", "process_weight");
  auto h1_VV_m_nocuts = rdf_with_process_weight.Histo1D({"h1_VV_m_nocuts", "Di-boson mass before cuts; m_{VV}; Events", 100, 0, 1000}, "reco.VV_m", "process_weight");
  h1_VV_m_nocuts->Draw("hist");
  h1_VV_m->SetLineColor(2);
  h1_VV_m->Draw("hist same");
  canvas_h1->Print("/afs/desy.de/user/b/beyerjac/flc/VBS/aQGC_analysis/macros/ROOT_macros/test_plot.pdf");
  delete canvas_h1;
}

//-------------------------------------------------------------------------------------------------


