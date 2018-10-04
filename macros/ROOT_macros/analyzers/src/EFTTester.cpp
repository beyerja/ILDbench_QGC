#include "../include/EFTTester.h"

void EFTTester::performAnalysis(){
  auto process_weight_lambda = this->getProcessWeightLambda();
  auto rdf_with_process_weight = m_dataframe->Define("process_weight", process_weight_lambda, {"process_name", "e_polarization", "p_polarization", "cross_section"});  
  auto event_weight_lambda = this->getEventWeightLambda();
  
  TCanvas *canvas_h1 = new TCanvas("test", "", 0, 0, 600, 600);
  vector<ROOT::RDF::RResultPtr<TH1D>> th1s{};
  for ( auto const& element: this->getParameterPointMap() ) {
    auto index = element.first;
    auto pp = element.second;
    auto rdf_with_event_weight = rdf_with_process_weight.Define( ("event_weight" + to_string(index)).c_str(), event_weight_lambda, {("weight"+to_string(index)).c_str(), "process_weight"});  

    auto h1_VV_m = rdf_with_event_weight.Histo1D({("h1_VV_m" + to_string(index)).c_str(), "Di-boson mass; m_{VV}; Events", 100, 0, 1000}, "mctruth.VV_m", ("event_weight" + to_string(index)).c_str());
    cout << " PP " << index << " : " << pp.first << " " << pp.second << endl;

    th1s.push_back(h1_VV_m);
    // canvas_h1->cd();
    // h1_VV_m->Draw("hist");
    // canvas_h1->Print("/afs/desy.de/user/b/beyerjac/flc/VBS/aQGC_analysis/macros/ROOT_macros/test_plot_1.pdf");
    // delete canvas_h1;
    // break;
  }
  
  for (auto & th1: th1s) {
    th1->Draw("hist same");
  }
  canvas_h1->Print("/afs/desy.de/user/b/beyerjac/flc/VBS/aQGC_analysis/macros/ROOT_macros/test_plot_1.pdf");
  delete canvas_h1;
  
  // auto rdf_with_event_weight1 = rdf_with_process_weight.Define("event_weight1", event_weight_lambda, {"weight1", "process_weight"});  
  // auto rdf_with_event_weight2 = rdf_with_process_weight.Define("event_weight2", event_weight_lambda, {"weight2", "process_weight"});  
  // auto rdf_with_event_weight3 = rdf_with_process_weight.Define("event_weight3", event_weight_lambda, {"weight3", "process_weight"});  
  // auto rdf_with_event_weight4 = rdf_with_process_weight.Define("event_weight4", event_weight_lambda, {"weight4", "process_weight"});  
  // auto rdf_with_event_weight5 = rdf_with_process_weight.Define("event_weight5", event_weight_lambda, {"weight5", "process_weight"});  
  // auto rdf_with_event_weight6 = rdf_with_process_weight.Define("event_weight6", event_weight_lambda, {"weight6", "process_weight"});  
  // 
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
  
  

}