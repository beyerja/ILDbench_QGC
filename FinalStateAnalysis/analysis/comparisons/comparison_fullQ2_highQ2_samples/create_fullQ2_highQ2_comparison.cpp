// WARNING: Needs to be run with ROOT 6.14 or newer! (Else creating .C files is a meeeeeess)

float string_to_float( string str ) {
  return strtof((str).c_str(),0);
}


shared_ptr<TLatex> add_ILD_mark( shared_ptr<TCanvas> canvas, Double_t x0, Double_t y0, Float_t text_size=0.25 ) {
  canvas->cd();
  shared_ptr<TLatex> ild_tex (new TLatex(x0,y0,"ILD"));
  ild_tex->SetName( (string(canvas->GetName()) + "_ildlogo").c_str() );
  ild_tex->SetTextSize(text_size);
  ild_tex->SetTextFont(62);
  ild_tex->SetLineWidth(2);
  ild_tex->Draw();
  return ild_tex;
}

shared_ptr<TLatex> add_prelim_mark( shared_ptr<TCanvas> canvas, Double_t x0, Double_t y0, Float_t text_size=0.2 ) {
  canvas->cd();
  shared_ptr<TLatex> prelim_tex (new TLatex(x0,y0,"preliminary"));
  prelim_tex->SetName( (string(canvas->GetName()) + "_prelim_text").c_str() );
  prelim_tex->SetTextSize(text_size);
  prelim_tex->SetTextFont(42);
  prelim_tex->SetLineWidth(2);
  prelim_tex->Draw();
  return prelim_tex;
}

void adjust_canvas_left_to_square_pad(shared_ptr<TCanvas> canvas) {
  double vertical_square_fraction   = 1.0 - canvas->GetTopMargin() - canvas->GetBottomMargin();
  double width_to_height_ratio      = double(canvas->GetWindowWidth()) / double(canvas->GetWindowHeight()); 
  double horizontal_square_fraction = vertical_square_fraction / width_to_height_ratio;
  double left_margin = 1.0 - canvas->GetRightMargin() - horizontal_square_fraction;
  canvas->SetLeftMargin(left_margin);
}

