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
  
  auto IsTrueWWSignalCut          = Cuts::getIntCutLambda( 24 );
  auto IsTrueZZSignalCut          = Cuts::getIntCutLambda( 23 );
  auto IsTrueBkgCut             = Cuts::getIntCutLambda( 0 );
  
  auto process_weight_lambda = this->getProcessWeightLambda();
  
  
  float min_m_Zregion = 0;
  float m_WZborder    = 86;
  float max_m_Wregion = 1000;
  auto ZMassRegionCut = Cuts::getMinMaxCutLambda( m_WZborder , max_m_Wregion );
  auto WMassRegionCut = Cuts::getMinMaxCutLambda( min_m_Zregion , m_WZborder );
  
  //============================================================================
  //============================================================================
  // Start coding lazy actions from here!
  //============================================================================
  //============================================================================
  
  
  // TODO make this standard when possible with newer root version!
  auto rdf_with_extra_variables = m_dataframe->Define( "recoVmMean", VarComb::getMeanLambda<float>(), {"reco.V1_m", "reco.V2_m"} );  
  auto rdf_with_process_weight = rdf_with_extra_variables.Define("process_weight", process_weight_lambda, {"process_name", "e_polarization", "p_polarization", "cross_section"});  
  
  auto rdf_WWsignal_no_cuts = rdf_with_process_weight.Filter( IsTrueWWSignalCut, {"mctruth.signal_type"} );
  auto rdf_ZZsignal_no_cuts = rdf_with_process_weight.Filter( IsTrueZZSignalCut, {"mctruth.signal_type"} );
  auto rdf_bkg_no_cuts    = rdf_with_process_weight.Filter( IsTrueBkgCut, {"mctruth.signal_type"} );
  
  //----------------------------------------------------------------------------
  // Apply all selection cuts
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
  //----------------------------------------------------------------------------

  auto rdf_WWsignal_with_cuts = rdf_data_after_cuts.Filter( IsTrueWWSignalCut, {"mctruth.signal_type"} );
  auto rdf_ZZsignal_with_cuts = rdf_data_after_cuts.Filter( IsTrueZZSignalCut, {"mctruth.signal_type"} );
  auto rdf_bkg_with_cuts    = rdf_data_after_cuts.Filter( IsTrueBkgCut, {"mctruth.signal_type"} );

  //----------------------------------------------------------------------------
  // Count signal/bkg numbers w/ and w/o cuts
  auto count_WWsignal_no_cuts   = rdf_WWsignal_no_cuts.Count();
  auto count_ZZsignal_no_cuts   = rdf_ZZsignal_no_cuts.Count();
  auto count_bkg_no_cuts        = rdf_bkg_no_cuts.Count();
  auto count_WWsignal_with_cuts = rdf_WWsignal_with_cuts.Count();
  auto count_ZZsignal_with_cuts = rdf_ZZsignal_with_cuts.Count();
  auto count_bkg_with_cuts      = rdf_bkg_with_cuts.Count();
  //----------------------------------------------------------------------------
  
  //----------------------------------------------------------------------------
  // Histograms of diboson mass
  auto h1_VV_m_nocuts = rdf_with_process_weight.Histo1D({"h1_VV_m_nocuts", "Di-boson mass before cuts; m_{VV}; Events", 100, 0, 1000}, "reco.VV_m", "process_weight");
  auto h1_VV_m_nocuts_WWsignal = rdf_WWsignal_no_cuts.Histo1D({"h1_VV_m_nocuts_WWsignal", "Di-boson mass before cuts; m_{VV}; Events", 100, 0, 1000}, "reco.VV_m", "process_weight");
  auto h1_VV_m_nocuts_ZZsignal = rdf_ZZsignal_no_cuts.Histo1D({"h1_VV_m_nocuts_ZZsignal", "Di-boson mass before cuts; m_{VV}; Events", 100, 0, 1000}, "reco.VV_m", "process_weight");
  auto h1_VV_m_nocuts_bkg = rdf_bkg_no_cuts.Histo1D({"h1_VV_m_nocuts_bkg", "Di-boson mass before cuts; m_{VV}; Events", 100, 0, 1000}, "reco.VV_m", "process_weight");
  auto h1_VV_m_withcuts = rdf_data_after_cuts.Histo1D({"h1_VV_m", "Di-boson mass after cuts; m_{VV}; Events", 30, 150, 450}, "reco.VV_m", "process_weight");
  auto h1_VV_m_withcuts_WWsignal = rdf_WWsignal_with_cuts.Histo1D({"h1_VV_m_WWsignal", "Di-boson mass after cuts; m_{VV}; Events", 30, 150, 450}, "reco.VV_m", "process_weight");
  auto h1_VV_m_withcuts_ZZsignal = rdf_ZZsignal_with_cuts.Histo1D({"h1_VV_m_ZZsignal", "Di-boson mass after cuts; m_{VV}; Events", 30, 150, 450}, "reco.VV_m", "process_weight");
  auto h1_VV_m_withcuts_bkg = rdf_bkg_with_cuts.Histo1D({"h1_VV_m_bkg", "Di-boson mass after cuts; m_{VV}; Events", 30, 150, 450}, "reco.VV_m", "process_weight");
  //----------------------------------------------------------------------------
  
  //----------------------------------------------------------------------------
  // Count of true WW/ZZ events in the reconstructed WW/ZZ regions
  auto rdf_ZZsignal_inZZregion_with_cuts = rdf_ZZsignal_with_cuts.Filter( ZMassRegionCut, {"recoVmMean"} );
  auto rdf_ZZsignal_inWWregion_with_cuts = rdf_ZZsignal_with_cuts.Filter( WMassRegionCut, {"recoVmMean"} );
  auto rdf_WWsignal_inZZregion_with_cuts = rdf_WWsignal_with_cuts.Filter( ZMassRegionCut, {"recoVmMean"} );
  auto rdf_WWsignal_inWWregion_with_cuts = rdf_WWsignal_with_cuts.Filter( WMassRegionCut, {"recoVmMean"} );
  
  auto count_ZZsignal_inZZregion_with_cuts = rdf_ZZsignal_inZZregion_with_cuts.Count();
  auto count_ZZsignal_inWWregion_with_cuts = rdf_ZZsignal_inWWregion_with_cuts.Count();
  auto count_WWsignal_inWWregion_with_cuts = rdf_WWsignal_inWWregion_with_cuts.Count();
  auto count_WWsignal_inZZregion_with_cuts = rdf_WWsignal_inZZregion_with_cuts.Count();
  //----------------------------------------------------------------------------

  //----------------------------------------------------------------------------
  // Observable histrograms
  auto h1_absCosThetaVstar_withcuts_WWsignal = rdf_WWsignal_with_cuts.Histo1D({"h1_absCosThetaVstar_withcuts_WWsignal", "V angle in VV system after cuts; |cos #theta*_{V}|; Events", 20, 0, 1}, "reco.VV_V_absCosThetaStar", "process_weight");
  auto h1_absCosThetaVstar_withcuts_ZZsignal = rdf_ZZsignal_with_cuts.Histo1D({"h1_absCosThetaVstar_withcuts_ZZsignal", "V angle in VV system after cuts; |cos #theta*_{V}|; Events", 20, 0, 1}, "reco.VV_V_absCosThetaStar", "process_weight");
  auto h1_absCosThetaVstar_withcuts_bkg      = rdf_bkg_with_cuts     .Histo1D({"h1_absCosThetaVstar_withcuts_bkg", "V angle in VV system after cuts; |cos #theta*_{V}|; Events", 20, 0, 1}, "reco.VV_V_absCosThetaStar", "process_weight");
  
  auto h1_absCosThetaJetstarV1_withcuts_WWsignal = rdf_WWsignal_with_cuts.Histo1D({"h1_absCosThetaJetstarV1_withcuts_WWsignal", "jet angle in V1 system after cuts; |cos #theta*_{jet}|_{V_{1}}; Events", 20, 0, 1}, "reco.V1_jet_absCosThetaStar", "process_weight");
  auto h1_absCosThetaJetstarV1_withcuts_ZZsignal = rdf_ZZsignal_with_cuts.Histo1D({"h1_absCosThetaJetstarV1_withcuts_ZZsignal", "jet angle in V1 system after cuts; |cos #theta*_{jet}|_{V_{1}}; Events", 20, 0, 1}, "reco.V1_jet_absCosThetaStar", "process_weight");
  auto h1_absCosThetaJetstarV1_withcuts_bkg      = rdf_bkg_with_cuts     .Histo1D({"h1_absCosThetaJetstarV1_withcuts_bkg", "jet angle in V1 system after cuts; |cos #theta*_{jet}|_{V_{1}}; Events", 20, 0, 1}, "reco.V1_jet_absCosThetaStar", "process_weight");
  
  auto h1_absCosThetaJetstarV2_withcuts_WWsignal = rdf_WWsignal_with_cuts.Histo1D({"h1_absCosThetaJetstarV2_withcuts_WWsignal", "jet angle in V2 system after cuts; |cos #theta*_{jet}|_{V_{2}}; Events", 20, 0, 1}, "reco.V2_jet_absCosThetaStar", "process_weight");
  auto h1_absCosThetaJetstarV2_withcuts_ZZsignal = rdf_ZZsignal_with_cuts.Histo1D({"h1_absCosThetaJetstarV2_withcuts_ZZsignal", "jet angle in V2 system after cuts; |cos #theta*_{jet}|_{V_{2}}; Events", 20, 0, 1}, "reco.V2_jet_absCosThetaStar", "process_weight");
  auto h1_absCosThetaJetstarV2_withcuts_bkg      = rdf_bkg_with_cuts     .Histo1D({"h1_absCosThetaJetstarV2_withcuts_bkg", "jet angle in V2 system after cuts; |cos #theta*_{jet}|_{V_{2}}; Events", 20, 0, 1}, "reco.V2_jet_absCosThetaStar", "process_weight");

  //----------------------------------------------------------------------------

  //============================================================================
  //============================================================================
  // CAREFUL!!!
  // Everything up to here should only be lazy actions! (See RDF manual)
  // From here on start using results (e.g. plotting etc.)
  //============================================================================
  //============================================================================

  cout << *count_WWsignal_no_cuts << endl;
  cout << *count_ZZsignal_no_cuts << endl;
  cout << *count_bkg_no_cuts << endl;
  cout << *count_WWsignal_with_cuts << endl;
  cout << *count_ZZsignal_with_cuts << endl;
  cout << *count_bkg_with_cuts << endl;
  // 
  TCanvas *canvas_h1 = new TCanvas("test", "", 0, 0, 600, 600);
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
  canvas_h1->Print( ( this->getOutputDirectory() + "/cutcomparison_VV_m.pdf").c_str() );
  delete canvas_h1;
  
  cout << endl;
  cout << "True ZZ events in: ZZ region: " << *count_ZZsignal_inZZregion_with_cuts << " , WW region: " << *count_ZZsignal_inWWregion_with_cuts << endl;
  cout << "True WW events in: WW region: " << *count_WWsignal_inWWregion_with_cuts << " , ZZ region: " << *count_WWsignal_inZZregion_with_cuts << endl;
  
  
  TCanvas *canvas_absCosThetaVstar = new TCanvas("absCosThetaVstar", "", 0, 0, 600, 600);
  THStack *absCosThetaVstar_stack = new THStack("absCosThetaVstar", "; |cos #theta*_{V}|; Events");
  TLegend *absCosThetaVstar_leg = new TLegend( 0.6, 0.6, 0.9, 0.9 ); // TODO
  h1_absCosThetaVstar_withcuts_WWsignal->SetLineColor(4);
  h1_absCosThetaVstar_withcuts_ZZsignal->SetLineColor(2);
  absCosThetaVstar_stack->Add(h1_absCosThetaVstar_withcuts_WWsignal.GetPtr());
  absCosThetaVstar_stack->Add(h1_absCosThetaVstar_withcuts_ZZsignal.GetPtr());
  absCosThetaVstar_stack->Add(h1_absCosThetaVstar_withcuts_bkg.GetPtr());
  absCosThetaVstar_leg->SetHeader("after cuts");
  absCosThetaVstar_leg->AddEntry( h1_absCosThetaVstar_withcuts_WWsignal.GetPtr(), "WW", "l" );
  absCosThetaVstar_leg->AddEntry( h1_absCosThetaVstar_withcuts_ZZsignal.GetPtr(), "ZZ", "l" );
  absCosThetaVstar_leg->AddEntry( h1_absCosThetaVstar_withcuts_bkg.GetPtr(), "SM bkg", "l" );
  absCosThetaVstar_stack->Draw("nostack hist");
  absCosThetaVstar_stack->GetYaxis()->SetMaxDigits(3);
  absCosThetaVstar_leg->Draw();
  canvas_absCosThetaVstar->Print( ( this->getOutputDirectory() + "/absCosThetaVstar.pdf").c_str() );
  delete canvas_absCosThetaVstar;
  delete absCosThetaVstar_stack;
  delete absCosThetaVstar_leg;
  
  
  TCanvas *canvas_VV_m = new TCanvas("VV_m", "", 0, 0, 600, 600);
  THStack *VV_m_stack = new THStack("VV_m", "; m_{VV}; Events");
  TLegend *VV_m_leg = new TLegend( 0.6, 0.6, 0.9, 0.9 ); // TODO
  h1_VV_m_withcuts_WWsignal->SetLineColor(4);
  h1_VV_m_withcuts_ZZsignal->SetLineColor(2);
  h1_VV_m_withcuts_WWsignal->SetLineStyle(1);
  h1_VV_m_withcuts_ZZsignal->SetLineStyle(1);
  VV_m_stack->Add(h1_VV_m_withcuts_WWsignal.GetPtr());
  VV_m_stack->Add(h1_VV_m_withcuts_ZZsignal.GetPtr());
  VV_m_stack->Add(h1_VV_m_withcuts_bkg.GetPtr());
  VV_m_leg->SetHeader("after cuts");
  VV_m_leg->AddEntry( h1_VV_m_withcuts_WWsignal.GetPtr(), "WW", "l" );
  VV_m_leg->AddEntry( h1_VV_m_withcuts_ZZsignal.GetPtr(), "ZZ", "l" );
  VV_m_leg->AddEntry( h1_VV_m_withcuts_bkg.GetPtr(), "SM bkg", "l" );
  VV_m_stack->Draw("nostack hist");
  VV_m_stack->GetYaxis()->SetMaxDigits(3);
  VV_m_leg->Draw();
  canvas_VV_m->Print( ( this->getOutputDirectory() + "/VV_m.pdf").c_str() );
  delete canvas_VV_m;
  delete VV_m_stack;
  delete VV_m_leg;
  
  
  

  

  //TODO CAREFUL WHEN COMBINING THE Y AXIS BECOMES 2*EVENTS
  auto h1_absCosThetaJetstar_combined_withcuts_WWsignal = new TH1D("h1_absCosThetaJetstar_combined_withcuts_WWsignal", "jet angle in V system after cuts, both V; |cos #theta*_{jet}|; 2*Events", 20, 0, 1 );
  auto h1_absCosThetaJetstar_combined_withcuts_ZZsignal = new TH1D("h1_absCosThetaJetstar_combined_withcuts_ZZsignal", "jet angle in V system after cuts, both V; |cos #theta*_{jet}|; 2*Events", 20, 0, 1 );
  auto h1_absCosThetaJetstar_combined_withcuts_bkg      = new TH1D("h1_absCosThetaJetstar_combined_withcuts_bkg", "jet angle in V system after cuts, both V; |cos #theta*_{jet}|; 2*Events", 20, 0, 1 );
  
  h1_absCosThetaJetstar_combined_withcuts_WWsignal->Add( h1_absCosThetaJetstarV1_withcuts_WWsignal.GetPtr(), h1_absCosThetaJetstarV1_withcuts_WWsignal.GetPtr() );
  h1_absCosThetaJetstar_combined_withcuts_ZZsignal->Add( h1_absCosThetaJetstarV1_withcuts_ZZsignal.GetPtr(), h1_absCosThetaJetstarV1_withcuts_ZZsignal.GetPtr() );
  h1_absCosThetaJetstar_combined_withcuts_bkg->Add( h1_absCosThetaJetstarV1_withcuts_bkg.GetPtr(), h1_absCosThetaJetstarV1_withcuts_bkg.GetPtr() );
  
  TCanvas *canvas_absCosThetaJetstar_combined = new TCanvas("absCosThetaJetstar_combined", "", 0, 0, 600, 600);
  THStack *absCosThetaJetstar_combined_stack = new THStack("absCosThetaJetstar_combined", "; |cos #theta*_{jet}|; 2*Events");
  TLegend *absCosThetaJetstar_combined_leg = new TLegend( 0.6, 0.6, 0.9, 0.9 ); // TODO
  h1_absCosThetaJetstar_combined_withcuts_WWsignal->SetLineColor(4);
  h1_absCosThetaJetstar_combined_withcuts_ZZsignal->SetLineColor(2);
  absCosThetaJetstar_combined_stack->Add(h1_absCosThetaJetstar_combined_withcuts_WWsignal);
  absCosThetaJetstar_combined_stack->Add(h1_absCosThetaJetstar_combined_withcuts_ZZsignal);
  absCosThetaJetstar_combined_stack->Add(h1_absCosThetaJetstar_combined_withcuts_bkg);
  absCosThetaJetstar_combined_leg->SetHeader("after cuts");
  absCosThetaJetstar_combined_leg->AddEntry( h1_absCosThetaJetstar_combined_withcuts_WWsignal, "WW", "l" );
  absCosThetaJetstar_combined_leg->AddEntry( h1_absCosThetaJetstar_combined_withcuts_ZZsignal, "ZZ", "l" );
  absCosThetaJetstar_combined_leg->AddEntry( h1_absCosThetaJetstar_combined_withcuts_bkg, "SM bkg", "l" );
  absCosThetaJetstar_combined_stack->Draw("nostack hist");
  absCosThetaJetstar_combined_stack->GetYaxis()->SetMaxDigits(3);
  absCosThetaJetstar_combined_leg->Draw();
  canvas_absCosThetaJetstar_combined->Print( ( this->getOutputDirectory() + "/absCosThetaJetstar_combined.pdf").c_str() );
  delete canvas_absCosThetaJetstar_combined;
  delete absCosThetaJetstar_combined_stack;
  delete absCosThetaJetstar_combined_leg;
  
  
}

//-------------------------------------------------------------------------------------------------