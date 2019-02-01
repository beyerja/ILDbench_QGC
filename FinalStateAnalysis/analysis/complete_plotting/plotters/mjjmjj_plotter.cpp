#include "../include/plotter.h"

class MjjMjjPlotter : public Plotter {
	/* Plotter for 1D and 2D plots of the dijet/dijet/boson masses
 	-> Plots mjj on different levels of reconstruction process
 	*/

	void set_plotter_settings() {
		set_output_folder_name("mjjmjj_plots");	
	}

	void define_plots(){
		add_new_TH1D("m_WW_genlevel", new TH1D("m_WW_genlevel", "generator level; (m_{jj,1} + m_{jj,2})/2 [GeV]; Events", 70, 50, 120));
		add_new_TH1D("m_ZZ_genlevel", new TH1D("m_ZZ_genlevel", "generator level; (m_{jj,1} + m_{jj,2})/2 [GeV]; Events", 70, 50, 120));
		add_new_TH2D("m_m_WW_genlevel", new TH2D("m_m_WW_genlevel", "generator level; m_{jj,1} [GeV];  m_{jj,2} [GeV]", 35, 50, 120, 35, 50, 120));
		add_new_TH2D("m_m_ZZ_genlevel", new TH2D("m_m_ZZ_genlevel", "generator level; m_{jj,1} [GeV];  m_{jj,2} [GeV]", 35, 50, 120, 35, 50, 120));
	
		add_new_TH1D("m_WW_nocuts", new TH1D("m_WW_nocuts", "w/o cuts; (m_{jj,1} + m_{jj,2})/2 [GeV]; Events", 70, 50, 120));
		add_new_TH1D("m_ZZ_nocuts", new TH1D("m_ZZ_nocuts", "w/o cuts; (m_{jj,1} + m_{jj,2})/2 [GeV]; Events", 70, 50, 120));
		add_new_TH2D("m_m_WW_nocuts", new TH2D("m_m_WW_nocuts", "w/o cuts; m_{jj,1} [GeV];  m_{jj,2} [GeV]", 35, 50, 120, 35, 50, 120));
		add_new_TH2D("m_m_ZZ_nocuts", new TH2D("m_m_ZZ_nocuts", "w/o cuts; m_{jj,1} [GeV];  m_{jj,2} [GeV]", 35, 50, 120, 35, 50, 120));
	
		add_new_TH1D("m_WW", new TH1D("m_WW", "w/ cuts; (m_{jj,1} + m_{jj,2})/2 [GeV]; Events", 70, 50, 120));
		add_new_TH1D("m_ZZ", new TH1D("m_ZZ", "w/ cuts; (m_{jj,1} + m_{jj,2})/2 [GeV]; Events", 70, 50, 120));
		add_new_TH2D("m_m_WW", new TH2D("m_m_WW", "w/ cuts; m_{jj,1} [GeV];  m_{jj,2} [GeV]", 35, 50, 120, 35, 50, 120));
		add_new_TH2D("m_m_ZZ", new TH2D("m_m_ZZ", "w/ cuts; m_{jj,1} [GeV];  m_{jj,2} [GeV]", 35, 50, 120, 35, 50, 120));
    
    add_new_TH1D("m_WW_genlevel_aftercuts", new TH1D("m_WW_genlevel_aftercuts", "generator level, w/ cuts; (m_{jj,1} + m_{jj,2})/2 [GeV]; Events", 70, 50, 120));
    add_new_TH1D("m_ZZ_genlevel_aftercuts", new TH1D("m_ZZ_genlevel_aftercuts", "generator level, w/ cuts; (m_{jj,1} + m_{jj,2})/2 [GeV]; Events", 70, 50, 120));
    add_new_TH2D("m_m_WW_genlevel_aftercuts", new TH2D("m_m_WW_genlevel_aftercuts", "generator level, w/ cuts; m_{jj,1} [GeV];  m_{jj,2} [GeV]", 35, 50, 120, 35, 50, 120));
    add_new_TH2D("m_m_ZZ_genlevel_aftercuts", new TH2D("m_m_ZZ_genlevel_aftercuts", "generator level, w/ cuts; m_{jj,1} [GeV];  m_{jj,2} [GeV]", 35, 50, 120, 35, 50, 120));
    
	}

