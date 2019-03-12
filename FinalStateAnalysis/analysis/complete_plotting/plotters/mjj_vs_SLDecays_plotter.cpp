#include "../include/plotter.h"

class MjjVsSLDecaysPlotter : public Plotter {
	/* Plotter for 1D and 2D plots of the dijet/dijet/boson masses
 	-> Plots mjj on different levels of reconstruction process
 	*/

	void set_plotter_settings() {
		set_output_folder_name("mjj_vs_SLDecays");	
	}

	void define_plots(){
		add_new_TH1D("m_WW_noBSLD", new TH1D("m_WW_noBSLD", "w/ cuts, no B-Had SLDecay; (m_{jj,1} + m_{jj,2})/2 [GeV]; Events", 70, 50, 120));
		add_new_TH1D("m_ZZ_noBSLD", new TH1D("m_ZZ_noBSLD", "w/ cuts, no B-Had SLDecay; (m_{jj,1} + m_{jj,2})/2 [GeV]; Events", 70, 50, 120));
		add_new_TH2D("m_m_WW_noBSLD", new TH2D("m_m_WW_noBSLD", "w/ cuts, no B-Had SLDecay; m_{jj,1} [GeV];  m_{jj,2} [GeV]", 35, 50, 120, 35, 50, 120));
		add_new_TH2D("m_m_ZZ_noBSLD", new TH2D("m_m_ZZ_noBSLD", "w/ cuts, no B-Had SLDecay; m_{jj,1} [GeV];  m_{jj,2} [GeV]", 35, 50, 120, 35, 50, 120));
		add_new_TH1D("m_WW_noCSLD", new TH1D("m_WW_noCSLD", "w/ cuts, no C-Had SLDecay; (m_{jj,1} + m_{jj,2})/2 [GeV]; Events", 70, 50, 120));
		add_new_TH1D("m_ZZ_noCSLD", new TH1D("m_ZZ_noCSLD", "w/ cuts, no C-Had SLDecay; (m_{jj,1} + m_{jj,2})/2 [GeV]; Events", 70, 50, 120));
		add_new_TH2D("m_m_WW_noCSLD", new TH2D("m_m_WW_noCSLD", "w/ cuts, no C-Had SLDecay; m_{jj,1} [GeV];  m_{jj,2} [GeV]", 35, 50, 120, 35, 50, 120));
		add_new_TH2D("m_m_ZZ_noCSLD", new TH2D("m_m_ZZ_noCSLD", "w/ cuts, no C-Had SLDecay; m_{jj,1} [GeV];  m_{jj,2} [GeV]", 35, 50, 120, 35, 50, 120));
		add_new_TH1D("m_WW_noSLD", new TH1D("m_WW_noSLD", "w/ cuts, no C/B-Had SLDecay; (m_{jj,1} + m_{jj,2})/2 [GeV]; Events", 70, 50, 120));
		add_new_TH1D("m_ZZ_noSLD", new TH1D("m_ZZ_noSLD", "w/ cuts, no C/B-Had SLDecay; (m_{jj,1} + m_{jj,2})/2 [GeV]; Events", 70, 50, 120));
		add_new_TH2D("m_m_WW_noSLD", new TH2D("m_m_WW_noSLD", "w/ cuts, no C/B-Had SLDecay; m_{jj,1} [GeV];  m_{jj,2} [GeV]", 35, 50, 120, 35, 50, 120));
		add_new_TH2D("m_m_ZZ_noSLD", new TH2D("m_m_ZZ_noSLD", "w/ cuts, no C/B-Had SLDecay; m_{jj,1} [GeV];  m_{jj,2} [GeV]", 35, 50, 120, 35, 50, 120));

		add_new_TH1D("m_WW_icn_noSLD", new TH1D("m_WW_icn_noSLD", "w/ cuts, from icns, no C/B-Had SLDecay; (m_{jj,1} + m_{jj,2})/2 [GeV]; Events", 70, 50, 120));
		add_new_TH1D("m_ZZ_icn_noSLD", new TH1D("m_ZZ_icn_noSLD", "w/ cuts, from icns, no C/B-Had SLDecay; (m_{jj,1} + m_{jj,2})/2 [GeV]; Events", 70, 50, 120));


		add_new_TH1D("m_WW_wBSLD", new TH1D("m_WW_wBSLD", "w/ cuts, any B-Had SLDecays; (m_{jj,1} + m_{jj,2})/2 [GeV]; Events", 70, 50, 120));
		add_new_TH1D("m_ZZ_wBSLD", new TH1D("m_ZZ_wBSLD", "w/ cuts, any B-Had SLDecays; (m_{jj,1} + m_{jj,2})/2 [GeV]; Events", 70, 50, 120));
		add_new_TH2D("m_m_WW_wBSLD", new TH2D("m_m_WW_wBSLD", "w/ cuts, any B-Had SLDecays; m_{jj,1} [GeV];  m_{jj,2} [GeV]", 35, 50, 120, 35, 50, 120));
		add_new_TH2D("m_m_ZZ_wBSLD", new TH2D("m_m_ZZ_wBSLD", "w/ cuts, any B-Had SLDecays; m_{jj,1} [GeV];  m_{jj,2} [GeV]", 35, 50, 120, 35, 50, 120));
		add_new_TH1D("m_WW_wCSLD", new TH1D("m_WW_wCSLD", "w/ cuts, any C-Had SLDecays; (m_{jj,1} + m_{jj,2})/2 [GeV]; Events", 70, 50, 120));
		add_new_TH1D("m_ZZ_wCSLD", new TH1D("m_ZZ_wCSLD", "w/ cuts, any C-Had SLDecays; (m_{jj,1} + m_{jj,2})/2 [GeV]; Events", 70, 50, 120));
		add_new_TH2D("m_m_WW_wCSLD", new TH2D("m_m_WW_wCSLD", "w/ cuts, any C-Had SLDecays; m_{jj,1} [GeV];  m_{jj,2} [GeV]", 35, 50, 120, 35, 50, 120));
		add_new_TH2D("m_m_ZZ_wCSLD", new TH2D("m_m_ZZ_wCSLD", "w/ cuts, any C-Had SLDecays; m_{jj,1} [GeV];  m_{jj,2} [GeV]", 35, 50, 120, 35, 50, 120));
		add_new_TH1D("m_WW_wSLD", new TH1D("m_WW_wSLD", "w/ cuts, any C/B-Had SLDecays; (m_{jj,1} + m_{jj,2})/2 [GeV]; Events", 70, 50, 120));
		add_new_TH1D("m_ZZ_wSLD", new TH1D("m_ZZ_wSLD", "w/ cuts, any C/B-Had SLDecays; (m_{jj,1} + m_{jj,2})/2 [GeV]; Events", 70, 50, 120));
		add_new_TH2D("m_m_WW_wSLD", new TH2D("m_m_WW_wSLD", "w/ cuts, any C/B-Had SLDecays; m_{jj,1} [GeV];  m_{jj,2} [GeV]", 35, 50, 120, 35, 50, 120));
		add_new_TH2D("m_m_ZZ_wSLD", new TH2D("m_m_ZZ_wSLD", "w/ cuts, any C/B-Had SLDecays; m_{jj,1} [GeV];  m_{jj,2} [GeV]", 35, 50, 120, 35, 50, 120));

	}

