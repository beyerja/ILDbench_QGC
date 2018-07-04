#include "../include/plotter.h"

// Very simple example

class ThetaMPlotter : public Plotter {

	void set_plotter_settings() {
		set_output_folder_name("theta_m_plots");	
	}

	void define_plots(){
		add_new_TH2D("m_theta_genlevel", new TH2D("m_theta_genlevel", "generator level ; m_{jj} [GeV];  #theta_{V}", 60, 30, 150, 32, 0, 3.2) );
		add_new_TH2D("m_theta_nocuts", new TH2D("m_theta_nocuts", "w/o cuts ; m_{jj} [GeV];  #theta_{V}", 60, 30, 150, 32, 0, 3.2) );
		add_new_TH2D("m_theta", new TH2D("m_theta", "w/ cuts; m_{jj} [GeV];  #theta_{V}", 60, 30, 150, 32, 0, 3.2) );
		add_new_TH2D("m_reco_theta_gen", new TH2D("m_reco_theta_gen", "w/ cuts; m_{jj,reco} [GeV];  #theta_{V,gen}", 60, 30, 150, 32, 0, 3.2) );
	
		add_new_TH1D("theta_genlevel", new TH1D("theta_genlevel", "generator level (m not in [65,105]GeV); #theta_{V}; Events", 32, 0, 3.2) );
		add_new_TH1D("theta_nocuts", new TH1D("theta_nocuts", "w/o cuts (m not in [65,105]GeV) ; #theta_{V}; Events", 32, 0, 3.2) );
		add_new_TH1D("theta", new TH1D("theta", "w/ cuts (m not in [65,105]GeV); #theta_{V}; Events", 32, 0, 3.2) );
		add_new_TH1D("theta_gen_mrecocut", new TH1D("theta_gen_mrecocut", "w/ cuts (m_{reco} not in [65,105]GeV); #theta_{V,gen}; Events", 32, 0, 3.2) );

	}

	void fill_plots(){
		float weight = get_current_weight();

		// This is the loop over all events
		while ( get_next_event() ) {

			if (true_evt_type == 0) {continue;} // Here only interested in WW/ZZ as defined on generator level
			if ( ( (pair1_mass < 65) || (pair1_mass > 105) ) ) { // && ( p1 > 100) ) {
				get_TH1D("theta_genlevel")->Fill(true_pair1_theta, weight);			
				get_TH1D("theta_nocuts")->Fill(pair1_theta, weight);			
				get_TH2D("m_theta_genlevel")->Fill(true_pair1_mass, true_pair1_theta, weight);			
				get_TH2D("m_theta_nocuts")->Fill(pair1_mass, pair1_theta, weight);			
				if ( pass_selection == 1 ) {
					get_TH1D("theta")->Fill(pair1_theta, weight);			
					get_TH2D("m_theta")->Fill(pair1_mass, pair1_theta, weight);			
					get_TH1D("theta_gen_mrecocut")->Fill(true_pair1_theta, weight);
					get_TH2D("m_reco_theta_gen")->Fill(pair1_mass, true_pair1_theta, weight);
				}
			}

			if ( ( (pair2_mass < 65) || (pair2_mass > 105) ) ) { // && ( p2 > 100) ) {
				get_TH1D("theta_genlevel")->Fill(true_pair2_theta, weight);			
				get_TH1D("theta_nocuts")->Fill(pair2_theta, weight);			
				get_TH2D("m_theta_genlevel")->Fill(true_pair2_mass, true_pair2_theta, weight);			
				get_TH2D("m_theta_nocuts")->Fill(pair2_mass, pair2_theta, weight);			
				if ( pass_selection == 1 ) {
					get_TH1D("theta")->Fill(pair2_theta, weight);			
					get_TH2D("m_theta")->Fill(pair2_mass, pair2_theta, weight);			
					get_TH1D("theta_gen_mrecocut")->Fill(true_pair2_theta, weight);
					get_TH2D("m_reco_theta_gen")->Fill(pair2_mass, true_pair2_theta, weight);
				}
			}


		}
	}

	void draw_plots(){
		vector<string> level_names = {"_genlevel", "_nocuts", "", "_theta_gen_m_reco"};
		vector<string> h2_plot_name = {"m_theta_genlevel", "m_theta_nocuts", "m_theta", "m_reco_theta_gen"};	
		vector<string> h1_plot_name = {"theta_genlevel", "theta_nocuts", "theta", "theta_gen_mrecocut"};
	
		for (int l=0; l<level_names.size(); l++) {
			TCanvas *canvas_h2 = new TCanvas("can_h2", "", 0, 0, 800, 800);
			get_TH2D(h2_plot_name[l].c_str())->Draw("colz");
			canvas_h2->SetRightMargin(0.22); // Adjust right margin so that color scheme is shown fully
			string plot_name_h2 = get_output_directory() + "/" + h2_plot_name[l] + ".pdf";
			canvas_h2->Print(plot_name_h2.c_str());
			canvas_h2->Close(); // Avoid memory leaks
	
			TCanvas *canvas_h1 = new TCanvas("can_h1", "", 0, 0, 800, 800);
			get_TH1D(h1_plot_name[l].c_str())->Draw("hist e");
			string plot_name_h1 = get_output_directory() + "/" + h1_plot_name[l] + ".pdf";
			canvas_h1->Print(plot_name_h1.c_str());
			canvas_h1->Close();
		}
	};
};
