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


void create_separation_curve_comparison() {
  // Standard ROOT plot setting
  gROOT->Reset();
  gStyle->SetOptStat(0);
  TH2::SetDefaultSumw2();

  string l5_directory = "/afs/desy.de/group/flc/pool/beyerjac/VBS/nunu_hadron/fullQ2range/v02-00-02_l5_o1_v02_output/separation_curves/";
  string s5_directory = "/afs/desy.de/group/flc/pool/beyerjac/VBS/nunu_hadron/fullQ2range/v02-00-02_s5_o1_v02_output/separation_curves/";
  string output_dir   = "/afs/desy.de/group/flc/pool/beyerjac/VBS/nunu_hadron/fullQ2range/comparisons/comparison_separation_curves/";

  shared_ptr<TFile> file_l5             = make_shared<TFile>( (l5_directory + "./_all_TGraphs.root").c_str() ) ;
  shared_ptr<TFile> file_s5             = make_shared<TFile>( (s5_directory + "./_all_TGraphs.root").c_str() ) ;
  vector<shared_ptr<TFile>> closables {file_l5, file_s5};
  
  TGraph *sep_curve_genlevel            =  (TGraph*)file_l5->Get("separation_curve_genlevel") ;  
  TGraph *sep_curve_l5_reco             =  (TGraph*)file_l5->Get("separation_curve_reco") ;  
  TGraph *sep_curve_l5_cheatoverlay     =  (TGraph*)file_l5->Get("separation_curve_cheated_overlay") ;  
  TGraph *sep_curve_l5_cheatcluster     =  (TGraph*)file_l5->Get("separation_curve_cheatjet_custompairing") ;  
  TGraph *sep_curve_l5_cheatjet         =  (TGraph*)file_l5->Get("separation_curve_cheatjet") ;  
  TGraph *sep_curve_l5_cheatjet_noSLDs  =  (TGraph*)file_l5->Get("separation_curve_cheatjet_noSLDs") ;  
  TGraph *sep_curve_s5_reco             =  (TGraph*)file_s5->Get("separation_curve_reco") ;  
  TGraph *sep_curve_s5_cheatoverlay     =  (TGraph*)file_s5->Get("separation_curve_cheated_overlay") ;  
  TGraph *sep_curve_s5_cheatcluster     =  (TGraph*)file_s5->Get("separation_curve_cheatjet_custompairing") ;  
  TGraph *sep_curve_s5_cheatjet         =  (TGraph*)file_s5->Get("separation_curve_cheatjet") ;  
  TGraph *sep_curve_s5_cheatjet_noSLDs  =  (TGraph*)file_s5->Get("separation_curve_cheatjet_noSLDs") ;  
  TGraph *sep_curve_l5_reco_perfectbtag             =  (TGraph*)file_l5->Get("separation_curve_reco_perfectbtag") ;  
  TGraph *sep_curve_l5_cheatjet_perfectbtag         =  (TGraph*)file_l5->Get("separation_curve_cheatjet_perfectbtag") ;  
  TGraph *sep_curve_l5_cheatcluster_perfectbtag     =  (TGraph*)file_l5->Get("separation_curve_cheatjet_custompairing_perfectbtag") ;  
  TGraph *sep_curve_s5_reco_perfectbtag             =  (TGraph*)file_s5->Get("separation_curve_reco_perfectbtag") ;  
  TGraph *sep_curve_s5_cheatjet_perfectbtag         =  (TGraph*)file_s5->Get("separation_curve_cheatjet_perfectbtag") ;  
  TGraph *sep_curve_s5_cheatcluster_perfectbtag     =  (TGraph*)file_s5->Get("separation_curve_cheatjet_custompairing_perfectbtag") ;  

  
  vector<TObject*> deletables {sep_curve_l5_reco, sep_curve_l5_cheatjet, sep_curve_s5_reco, sep_curve_s5_cheatjet, sep_curve_l5_reco_perfectbtag, sep_curve_l5_cheatjet_perfectbtag, sep_curve_s5_reco_perfectbtag, sep_curve_s5_cheatjet_perfectbtag};
  
  unique_ptr<TColor> weak_blue { new TColor( 9024, 5./256., 113./256., 176./256. ) } ;
  unique_ptr<TColor> dark_blue { new TColor( 9124, 146./256., 197./256., 222./256. ) } ;
  unique_ptr<TColor> weak_red  { new TColor( 9023, 202./256., 0./256., 32./256. ) } ;
  unique_ptr<TColor> dark_red  { new TColor( 9123, 244./256., 165./256., 130./256. ) } ;
   // ---------------------------------------------------------------------------
  shared_ptr<TCanvas> canvas_sep_curve_comp_reco (new TCanvas("canvas_sep_curve_comp_reco", "", 0, 0, 600, 600));

  TMultiGraph *sep_curve_comp_reco = new TMultiGraph();
  sep_curve_comp_reco->SetTitle("; #font[12]{WW} ID eff.; #font[12]{ZZ} ID eff.");

  sep_curve_genlevel->SetLineColor(kBlack);
  sep_curve_l5_reco->SetLineColor(kBlue);
  sep_curve_l5_reco_perfectbtag->SetLineColor(kBlue-6);
  sep_curve_s5_reco->SetLineColor(kRed);
  sep_curve_s5_reco_perfectbtag->SetLineColor(kRed-8);

  sep_curve_genlevel->SetLineStyle(1);
  sep_curve_l5_reco->SetLineStyle(1);
  sep_curve_l5_reco_perfectbtag->SetLineStyle(1);
  sep_curve_s5_reco->SetLineStyle(2);
  sep_curve_s5_reco_perfectbtag->SetLineStyle(2);

  sep_curve_genlevel->SetLineWidth(4);
  sep_curve_l5_reco->SetLineWidth(4);
  sep_curve_l5_reco_perfectbtag->SetLineWidth(4);
  sep_curve_s5_reco->SetLineWidth(4);
  sep_curve_s5_reco_perfectbtag->SetLineWidth(4);

  int n_points_reco = sep_curve_l5_reco->GetN();
  TGraph *shade_l5_reco = new TGraph(2*n_points_reco);
  TGraph *shade_s5_reco = new TGraph(2*n_points_reco);
  for (unsigned int i=0;i<n_points_reco;i++) {
    double shade_x, shade_y;
    sep_curve_l5_reco_perfectbtag->GetPoint(i, shade_x, shade_y);
    shade_l5_reco->SetPoint(i, shade_x, shade_y);
    sep_curve_l5_reco->GetPoint(n_points_reco-i-1, shade_x, shade_y);
    shade_l5_reco->SetPoint(n_points_reco+i, shade_x, shade_y);
    sep_curve_s5_reco_perfectbtag->GetPoint(i, shade_x, shade_y);
    shade_s5_reco->SetPoint(i, shade_x, shade_y);
    sep_curve_s5_reco->GetPoint(n_points_reco-i-1, shade_x, shade_y);
    shade_s5_reco->SetPoint(n_points_reco+i, shade_x, shade_y);
  }
  shade_l5_reco->SetFillStyle(3001);
  shade_s5_reco->SetFillStyle(3345);
  shade_l5_reco->SetFillColorAlpha(kBlue-8, 0.5);
  shade_s5_reco->SetFillColorAlpha(kRed, 0.3);
  sep_curve_comp_reco->Add(shade_l5_reco,"f");
  sep_curve_comp_reco->Add(shade_s5_reco,"f");

  //sep_curve_comp_reco->Add(sep_curve_genlevel,"l");
  sep_curve_comp_reco->Add(sep_curve_l5_reco,"l");
  sep_curve_comp_reco->Add(sep_curve_l5_reco_perfectbtag,"l");
  sep_curve_comp_reco->Add(sep_curve_s5_reco,"l");
  sep_curve_comp_reco->Add(sep_curve_s5_reco_perfectbtag,"l");
 
  sep_curve_comp_reco->Draw("a");
  sep_curve_comp_reco->GetXaxis()->SetRangeUser(0,1);
  sep_curve_comp_reco->GetYaxis()->SetRangeUser(0,1);


  unique_ptr<TLegend> leg_sep_curve_comp_reco (new TLegend(0.2, 0.43, 0.65, 0.65));
  TLine *full_line_reco = new TLine(); 
  TLine *dash_line_reco = new TLine(); 
  full_line_reco->SetLineStyle(1); full_line_reco->SetLineWidth(5); full_line_reco->SetLineColor(kBlue); full_line_reco->Draw();
  dash_line_reco->SetLineStyle(2); dash_line_reco->SetLineWidth(5); dash_line_reco->SetLineColor(kRed); dash_line_reco->Draw();
  deletables.push_back(full_line_reco);
  deletables.push_back(dash_line_reco);
  leg_sep_curve_comp_reco->SetHeader("1D cut separation");
  leg_sep_curve_comp_reco->AddEntry(full_line_reco,   "IDR-L", "l");
  leg_sep_curve_comp_reco->AddEntry(dash_line_reco,   "IDR-S", "l");
  leg_sep_curve_comp_reco->Draw();
 
  shared_ptr<TLatex> sep_curve_comp_reco_logo = add_ILD_mark( canvas_sep_curve_comp_reco, 0.06, 0.65, 0.1);
  // shared_ptr<TLatex> sep_curve_comp_reco_prelim = add_prelim_mark( canvas_sep_curve_comp_reco, 0.28, 0.65, 0.07); 

  shared_ptr<TLatex> level_tex_reco (new TLatex(0.06, 0.25,"dark: full reconstruction"));
  level_tex_reco->SetName( (string(canvas_sep_curve_comp_reco->GetName()) + "_level_tex_reco").c_str() );
  level_tex_reco->SetTextSize(0.04);
  level_tex_reco->SetTextFont(62);
  level_tex_reco->SetLineWidth(2);
  level_tex_reco->Draw();

  shared_ptr<TLatex> level_tex_reco_perfectbtag (new TLatex(0.06, 0.18,"light: ideal b tag"));
  level_tex_reco_perfectbtag->SetName( (string(canvas_sep_curve_comp_reco->GetName()) + "_level_tex_reco_perfectbtag").c_str() );
  level_tex_reco_perfectbtag->SetTextSize(0.04);
  level_tex_reco_perfectbtag->SetTextFont(62);
  level_tex_reco_perfectbtag->SetLineWidth(2);
  level_tex_reco_perfectbtag->Draw();

  shared_ptr<TLatex> level_tex_reco_area (new TLatex(0.06, 0.11,"area: realistic b tag"));
  level_tex_reco_area->SetName( (string(canvas_sep_curve_comp_reco->GetName()) + "_level_tex_reco_area").c_str() );
  level_tex_reco_area->SetTextSize(0.04);
  level_tex_reco_area->SetTextFont(62);
  level_tex_reco_area->SetLineWidth(2);
  level_tex_reco_area->Draw();

  string plot_name_sep_curve_comp_reco = "./sep_curve_comp_reco";
  canvas_sep_curve_comp_reco->Print((output_dir + plot_name_sep_curve_comp_reco + ".pdf").c_str());
  canvas_sep_curve_comp_reco->Print((output_dir + plot_name_sep_curve_comp_reco + ".C").c_str());

  // ---------------------------------------------------------------------------
  
  // ---------------------------------------------------------------------------
  shared_ptr<TCanvas> canvas_sep_curve_comp_cheatjet (new TCanvas("canvas_sep_curve_comp_cheatjet", "", 0, 0, 600, 600));

  TMultiGraph *sep_curve_comp_cheatjet = new TMultiGraph();
  sep_curve_comp_cheatjet->SetTitle("; #font[12]{WW} ID eff.; #font[12]{ZZ} ID eff.");

  sep_curve_genlevel->SetLineColor(kBlack);
  sep_curve_l5_cheatjet->SetLineColor(kBlue);
  sep_curve_l5_cheatjet_perfectbtag->SetLineColor(kBlue-6);
  sep_curve_s5_cheatjet->SetLineColor(kRed);
  sep_curve_s5_cheatjet_perfectbtag->SetLineColor(kRed-8);

  sep_curve_genlevel->SetLineStyle(1);
  sep_curve_l5_cheatjet->SetLineStyle(1);
  sep_curve_l5_cheatjet_perfectbtag->SetLineStyle(1);
  sep_curve_s5_cheatjet->SetLineStyle(2);
  sep_curve_s5_cheatjet_perfectbtag->SetLineStyle(2);

  sep_curve_genlevel->SetLineWidth(4);
  sep_curve_l5_cheatjet->SetLineWidth(4);
  sep_curve_l5_cheatjet_perfectbtag->SetLineWidth(4);
  sep_curve_s5_cheatjet->SetLineWidth(4);
  sep_curve_s5_cheatjet_perfectbtag->SetLineWidth(4);

  int n_points_cheatjet = sep_curve_l5_cheatjet->GetN();
  TGraph *shade_l5_cheatjet = new TGraph(2*n_points_cheatjet);
  TGraph *shade_s5_cheatjet = new TGraph(2*n_points_cheatjet);
  for (unsigned int i=0;i<n_points_cheatjet;i++) {
    double shade_x, shade_y;
    sep_curve_l5_cheatjet_perfectbtag->GetPoint(i, shade_x, shade_y);
    shade_l5_cheatjet->SetPoint(i, shade_x, shade_y);
    sep_curve_l5_cheatjet->GetPoint(n_points_cheatjet-i-1, shade_x, shade_y);
    shade_l5_cheatjet->SetPoint(n_points_cheatjet+i, shade_x, shade_y);
    sep_curve_s5_cheatjet_perfectbtag->GetPoint(i, shade_x, shade_y);
    shade_s5_cheatjet->SetPoint(i, shade_x, shade_y);
    sep_curve_s5_cheatjet->GetPoint(n_points_cheatjet-i-1, shade_x, shade_y);
    shade_s5_cheatjet->SetPoint(n_points_cheatjet+i, shade_x, shade_y);
  }
  shade_l5_cheatjet->SetFillStyle(3001);
  shade_s5_cheatjet->SetFillStyle(3345);
  shade_l5_cheatjet->SetFillColorAlpha(kBlue-8, 0.5);
  shade_s5_cheatjet->SetFillColorAlpha(kRed, 0.3);
  sep_curve_comp_cheatjet->Add(shade_l5_cheatjet,"f");
  sep_curve_comp_cheatjet->Add(shade_s5_cheatjet,"f");

  //sep_curve_comp_cheatjet->Add(sep_curve_genlevel,"l");
  sep_curve_comp_cheatjet->Add(sep_curve_l5_cheatjet,"l");
  sep_curve_comp_cheatjet->Add(sep_curve_l5_cheatjet_perfectbtag,"l");
  sep_curve_comp_cheatjet->Add(sep_curve_s5_cheatjet,"l");
  sep_curve_comp_cheatjet->Add(sep_curve_s5_cheatjet_perfectbtag,"l");
 
  sep_curve_comp_cheatjet->Draw("a");
  sep_curve_comp_cheatjet->GetXaxis()->SetRangeUser(0,1);
  sep_curve_comp_cheatjet->GetYaxis()->SetRangeUser(0,1);


  unique_ptr<TLegend> leg_sep_curve_comp_cheatjet (new TLegend(0.2, 0.43, 0.65, 0.65));
  TLine *full_line_cheatjet = new TLine(); 
  TLine *dash_line_cheatjet = new TLine(); 
  full_line_cheatjet->SetLineStyle(1); full_line_cheatjet->SetLineWidth(5); full_line_cheatjet->SetLineColor(kBlue); full_line_cheatjet->Draw();
  dash_line_cheatjet->SetLineStyle(2); dash_line_cheatjet->SetLineWidth(5); dash_line_cheatjet->SetLineColor(kRed); dash_line_cheatjet->Draw();
  deletables.push_back(full_line_cheatjet);
  deletables.push_back(dash_line_cheatjet);
  leg_sep_curve_comp_cheatjet->SetHeader("1D cut separation");
  leg_sep_curve_comp_cheatjet->AddEntry(full_line_cheatjet,   "IDR-L", "l");
  leg_sep_curve_comp_cheatjet->AddEntry(dash_line_cheatjet,   "IDR-S", "l");
  leg_sep_curve_comp_cheatjet->Draw();
 
  shared_ptr<TLatex> sep_curve_comp_cheatjet_logo = add_ILD_mark( canvas_sep_curve_comp_cheatjet, 0.06, 0.65, 0.1);
  // shared_ptr<TLatex> sep_curve_comp_cheatjet_prelim = add_prelim_mark( canvas_sep_curve_comp_cheatjet, 0.28, 0.65, 0.07); 

  shared_ptr<TLatex> level_tex_cheatjet (new TLatex(0.06, 0.25,"dark: cheated jet finding"));
  level_tex_cheatjet->SetName( (string(canvas_sep_curve_comp_cheatjet->GetName()) + "_level_tex_cheatjet").c_str() );
  level_tex_cheatjet->SetTextSize(0.04);
  level_tex_cheatjet->SetTextFont(62);
  level_tex_cheatjet->SetLineWidth(2);
  level_tex_cheatjet->Draw();

  shared_ptr<TLatex> level_tex_cheatjet_perfectbtag (new TLatex(0.06, 0.18,"light: + ideal b tag"));
  level_tex_cheatjet_perfectbtag->SetName( (string(canvas_sep_curve_comp_cheatjet->GetName()) + "_level_tex_cheatjet_perfectbtag").c_str() );
  level_tex_cheatjet_perfectbtag->SetTextSize(0.04);
  level_tex_cheatjet_perfectbtag->SetTextFont(62);
  level_tex_cheatjet_perfectbtag->SetLineWidth(2);
  level_tex_cheatjet_perfectbtag->Draw();

  shared_ptr<TLatex> level_tex_cheatjet_area (new TLatex(0.06, 0.11,"area: + realistic b tag"));
  level_tex_cheatjet_area->SetName( (string(canvas_sep_curve_comp_cheatjet->GetName()) + "_level_tex_cheatjet_area").c_str() );
  level_tex_cheatjet_area->SetTextSize(0.04);
  level_tex_cheatjet_area->SetTextFont(62);
  level_tex_cheatjet_area->SetLineWidth(2);
  level_tex_cheatjet_area->Draw();

  string plot_name_sep_curve_comp_cheatjet = "./sep_curve_comp_cheatjet";
  canvas_sep_curve_comp_cheatjet->Print((output_dir + plot_name_sep_curve_comp_cheatjet + ".pdf").c_str());
  canvas_sep_curve_comp_cheatjet->Print((output_dir + plot_name_sep_curve_comp_cheatjet + ".C").c_str());

  // ---------------------------------------------------------------------------
  
  // ---------------------------------------------------------------------------
  
  shared_ptr<TCanvas> canvas_sep_curve_comp_l5 (new TCanvas("canvas_sep_curve_comp_l5", "", 0, 0, 600, 600));

  TMultiGraph *sep_curve_comp_l5 = new TMultiGraph();
  sep_curve_comp_l5->SetTitle("; #font[12]{WW} ID eff.; #font[12]{ZZ} ID eff.");

  sep_curve_l5_reco->SetLineColor(kBlue);
  sep_curve_l5_cheatcluster->SetLineColor(kBlue-3);
  sep_curve_l5_cheatjet->SetLineColor(kBlue-6);
  sep_curve_l5_cheatjet_perfectbtag->SetLineColor(kBlue-5);

  sep_curve_l5_reco->SetLineStyle(1);
  sep_curve_l5_cheatcluster->SetLineStyle(10);
  sep_curve_l5_cheatjet->SetLineStyle(8);
  sep_curve_l5_cheatjet_perfectbtag->SetLineStyle(6);

  sep_curve_l5_reco->SetLineWidth(4);
  sep_curve_l5_cheatcluster->SetLineWidth(4);
  sep_curve_l5_cheatjet->SetLineWidth(4);
  sep_curve_l5_cheatjet_perfectbtag->SetLineWidth(4);

  sep_curve_comp_l5->Add(sep_curve_l5_reco,"l");
  sep_curve_comp_l5->Add(sep_curve_l5_cheatcluster,"l");
  sep_curve_comp_l5->Add(sep_curve_l5_cheatjet,"l");
  sep_curve_comp_l5->Add(sep_curve_l5_cheatjet_perfectbtag,"l");
 
  sep_curve_comp_l5->Draw("a");
  sep_curve_comp_l5->GetXaxis()->SetRangeUser(0,1);
  sep_curve_comp_l5->GetYaxis()->SetRangeUser(0,1);


  unique_ptr<TLegend> leg_sep_curve_comp_l5 (new TLegend(0.2, 0.2, 0.7, 0.6));
  leg_sep_curve_comp_l5->SetHeader("#splitline{IDR-L}{1D cut separation}");
  leg_sep_curve_comp_l5->AddEntry(sep_curve_l5_reco,                   "full reconstruction", "l");
  leg_sep_curve_comp_l5->AddEntry(sep_curve_l5_cheatcluster,           "ideal clustering", "l");
  leg_sep_curve_comp_l5->AddEntry(sep_curve_l5_cheatjet,               "ideal clust. & pairing", "l");
  leg_sep_curve_comp_l5->AddEntry(sep_curve_l5_cheatjet_perfectbtag,   "+ ideal b tag (for #font[12]{ZZ}-ID)", "l");
  leg_sep_curve_comp_l5->Draw();
  // 
  shared_ptr<TLatex> sep_curve_comp_l5_logo = add_ILD_mark( canvas_sep_curve_comp_l5, 0.06, 0.65, 0.1);
  // shared_ptr<TLatex> sep_curve_comp_l5_prelim = add_prelim_mark( canvas_sep_curve_comp_l5, 0.28, 0.65, 0.07); 
  
  string plot_name_sep_curve_comp_l5 = "./sep_curve_comp_l5";
  canvas_sep_curve_comp_l5->Print((output_dir + plot_name_sep_curve_comp_l5 + ".pdf").c_str());
  canvas_sep_curve_comp_l5->Print((output_dir + plot_name_sep_curve_comp_l5 + ".C").c_str());

  // ---------------------------------------------------------------------------
  
  // ---------------------------------------------------------------------------
  
  shared_ptr<TCanvas> canvas_sep_curve_comp_l5_IDR (new TCanvas("canvas_sep_curve_comp_l5_IDR", "", 0, 0, 600, 600));

  TMultiGraph *sep_curve_comp_l5_IDR = new TMultiGraph();
  sep_curve_comp_l5_IDR->SetTitle("; #font[12]{WW} ID eff.; #font[12]{ZZ} ID eff.");

  sep_curve_l5_reco->SetLineColor(kBlue);
  sep_curve_l5_cheatoverlay->SetLineColor(kBlue-3);
  sep_curve_l5_cheatcluster->SetLineColor(kBlue-6);
  sep_curve_l5_cheatjet->SetLineColor(kBlue-5);
  sep_curve_l5_cheatjet_noSLDs->SetLineColor(kBlue-1);

  sep_curve_l5_reco->SetLineStyle(1);
  sep_curve_l5_cheatoverlay->SetLineStyle(10);
  sep_curve_l5_cheatcluster->SetLineStyle(4);
  sep_curve_l5_cheatjet->SetLineStyle(8);
  sep_curve_l5_cheatjet_noSLDs->SetLineStyle(6);

  sep_curve_l5_reco->SetLineWidth(4);
  sep_curve_l5_cheatoverlay->SetLineWidth(4);
  sep_curve_l5_cheatcluster->SetLineWidth(4);
  sep_curve_l5_cheatjet->SetLineWidth(4);
  sep_curve_l5_cheatjet_noSLDs->SetLineWidth(4);

  sep_curve_comp_l5_IDR->Add(sep_curve_l5_reco,"l");
  sep_curve_comp_l5_IDR->Add(sep_curve_l5_cheatoverlay,"l");
  sep_curve_comp_l5_IDR->Add(sep_curve_l5_cheatcluster,"l");
  sep_curve_comp_l5_IDR->Add(sep_curve_l5_cheatjet,"l");
  sep_curve_comp_l5_IDR->Add(sep_curve_l5_cheatjet_noSLDs,"l");
 
  sep_curve_comp_l5_IDR->Draw("a");
  sep_curve_comp_l5_IDR->GetXaxis()->SetRangeUser(0,1);
  sep_curve_comp_l5_IDR->GetYaxis()->SetRangeUser(0,1);


  unique_ptr<TLegend> leg_sep_curve_comp_l5_IDR (new TLegend(0.2, 0.23, 0.62, 0.65));
  leg_sep_curve_comp_l5_IDR->SetHeader("#splitline{IDR-L}{1D cut separation}");
  leg_sep_curve_comp_l5_IDR->AddEntry(sep_curve_l5_reco,            "full reconstruction", "l");
  leg_sep_curve_comp_l5_IDR->AddEntry(sep_curve_l5_cheatoverlay,    "cheated overlay", "l");
  leg_sep_curve_comp_l5_IDR->AddEntry(sep_curve_l5_cheatcluster,    "cheated jets", "l");
  leg_sep_curve_comp_l5_IDR->AddEntry(sep_curve_l5_cheatjet,        "cheated bosons", "l");
  leg_sep_curve_comp_l5_IDR->AddEntry(sep_curve_l5_cheatjet_noSLDs, "#splitline{cheated bosons,}{no semi-lep. decays}", "l");
  leg_sep_curve_comp_l5_IDR->Draw();
  // 
  shared_ptr<TLatex> sep_curve_comp_l5_IDR_logo = add_ILD_mark( canvas_sep_curve_comp_l5_IDR, 0.06, 0.65, 0.1);
  // shared_ptr<TLatex> sep_curve_comp_l5_IDR_prelim = add_prelim_mark( canvas_sep_curve_comp_l5_IDR, 0.28, 0.65, 0.07); 
  
  string plot_name_sep_curve_comp_l5_IDR = "./sep_curve_comp_l5_IDR";
  canvas_sep_curve_comp_l5_IDR->Print((output_dir + plot_name_sep_curve_comp_l5_IDR + ".pdf").c_str());
  canvas_sep_curve_comp_l5_IDR->Print((output_dir + plot_name_sep_curve_comp_l5_IDR + ".C").c_str());

  // ---------------------------------------------------------------------------
  
  // ---------------------------------------------------------------------------
  
  shared_ptr<TCanvas> canvas_sep_curve_comp_s5_IDR (new TCanvas("canvas_sep_curve_comp_s5_IDR", "", 0, 0, 600, 600));

  TMultiGraph *sep_curve_comp_s5_IDR = new TMultiGraph();
  sep_curve_comp_s5_IDR->SetTitle("; #font[12]{WW} ID eff.; #font[12]{ZZ} ID eff.");

  sep_curve_s5_reco->SetLineColor(kRed);
  sep_curve_s5_cheatoverlay->SetLineColor(kRed-3);
  sep_curve_s5_cheatcluster->SetLineColor(kRed-6);
  sep_curve_s5_cheatjet->SetLineColor(kRed-5);
  sep_curve_s5_cheatjet_noSLDs->SetLineColor(kRed-1);

  sep_curve_s5_reco->SetLineStyle(1);
  sep_curve_s5_cheatoverlay->SetLineStyle(10);
  sep_curve_s5_cheatcluster->SetLineStyle(4);
  sep_curve_s5_cheatjet->SetLineStyle(8);
  sep_curve_s5_cheatjet_noSLDs->SetLineStyle(6);

  sep_curve_s5_reco->SetLineWidth(4);
  sep_curve_s5_cheatoverlay->SetLineWidth(4);
  sep_curve_s5_cheatcluster->SetLineWidth(4);
  sep_curve_s5_cheatjet->SetLineWidth(4);
  sep_curve_s5_cheatjet_noSLDs->SetLineWidth(4);

  sep_curve_comp_s5_IDR->Add(sep_curve_s5_reco,"l");
  sep_curve_comp_s5_IDR->Add(sep_curve_s5_cheatoverlay,"l");
  sep_curve_comp_s5_IDR->Add(sep_curve_s5_cheatcluster,"l");
  sep_curve_comp_s5_IDR->Add(sep_curve_s5_cheatjet,"l");
  sep_curve_comp_s5_IDR->Add(sep_curve_s5_cheatjet_noSLDs,"l");
 
  sep_curve_comp_s5_IDR->Draw("a");
  sep_curve_comp_s5_IDR->GetXaxis()->SetRangeUser(0,1);
  sep_curve_comp_s5_IDR->GetYaxis()->SetRangeUser(0,1);


  unique_ptr<TLegend> leg_sep_curve_comp_s5_IDR (new TLegend(0.2, 0.23, 0.62, 0.65));
  leg_sep_curve_comp_s5_IDR->SetHeader("#splitline{IDR-S}{1D cut separation}");
  leg_sep_curve_comp_s5_IDR->AddEntry(sep_curve_s5_reco,            "full reconstruction", "l");
  leg_sep_curve_comp_s5_IDR->AddEntry(sep_curve_s5_cheatoverlay,    "cheated overlay", "l");
  leg_sep_curve_comp_s5_IDR->AddEntry(sep_curve_s5_cheatcluster,    "cheated jets", "l");
  leg_sep_curve_comp_s5_IDR->AddEntry(sep_curve_s5_cheatjet,        "cheated bosons", "l");
  leg_sep_curve_comp_s5_IDR->AddEntry(sep_curve_s5_cheatjet_noSLDs, "#splitline{cheated bosons,}{no semi-lep. decays}", "l");
  leg_sep_curve_comp_s5_IDR->Draw();
  // 
  shared_ptr<TLatex> sep_curve_comp_s5_IDR_logo = add_ILD_mark( canvas_sep_curve_comp_s5_IDR, 0.06, 0.65, 0.1);
  // shared_ptr<TLatex> sep_curve_comp_s5_IDR_prelim = add_prelim_mark( canvas_sep_curve_comp_s5_IDR, 0.28, 0.65, 0.07); 
  
  string plot_name_sep_curve_comp_s5_IDR = "./sep_curve_comp_s5_IDR";
  canvas_sep_curve_comp_s5_IDR->Print((output_dir + plot_name_sep_curve_comp_s5_IDR + ".pdf").c_str());
  canvas_sep_curve_comp_s5_IDR->Print((output_dir + plot_name_sep_curve_comp_s5_IDR + ".C").c_str());

  // ---------------------------------------------------------------------------
  
  // ---------------------------------------------------------------------------
  
  shared_ptr<TCanvas> canvas_sep_curve_comp_ls_IDR (new TCanvas("canvas_sep_curve_comp_ls_IDR", "", 0, 0, 600, 600));

  TMultiGraph *sep_curve_comp_ls_IDR = new TMultiGraph();
  sep_curve_comp_ls_IDR->SetTitle("; #font[12]{WW} ID eff.; #font[12]{ZZ} ID eff.");

  sep_curve_l5_reco->SetLineColor(kBlue);
  sep_curve_l5_cheatjet_noSLDs->SetLineColor(kBlue-6);
  sep_curve_s5_reco->SetLineColor(kRed);
  sep_curve_s5_cheatjet_noSLDs->SetLineColor(kRed-6);
  
  sep_curve_l5_reco->SetLineStyle(1);
  sep_curve_l5_cheatjet_noSLDs->SetLineStyle(1);
  sep_curve_s5_reco->SetLineStyle(2);
  sep_curve_s5_cheatjet_noSLDs->SetLineStyle(2);

  // sep_curve_s5_reco->SetLineStyle(1);
  // sep_curve_s5_cheatoverlay->SetLineStyle(10);
  // sep_curve_s5_cheatcluster->SetLineStyle(4);
  // sep_curve_s5_cheatjet->SetLineStyle(8);
  // sep_curve_s5_cheatjet_noSLDs->SetLineStyle(6);
  // 
  // sep_curve_s5_reco->SetLineWidth(4);
  // sep_curve_s5_cheatoverlay->SetLineWidth(4);
  // sep_curve_s5_cheatcluster->SetLineWidth(4);
  // sep_curve_s5_cheatjet->SetLineWidth(4);
  // sep_curve_s5_cheatjet_noSLDs->SetLineWidth(4);

  sep_curve_comp_ls_IDR->Add(sep_curve_l5_reco,"l");
  sep_curve_comp_ls_IDR->Add(sep_curve_l5_cheatjet_noSLDs,"l");
  sep_curve_comp_ls_IDR->Add(sep_curve_s5_reco,"l");
  sep_curve_comp_ls_IDR->Add(sep_curve_s5_cheatjet_noSLDs,"l");
  
  sep_curve_comp_ls_IDR->Draw("a");
  sep_curve_comp_ls_IDR->GetXaxis()->SetRangeUser(0,1);
  sep_curve_comp_ls_IDR->GetYaxis()->SetRangeUser(0,1);


  unique_ptr<TLegend> leg_sep_curve_comp_ls_IDR (new TLegend(0.2, 0.45, 0.62, 0.65));
  leg_sep_curve_comp_ls_IDR->SetHeader("1D cut separation");
  TLine *full_line_ls_IDR = new TLine(); 
  TLine *dash_line_ls_IDR = new TLine(); 
  full_line_ls_IDR->SetLineStyle(1); full_line_ls_IDR->SetLineWidth(5); full_line_ls_IDR->Draw();
  dash_line_ls_IDR->SetLineStyle(2); dash_line_ls_IDR->SetLineWidth(5); dash_line_ls_IDR->Draw();
  deletables.push_back(full_line_ls_IDR);
  deletables.push_back(dash_line_ls_IDR);
  leg_sep_curve_comp_ls_IDR->AddEntry(full_line_ls_IDR,   "IDR-L", "l");
  leg_sep_curve_comp_ls_IDR->AddEntry(dash_line_ls_IDR,   "IDR-S", "l");
  leg_sep_curve_comp_ls_IDR->Draw();

  shared_ptr<TLatex> level_tex_bright (new TLatex(0.06, 0.28,"bright: full reconstruction"));
  level_tex_bright->SetTextSize(0.04); level_tex_bright->SetTextFont(62); level_tex_bright->SetLineWidth(2); level_tex_bright->Draw();
  shared_ptr<TLatex> level_tex_dim (new TLatex(0.06, 0.16,"dim: #splitline{cheated bosons,}{no semi-lep. decays}"));
  level_tex_dim->SetTextSize(0.04); level_tex_dim->SetTextFont(62); level_tex_dim->SetLineWidth(2); level_tex_dim->Draw();

  // 
  shared_ptr<TLatex> sep_curve_comp_ls_IDR_logo = add_ILD_mark( canvas_sep_curve_comp_ls_IDR, 0.06, 0.65, 0.1);
  // shared_ptr<TLatex> sep_curve_comp_ls_IDR_prelim = add_prelim_mark( canvas_sep_curve_comp_ls_IDR, 0.28, 0.65, 0.07); 
  
  string plot_name_sep_curve_comp_ls_IDR = "./sep_curve_comp_ls_IDR";
  canvas_sep_curve_comp_ls_IDR->Print((output_dir + plot_name_sep_curve_comp_ls_IDR + ".pdf").c_str());
  canvas_sep_curve_comp_ls_IDR->Print((output_dir + plot_name_sep_curve_comp_ls_IDR + ".C").c_str());

  // ---------------------------------------------------------------------------
  

  for ( auto & deletable: deletables ) { delete deletable; }
  for ( auto & closable: closables ) { closable->Close(); }
}
