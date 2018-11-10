#include "../include/aQGCAnalyzer.h"

//-------------------------------------------------------------------------------------------------

void aQGCAnalyzer::performAnalysis(){
  // final cuts:
  // float V_m_min         = 50;
  // float V_m_max         = 110;
  // float V_pT_min        = 10;
  // float V_cosTheta_max  = 0.8;
  
  // float VV_m_min        = 200;
  // float VV_m_max        = 400;
  float VV_pT_min       = 40;
  float VV_ET_min       = 150;
  
  float m_recoil_min    = 200;
  float recoil_cosTheta_max = 0.99;
  
  float y_34_min        = 0.0001;
  
  float min_jetE_min            = 10;
  float min_jetNparticles_min   = 2;
  float min_jetNcharged_min     = 3;
  
  float leadEtrack_cosTheta_max = 0.99;
  float leadEtrack_coneE_min    = 2;
  
  // auto VMassCut                 = Cuts::getMinMaxCutLambda( V_m_min , V_m_max );
  // auto VpTCut                   = Cuts::getMinCutLambda( V_pT_min );
  // auto VAbsCosThetaCut          = Cuts::getMaxCutLambda( V_cosTheta_max );
  // auto VVMassCut                = Cuts::getMinMaxCutLambda( VV_m_min, VV_m_max );
  auto VVpTCut                  = Cuts::getMinCutLambda( VV_pT_min );
  auto VVETCut                  = Cuts::getMinCutLambda( VV_ET_min );
  auto MRecoilCut               = Cuts::getMinCutLambda( m_recoil_min );
  auto RecoilAbsCosThetaCut     = Cuts::getMinMaxCutLambda( float(-1.0*recoil_cosTheta_max) , recoil_cosTheta_max );
  auto Y34Cut                   = Cuts::getMinCutLambda( y_34_min );
  auto JetECut                  = Cuts::getMinCutLambda( min_jetE_min );
  auto JetNParticlesCut         = Cuts::getMinCutLambda( min_jetNparticles_min );
  auto JetNChargedCut           = Cuts::getMinCutLambda( min_jetNcharged_min );
  auto LeadETrackAbsCosThetaCut = Cuts::getMinMaxCutLambda( float(-1.0*leadEtrack_cosTheta_max) , leadEtrack_cosTheta_max );
  auto LeadETrackConeECut       = Cuts::getMinCutLambda( leadEtrack_coneE_min );
  auto IsoLepsCut               = Cuts::getBoolCutLambda( false );
  
  auto IsTrueWWSignalCut          = Cuts::getIntCutLambda( 24 );
  auto IsTrueZZSignalCut          = Cuts::getIntCutLambda( 23 );
  auto IsTrueBkgCut             = Cuts::getIntCutLambda( 0 );
  
  auto process_weight_lambda = this->getProcessWeightLambda();
  
  
  float min_VmSum_WW = 145;
  float max_VmSum_ZZ = 205;
  float border_VmSum = 178;
  float max_Vm_diff = 25;
  
  auto WWRegionVmSumCut = Cuts::getMinMaxCutLambda(min_VmSum_WW, border_VmSum);
  auto ZZRegionVmSumCut = Cuts::getMinMaxCutLambda(border_VmSum, max_VmSum_ZZ);
  auto VVRegionVmDiffCut = Cuts::getMinMaxCutLambda(float(-1.*border_VmSum),border_VmSum);
  
  //============================================================================
  //============================================================================
  // Start coding lazy actions from here!
  //============================================================================
  //============================================================================
  
  
  // TODO make this standard when possible with newer root version!
  auto rdf_with_extra_variables = m_dataframe->Define( "recoVmMean", VarComb::getMeanLambda<float>(), {"reco.V1_m", "reco.V2_m"} )
                                              .Define( "recoVmSum", VarComb::getPlusLambda<float>(), {"reco.V1_m", "reco.V2_m"})
                                              .Define( "recoVmDiff", VarComb::getMinusLambda<float>(), {"reco.V1_m", "reco.V2_m"});  
  auto rdf_with_process_weight = rdf_with_extra_variables.Define("process_weight", process_weight_lambda, {"process_name", "e_polarization", "p_polarization", "cross_section"});  
  
  // auto testing_h = rdf_with_process_weight.Histo1D({"testing_h", "Process weight; weight; Events", 100, 0, 100}, "process_weight");
  // unique_ptr<TCanvas> testing_c (new TCanvas("testing_c", "", 0, 0, 700, 600));
  // testing_h->Draw("hist");
  // testing_c->Print( ( this->getOutputDirectory() + "/testing_c.pdf").c_str() );
  // 
  // return;
  
  auto rdf_WWsignal_no_cuts = rdf_with_process_weight.Filter( IsTrueWWSignalCut, {"mctruth.signal_type"} );
  auto rdf_ZZsignal_no_cuts = rdf_with_process_weight.Filter( IsTrueZZSignalCut, {"mctruth.signal_type"} );
  auto rdf_bkg_no_cuts    = rdf_with_process_weight.Filter( IsTrueBkgCut, {"mctruth.signal_type"} );
  
  //----------------------------------------------------------------------------
  // Apply all selection cuts
  auto rdf_data_after_cuts = rdf_with_process_weight
                                    // .Filter( VMassCut, {"reco.V1_m"} )
                                    // .Filter( VMassCut, {"reco.V2_m"} )
                                    // .Filter( VAbsCosThetaCut, {"reco.V1_cosTheta"} )
                                    // .Filter( VAbsCosThetaCut, {"reco.V2_cosTheta"} )
                                    // .Filter( VVMassCut, {"reco.VV_m"} )
                                    .Filter( VVpTCut, {"reco.VV_pT"} )
                                    .Filter( VVETCut, {"reco.VV_ET"} )
                                    .Filter( MRecoilCut, {"reco.m_recoil"} )
                                    // .Filter( RecoilAbsCosThetaCut, {"reco.cosTheta_recoil"} )
                                    .Filter( Y34Cut, {"reco.y_34"} )
                                    .Filter( JetECut, {"reco.min_jetE"} )
                                    .Filter( JetNParticlesCut, {"reco.min_jetNparticles"} )
                                    .Filter( JetNChargedCut, {"reco.min_jetNcharged"} )
                                    .Filter( LeadETrackAbsCosThetaCut, {"reco.leadEtrack_cosTheta"} )
                                    .Filter( LeadETrackConeECut, {"reco.leadEtrack_coneE"} )
                                    .Filter( IsoLepsCut, {"reco.found_isolep"} );
  //----------------------------------------------------------------------------

  auto rdf_WWsignal_with_cuts = rdf_data_after_cuts.Filter( IsTrueWWSignalCut, {"mctruth.signal_type"} );
  auto rdf_ZZsignal_with_cuts = rdf_data_after_cuts.Filter( IsTrueZZSignalCut, {"mctruth.signal_type"} );
  auto rdf_bkg_with_cuts    = rdf_data_after_cuts.Filter( IsTrueBkgCut, {"mctruth.signal_type"} );

  auto rdf_inZZregion_with_cuts = rdf_data_after_cuts.Filter( ZZRegionVmSumCut, {"recoVmSum"} ).Filter( VVRegionVmDiffCut, {"recoVmDiff"} );
  auto rdf_inWWregion_with_cuts = rdf_data_after_cuts.Filter( WWRegionVmSumCut, {"recoVmSum"} ).Filter( VVRegionVmDiffCut, {"recoVmDiff"} );

  auto rdf_ZZsignal_inZZregion_with_cuts = rdf_inZZregion_with_cuts.Filter( IsTrueZZSignalCut, {"mctruth.signal_type"} );
  auto rdf_ZZsignal_inWWregion_with_cuts = rdf_inWWregion_with_cuts.Filter( IsTrueZZSignalCut, {"mctruth.signal_type"} );
  auto rdf_WWsignal_inZZregion_with_cuts = rdf_inZZregion_with_cuts.Filter( IsTrueWWSignalCut, {"mctruth.signal_type"} );
  auto rdf_WWsignal_inWWregion_with_cuts = rdf_inWWregion_with_cuts.Filter( IsTrueWWSignalCut, {"mctruth.signal_type"} );
  auto rdf_bkg_inZZregion_with_cuts = rdf_inZZregion_with_cuts.Filter( IsTrueBkgCut, {"mctruth.signal_type"} );
  auto rdf_bkg_inWWregion_with_cuts = rdf_inWWregion_with_cuts.Filter( IsTrueBkgCut, {"mctruth.signal_type"} );
  
  //----------------------------------------------------------------------------
  // Counts on generator level
  
  // TODO THIS LEVEL DISTINCTION DOESN'T MAKE SENSE, REMOVE IT!
  
  //----------------------------------------------------------------------------
  // Histograms on generator level
  auto h1_genlevel_VV_m_WWsignal = rdf_WWsignal_no_cuts.Histo1D({"h1_VV_m_gen_level_WWsignal", "Di-boson mass, generator level; m_{VV}; Events", 100, 0, 1000}, "mctruth.VV_m", "process_weight");
  auto h1_genlevel_VV_m_ZZsignal = rdf_ZZsignal_no_cuts.Histo1D({"h1_VV_m_gen_level_ZZsignal", "Di-boson mass, generator level; m_{VV}; Events", 100, 0, 1000}, "mctruth.VV_m", "process_weight");
  
  //----------------------------------------------------------------------------
  // Counts before cuts
  auto count_WWsignal_no_cuts   = rdf_WWsignal_no_cuts.Sum("process_weight");
  auto count_ZZsignal_no_cuts   = rdf_ZZsignal_no_cuts.Sum("process_weight");
  auto count_bkg_no_cuts        = rdf_bkg_no_cuts.Sum("process_weight");
  //----------------------------------------------------------------------------
  
  //----------------------------------------------------------------------------
  // Histograms before cuts
  auto h1_VV_m_nocuts = rdf_with_process_weight.Histo1D({"h1_VV_m_nocuts", "Di-boson mass before cuts; m_{VV}; Events", 100, 0, 1000}, "reco.VV_m", "process_weight");
  auto h1_VV_m_nocuts_WWsignal = rdf_WWsignal_no_cuts.Histo1D({"h1_VV_m_nocuts_WWsignal", "Di-boson mass before cuts; m_{VV}; Events", 100, 0, 1000}, "reco.VV_m", "process_weight");
  auto h1_VV_m_nocuts_ZZsignal = rdf_ZZsignal_no_cuts.Histo1D({"h1_VV_m_nocuts_ZZsignal", "Di-boson mass before cuts; m_{VV}; Events", 100, 0, 1000}, "reco.VV_m", "process_weight");
  auto h1_VV_m_nocuts_bkg = rdf_bkg_no_cuts.Histo1D({"h1_VV_m_nocuts_bkg", "Di-boson mass before cuts; m_{VV}; Events", 100, 0, 1000}, "reco.VV_m", "process_weight");
  //----------------------------------------------------------------------------
  
  //----------------------------------------------------------------------------
  // Counts after cuts
  auto count_WWsignal_with_cuts = rdf_WWsignal_with_cuts.Sum("process_weight");
  auto count_ZZsignal_with_cuts = rdf_ZZsignal_with_cuts.Sum("process_weight");
  auto count_bkg_with_cuts      = rdf_bkg_with_cuts.Sum("process_weight");
  
  auto count_ZZsignal_inZZregion_with_cuts = rdf_ZZsignal_inZZregion_with_cuts.Sum("process_weight");
  auto count_ZZsignal_inWWregion_with_cuts = rdf_ZZsignal_inWWregion_with_cuts.Sum("process_weight");
  auto count_WWsignal_inWWregion_with_cuts = rdf_WWsignal_inWWregion_with_cuts.Sum("process_weight");
  auto count_WWsignal_inZZregion_with_cuts = rdf_WWsignal_inZZregion_with_cuts.Sum("process_weight");
  
  vector<pair<string, pair<ResultSumDouble, ResultSumDouble>>> bkg_final_state_counts {};
  for ( auto & final_state: this->getFinalStatesSet() ) {
    auto FinalStateTest = Cuts::getStringTestLambda(final_state);
    auto final_state_count_with_cuts = rdf_bkg_with_cuts.Filter(FinalStateTest, {"process_name"}).Sum("process_weight");
    auto final_state_count_no_cuts = rdf_bkg_no_cuts.Filter(FinalStateTest, {"process_name"}).Sum("process_weight");
    bkg_final_state_counts.push_back( make_pair(final_state, make_pair(final_state_count_no_cuts, final_state_count_with_cuts)) );
  }
  
  //----------------------------------------------------------------------------

  //----------------------------------------------------------------------------
  // Histograms after cuts
  auto h1_VmMean_withcuts_WWsignal  = rdf_WWsignal_with_cuts.Histo1D({"h1_VmMean_withcuts_WWsignal", "w/ cuts; m_{jj,1}+m_{jj,2}/2 [GeV]; Events", 70, 50, 120}, "recoVmMean", "process_weight");
  auto h1_VmMean_withcuts_ZZsignal  = rdf_ZZsignal_with_cuts.Histo1D({"h1_VmMean_withcuts_ZZsignal", "w/ cuts; m_{jj,1}+m_{jj,2}/2 [GeV]; Events", 70, 50, 120}, "recoVmMean", "process_weight");
  auto h1_VmMean_withcuts_bkg       = rdf_bkg_with_cuts.Histo1D({"h1_VmMean_withcuts_bkg", "w/ cuts; m_{jj,1}+m_{jj,2}/2 [GeV]; Events", 70, 50, 120}, "recoVmMean", "process_weight");
  
  auto h2_Vm_withcuts_WWsignal  = rdf_WWsignal_with_cuts.Histo2D({"h2_Vm_withcuts_WWsignal", "w/ cuts ; m_{jj,1} [GeV]; m_{jj,2} [GeV]; Events", 35, 50, 120, 35, 50, 120}, "reco.V1_m", "reco.V2_m", "process_weight");
  auto h2_Vm_withcuts_ZZsignal  = rdf_ZZsignal_with_cuts.Histo2D({"h2_Vm_withcuts_ZZsignal", "w/ cuts ; m_{jj,1} [GeV]; m_{jj,2} [GeV]; Events", 35, 50, 120, 35, 50, 120}, "reco.V1_m", "reco.V2_m", "process_weight");
  auto h2_Vm_withcuts_bkg       = rdf_bkg_with_cuts.Histo2D({"h2_Vm_withcuts_bkg", "w/ cuts ; m_{jj,1} [GeV]; m_{jj,2} [GeV]; Events", 35, 50, 120, 35, 50, 120}, "reco.V1_m", "reco.V2_m", "process_weight");
  
  auto h1_VV_m_withcuts           = rdf_data_after_cuts.Histo1D({"h1_VV_m", "Di-boson mass after cuts; m_{VV}; Events", 50, 0, 1000}, "reco.VV_m", "process_weight");
  auto h1_VV_m_withcuts_WWsignal  = rdf_WWsignal_with_cuts.Histo1D({"h1_VV_m_WWsignal", "Di-boson mass after cuts; m_{VV}; Events", 50, 0, 1000}, "reco.VV_m", "process_weight");
  auto h1_VV_m_withcuts_ZZsignal  = rdf_ZZsignal_with_cuts.Histo1D({"h1_VV_m_ZZsignal", "Di-boson mass after cuts; m_{VV}; Events", 50, 0, 1000}, "reco.VV_m", "process_weight");
  auto h1_VV_m_withcuts_bkg       = rdf_bkg_with_cuts.Histo1D({"h1_VV_m_bkg", "Di-boson mass after cuts; m_{VV}; Events", 50, 0, 1000}, "reco.VV_m", "process_weight");
  
  auto h1_absCosThetaVstar_withcuts_WWsignal = rdf_WWsignal_with_cuts.Histo1D({"h1_absCosThetaVstar_withcuts_WWsignal", "V angle in VV system after cuts; |cos #theta*_{V}|; Events", 20, 0, 1}, "reco.VV_V_absCosThetaStar", "process_weight");
  auto h1_absCosThetaVstar_withcuts_ZZsignal = rdf_ZZsignal_with_cuts.Histo1D({"h1_absCosThetaVstar_withcuts_ZZsignal", "V angle in VV system after cuts; |cos #theta*_{V}|; Events", 20, 0, 1}, "reco.VV_V_absCosThetaStar", "process_weight");
  auto h1_absCosThetaVstar_withcuts_bkg      = rdf_bkg_with_cuts     .Histo1D({"h1_absCosThetaVstar_withcuts_bkg", "V angle in VV system after cuts; |cos #theta*_{V}|; Events", 20, 0, 1}, "reco.VV_V_absCosThetaStar", "process_weight");
  
  auto h1_absCosThetaJetstarV1_withcuts_WWsignal = rdf_WWsignal_with_cuts.Histo1D({"h1_absCosThetaJetstarV1_withcuts_WWsignal", "jet angle in V1 system after cuts; |cos #theta*_{jet}|_{V_{1}}; Events", 20, 0, 1}, "reco.V1_jet_absCosThetaStar", "process_weight");
  auto h1_absCosThetaJetstarV1_withcuts_ZZsignal = rdf_ZZsignal_with_cuts.Histo1D({"h1_absCosThetaJetstarV1_withcuts_ZZsignal", "jet angle in V1 system after cuts; |cos #theta*_{jet}|_{V_{1}}; Events", 20, 0, 1}, "reco.V1_jet_absCosThetaStar", "process_weight");
  auto h1_absCosThetaJetstarV1_withcuts_bkg      = rdf_bkg_with_cuts     .Histo1D({"h1_absCosThetaJetstarV1_withcuts_bkg", "jet angle in V1 system after cuts; |cos #theta*_{jet}|_{V_{1}}; Events", 20, 0, 1}, "reco.V1_jet_absCosThetaStar", "process_weight");
  
  auto h1_absCosThetaJetstarV2_withcuts_WWsignal = rdf_WWsignal_with_cuts.Histo1D({"h1_absCosThetaJetstarV2_withcuts_WWsignal", "jet angle in V2 system after cuts; |cos #theta*_{jet}|_{V_{2}}; Events", 20, 0, 1}, "reco.V2_jet_absCosThetaStar", "process_weight");
  auto h1_absCosThetaJetstarV2_withcuts_ZZsignal = rdf_ZZsignal_with_cuts.Histo1D({"h1_absCosThetaJetstarV2_withcuts_ZZsignal", "jet angle in V2 system after cuts; |cos #theta*_{jet}|_{V_{2}}; Events", 20, 0, 1}, "reco.V2_jet_absCosThetaStar", "process_weight");
  auto h1_absCosThetaJetstarV2_withcuts_bkg      = rdf_bkg_with_cuts     .Histo1D({"h1_absCosThetaJetstarV2_withcuts_bkg", "jet angle in V2 system after cuts; |cos #theta*_{jet}|_{V_{2}}; Events", 20, 0, 1}, "reco.V2_jet_absCosThetaStar", "process_weight");
  
  // ----------- ZZ region
  auto h1_VV_m_withcuts_WWsignal_inZZregion  = rdf_WWsignal_inZZregion_with_cuts.Histo1D({"h1_VV_m_WWsignal_inZZregion", "Di-boson mass after cuts; m_{VV}; Events", 50, 0, 1000}, "reco.VV_m", "process_weight");
  auto h1_VV_m_withcuts_ZZsignal_inZZregion  = rdf_ZZsignal_inZZregion_with_cuts.Histo1D({"h1_VV_m_ZZsignal_inZZregion", "Di-boson mass after cuts; m_{VV}; Events", 50, 0, 1000}, "reco.VV_m", "process_weight");
  auto h1_VV_m_withcuts_bkg_inZZregion       = rdf_bkg_inZZregion_with_cuts.Histo1D({"h1_VV_m_bkg_inZZregion", "Di-boson mass after cuts; m_{VV}; Events", 50, 0, 1000}, "reco.VV_m", "process_weight");
  
  auto h1_absCosThetaVstar_withcuts_WWsignal_inZZregion = rdf_WWsignal_inZZregion_with_cuts.Histo1D({"h1_absCosThetaVstar_withcuts_WWsignal_inZZregion", "V angle in VV system after cuts;  |cos #theta*_{V}|; Events", 20, 0, 1}, "reco.VV_V_absCosThetaStar", "process_weight");
  auto h1_absCosThetaVstar_withcuts_ZZsignal_inZZregion = rdf_ZZsignal_inZZregion_with_cuts.Histo1D({"h1_absCosThetaVstar_withcuts_ZZsignal_inZZregion", "V angle in VV system after cuts;  |cos #theta*_{V}|; Events", 20, 0, 1}, "reco.VV_V_absCosThetaStar", "process_weight");
  auto h1_absCosThetaVstar_withcuts_bkg_inZZregion      = rdf_bkg_inZZregion_with_cuts     .Histo1D({"h1_absCosThetaVstar_withcuts_bkg_inZZregion", "V angle in VV system after cuts;  |cos #theta*_{V}|; Events", 20, 0, 1}, "reco.VV_V_absCosThetaStar", "process_weight");
  
  auto h1_absCosThetaJetstarV1_withcuts_WWsignal_inZZregion = rdf_WWsignal_inZZregion_with_cuts.Histo1D({"h1_absCosThetaJetstarV1_withcuts_WWsignal_inZZregion", "jet angle in V1 system after cuts ; |cos #theta*_{jet}|_{V_{1}}; Events", 20, 0, 1}, "reco.V1_jet_absCosThetaStar", "process_weight");
  auto h1_absCosThetaJetstarV1_withcuts_ZZsignal_inZZregion = rdf_ZZsignal_inZZregion_with_cuts.Histo1D({"h1_absCosThetaJetstarV1_withcuts_ZZsignal_inZZregion", "jet angle in V1 system after cuts ; |cos #theta*_{jet}|_{V_{1}}; Events", 20, 0, 1}, "reco.V1_jet_absCosThetaStar", "process_weight");
  auto h1_absCosThetaJetstarV1_withcuts_bkg_inZZregion      = rdf_bkg_inZZregion_with_cuts     .Histo1D({"h1_absCosThetaJetstarV1_withcuts_bkg_inZZregion", "jet angle in V1 system after cuts ; |cos #theta*_{jet}|_{V_{1}}; Events", 20, 0, 1}, "reco.V1_jet_absCosThetaStar", "process_weight");
  
  auto h1_absCosThetaJetstarV2_withcuts_WWsignal_inZZregion = rdf_WWsignal_inZZregion_with_cuts.Histo1D({"h1_absCosThetaJetstarV2_withcuts_WWsignal_inZZregion", "jet angle in V2 system after cuts ; |cos #theta*_{jet}|_{V_{2}}; Events", 20, 0, 1}, "reco.V2_jet_absCosThetaStar", "process_weight");
  auto h1_absCosThetaJetstarV2_withcuts_ZZsignal_inZZregion = rdf_ZZsignal_inZZregion_with_cuts.Histo1D({"h1_absCosThetaJetstarV2_withcuts_ZZsignal_inZZregion", "jet angle in V2 system after cuts ; |cos #theta*_{jet}|_{V_{2}}; Events", 20, 0, 1}, "reco.V2_jet_absCosThetaStar", "process_weight");
  auto h1_absCosThetaJetstarV2_withcuts_bkg_inZZregion      = rdf_bkg_inZZregion_with_cuts     .Histo1D({"h1_absCosThetaJetstarV2_withcuts_bkg_inZZregion", "jet angle in V2 system after cuts ; |cos #theta*_{jet}|_{V_{2}}; Events", 20, 0, 1}, "reco.V2_jet_absCosThetaStar", "process_weight");
  // ---------------------
  
  // ----------- WW region
  auto h1_VV_m_withcuts_WWsignal_inWWregion  = rdf_WWsignal_inWWregion_with_cuts.Histo1D({"h1_VV_m_WWsignal_inWWregion", "Di-boson mass after cuts; m_{VV}; Events", 50, 0, 1000}, "reco.VV_m", "process_weight");
  auto h1_VV_m_withcuts_ZZsignal_inWWregion  = rdf_ZZsignal_inWWregion_with_cuts.Histo1D({"h1_VV_m_ZZsignal_inWWregion", "Di-boson mass after cuts; m_{VV}; Events", 50, 0, 1000}, "reco.VV_m", "process_weight");
  auto h1_VV_m_withcuts_bkg_inWWregion       = rdf_bkg_inWWregion_with_cuts.Histo1D({"h1_VV_m_bkg_inWWregion", "Di-boson mass after cuts; m_{VV}; Events", 50, 0, 1000}, "reco.VV_m", "process_weight");
  
  auto h1_absCosThetaVstar_withcuts_WWsignal_inWWregion = rdf_WWsignal_inWWregion_with_cuts.Histo1D({"h1_absCosThetaVstar_withcuts_WWsignal_inWWregion", "V angle in VV system after cuts;  |cos #theta*_{V}|; Events", 20, 0, 1}, "reco.VV_V_absCosThetaStar", "process_weight");
  auto h1_absCosThetaVstar_withcuts_ZZsignal_inWWregion = rdf_ZZsignal_inWWregion_with_cuts.Histo1D({"h1_absCosThetaVstar_withcuts_ZZsignal_inWWregion", "V angle in VV system after cuts;  |cos #theta*_{V}|; Events", 20, 0, 1}, "reco.VV_V_absCosThetaStar", "process_weight");
  auto h1_absCosThetaVstar_withcuts_bkg_inWWregion      = rdf_bkg_inWWregion_with_cuts     .Histo1D({"h1_absCosThetaVstar_withcuts_bkg_inWWregion", "V angle in VV system after cuts;  |cos #theta*_{V}|; Events", 20, 0, 1}, "reco.VV_V_absCosThetaStar", "process_weight");
  
  auto h1_absCosThetaJetstarV1_withcuts_WWsignal_inWWregion = rdf_WWsignal_inWWregion_with_cuts.Histo1D({"h1_absCosThetaJetstarV1_withcuts_WWsignal_inWWregion", "jet angle in V1 system after cuts ; |cos #theta*_{jet}|_{V_{1}}; Events", 20, 0, 1}, "reco.V1_jet_absCosThetaStar", "process_weight");
  auto h1_absCosThetaJetstarV1_withcuts_ZZsignal_inWWregion = rdf_ZZsignal_inWWregion_with_cuts.Histo1D({"h1_absCosThetaJetstarV1_withcuts_ZZsignal_inWWregion", "jet angle in V1 system after cuts ; |cos #theta*_{jet}|_{V_{1}}; Events", 20, 0, 1}, "reco.V1_jet_absCosThetaStar", "process_weight");
  auto h1_absCosThetaJetstarV1_withcuts_bkg_inWWregion      = rdf_bkg_inWWregion_with_cuts     .Histo1D({"h1_absCosThetaJetstarV1_withcuts_bkg_inWWregion", "jet angle in V1 system after cuts ; |cos #theta*_{jet}|_{V_{1}}; Events", 20, 0, 1}, "reco.V1_jet_absCosThetaStar", "process_weight");
  
  auto h1_absCosThetaJetstarV2_withcuts_WWsignal_inWWregion = rdf_WWsignal_inWWregion_with_cuts.Histo1D({"h1_absCosThetaJetstarV2_withcuts_WWsignal_inWWregion", "jet angle in V2 system after cuts ; |cos #theta*_{jet}|_{V_{2}}; Events", 20, 0, 1}, "reco.V2_jet_absCosThetaStar", "process_weight");
  auto h1_absCosThetaJetstarV2_withcuts_ZZsignal_inWWregion = rdf_ZZsignal_inWWregion_with_cuts.Histo1D({"h1_absCosThetaJetstarV2_withcuts_ZZsignal_inWWregion", "jet angle in V2 system after cuts ; |cos #theta*_{jet}|_{V_{2}}; Events", 20, 0, 1}, "reco.V2_jet_absCosThetaStar", "process_weight");
  auto h1_absCosThetaJetstarV2_withcuts_bkg_inWWregion      = rdf_bkg_inWWregion_with_cuts     .Histo1D({"h1_absCosThetaJetstarV2_withcuts_bkg_inWWregion", "jet angle in V2 system after cuts ; |cos #theta*_{jet}|_{V_{2}}; Events", 20, 0, 1}, "reco.V2_jet_absCosThetaStar", "process_weight");
  // ---------------------
  
  vector<pair<string, ResultTH1D>>  bkg_final_state_h1_VV_m_withcuts {}, 
                                    bkg_final_state_h1_absCosThetaVstar_withcuts {};
  vector<pair<string,pair<ResultTH1D,ResultTH1D>>> bkg_final_state_h1_absCosThetaJetstar_withcuts {}; 
  
  vector<pair<string, ResultTH1D>>  bkg_final_state_inZZregion_h1_VV_m_withcuts {}, 
                                    bkg_final_state_inZZregion_h1_absCosThetaVstar_withcuts {};
  vector<pair<string,pair<ResultTH1D,ResultTH1D>>> bkg_final_state_inZZregion_h1_absCosThetaJetstar_withcuts {}; 
  
  vector<pair<string, ResultTH1D>>  bkg_final_state_inWWregion_h1_VV_m_withcuts {}, 
                                    bkg_final_state_inWWregion_h1_absCosThetaVstar_withcuts {};
  vector<pair<string,pair<ResultTH1D,ResultTH1D>>> bkg_final_state_inWWregion_h1_absCosThetaJetstar_withcuts {}; 
                                    
                                    
  for ( auto & final_state: this->getFinalStatesSet() ) {
    auto FinalStateTest = Cuts::getStringTestLambda(final_state);
    auto rdf_final_state = rdf_bkg_with_cuts.Filter(FinalStateTest, {"process_name"});
    auto h1_VV_m_withcuts_final_state                 = rdf_final_state.Histo1D({("h1_VV_m_" + final_state).c_str(), "Di-boson mass after cuts; m_{VV}; Events", 50, 0, 1000}, "reco.VV_m", "process_weight");
    auto h1_absCosThetaVstar_withcuts_final_state     = rdf_final_state.Histo1D({("h1_absCosThetaVstar_withcuts_" + final_state).c_str(), "V angle in VV system after cuts; |cos #theta*_{V}|; Events", 20, 0, 1}, "reco.VV_V_absCosThetaStar", "process_weight");
    auto h1_absCosThetaJetstarV1_withcuts_final_state = rdf_final_state.Histo1D({("h1_absCosThetaJetstarV1_withcuts_" + final_state).c_str(), "jet angle in V1 system after cuts; |cos #theta*_{jet}|_{V_{1}}; Events", 20, 0, 1}, "reco.V1_jet_absCosThetaStar", "process_weight");
    auto h1_absCosThetaJetstarV2_withcuts_final_state = rdf_final_state.Histo1D({("h1_absCosThetaJetstarV2_withcuts_" + final_state).c_str(), "jet angle in V2 system after cuts; |cos #theta*_{jet}|_{V_{2}}; Events", 20, 0, 1}, "reco.V2_jet_absCosThetaStar", "process_weight");
    
    // ----------- ZZ region
    auto rdf_final_state_inZZregion = rdf_bkg_inZZregion_with_cuts.Filter(FinalStateTest, {"process_name"});
    auto h1_VV_m_withcuts_final_state_inZZregion                 = rdf_final_state_inZZregion.Histo1D({("h1_VV_m_" + final_state + "_inZZregion").c_str(), "Di-boson mass after cuts; m_{VV}; Events", 50, 0, 1000}, "reco.VV_m", "process_weight");
    auto h1_absCosThetaVstar_withcuts_final_state_inZZregion     = rdf_final_state_inZZregion.Histo1D({("h1_absCosThetaVstar_withcuts_" + final_state + "_inZZregion").c_str(), "V angle in VV system after cuts; |cos #theta*_{V}|; Events", 20, 0, 1}, "reco.VV_V_absCosThetaStar", "process_weight");
    auto h1_absCosThetaJetstarV1_withcuts_final_state_inZZregion = rdf_final_state_inZZregion.Histo1D({("h1_absCosThetaJetstarV1_withcuts_" + final_state + "_inZZregion").c_str(), "jet angle in V1 system after cuts; |cos #theta*_{jet}|_{V_{1}}; Events", 20, 0, 1}, "reco.V1_jet_absCosThetaStar", "process_weight");
    auto h1_absCosThetaJetstarV2_withcuts_final_state_inZZregion = rdf_final_state_inZZregion.Histo1D({("h1_absCosThetaJetstarV2_withcuts_" + final_state + "_inZZregion").c_str(), "jet angle in V2 system after cuts; |cos #theta*_{jet}|_{V_{2}}; Events", 20, 0, 1}, "reco.V2_jet_absCosThetaStar", "process_weight");
    // ---------------------
    
    // ----------- WW region
    auto rdf_final_state_inWWregion = rdf_bkg_inWWregion_with_cuts.Filter(FinalStateTest, {"process_name"});
    auto h1_VV_m_withcuts_final_state_inWWregion                 = rdf_final_state_inWWregion.Histo1D({("h1_VV_m_" + final_state + "_inWWregion").c_str(), "Di-boson mass after cuts; m_{VV}; Events", 50, 0, 1000}, "reco.VV_m", "process_weight");
    auto h1_absCosThetaVstar_withcuts_final_state_inWWregion     = rdf_final_state_inWWregion.Histo1D({("h1_absCosThetaVstar_withcuts_" + final_state + "_inWWregion").c_str(), "V angle in VV system after cuts; |cos #theta*_{V}|; Events", 20, 0, 1}, "reco.VV_V_absCosThetaStar", "process_weight");
    auto h1_absCosThetaJetstarV1_withcuts_final_state_inWWregion = rdf_final_state_inWWregion.Histo1D({("h1_absCosThetaJetstarV1_withcuts_" + final_state + "_inWWregion").c_str(), "jet angle in V1 system after cuts; |cos #theta*_{jet}|_{V_{1}}; Events", 20, 0, 1}, "reco.V1_jet_absCosThetaStar", "process_weight");
    auto h1_absCosThetaJetstarV2_withcuts_final_state_inWWregion = rdf_final_state_inWWregion.Histo1D({("h1_absCosThetaJetstarV2_withcuts_" + final_state + "_inWWregion").c_str(), "jet angle in V2 system after cuts; |cos #theta*_{jet}|_{V_{2}}; Events", 20, 0, 1}, "reco.V2_jet_absCosThetaStar", "process_weight");
    // ---------------------
    
    bkg_final_state_h1_VV_m_withcuts.push_back( make_pair( final_state, h1_VV_m_withcuts_final_state ) );
    bkg_final_state_h1_absCosThetaVstar_withcuts.push_back( make_pair( final_state, h1_absCosThetaVstar_withcuts_final_state ) );
    bkg_final_state_h1_absCosThetaJetstar_withcuts.push_back( make_pair( final_state, make_pair(h1_absCosThetaJetstarV1_withcuts_final_state,h1_absCosThetaJetstarV2_withcuts_final_state) ));
    
    bkg_final_state_inZZregion_h1_VV_m_withcuts.push_back( make_pair( final_state, h1_VV_m_withcuts_final_state_inZZregion ) );
    bkg_final_state_inZZregion_h1_absCosThetaVstar_withcuts.push_back( make_pair( final_state, h1_absCosThetaVstar_withcuts_final_state_inZZregion ) );
    bkg_final_state_inZZregion_h1_absCosThetaJetstar_withcuts.push_back( make_pair( final_state, make_pair(h1_absCosThetaJetstarV1_withcuts_final_state_inZZregion,h1_absCosThetaJetstarV2_withcuts_final_state_inZZregion) ));
    
    bkg_final_state_inWWregion_h1_VV_m_withcuts.push_back( make_pair( final_state, h1_VV_m_withcuts_final_state_inWWregion ) );
    bkg_final_state_inWWregion_h1_absCosThetaVstar_withcuts.push_back( make_pair( final_state, h1_absCosThetaVstar_withcuts_final_state_inWWregion ) );
    bkg_final_state_inWWregion_h1_absCosThetaJetstar_withcuts.push_back( make_pair( final_state, make_pair(h1_absCosThetaJetstarV1_withcuts_final_state_inWWregion,h1_absCosThetaJetstarV2_withcuts_final_state_inWWregion) ));
  }
  //----------------------------------------------------------------------------

  //============================================================================
  //============================================================================
  // CAREFUL!!!
  // Everything up to here should only be lazy actions! (See RDF manual)
  // From here on start using results (e.g. plotting etc.)
  //============================================================================
  //============================================================================
  
  
  //----------------------------------------------------------------------------
  // Generator level
  
  TCanvas *canvas_genlevel_VV_m = new TCanvas("genlevel_VV_m", "", 0, 0, 600, 600);
  THStack *genlevel_VV_m_stack = new THStack("genlevel_VV_m", "; m_{VV}; Events");
  TLegend *genlevel_VV_m_leg = new TLegend( 0.6, 0.6, 0.9, 0.9 ); // TODO
  h1_genlevel_VV_m_WWsignal->SetLineColor(4);
  h1_genlevel_VV_m_ZZsignal->SetLineColor(2);
  h1_genlevel_VV_m_WWsignal->SetLineStyle(1);
  h1_genlevel_VV_m_ZZsignal->SetLineStyle(1);
  genlevel_VV_m_stack->Add( h1_genlevel_VV_m_WWsignal.GetPtr());
  genlevel_VV_m_stack->Add( h1_genlevel_VV_m_ZZsignal.GetPtr());
  genlevel_VV_m_leg->SetHeader("generator level");
  genlevel_VV_m_leg->AddEntry( h1_genlevel_VV_m_WWsignal.GetPtr(), "WW", "l" );
  genlevel_VV_m_leg->AddEntry( h1_genlevel_VV_m_ZZsignal.GetPtr(), "ZZ", "l" );
  genlevel_VV_m_stack->Draw("nostack hist");
  genlevel_VV_m_stack->GetYaxis()->SetMaxDigits(3);
  genlevel_VV_m_leg->Draw();
  canvas_genlevel_VV_m->Print( ( this->getOutputDirectory() + "/genlevel_VV_m.pdf").c_str() );
  unique_ptr<TFile> genlevel_VV_m_file ( new TFile( ( this->getOutputDirectory() + "/genlevel_VV_m.root").c_str(), "RECREATE" ) );
  h1_genlevel_VV_m_WWsignal->Write();
  h1_genlevel_VV_m_ZZsignal->Write();
  genlevel_VV_m_file->Close();
  delete canvas_genlevel_VV_m;
  delete genlevel_VV_m_stack;
  delete genlevel_VV_m_leg;
  
  
  
  //----------------------------------------------------------------------------
  // Before cuts
  
  
  cout << "Getting event numbers." << endl;
  ofstream bkg_count_file;
  bkg_count_file.open((this->getOutputDirectory() + "/bkg_count.txt"));
  bkg_count_file << "Final state: Before cuts | After cuts | Efficiency\n";
  for ( auto & final_state_count: bkg_final_state_counts ) {
    bkg_count_file << final_state_count.first << " : "
          << *(final_state_count.second.first) << " | "
          << *(final_state_count.second.second) << " | "
          << *(final_state_count.second.second) / *(final_state_count.second.first) << "\n";
  }
  bkg_count_file.close();

  ofstream signal_count_file;
  signal_count_file.open((this->getOutputDirectory() + "/signal_count.txt"));
  signal_count_file << "Final state: Before cuts | After cuts | Efficiency\n";
  signal_count_file << "WW : " << *count_WWsignal_no_cuts << " | " << *count_WWsignal_with_cuts << " | "
                    << *count_WWsignal_with_cuts / *count_WWsignal_no_cuts << "\n";
  signal_count_file << "ZZ : " << *count_ZZsignal_no_cuts << " | " << *count_ZZsignal_with_cuts << " | "
                    << *count_ZZsignal_with_cuts / *count_ZZsignal_no_cuts << "\n\n";
  signal_count_file << "True ZZ events in: ZZ region: " << *count_ZZsignal_inZZregion_with_cuts << " , WW region: " << *count_ZZsignal_inWWregion_with_cuts << endl;
  signal_count_file << "True WW events in: WW region: " << *count_WWsignal_inWWregion_with_cuts << " , ZZ region: " << *count_WWsignal_inZZregion_with_cuts << endl;
  signal_count_file.close();

  
  //----------------------------------------------------------------------------
  // After cuts
  
  // TODO Unify this as function (?)
  unique_ptr<TCanvas> canvas_absCosThetaVstar_final_states (new TCanvas("absCosThetaVstar_final_states", "", 0, 0, 700, 600));
  canvas_absCosThetaVstar_final_states->SetTopMargin(0.11);
  canvas_absCosThetaVstar_final_states->SetRightMargin(0.24);
  unique_ptr<THStack> absCosThetaVstar_stack_signals (new THStack("absCosThetaVstar_signals", "after cuts; |cos #theta*_{V}|; Events"));
  unique_ptr<TLegend> absCosThetaVstar_leg_signals (new TLegend( 0.8, 0.76, 1.0, 0.9 ));
  absCosThetaVstar_leg_signals->SetHeader("Signals");
  h1_absCosThetaVstar_withcuts_WWsignal->SetLineColor(4);
  h1_absCosThetaVstar_withcuts_ZZsignal->SetLineColor(2);
  absCosThetaVstar_stack_signals->Add(h1_absCosThetaVstar_withcuts_WWsignal.GetPtr());
  absCosThetaVstar_stack_signals->Add(h1_absCosThetaVstar_withcuts_ZZsignal.GetPtr());
  absCosThetaVstar_leg_signals->AddEntry( h1_absCosThetaVstar_withcuts_WWsignal.GetPtr(), "WW", "l" );
  absCosThetaVstar_leg_signals->AddEntry( h1_absCosThetaVstar_withcuts_ZZsignal.GetPtr(), "ZZ", "l" );

  unique_ptr<TLegend> absCosThetaVstar_leg_bkgs (new TLegend( 0.8, 0.1, 1.0, 0.76 ));
  absCosThetaVstar_leg_bkgs->SetHeader("Bkgs");
  unique_ptr<THStack> absCosThetaVstar_stack_bkgs (new THStack("absCosThetaVstar_bkgs", "after cuts; |cos #theta*_{V}|}; Events"));
  gStyle->SetPalette(kNeon);
  for ( auto & final_state_absCosThetaVstar: bkg_final_state_h1_absCosThetaVstar_withcuts ) {
    absCosThetaVstar_stack_bkgs->Add(final_state_absCosThetaVstar.second.GetPtr());
    absCosThetaVstar_leg_bkgs->AddEntry( final_state_absCosThetaVstar.second.GetPtr(), final_state_absCosThetaVstar.first.c_str(), "l" );
  }
  
  absCosThetaVstar_stack_signals->Draw("hist nostack");
  absCosThetaVstar_stack_signals->GetYaxis()->SetMaxDigits(3);
  absCosThetaVstar_stack_bkgs->Draw("hist pfc plc same");
  absCosThetaVstar_stack_signals->Draw("hist nostack same");
  absCosThetaVstar_leg_bkgs->Draw();
  absCosThetaVstar_leg_signals->Draw();
  gPad->Modified();
  gPad->Update();
  canvas_absCosThetaVstar_final_states->Print( ( this->getOutputDirectory() + "/absCosThetaVstar_stackedbkg.pdf").c_str() );
  
  
  unique_ptr<TCanvas> canvas_VV_m_final_states (new TCanvas("VV_m_final_states", "", 0, 0, 700, 600));
  canvas_VV_m_final_states->SetTopMargin(0.11);
  canvas_VV_m_final_states->SetRightMargin(0.24);
  unique_ptr<THStack> VV_m_stack_signals (new THStack("VV_m_signals", "after cuts; m_{VV}; Events"));
  unique_ptr<TLegend> VV_m_leg_signals (new TLegend( 0.8, 0.76, 1.0, 0.9 ));
  VV_m_leg_signals->SetHeader("Signals");
  h1_VV_m_withcuts_WWsignal->SetLineColor(4);
  h1_VV_m_withcuts_ZZsignal->SetLineColor(2);
  VV_m_stack_signals->Add(h1_VV_m_withcuts_WWsignal.GetPtr());
  VV_m_stack_signals->Add(h1_VV_m_withcuts_ZZsignal.GetPtr());
  VV_m_leg_signals->AddEntry( h1_VV_m_withcuts_WWsignal.GetPtr(), "WW", "l" );
  VV_m_leg_signals->AddEntry( h1_VV_m_withcuts_ZZsignal.GetPtr(), "ZZ", "l" );

  unique_ptr<TLegend> VV_m_leg_bkgs (new TLegend( 0.8, 0.1, 1.0, 0.76 ));
  VV_m_leg_bkgs->SetHeader("Bkgs");
  unique_ptr<THStack> VV_m_stack_bkgs (new THStack("VV_m_bkgs", "after cuts; m_{VV}; Events"));
  gStyle->SetPalette(kNeon);
  for ( auto & final_state_VV_m: bkg_final_state_h1_VV_m_withcuts ) {
    VV_m_stack_bkgs->Add(final_state_VV_m.second.GetPtr());
    VV_m_leg_bkgs->AddEntry( final_state_VV_m.second.GetPtr(), final_state_VV_m.first.c_str(), "l" );
  }
  
  VV_m_stack_signals->Draw("hist nostack");
  VV_m_stack_signals->GetYaxis()->SetMaxDigits(3);
  VV_m_stack_bkgs->Draw("hist pfc plc same");
  VV_m_stack_signals->Draw("hist nostack same");
  VV_m_leg_bkgs->Draw();
  VV_m_leg_signals->Draw();
  gPad->Modified();
  gPad->Update();
  canvas_VV_m_final_states->Print( ( this->getOutputDirectory() + "/VV_m_stackedbkg.pdf").c_str() );
  
  
  
  unique_ptr<TCanvas> canvas_h1_absCosThetaJetstar_combined_final_states (new TCanvas("h1_absCosThetaJetstar_combined_final_states", "", 0, 0, 700, 600));
  canvas_h1_absCosThetaJetstar_combined_final_states->SetTopMargin(0.11);
  canvas_h1_absCosThetaJetstar_combined_final_states->SetRightMargin(0.24);
  
  unique_ptr<THStack> h1_absCosThetaJetstar_combined_stack_signals (new THStack("h1_absCosThetaJetstar_combined_signals", "after cuts; |cos #theta*_{jet}|; 2*Events"));
  auto h1_absCosThetaJetstar_combined_withcuts_WWsignal = new TH1D("h1_absCosThetaJetstar_combined_withcuts_WWsignal", "jet angle in V system after cuts, both V; |cos #theta*_{jet}|; 2*Events", 20, 0, 1 );
  auto h1_absCosThetaJetstar_combined_withcuts_ZZsignal = new TH1D("h1_absCosThetaJetstar_combined_withcuts_ZZsignal", "jet angle in V system after cuts, both V; |cos #theta*_{jet}|; 2*Events", 20, 0, 1 );
  h1_absCosThetaJetstar_combined_withcuts_WWsignal->Add( h1_absCosThetaJetstarV1_withcuts_WWsignal.GetPtr(), h1_absCosThetaJetstarV2_withcuts_WWsignal.GetPtr() );
  h1_absCosThetaJetstar_combined_withcuts_ZZsignal->Add( h1_absCosThetaJetstarV1_withcuts_ZZsignal.GetPtr(), h1_absCosThetaJetstarV2_withcuts_ZZsignal.GetPtr() );
  unique_ptr<TLegend> h1_absCosThetaJetstar_combined_leg_signals (new TLegend( 0.8, 0.76, 1.0, 0.9 ));
  h1_absCosThetaJetstar_combined_leg_signals->SetHeader("Signals");
  h1_absCosThetaJetstar_combined_withcuts_WWsignal->SetLineColor(4);
  h1_absCosThetaJetstar_combined_withcuts_ZZsignal->SetLineColor(2);
  h1_absCosThetaJetstar_combined_stack_signals->Add(h1_absCosThetaJetstar_combined_withcuts_WWsignal);
  h1_absCosThetaJetstar_combined_stack_signals->Add(h1_absCosThetaJetstar_combined_withcuts_ZZsignal);
  h1_absCosThetaJetstar_combined_leg_signals->AddEntry( h1_absCosThetaJetstar_combined_withcuts_WWsignal, "WW", "l" );
  h1_absCosThetaJetstar_combined_leg_signals->AddEntry( h1_absCosThetaJetstar_combined_withcuts_ZZsignal, "ZZ", "l" );

  unique_ptr<TLegend> h1_absCosThetaJetstar_combined_leg_bkgs (new TLegend( 0.8, 0.1, 1.0, 0.76 ));
  h1_absCosThetaJetstar_combined_leg_bkgs->SetHeader("Bkgs");
  unique_ptr<THStack> h1_absCosThetaJetstar_combined_stack_bkgs (new THStack("h1_absCosThetaJetstar_combined_bkgs", "after cuts; |cos #theta*_{jet}|; Events"));
  gStyle->SetPalette(kNeon);
  vector<TH1D*> to_delete{};
  for ( auto & final_state_absCosThetaJetstar_combined: bkg_final_state_h1_absCosThetaJetstar_withcuts ) {
    auto h1_final_state_absCosThetaJetstar_combined = new TH1D("h1_absCosThetaJetstar_combined_withcuts_bkg", "jet angle in V system after cuts, both V; |cos #theta*_{jet}|; 2*Events", 20, 0, 1 );
    h1_final_state_absCosThetaJetstar_combined->Add( final_state_absCosThetaJetstar_combined.second.first.GetPtr(), final_state_absCosThetaJetstar_combined.second.second.GetPtr() );
    h1_absCosThetaJetstar_combined_stack_bkgs->Add(h1_final_state_absCosThetaJetstar_combined);
    h1_absCosThetaJetstar_combined_leg_bkgs->AddEntry( h1_final_state_absCosThetaJetstar_combined, final_state_absCosThetaJetstar_combined.first.c_str(), "l" );
    to_delete.push_back(h1_final_state_absCosThetaJetstar_combined);
  }
  
  h1_absCosThetaJetstar_combined_stack_signals->Draw("hist nostack");
  h1_absCosThetaJetstar_combined_stack_signals->GetYaxis()->SetMaxDigits(3);
  h1_absCosThetaJetstar_combined_stack_bkgs->Draw("hist pfc plc same");
  h1_absCosThetaJetstar_combined_stack_signals->Draw("hist nostack same");
  h1_absCosThetaJetstar_combined_leg_bkgs->Draw();
  h1_absCosThetaJetstar_combined_leg_signals->Draw();
  gPad->Modified();
  gPad->Update();
  canvas_h1_absCosThetaJetstar_combined_final_states->Print( ( this->getOutputDirectory() + "/absCosThetaJetstar_combined_stackedbkg.pdf").c_str() );
  delete h1_absCosThetaJetstar_combined_withcuts_WWsignal;
  delete h1_absCosThetaJetstar_combined_withcuts_ZZsignal;
  for (auto & deletable: to_delete) { delete deletable; }
  
  
  unique_ptr<TCanvas> canvas_VmMean (new TCanvas("VmMean", "", 0, 0, 700, 600));
  canvas_VmMean->SetTopMargin(0.11);
  canvas_VmMean->SetRightMargin(0.24);
  unique_ptr<THStack> VmMean_stack (new THStack("VmMean", "; m_{jj,1}+m_{jj,2}/2 [GeV]; Events"));
  unique_ptr<TLegend> VmMean_leg (new TLegend( 0.8, 0.76, 1.0, 0.9 ));
  VmMean_leg->SetHeader("w/ cuts");
  h1_VmMean_withcuts_WWsignal->SetLineColor(4);
  h1_VmMean_withcuts_ZZsignal->SetLineColor(2);
  VmMean_stack->Add(h1_VmMean_withcuts_WWsignal.GetPtr());
  VmMean_stack->Add(h1_VmMean_withcuts_ZZsignal.GetPtr());
  VmMean_stack->Add(h1_VmMean_withcuts_bkg.GetPtr());
  VmMean_leg->AddEntry( h1_VmMean_withcuts_WWsignal.GetPtr(), "WW", "l" );
  VmMean_leg->AddEntry( h1_VmMean_withcuts_ZZsignal.GetPtr(), "ZZ", "l" );
  VmMean_leg->AddEntry( h1_VmMean_withcuts_bkg.GetPtr(), "bkg", "l" );
  VmMean_stack->Draw("hist nostack");
  VmMean_stack->GetYaxis()->SetMaxDigits(3);
  VmMean_leg->Draw();
  gPad->Modified();
  gPad->Update();
  canvas_VmMean->Print( ( this->getOutputDirectory() + "/VmMean.pdf").c_str() );


  unique_ptr<TCanvas> canvas_Vm2D (new TCanvas("Vm2D", "", 0, 0, 700, 600));
  canvas_Vm2D->SetRightMargin(0.24);
  unique_ptr<TLegend> Vm2D_leg (new TLegend( 0.8, 0.1, 1.0, 0.9 ));
  h2_Vm_withcuts_WWsignal->SetLineColor(4);
  h2_Vm_withcuts_ZZsignal->SetLineColor(2);
  Vm2D_leg->AddEntry( h2_Vm_withcuts_WWsignal.GetPtr(), "WW", "l" );
  Vm2D_leg->AddEntry( h2_Vm_withcuts_ZZsignal.GetPtr(), "ZZ", "l" );
  h2_Vm_withcuts_WWsignal->Draw("box same");
  h2_Vm_withcuts_ZZsignal->Draw("box same");
  h2_Vm_withcuts_WWsignal->GetYaxis()->SetMaxDigits(3);
  Vm2D_leg->Draw();
  gPad->Modified();
  gPad->Update();
  canvas_Vm2D->Print( ( this->getOutputDirectory() + "/Vm2D.pdf").c_str() );




  
  
  
  
  
  
  
  
  
  
  unique_ptr<TCanvas> canvas_absCosThetaVstar_final_states_inZZregion (new TCanvas("absCosThetaVstar_final_states_inZZregion", "", 0, 0, 700, 600));
  canvas_absCosThetaVstar_final_states_inZZregion->SetTopMargin(0.11);
  canvas_absCosThetaVstar_final_states_inZZregion->SetRightMargin(0.24);
  unique_ptr<THStack> absCosThetaVstar_stack_inZZregion (new THStack("absCosThetaVstar_inZZregion", "after cuts, ZZ region; |cos #theta*_{V}|; Events"));
  unique_ptr<THStack> absCosThetaVstar_stack_wSignal_inZZregion (new THStack("absCosThetaVstarsignals_wSignal_inZZregion", "after cuts, ZZ region; |cos #theta*_{V}|; Events"));
  unique_ptr<TLegend> absCosThetaVstar_leg_signals_inZZregion (new TLegend( 0.8, 0.76, 1.0, 0.9 ));
  unique_ptr<TLegend> absCosThetaVstar_leg_bkgs_inZZregion (new TLegend( 0.8, 0.1, 1.0, 0.76 ));
  absCosThetaVstar_leg_signals_inZZregion->SetHeader("Signals");
  absCosThetaVstar_leg_bkgs_inZZregion->SetHeader("Bkgs");
  gStyle->SetPalette(kNeon);
  for ( auto & final_state_absCosThetaVstar: bkg_final_state_inZZregion_h1_absCosThetaVstar_withcuts ) {
    absCosThetaVstar_stack_inZZregion->Add(final_state_absCosThetaVstar.second.GetPtr());
    absCosThetaVstar_stack_wSignal_inZZregion->Add(final_state_absCosThetaVstar.second.GetPtr());
    absCosThetaVstar_leg_bkgs_inZZregion->AddEntry( final_state_absCosThetaVstar.second.GetPtr(), final_state_absCosThetaVstar.first.c_str(), "l" );
  }
  h1_absCosThetaVstar_withcuts_WWsignal_inZZregion->SetLineColor(4);
  h1_absCosThetaVstar_withcuts_WWsignal_inZZregion->SetFillColor(4);
  h1_absCosThetaVstar_withcuts_ZZsignal_inZZregion->SetLineColor(2);
  h1_absCosThetaVstar_withcuts_ZZsignal_inZZregion->SetFillColor(2);
  absCosThetaVstar_stack_wSignal_inZZregion->Add(h1_absCosThetaVstar_withcuts_WWsignal_inZZregion.GetPtr());
  absCosThetaVstar_stack_wSignal_inZZregion->Add(h1_absCosThetaVstar_withcuts_ZZsignal_inZZregion.GetPtr());
  absCosThetaVstar_leg_signals_inZZregion->AddEntry( h1_absCosThetaVstar_withcuts_WWsignal_inZZregion.GetPtr(), "WW", "l" );
  absCosThetaVstar_leg_signals_inZZregion->AddEntry( h1_absCosThetaVstar_withcuts_ZZsignal_inZZregion.GetPtr(), "ZZ", "l" );
  absCosThetaVstar_stack_wSignal_inZZregion->Draw("hist");
  absCosThetaVstar_stack_inZZregion->Draw("hist pfc plc same");
  absCosThetaVstar_stack_inZZregion->GetYaxis()->SetMaxDigits(3);
  absCosThetaVstar_leg_bkgs_inZZregion->Draw();
  absCosThetaVstar_leg_signals_inZZregion->Draw();
  gPad->Modified();
  gPad->Update();
  canvas_absCosThetaVstar_final_states_inZZregion->Print( ( this->getOutputDirectory() + "/absCosThetaVstar_ZZregion_stackedbkg.pdf").c_str() );
  
  
  unique_ptr<TCanvas> canvas_VV_m_final_states_inZZregion (new TCanvas("VV_m_final_states_inZZregion", "", 0, 0, 700, 600));
  canvas_VV_m_final_states_inZZregion->SetTopMargin(0.11);
  canvas_VV_m_final_states_inZZregion->SetRightMargin(0.24);
  unique_ptr<THStack> VV_m_stack_inZZregion (new THStack("VV_m_signals_inZZregion", "after cuts, ZZ region; m_{VV}; Events"));
  unique_ptr<THStack> VV_m_stack_wSignal_inZZregion (new THStack("VV_m_signals_wSignal_inZZregion", "after cuts, ZZ region; m_{VV}; Events"));
  unique_ptr<TLegend> VV_m_leg_signals_inZZregion (new TLegend( 0.8, 0.76, 1.0, 0.9 ));
  unique_ptr<TLegend> VV_m_leg_bkgs_inZZregion (new TLegend( 0.8, 0.1, 1.0, 0.76 ));
  VV_m_leg_signals_inZZregion->SetHeader("Signals");
  VV_m_leg_bkgs_inZZregion->SetHeader("Bkgs");
  gStyle->SetPalette(kNeon);
  for ( auto & final_state_VV_m: bkg_final_state_inZZregion_h1_VV_m_withcuts ) {
    VV_m_stack_inZZregion->Add(final_state_VV_m.second.GetPtr());
    VV_m_stack_wSignal_inZZregion->Add(final_state_VV_m.second.GetPtr());
    VV_m_leg_bkgs_inZZregion->AddEntry( final_state_VV_m.second.GetPtr(), final_state_VV_m.first.c_str(), "l" );
  }
  h1_VV_m_withcuts_WWsignal_inZZregion->SetLineColor(4);
  h1_VV_m_withcuts_WWsignal_inZZregion->SetFillColor(4);
  h1_VV_m_withcuts_ZZsignal_inZZregion->SetLineColor(2);
  h1_VV_m_withcuts_ZZsignal_inZZregion->SetFillColor(2);
  VV_m_stack_wSignal_inZZregion->Add(h1_VV_m_withcuts_WWsignal_inZZregion.GetPtr());
  VV_m_stack_wSignal_inZZregion->Add(h1_VV_m_withcuts_ZZsignal_inZZregion.GetPtr());
  VV_m_leg_signals_inZZregion->AddEntry( h1_VV_m_withcuts_WWsignal_inZZregion.GetPtr(), "WW", "l" );
  VV_m_leg_signals_inZZregion->AddEntry( h1_VV_m_withcuts_ZZsignal_inZZregion.GetPtr(), "ZZ", "l" );
  VV_m_stack_wSignal_inZZregion->Draw("hist"); 
  VV_m_stack_inZZregion->Draw("hist pfc plc same");
  VV_m_stack_inZZregion->GetYaxis()->SetMaxDigits(3);
  VV_m_leg_bkgs_inZZregion->Draw();
  VV_m_leg_signals_inZZregion->Draw();
  gPad->Modified();
  gPad->Update();
  canvas_VV_m_final_states_inZZregion->Print( ( this->getOutputDirectory() + "/VV_m_ZZregion_stackedbkg.pdf").c_str() );
  
  
  
  unique_ptr<TCanvas> canvas_h1_absCosThetaJetstar_combined_final_states_inZZregion (new TCanvas("h1_absCosThetaJetstar_combined_final_states_inZZregion", "", 0, 0, 700, 600));
  canvas_h1_absCosThetaJetstar_combined_final_states_inZZregion->SetTopMargin(0.11);
  canvas_h1_absCosThetaJetstar_combined_final_states_inZZregion->SetRightMargin(0.24);
  unique_ptr<THStack> h1_absCosThetaJetstar_combined_stack_inZZregion (new THStack("h1_absCosThetaJetstar_combined_signals_inZZregion", "after cuts; |cos #theta*_{jet}|; 2*Events"));
  unique_ptr<THStack> h1_absCosThetaJetstar_combined_stack_wSignal_inZZregion (new THStack("h1_absCosThetaJetstar_combined_wSignal_inZZregion", "after cuts; |cos #theta*_{jet}|; 2*Events"));
  auto h1_absCosThetaJetstar_combined_withcuts_WWsignal_inZZregion = new TH1D("h1_absCosThetaJetstar_combined_withcuts_WWsignal_inZZregion", "jet angle in V system after cuts, both V; |cos #theta*_{jet}|; 2*Events", 20, 0, 1 );
  auto h1_absCosThetaJetstar_combined_withcuts_ZZsignal_inZZregion = new TH1D("h1_absCosThetaJetstar_combined_withcuts_ZZsignal_inZZregion", "jet angle in V system after cuts, both V; |cos #theta*_{jet}|; 2*Events", 20, 0, 1 );
  h1_absCosThetaJetstar_combined_withcuts_WWsignal_inZZregion->Add( h1_absCosThetaJetstarV1_withcuts_WWsignal_inZZregion.GetPtr(), h1_absCosThetaJetstarV2_withcuts_WWsignal_inZZregion.GetPtr() );
  h1_absCosThetaJetstar_combined_withcuts_ZZsignal_inZZregion->Add( h1_absCosThetaJetstarV1_withcuts_ZZsignal_inZZregion.GetPtr(), h1_absCosThetaJetstarV2_withcuts_ZZsignal_inZZregion.GetPtr() );
  unique_ptr<TLegend> h1_absCosThetaJetstar_combined_leg_signals_inZZregion (new TLegend( 0.8, 0.76, 1.0, 0.9 ));
  unique_ptr<TLegend> h1_absCosThetaJetstar_combined_leg_bkgs_inZZregion (new TLegend( 0.8, 0.1, 1.0, 0.76 ));
  h1_absCosThetaJetstar_combined_leg_signals_inZZregion->SetHeader("Signals");
  h1_absCosThetaJetstar_combined_leg_bkgs_inZZregion->SetHeader("Bkgs");
  gStyle->SetPalette(kNeon);
  vector<TH1D*> to_delete_inZZregion{};
  for ( auto & final_state_absCosThetaJetstar_combined: bkg_final_state_inZZregion_h1_absCosThetaJetstar_withcuts ) {
    auto h1_final_state_absCosThetaJetstar_combined = new TH1D("h1_absCosThetaJetstar_combined_withcuts_bkg", "jet angle in V system after cuts, both V; |cos #theta*_{jet}|; 2*Events", 20, 0, 1 );
    h1_final_state_absCosThetaJetstar_combined->Add( final_state_absCosThetaJetstar_combined.second.first.GetPtr(), final_state_absCosThetaJetstar_combined.second.second.GetPtr() );
    h1_absCosThetaJetstar_combined_stack_inZZregion->Add(h1_final_state_absCosThetaJetstar_combined);
    h1_absCosThetaJetstar_combined_stack_wSignal_inZZregion->Add(h1_final_state_absCosThetaJetstar_combined);
    h1_absCosThetaJetstar_combined_leg_bkgs_inZZregion->AddEntry( h1_final_state_absCosThetaJetstar_combined, final_state_absCosThetaJetstar_combined.first.c_str(), "l" );
    to_delete_inZZregion.push_back(h1_final_state_absCosThetaJetstar_combined);
  }
  h1_absCosThetaJetstar_combined_withcuts_WWsignal_inZZregion->SetLineColor(4);
  h1_absCosThetaJetstar_combined_withcuts_WWsignal_inZZregion->SetFillColor(4);
  h1_absCosThetaJetstar_combined_withcuts_ZZsignal_inZZregion->SetLineColor(2);
  h1_absCosThetaJetstar_combined_withcuts_ZZsignal_inZZregion->SetFillColor(2);
  h1_absCosThetaJetstar_combined_stack_wSignal_inZZregion->Add(h1_absCosThetaJetstar_combined_withcuts_WWsignal_inZZregion);
  h1_absCosThetaJetstar_combined_stack_wSignal_inZZregion->Add(h1_absCosThetaJetstar_combined_withcuts_ZZsignal_inZZregion);
  h1_absCosThetaJetstar_combined_leg_signals_inZZregion->AddEntry( h1_absCosThetaJetstar_combined_withcuts_WWsignal_inZZregion, "WW", "l" );
  h1_absCosThetaJetstar_combined_leg_signals_inZZregion->AddEntry( h1_absCosThetaJetstar_combined_withcuts_ZZsignal_inZZregion, "ZZ", "l" );
  
  h1_absCosThetaJetstar_combined_stack_wSignal_inZZregion->Draw("hist");
  h1_absCosThetaJetstar_combined_stack_inZZregion->Draw("hist pfc plc same");
  h1_absCosThetaJetstar_combined_stack_inZZregion->GetYaxis()->SetMaxDigits(3);
  h1_absCosThetaJetstar_combined_leg_bkgs_inZZregion->Draw();
  h1_absCosThetaJetstar_combined_leg_signals_inZZregion->Draw();
  gPad->Modified();
  gPad->Update();
  canvas_h1_absCosThetaJetstar_combined_final_states_inZZregion->Print( ( this->getOutputDirectory() + "/absCosThetaJetstar_combined_ZZregion_stackedbkg.pdf").c_str() );
  delete h1_absCosThetaJetstar_combined_withcuts_WWsignal_inZZregion;
  delete h1_absCosThetaJetstar_combined_withcuts_ZZsignal_inZZregion;
  for (auto & deletable: to_delete_inZZregion) { delete deletable; }
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  unique_ptr<TCanvas> canvas_absCosThetaVstar_final_states_inWWregion (new TCanvas("absCosThetaVstar_final_states_inWWregion", "", 0, 0, 700, 600));
  canvas_absCosThetaVstar_final_states_inWWregion->SetTopMargin(0.11);
  canvas_absCosThetaVstar_final_states_inWWregion->SetRightMargin(0.24);
  unique_ptr<THStack> absCosThetaVstar_stack_inWWregion (new THStack("absCosThetaVstar_inWWregion", "after cuts, ZZ region; |cos #theta*_{V}|; Events"));
  unique_ptr<THStack> absCosThetaVstar_stack_wSignal_inWWregion (new THStack("absCosThetaVstarsignals_wSignal_inWWregion", "after cuts, ZZ region; |cos #theta*_{V}|; Events"));
  unique_ptr<TLegend> absCosThetaVstar_leg_signals_inWWregion (new TLegend( 0.8, 0.76, 1.0, 0.9 ));
  unique_ptr<TLegend> absCosThetaVstar_leg_bkgs_inWWregion (new TLegend( 0.8, 0.1, 1.0, 0.76 ));
  absCosThetaVstar_leg_signals_inWWregion->SetHeader("Signals");
  absCosThetaVstar_leg_bkgs_inWWregion->SetHeader("Bkgs");
  gStyle->SetPalette(kNeon);
  for ( auto & final_state_absCosThetaVstar: bkg_final_state_inWWregion_h1_absCosThetaVstar_withcuts ) {
    absCosThetaVstar_stack_inWWregion->Add(final_state_absCosThetaVstar.second.GetPtr());
    absCosThetaVstar_stack_wSignal_inWWregion->Add(final_state_absCosThetaVstar.second.GetPtr());
    absCosThetaVstar_leg_bkgs_inWWregion->AddEntry( final_state_absCosThetaVstar.second.GetPtr(), final_state_absCosThetaVstar.first.c_str(), "l" );
  }
  h1_absCosThetaVstar_withcuts_WWsignal_inWWregion->SetLineColor(4);
  h1_absCosThetaVstar_withcuts_WWsignal_inWWregion->SetFillColor(4);
  h1_absCosThetaVstar_withcuts_ZZsignal_inWWregion->SetLineColor(2);
  h1_absCosThetaVstar_withcuts_ZZsignal_inWWregion->SetFillColor(2);
  absCosThetaVstar_stack_wSignal_inWWregion->Add(h1_absCosThetaVstar_withcuts_ZZsignal_inWWregion.GetPtr());
  absCosThetaVstar_stack_wSignal_inWWregion->Add(h1_absCosThetaVstar_withcuts_WWsignal_inWWregion.GetPtr());
  absCosThetaVstar_leg_signals_inWWregion->AddEntry( h1_absCosThetaVstar_withcuts_WWsignal_inWWregion.GetPtr(), "WW", "l" );
  absCosThetaVstar_leg_signals_inWWregion->AddEntry( h1_absCosThetaVstar_withcuts_ZZsignal_inWWregion.GetPtr(), "ZZ", "l" );
  absCosThetaVstar_stack_wSignal_inWWregion->Draw("hist");
  absCosThetaVstar_stack_inWWregion->Draw("hist pfc plc same");
  absCosThetaVstar_stack_inWWregion->GetYaxis()->SetMaxDigits(3);
  absCosThetaVstar_leg_bkgs_inWWregion->Draw();
  absCosThetaVstar_leg_signals_inWWregion->Draw();
  gPad->Modified();
  gPad->Update();
  canvas_absCosThetaVstar_final_states_inWWregion->Print( ( this->getOutputDirectory() + "/absCosThetaVstar_WWregion_stackedbkg.pdf").c_str() );
  
  
  unique_ptr<TCanvas> canvas_VV_m_final_states_inWWregion (new TCanvas("VV_m_final_states_inWWregion", "", 0, 0, 700, 600));
  canvas_VV_m_final_states_inWWregion->SetTopMargin(0.11);
  canvas_VV_m_final_states_inWWregion->SetRightMargin(0.24);
  unique_ptr<THStack> VV_m_stack_inWWregion (new THStack("VV_m_signals_inWWregion", "after cuts, ZZ region; m_{VV}; Events"));
  unique_ptr<THStack> VV_m_stack_wSignal_inWWregion (new THStack("VV_m_signals_wSignal_inWWregion", "after cuts, ZZ region; m_{VV}; Events"));
  unique_ptr<TLegend> VV_m_leg_signals_inWWregion (new TLegend( 0.8, 0.76, 1.0, 0.9 ));
  unique_ptr<TLegend> VV_m_leg_bkgs_inWWregion (new TLegend( 0.8, 0.1, 1.0, 0.76 ));
  VV_m_leg_signals_inWWregion->SetHeader("Signals");
  VV_m_leg_bkgs_inWWregion->SetHeader("Bkgs");
  gStyle->SetPalette(kNeon);
  for ( auto & final_state_VV_m: bkg_final_state_inWWregion_h1_VV_m_withcuts ) {
    VV_m_stack_inWWregion->Add(final_state_VV_m.second.GetPtr());
    VV_m_stack_wSignal_inWWregion->Add(final_state_VV_m.second.GetPtr());
    VV_m_leg_bkgs_inWWregion->AddEntry( final_state_VV_m.second.GetPtr(), final_state_VV_m.first.c_str(), "l" );
  }
  h1_VV_m_withcuts_WWsignal_inWWregion->SetLineColor(4);
  h1_VV_m_withcuts_WWsignal_inWWregion->SetFillColor(4);
  h1_VV_m_withcuts_ZZsignal_inWWregion->SetLineColor(2);
  h1_VV_m_withcuts_ZZsignal_inWWregion->SetFillColor(2);
  VV_m_stack_wSignal_inWWregion->Add(h1_VV_m_withcuts_ZZsignal_inWWregion.GetPtr());
  VV_m_stack_wSignal_inWWregion->Add(h1_VV_m_withcuts_WWsignal_inWWregion.GetPtr());
  VV_m_leg_signals_inWWregion->AddEntry( h1_VV_m_withcuts_WWsignal_inWWregion.GetPtr(), "WW", "l" );
  VV_m_leg_signals_inWWregion->AddEntry( h1_VV_m_withcuts_ZZsignal_inWWregion.GetPtr(), "ZZ", "l" );
  VV_m_stack_wSignal_inWWregion->Draw("hist"); 
  VV_m_stack_inWWregion->Draw("hist pfc plc same");
  VV_m_stack_inWWregion->GetYaxis()->SetMaxDigits(3);
  VV_m_leg_bkgs_inWWregion->Draw();
  VV_m_leg_signals_inWWregion->Draw();
  gPad->Modified();
  gPad->Update();
  canvas_VV_m_final_states_inWWregion->Print( ( this->getOutputDirectory() + "/VV_m_WWregion_stackedbkg.pdf").c_str() );
  
  
  
  unique_ptr<TCanvas> canvas_h1_absCosThetaJetstar_combined_final_states_inWWregion (new TCanvas("h1_absCosThetaJetstar_combined_final_states_inWWregion", "", 0, 0, 700, 600));
  canvas_h1_absCosThetaJetstar_combined_final_states_inWWregion->SetTopMargin(0.11);
  canvas_h1_absCosThetaJetstar_combined_final_states_inWWregion->SetRightMargin(0.24);
  unique_ptr<THStack> h1_absCosThetaJetstar_combined_stack_inWWregion (new THStack("h1_absCosThetaJetstar_combined_signals_inWWregion", "after cuts; |cos #theta*_{jet}|; 2*Events"));
  unique_ptr<THStack> h1_absCosThetaJetstar_combined_stack_wSignal_inWWregion (new THStack("h1_absCosThetaJetstar_combined_wSignal_inWWregion", "after cuts; |cos #theta*_{jet}|; 2*Events"));
  auto h1_absCosThetaJetstar_combined_withcuts_WWsignal_inWWregion = new TH1D("h1_absCosThetaJetstar_combined_withcuts_WWsignal_inWWregion", "jet angle in V system after cuts, both V; |cos #theta*_{jet}|; 2*Events", 20, 0, 1 );
  auto h1_absCosThetaJetstar_combined_withcuts_ZZsignal_inWWregion = new TH1D("h1_absCosThetaJetstar_combined_withcuts_ZZsignal_inWWregion", "jet angle in V system after cuts, both V; |cos #theta*_{jet}|; 2*Events", 20, 0, 1 );
  h1_absCosThetaJetstar_combined_withcuts_WWsignal_inWWregion->Add( h1_absCosThetaJetstarV1_withcuts_WWsignal_inWWregion.GetPtr(), h1_absCosThetaJetstarV2_withcuts_WWsignal_inWWregion.GetPtr() );
  h1_absCosThetaJetstar_combined_withcuts_ZZsignal_inWWregion->Add( h1_absCosThetaJetstarV1_withcuts_ZZsignal_inWWregion.GetPtr(), h1_absCosThetaJetstarV2_withcuts_ZZsignal_inWWregion.GetPtr() );
  unique_ptr<TLegend> h1_absCosThetaJetstar_combined_leg_signals_inWWregion (new TLegend( 0.8, 0.76, 1.0, 0.9 ));
  unique_ptr<TLegend> h1_absCosThetaJetstar_combined_leg_bkgs_inWWregion (new TLegend( 0.8, 0.1, 1.0, 0.76 ));
  h1_absCosThetaJetstar_combined_leg_signals_inWWregion->SetHeader("Signals");
  h1_absCosThetaJetstar_combined_leg_bkgs_inWWregion->SetHeader("Bkgs");
  gStyle->SetPalette(kNeon);
  vector<TH1D*> to_delete_inWWregion{};
  for ( auto & final_state_absCosThetaJetstar_combined: bkg_final_state_inWWregion_h1_absCosThetaJetstar_withcuts ) {
    auto h1_final_state_absCosThetaJetstar_combined = new TH1D("h1_absCosThetaJetstar_combined_withcuts_bkg", "jet angle in V system after cuts, both V; |cos #theta*_{jet}|; 2*Events", 20, 0, 1 );
    h1_final_state_absCosThetaJetstar_combined->Add( final_state_absCosThetaJetstar_combined.second.first.GetPtr(), final_state_absCosThetaJetstar_combined.second.second.GetPtr() );
    h1_absCosThetaJetstar_combined_stack_inWWregion->Add(h1_final_state_absCosThetaJetstar_combined);
    h1_absCosThetaJetstar_combined_stack_wSignal_inWWregion->Add(h1_final_state_absCosThetaJetstar_combined);
    h1_absCosThetaJetstar_combined_leg_bkgs_inWWregion->AddEntry( h1_final_state_absCosThetaJetstar_combined, final_state_absCosThetaJetstar_combined.first.c_str(), "l" );
    to_delete_inWWregion.push_back(h1_final_state_absCosThetaJetstar_combined);
  }
  h1_absCosThetaJetstar_combined_withcuts_WWsignal_inWWregion->SetLineColor(4);
  h1_absCosThetaJetstar_combined_withcuts_WWsignal_inWWregion->SetFillColor(4);
  h1_absCosThetaJetstar_combined_withcuts_ZZsignal_inWWregion->SetLineColor(2);
  h1_absCosThetaJetstar_combined_withcuts_ZZsignal_inWWregion->SetFillColor(2);
  h1_absCosThetaJetstar_combined_stack_wSignal_inWWregion->Add(h1_absCosThetaJetstar_combined_withcuts_ZZsignal_inWWregion);
  h1_absCosThetaJetstar_combined_stack_wSignal_inWWregion->Add(h1_absCosThetaJetstar_combined_withcuts_WWsignal_inWWregion);
  h1_absCosThetaJetstar_combined_leg_signals_inWWregion->AddEntry( h1_absCosThetaJetstar_combined_withcuts_WWsignal_inWWregion, "WW", "l" );
  h1_absCosThetaJetstar_combined_leg_signals_inWWregion->AddEntry( h1_absCosThetaJetstar_combined_withcuts_ZZsignal_inWWregion, "ZZ", "l" );
  
  h1_absCosThetaJetstar_combined_stack_wSignal_inWWregion->Draw("hist");
  h1_absCosThetaJetstar_combined_stack_inWWregion->Draw("hist pfc plc same");
  h1_absCosThetaJetstar_combined_stack_inWWregion->GetYaxis()->SetMaxDigits(3);
  h1_absCosThetaJetstar_combined_leg_bkgs_inWWregion->Draw();
  h1_absCosThetaJetstar_combined_leg_signals_inWWregion->Draw();
  gPad->Modified();
  gPad->Update();
  canvas_h1_absCosThetaJetstar_combined_final_states_inWWregion->Print( ( this->getOutputDirectory() + "/absCosThetaJetstar_combined_WWregion_stackedbkg.pdf").c_str() );
  delete h1_absCosThetaJetstar_combined_withcuts_WWsignal_inWWregion;
  delete h1_absCosThetaJetstar_combined_withcuts_ZZsignal_inWWregion;
  for (auto & deletable: to_delete_inWWregion) { delete deletable; }
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  //----------------------------------------------------------------------------
  // Level comparisons
  TCanvas *canvas_cutcomparison_VV_m = new TCanvas("cutcomparison_VV_m", "", 0, 0, 600, 600);
  h1_VV_m_nocuts->Draw("hist");
  h1_VV_m_nocuts_WWsignal->SetLineStyle(2);
  h1_VV_m_nocuts_ZZsignal->SetLineStyle(3);
  h1_VV_m_nocuts_WWsignal->Draw("hist same");
  h1_VV_m_nocuts_ZZsignal->Draw("hist same");
  h1_VV_m_withcuts->SetLineColor(2);
  h1_VV_m_withcuts_WWsignal->SetLineColor(2);
  h1_VV_m_withcuts_ZZsignal->SetLineColor(2);
  h1_VV_m_withcuts_WWsignal->SetLineStyle(2);
  h1_VV_m_withcuts_ZZsignal->SetLineStyle(3);
  h1_VV_m_withcuts->Draw("hist same");
  h1_VV_m_withcuts_WWsignal->Draw("hist same");
  h1_VV_m_withcuts_ZZsignal->Draw("hist same");
  canvas_cutcomparison_VV_m->Print( ( this->getOutputDirectory() + "/cutcomparison_VV_m.pdf").c_str() );
  delete canvas_cutcomparison_VV_m;

}

//-------------------------------------------------------------------------------------------------