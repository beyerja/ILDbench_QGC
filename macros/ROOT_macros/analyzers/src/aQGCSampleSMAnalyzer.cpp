#include "../include/aQGCSampleSMAnalyzer.h"

void aQGCSampleSMAnalyzer::performAnalysis(){
  // for (auto &name: m_dataframe->GetColumnNames()) {
  //   cout <<name  << endl;
  // }

  auto process_weight_lambda = this->getProcessWeightLambda();
  auto IsTrueWWSignalCut          = Cuts::getIntCutLambda( 24 );
  auto IsTrueZZSignalCut          = Cuts::getIntCutLambda( 23 );
  
  auto rdf_with_process_weight = m_dataframe->Define("process_weight", process_weight_lambda, {"process_name", "e_polarization", "p_polarization", "cross_section"});  
  auto rdf_WWsignal = rdf_with_process_weight.Filter( IsTrueWWSignalCut, {"mctruth.signal_type"} );
  auto rdf_ZZsignal = rdf_with_process_weight.Filter( IsTrueZZSignalCut, {"mctruth.signal_type"} );
  
  auto h1_VV_m = m_dataframe->Histo1D({"h1_VV_m", "Di-boson mass; m_{VV}; Events", 50, 0, 1000}, "mctruth.VV_m");
  auto h1_VV_m_WWsignal = rdf_WWsignal.Histo1D({"h1_VV_m_WWsignal", "Di-boson mass; m_{VV}; Events", 100, 0, 1000}, "mctruth.VV_m", "process_weight");
  auto h1_VV_m_ZZsignal = rdf_ZZsignal.Histo1D({"h1_VV_m_ZZsignal", "Di-boson mass; m_{VV}; Events", 100, 0, 1000}, "mctruth.VV_m", "process_weight");

  unique_ptr<TCanvas> canvas_VV_m (new TCanvas("VV_m", "", 0, 0, 600, 600));
  unique_ptr<THStack> VV_m_stack (new THStack("VV_m", "; m_{VV}; Events")); 
  unique_ptr<TLegend> VV_m_leg (new TLegend( 0.6, 0.6, 0.9, 0.9 )); 
  h1_VV_m_WWsignal->SetLineColor(4);
  h1_VV_m_ZZsignal->SetLineColor(2);
  h1_VV_m_WWsignal->SetLineStyle(1);
  h1_VV_m_ZZsignal->SetLineStyle(1);
  VV_m_stack->Add( h1_VV_m_WWsignal.GetPtr());
  VV_m_stack->Add( h1_VV_m_ZZsignal.GetPtr());
  VV_m_leg->SetHeader("generator level");
  VV_m_leg->AddEntry( h1_VV_m_WWsignal.GetPtr(), "WW", "l" );
  VV_m_leg->AddEntry( h1_VV_m_ZZsignal.GetPtr(), "ZZ", "l" );
  VV_m_stack->Draw("nostack hist");
  VV_m_stack->GetYaxis()->SetMaxDigits(3);
  VV_m_leg->Draw();
  canvas_VV_m->Print( (this->getOutputDirectory() + "/VV_m.pdf").c_str() );
  unique_ptr<TFile> VV_m_file ( new TFile( ( this->getOutputDirectory() + "/VV_m.root").c_str(), "RECREATE" ) );
  h1_VV_m_WWsignal->Write();
  h1_VV_m_ZZsignal->Write();
  VV_m_file->Close();
}