	void fill_plots(){
		float weight = get_current_weight();

		// This is the loop over all events
		while ( get_next_event() ) {
			if ( true_evt_type == 1 ) {
				//if ( pass_selection == 1 ) {
					if (n_BSLD == 0) {
						get_TH1D("m_WW_noBSLD")->Fill((pair1_mass+pair2_mass)/2., weight);			
						get_TH2D("m_m_WW_noBSLD")->Fill(pair1_mass, pair2_mass, weight);			
					} else {
						get_TH1D("m_WW_wBSLD")->Fill((pair1_mass+pair2_mass)/2., weight);			
						get_TH2D("m_m_WW_wBSLD")->Fill(pair1_mass, pair2_mass, weight);			
					}

					if (n_CSLD == 0) {
						get_TH1D("m_WW_noCSLD")->Fill((pair1_mass+pair2_mass)/2., weight);			
						get_TH2D("m_m_WW_noCSLD")->Fill(pair1_mass, pair2_mass, weight);			
					} else {
						get_TH1D("m_WW_wCSLD")->Fill((pair1_mass+pair2_mass)/2., weight);			
						get_TH2D("m_m_WW_wCSLD")->Fill(pair1_mass, pair2_mass, weight);			
					}

					if (n_BSLD == 0 && n_CSLD == 0) {
						get_TH1D("m_WW_noSLD")->Fill((pair1_mass+pair2_mass)/2., weight);			
						get_TH2D("m_m_WW_noSLD")->Fill(pair1_mass, pair2_mass, weight);			
						get_TH1D("m_WW_icn_noSLD")->Fill((TJ_pair1_mass_from_icn+TJ_pair2_mass_from_icn)/2., weight);			
					} else {
						get_TH1D("m_WW_wSLD")->Fill((pair1_mass+pair2_mass)/2., weight);			
						get_TH2D("m_m_WW_wSLD")->Fill(pair1_mass, pair2_mass, weight);			
					}
			//	}
			}
			else if ( true_evt_type == 2 ) {
			//	if ( pass_selection == 1 ) {
					if (n_BSLD == 0) {
						get_TH1D("m_ZZ_noBSLD")->Fill((pair1_mass+pair2_mass)/2., weight);			
						get_TH2D("m_m_ZZ_noBSLD")->Fill(pair1_mass, pair2_mass, weight);			
					} else {
						get_TH1D("m_ZZ_wBSLD")->Fill((pair1_mass+pair2_mass)/2., weight);			
						get_TH2D("m_m_ZZ_wBSLD")->Fill(pair1_mass, pair2_mass, weight);			
					}

					if (n_CSLD == 0) {
						get_TH1D("m_ZZ_noCSLD")->Fill((pair1_mass+pair2_mass)/2., weight);			
						get_TH2D("m_m_ZZ_noCSLD")->Fill(pair1_mass, pair2_mass, weight);			
					} else {
						get_TH1D("m_ZZ_wCSLD")->Fill((pair1_mass+pair2_mass)/2., weight);			
						get_TH2D("m_m_ZZ_wCSLD")->Fill(pair1_mass, pair2_mass, weight);			
					}

					if (n_BSLD == 0 && n_CSLD == 0) {
						get_TH1D("m_ZZ_noSLD")->Fill((pair1_mass+pair2_mass)/2., weight);			
						get_TH2D("m_m_ZZ_noSLD")->Fill(pair1_mass, pair2_mass, weight);			
						get_TH1D("m_ZZ_icn_noSLD")->Fill((TJ_pair1_mass_from_icn+TJ_pair2_mass_from_icn)/2., weight);			
					} else {
						get_TH1D("m_ZZ_wSLD")->Fill((pair1_mass+pair2_mass)/2., weight);			
						get_TH2D("m_m_ZZ_wSLD")->Fill(pair1_mass, pair2_mass, weight);			
					}
			//	}
			}
		}
	}

