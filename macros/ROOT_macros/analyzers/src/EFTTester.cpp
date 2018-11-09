#include "../include/EFTTester.h"

void EFTTester::performAnalysis(){
  // for (auto &name: m_dataframe->GetColumnNames()) {
  //   cout <<name  << endl;
  // }

  auto process_weight_lambda = this->getProcessWeightLambda();
  auto rdf_with_process_weight = m_dataframe->Define("process_weight", process_weight_lambda, {"process_name", "e_polarization", "p_polarization", "cross_section"});  
  auto event_weight_lambda = this->getEventWeightLambda();

  vector<pair<ROOT::RDF::RResultPtr<TH1D>, string>> th1s_w_leg{};
  TLegend *leg = new TLegend( 0.55, 0.4, 0.9, 0.85 );
  for ( auto const& element: this->getParameterPointMap() ) {
    auto index = element.first;
    auto pp = element.second;
    auto rdf_with_event_weight = rdf_with_process_weight.Define( ("event_weight" + to_string(index)).c_str(), event_weight_lambda, {("weight"+to_string(index)).c_str(), "process_weight"});  

    auto h1_VV_m = rdf_with_event_weight.Histo1D({("h1_VV_m" + to_string(index)).c_str(), "Di-boson mass; m_{VV}; Events", 50, 0, 1000}, "mctruth.VV_m", ("event_weight" + to_string(index)).c_str());

    string leg_entry = "FS0: " + to_string(int(pp.first)) + "TeV^{-4}, FS1: " + to_string(int(pp.second)) + "TeV^{-4}";
    th1s_w_leg.push_back(make_pair(h1_VV_m, leg_entry));
  }

  auto h1_VV_m_SM = rdf_with_process_weight.Histo1D({"h1_VV_m_SM", "Di-boson mass; m_{VV}; Events", 50, 0, 1000}, "mctruth.VV_m", "process_weight");

  cout << "Start plotting." << endl;
  TCanvas *canvas_h1 = new TCanvas("test", "", 0, 0, 600, 600);
  canvas_h1->SetTopMargin(1.5);
  int colour_count = 1;
  THStack* all_VV_m = new THStack("all_VV_m", "Di-boson mass; m_{VV}; Events");
  
  h1_VV_m_SM->SetLineColor(colour_count++);
  all_VV_m->Add(h1_VV_m_SM.GetPtr());
  leg->AddEntry(h1_VV_m_SM.GetPtr(), "Standard Model", "l");
  
  colour_count=30;
  for (auto & th1_w_leg: th1s_w_leg) {
    th1_w_leg.first->SetLineColor(colour_count++);
    all_VV_m->Add(th1_w_leg.first.GetPtr());
    leg->AddEntry(th1_w_leg.first.GetPtr(), th1_w_leg.second.c_str(), "l");
  }
  
  // canvas_h1->cd()->SetLogy();
  all_VV_m->Draw("hist nostack");
  leg->Draw();
  cout << "Save file." << endl;
  
  canvas_h1->Print( (this->getOutputDirectory() + "/test_plot_1.pdf").c_str() );
  canvas_h1->Print( (this->getOutputDirectory() + "/test_plot_1.root").c_str() );
  delete canvas_h1;


// 
// auto process_weight_lambda = this->getProcessWeightLambda();
// auto rdf_with_process_weight = m_dataframe->Define("process_weight", process_weight_lambda, {"process_name", "e_polarization", "p_polarization", "cross_section"});  
// auto event_weight_lambda = this->getEventWeightLambda();
// auto rdf_with_event_weight1 = rdf_with_process_weight.Define("event_weight1", event_weight_lambda, {"weight1", "process_weight"});  
// auto rdf_with_event_weight2 = rdf_with_process_weight.Define("event_weight2", event_weight_lambda, {"weight2", "process_weight"});  
// auto rdf_with_event_weight3 = rdf_with_process_weight.Define("event_weight3", event_weight_lambda, {"weight3", "process_weight"});  
// auto rdf_with_event_weight4 = rdf_with_process_weight.Define("event_weight4", event_weight_lambda, {"weight4", "process_weight"});  
// auto rdf_with_event_weight5 = rdf_with_process_weight.Define("event_weight5", event_weight_lambda, {"weight5", "process_weight"});  
// auto rdf_with_event_weight6 = rdf_with_process_weight.Define("event_weight6", event_weight_lambda, {"weight6", "process_weight"});  
// 
// TCanvas *canvas_h1 = new TCanvas("test", "", 0, 0, 600, 600);
// auto h1_VV_m_1 = rdf_with_event_weight1.Histo1D({"h1_VV_m_1", "Di-boson mass; m_{VV}; Events", 100, 0, 1000}, "mctruth.VV_m", "event_weight1");
// auto h1_VV_m_2 = rdf_with_event_weight2.Histo1D({"h1_VV_m_2", "Di-boson mass; m_{VV}; Events", 100, 0, 1000}, "mctruth.VV_m", "event_weight2");
// auto h1_VV_m_3 = rdf_with_event_weight3.Histo1D({"h1_VV_m_3", "Di-boson mass; m_{VV}; Events", 100, 0, 1000}, "mctruth.VV_m", "event_weight3");
// auto h1_VV_m_4 = rdf_with_event_weight4.Histo1D({"h1_VV_m_4", "Di-boson mass; m_{VV}; Events", 100, 0, 1000}, "mctruth.VV_m", "event_weight4");
// auto h1_VV_m_5 = rdf_with_event_weight5.Histo1D({"h1_VV_m_5", "Di-boson mass; m_{VV}; Events", 100, 0, 1000}, "mctruth.VV_m", "event_weight5");
// auto h1_VV_m_6 = rdf_with_event_weight6.Histo1D({"h1_VV_m_6", "Di-boson mass; m_{VV}; Events", 100, 0, 1000}, "mctruth.VV_m", "event_weight6");
// cout << " PP 1 : " << this->getParameterPoint(1).first << " " <<this->getParameterPoint(1).second << endl;
// cout << " PP 2 : " << this->getParameterPoint(2).first << " " <<this->getParameterPoint(2).second << endl;
// cout << " PP 3 : " << this->getParameterPoint(3).first << " " <<this->getParameterPoint(3).second << endl;
// cout << " PP 4 : " << this->getParameterPoint(4).first << " " <<this->getParameterPoint(4).second << endl;
// cout << " PP 5 : " << this->getParameterPoint(5).first << " " <<this->getParameterPoint(5).second << endl;
// cout << " PP 6 : " << this->getParameterPoint(6).first << " " <<this->getParameterPoint(6).second << endl;
// h1_VV_m_1->Draw("hist");
// h1_VV_m_2->Draw("hist same");
// h1_VV_m_3->Draw("hist same");
// h1_VV_m_4->Draw("hist same");
// h1_VV_m_5->Draw("hist same");
// h1_VV_m_6->Draw("hist same");
//   canvas_h1->Print("/afs/desy.de/user/b/beyerjac/flc/VBS/aQGC_analysis/macros/ROOT_macros/test_plot_1.pdf");
// 
// delete canvas_h1;


}