	void fill_plots(){
		float weight = get_current_weight();

		// This is the loop over all events
		while ( get_next_event() ) {
			if ( true_evt_type == 1 ) {
				get_TH1D("m_WW_genlevel")->Fill((true_pair1_mass+true_pair2_mass)/2., weight);			
				get_TH2D("m_m_WW_genlevel")->Fill(true_pair1_mass, true_pair2_mass, weight);			
				get_TH1D("m_WW_nocuts")->Fill((pair1_mass+pair2_mass)/2., weight);			
				get_TH2D("m_m_WW_nocuts")->Fill(pair1_mass, pair2_mass, weight);			
				if ( pass_selection == 1 ) {
					get_TH1D("m_WW")->Fill((pair1_mass+pair2_mass)/2., weight);			
					get_TH2D("m_m_WW")->Fill(pair1_mass, pair2_mass, weight);			
          get_TH1D("m_WW_genlevel_aftercuts")->Fill((true_pair1_mass+true_pair2_mass)/2., weight);			
          get_TH2D("m_m_WW_genlevel_aftercuts")->Fill(true_pair1_mass, true_pair2_mass, weight);			
				}
			}
			else if ( true_evt_type == 2 ) {
				get_TH1D("m_ZZ_genlevel")->Fill((true_pair1_mass+true_pair2_mass)/2., weight);			
				get_TH2D("m_m_ZZ_genlevel")->Fill(true_pair1_mass, true_pair2_mass, weight);			
				get_TH1D("m_ZZ_nocuts")->Fill((pair1_mass+pair2_mass)/2., weight);			
				get_TH2D("m_m_ZZ_nocuts")->Fill(pair1_mass, pair2_mass, weight);			
				if ( pass_selection == 1 ) {
					get_TH1D("m_ZZ")->Fill((pair1_mass+pair2_mass)/2., weight);			
					get_TH2D("m_m_ZZ")->Fill(pair1_mass, pair2_mass, weight);			
          get_TH1D("m_ZZ_genlevel_aftercuts")->Fill((true_pair1_mass+true_pair2_mass)/2., weight);			
          get_TH2D("m_m_ZZ_genlevel_aftercuts")->Fill(true_pair1_mass, true_pair2_mass, weight);			
				}
			}
		}
	}

	void draw_plots(){

		vector<string> level_names = {"_genlevel", "_nocuts", ""};

		for (int i=0; i<level_names.size(); i++) {
			TH1D* h1_WW = get_TH1D("m_WW" + level_names[i]);	
			TH1D* h1_ZZ = get_TH1D("m_ZZ" + level_names[i]);	
			h1_WW->SetLineColor(4);	
			h1_ZZ->SetLineColor(2);	



			TCanvas *canvas_h1 = new TCanvas("can_h1", "", 0, 0, 600, 600);
			h1_WW->Draw("hist");
			h1_ZZ->Draw("hist same");
			
			draw_mass_line("W", "DBD", "y");
			draw_mass_line("Z", "DBD", "y");

			canvas_h1->SetLeftMargin(0.19);
			canvas_h1->SetFrameBorderMode(0);
  			h1_WW->GetYaxis()->SetTitleOffset(1.38);
			gPad->Modified();
			gPad->Update();

			string plot_name_h1 = get_output_directory() + "/m" + level_names[i] + ".pdf";
			canvas_h1->Print(plot_name_h1.c_str());
			canvas_h1->Close();
			delete canvas_h1;

			TH2D* h2_WW = get_TH2D("m_m_WW" + level_names[i]);	
			TH2D* h2_ZZ = get_TH2D("m_m_ZZ" + level_names[i]);
			h2_WW->Scale(1./ h2_WW->Integral());
			h2_ZZ->Scale(1./ h2_ZZ->Integral());

			h2_WW->SetLineColor(4);	
			h2_ZZ->SetLineColor(2);	

			TCanvas *canvas_h2 = new TCanvas("can_h2", "", 0, 0, 800, 800);
			h2_WW->Draw("box");
			h2_ZZ->Draw("box same");

			draw_mass_line("W", "DBD", "x");
			draw_mass_line("Z", "DBD", "x");
			draw_mass_line("W", "DBD", "y");
			draw_mass_line("Z", "DBD", "y");

			canvas_h2->SetLeftMargin(0.19);
			canvas_h2->SetFrameBorderMode(0);
  			h2_WW->GetYaxis()->SetTitleOffset(1.38);
			gPad->Modified();
			gPad->Update();

			string plot_name_h2 = get_output_directory() + "/m_m" + level_names[i] + ".pdf";
			canvas_h2->Print(plot_name_h2.c_str());
			canvas_h2->Close();
			delete canvas_h2;
		}
	};
};