	void draw_plots(){

//		vector<string> level_names = {"_genlevel", "_nocuts", ""};
//
//		for (int i=0; i<level_names.size(); i++) {
//			TH1D* h1_WW = get_TH1D("m_WW" + level_names[i]);	
//			TH1D* h1_ZZ = get_TH1D("m_ZZ" + level_names[i]);	
//			h1_WW->SetLineColor(4);	
//			h1_ZZ->SetLineColor(2);	
//
//
//
//			TCanvas *canvas_h1 = new TCanvas("can_h1", "", 0, 0, 600, 600);
//			h1_WW->Draw("hist");
//			h1_ZZ->Draw("hist same");
//			
//			draw_mass_line("W", "DBD", "y");
//			draw_mass_line("Z", "DBD", "y");
//
//			canvas_h1->SetLeftMargin(0.19);
//			canvas_h1->SetFrameBorderMode(0);
//  			h1_WW->GetYaxis()->SetTitleOffset(1.38);
//			gPad->Modified();
//			gPad->Update();
//
//			string plot_name_h1 = get_output_directory() + "/m" + level_names[i] + ".pdf";
//			canvas_h1->Print(plot_name_h1.c_str());
//			canvas_h1->Close();
//			delete canvas_h1;
//
//			TH2D* h2_WW = get_TH2D("m_m_WW" + level_names[i]);	
//			TH2D* h2_ZZ = get_TH2D("m_m_ZZ" + level_names[i]);
//			h2_WW->Scale(1./ h2_WW->Integral());
//			h2_ZZ->Scale(1./ h2_ZZ->Integral());
//
//			h2_WW->SetLineColor(4);	
//			h2_ZZ->SetLineColor(2);	
//
//			TCanvas *canvas_h2 = new TCanvas("can_h2", "", 0, 0, 800, 800);
//			h2_WW->Draw("box");
//			h2_ZZ->Draw("box same");
//
//			draw_mass_line("W", "DBD", "x");
//			draw_mass_line("Z", "DBD", "x");
//			draw_mass_line("W", "DBD", "y");
//			draw_mass_line("Z", "DBD", "y");
//
//			canvas_h2->SetLeftMargin(0.19);
//			canvas_h2->SetFrameBorderMode(0);
//  			h2_WW->GetYaxis()->SetTitleOffset(1.38);
//			gPad->Modified();
//			gPad->Update();
//
//			string plot_name_h2 = get_output_directory() + "/m_m" + level_names[i] + ".pdf";
//			canvas_h2->Print(plot_name_h2.c_str());
//			canvas_h2->Close();
//			delete canvas_h2;
//		}
	};
};