void create_fullQ2_highQ2_comparison() {
  // Standard ROOT plot setting
  gROOT->Reset();
  gStyle->SetOptStat(0);
  TH2::SetDefaultSumw2();

  string fullQ2_directory = "/afs/desy.de/group/flc/pool/beyerjac/VBS/nunu_hadron/fullQ2range/v02-00-02_l5_o1_v02_output/all_signals_plots/";
  string highQ2_directory = "/afs/desy.de/group/flc/pool/beyerjac/VBS/nunu_hadron/v02-00-02_l5_o1_v02_output/all_signals_plots/";
  string output_dir   = "/afs/desy.de/group/flc/pool/beyerjac/VBS/nunu_hadron/comparisons/comparison_fullQ2_highQ2_samples/";

  shared_ptr<TFile> file_fullQ2             = make_shared<TFile>( (fullQ2_directory + "./_all_TH1Ds.root").c_str() ) ;
  shared_ptr<TFile> file_highQ2             = make_shared<TFile>( (highQ2_directory + "./_all_TH1Ds.root").c_str() ) ;
  vector<shared_ptr<TFile>> closables {file_fullQ2, file_highQ2};
  
  TH1D* true_pair_E_fullQ2 = (TH1D*)file_fullQ2->Get("true_pair_E");
  TH1D* true_pair_E_highQ2 = (TH1D*)file_highQ2->Get("true_pair_E");
  TH1D* true_pair_theta_fullQ2 = (TH1D*)file_fullQ2->Get("true_pair_theta");
  TH1D* true_pair_theta_highQ2 = (TH1D*)file_highQ2->Get("true_pair_theta");
  
  vector<TObject*> deletables {true_pair_E_fullQ2,true_pair_E_highQ2,true_pair_theta_fullQ2,true_pair_theta_highQ2};
  
  unique_ptr<TColor> orange { new TColor( 9024, 241./256., 163./256., 64./256. ) } ;
  unique_ptr<TColor> violet { new TColor( 9124, 153./256., 142./256., 195./256. ) } ;
  // ---------------------------------------------------------------------------
  
  // ---------------------------------------------------------------------------
  double canvas_comp_true_pair_E_height = 1200;
  double canvas_comp_true_pair_E_width  = 1250;

  shared_ptr<TCanvas> canvas_comp_true_pair_E (new TCanvas("canvas_comp_true_pair_E", "", 0, 0, canvas_comp_true_pair_E_width, canvas_comp_true_pair_E_height));
  shared_ptr<THStack> stack_comp_true_pair_E = make_shared<THStack>( "comp_true_pair_E", "; E_{V(qq)} [GeV]; a.u." );

  TH1D* clone_true_pair_E_fullQ2 = (TH1D*)true_pair_E_fullQ2->Clone();  clone_true_pair_E_fullQ2->SetName((string(clone_true_pair_E_fullQ2->GetName())+"_clone").c_str());  deletables.push_back(clone_true_pair_E_fullQ2); clone_true_pair_E_fullQ2->Scale(1.0/clone_true_pair_E_fullQ2->Integral()); clone_true_pair_E_fullQ2->Rebin(4);
  TH1D* clone_true_pair_E_highQ2 = (TH1D*)true_pair_E_highQ2->Clone();  clone_true_pair_E_highQ2->SetName((string(clone_true_pair_E_highQ2->GetName())+"_clone").c_str());  deletables.push_back(clone_true_pair_E_highQ2); clone_true_pair_E_highQ2->Scale(1.0/clone_true_pair_E_highQ2->Integral()); clone_true_pair_E_highQ2->Rebin(4);
  
  stack_comp_true_pair_E->Add(clone_true_pair_E_fullQ2);  clone_true_pair_E_fullQ2->SetLineColor(9124);    clone_true_pair_E_fullQ2->SetLineWidth(3);  clone_true_pair_E_fullQ2->SetLineStyle(1);
  stack_comp_true_pair_E->Add(clone_true_pair_E_highQ2);  clone_true_pair_E_highQ2->SetLineColor(9024);   clone_true_pair_E_highQ2->SetLineWidth(3);  clone_true_pair_E_highQ2->SetLineStyle(1);
  // stack_comp_true_pair_E->Draw("axis"); // Draw only axis
  
  unique_ptr<TLegend> leg_comp_true_pair_E (new TLegend(0.55, 0.63, 0.9, 0.85));
  
  leg_comp_true_pair_E->SetHeader("Generator level");
  leg_comp_true_pair_E->AddEntry(clone_true_pair_E_fullQ2, "full m_{VV} range", "l");
  leg_comp_true_pair_E->AddEntry(clone_true_pair_E_highQ2, "m_{VV} > 500GeV", "l");
  // 
  // stack_comp_true_pair_E->Draw("axis same"); // Draw only axis
  // 
  stack_comp_true_pair_E->Draw("hist nostack");
  leg_comp_true_pair_E->Draw();
  
  double max_comp_true_pair_E = std::max(clone_true_pair_E_fullQ2->GetMaximum(),clone_true_pair_E_highQ2->GetMaximum());
  stack_comp_true_pair_E->SetMaximum( max_comp_true_pair_E * 1.3 );
  double comp_true_pair_E_old_left_margin = canvas_comp_true_pair_E->GetLeftMargin();
  adjust_canvas_left_to_square_pad(canvas_comp_true_pair_E);
  stack_comp_true_pair_E->GetYaxis()->SetTitleOffset( stack_comp_true_pair_E->GetYaxis()->GetTitleOffset() * canvas_comp_true_pair_E->GetLeftMargin()/comp_true_pair_E_old_left_margin );
  // 
  shared_ptr<TLatex> comp_true_pair_E_logo = add_ILD_mark( canvas_comp_true_pair_E, 50, 1.17 * max_comp_true_pair_E, 0.1);
  shared_ptr<TLatex> comp_true_pair_E_prelim = add_prelim_mark( canvas_comp_true_pair_E, 180, 1.17 * max_comp_true_pair_E, 0.07); 

  string plot_name_comp_true_pair_E = "./comp_true_pair_E";
  canvas_comp_true_pair_E->Print((output_dir + plot_name_comp_true_pair_E + ".pdf").c_str());
  canvas_comp_true_pair_E->Print((output_dir + plot_name_comp_true_pair_E + ".jpg").c_str());
  canvas_comp_true_pair_E->Print((output_dir + plot_name_comp_true_pair_E + ".C").c_str());
  // ---------------------------------------------------------------------------
  
  // ---------------------------------------------------------------------------
  double canvas_comp_true_pair_theta_height = 1200;
  double canvas_comp_true_pair_theta_width  = 1250;

  shared_ptr<TCanvas> canvas_comp_true_pair_theta (new TCanvas("canvas_comp_true_pair_theta", "", 0, 0, canvas_comp_true_pair_theta_width, canvas_comp_true_pair_theta_height));
  shared_ptr<THStack> stack_comp_true_pair_theta = make_shared<THStack>( "comp_true_pair_theta", "; #theta_{V(qq)} [rad]; a.u." );

  TH1D* clone_true_pair_theta_fullQ2 = (TH1D*)true_pair_theta_fullQ2->Clone();  clone_true_pair_theta_fullQ2->SetName((string(clone_true_pair_theta_fullQ2->GetName())+"_clone").c_str());  deletables.push_back(clone_true_pair_theta_fullQ2); clone_true_pair_theta_fullQ2->Scale(1.0/clone_true_pair_theta_fullQ2->Integral());
  TH1D* clone_true_pair_theta_highQ2 = (TH1D*)true_pair_theta_highQ2->Clone();  clone_true_pair_theta_highQ2->SetName((string(clone_true_pair_theta_highQ2->GetName())+"_clone").c_str());  deletables.push_back(clone_true_pair_theta_highQ2); clone_true_pair_theta_highQ2->Scale(1.0/clone_true_pair_theta_highQ2->Integral());
  
  stack_comp_true_pair_theta->Add(clone_true_pair_theta_fullQ2);  clone_true_pair_theta_fullQ2->SetLineColor(9124);    clone_true_pair_theta_fullQ2->SetLineWidth(3);  clone_true_pair_theta_fullQ2->SetLineStyle(1);
  stack_comp_true_pair_theta->Add(clone_true_pair_theta_highQ2);  clone_true_pair_theta_highQ2->SetLineColor(9024);   clone_true_pair_theta_highQ2->SetLineWidth(3);  clone_true_pair_theta_highQ2->SetLineStyle(1);
  // stack_comp_true_pair_theta->Draw("axis"); // Draw only axis
  
  unique_ptr<TLegend> leg_comp_true_pair_theta (new TLegend(0.35, 0.63, 0.75, 0.83));
  
  leg_comp_true_pair_theta->SetHeader("Generator level");
  leg_comp_true_pair_theta->AddEntry(clone_true_pair_theta_fullQ2, "full m_{VV} range", "l");
  leg_comp_true_pair_theta->AddEntry(clone_true_pair_theta_highQ2, "m_{VV} > 500GeV", "l");
  // 
  // stack_comp_true_pair_theta->Draw("axis same"); // Draw only axis
  // 
  stack_comp_true_pair_theta->Draw("hist nostack");
  leg_comp_true_pair_theta->Draw();
  
  double max_comp_true_pair_theta = std::max(clone_true_pair_theta_fullQ2->GetMaximum(),clone_true_pair_theta_highQ2->GetMaximum());
  stack_comp_true_pair_theta->SetMaximum( max_comp_true_pair_theta * 1.15 );
  double comp_true_pair_theta_old_left_margin = canvas_comp_true_pair_theta->GetLeftMargin();
  adjust_canvas_left_to_square_pad(canvas_comp_true_pair_theta);
  stack_comp_true_pair_theta->GetYaxis()->SetTitleOffset( stack_comp_true_pair_theta->GetYaxis()->GetTitleOffset() * canvas_comp_true_pair_theta->GetLeftMargin()/comp_true_pair_theta_old_left_margin );
  // 
  shared_ptr<TLatex> comp_true_pair_theta_logo = add_ILD_mark( canvas_comp_true_pair_theta, 0.2, 1.03 * max_comp_true_pair_theta, 0.1);
  shared_ptr<TLatex> comp_true_pair_theta_prelim = add_prelim_mark( canvas_comp_true_pair_theta, 0.9, 1.03 * max_comp_true_pair_theta, 0.07); 

  string plot_name_comp_true_pair_theta = "./comp_true_pair_theta";
  canvas_comp_true_pair_theta->Print((output_dir + plot_name_comp_true_pair_theta + ".pdf").c_str());
  canvas_comp_true_pair_theta->Print((output_dir + plot_name_comp_true_pair_theta + ".jpg").c_str());
  canvas_comp_true_pair_theta->Print((output_dir + plot_name_comp_true_pair_theta + ".C").c_str());
  // ---------------------------------------------------------------------------
  

  for ( auto & deletable: deletables ) { delete deletable; }
  for ( auto & closable: closables ) { closable->Close(); }
}
