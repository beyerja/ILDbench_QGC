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
  
  auto h1_VV_m = rdf_with_process_weight.Histo1D({"h1_VV_m", "Di-boson mass; m_{VV}; Events", 50, 0, 1000}, "mctruth.VV_m", "process_weight");
  auto h1_VV_m_WWsignal = rdf_WWsignal.Histo1D({"h1_VV_m_WWsignal", "Di-boson mass; m_{VV}; Events", 100, 0, 1000}, "mctruth.VV_m", "process_weight");
  auto h1_VV_m_ZZsignal = rdf_ZZsignal.Histo1D({"h1_VV_m_ZZsignal", "Di-boson mass; m_{VV}; Events", 100, 0, 1000}, "mctruth.VV_m", "process_weight");

  auto h1_V1_m_WWsignal = rdf_WWsignal.Histo1D({"h1_V1_m_WWsignal", "Boson mass, WW like events; m_{V1}; Events", 75, 0, 150}, "mctruth.V1_m", "process_weight");
  auto h1_V2_m_WWsignal = rdf_WWsignal.Histo1D({"h1_V2_m_WWsignal", "Boson mass, WW like events; m_{V2}; Events", 75, 0, 150}, "mctruth.V2_m", "process_weight");
  auto h1_V1_m_ZZsignal = rdf_ZZsignal.Histo1D({"h1_V1_m_ZZsignal", "Boson mass, ZZ like events; m_{V1}; Events", 75, 0, 150}, "mctruth.V1_m", "process_weight");
  auto h1_V2_m_ZZsignal = rdf_ZZsignal.Histo1D({"h1_V2_m_ZZsignal", "Boson mass, ZZ like events; m_{V2}; Events", 75, 0, 150}, "mctruth.V2_m", "process_weight");

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

  unique_ptr<TCanvas> canvas_V_m (new TCanvas("V_m", "", 0, 0, 600, 600));
  unique_ptr<THStack> V_m_stack (new THStack("V_m", "; m_{V}; 2*Events")); 
  unique_ptr<TLegend> V_m_leg (new TLegend( 0.6, 0.6, 0.9, 0.9 )); 
  auto h1_V_m_WWsignal = new TH1D( "h1_V_m_WWsignal", "Boson mass, WW like events; m_{V}; 2*Events", 75, 0, 150 );
  auto h1_V_m_ZZsignal = new TH1D( "h1_V_m_ZZsignal", "Boson mass, ZZ like events; m_{V}; 2*Events", 75, 0, 150 );
  h1_V_m_WWsignal->Add( h1_V1_m_WWsignal.GetPtr(), h1_V2_m_WWsignal.GetPtr() );
  h1_V_m_ZZsignal->Add( h1_V1_m_ZZsignal.GetPtr(), h1_V2_m_ZZsignal.GetPtr() );
  h1_V_m_WWsignal->SetLineColor(4);
  h1_V_m_ZZsignal->SetLineColor(2);
  h1_V_m_WWsignal->SetLineStyle(1);
  h1_V_m_ZZsignal->SetLineStyle(1);
  V_m_stack->Add( h1_V_m_WWsignal );
  V_m_stack->Add( h1_V_m_ZZsignal );
  V_m_leg->SetHeader("generator level");
  V_m_leg->AddEntry( h1_V_m_WWsignal, "WW", "l" );
  V_m_leg->AddEntry( h1_V_m_ZZsignal, "ZZ", "l" );
  V_m_stack->Draw("nostack hist");
  V_m_stack->GetYaxis()->SetMaxDigits(3);
  V_m_leg->Draw();
  canvas_V_m->Print( (this->getOutputDirectory() + "/V_m.pdf").c_str() );
  unique_ptr<TFile> V_m_file ( new TFile( ( this->getOutputDirectory() + "/V_m.root").c_str(), "RECREATE" ) );
  h1_V_m_WWsignal->Write();
  h1_V_m_ZZsignal->Write();
  V_m_file->Close();
}