#include "../include/plotter.h"

// Very simple example

class TrueISRvsMjjPlotter : public Plotter {

	void set_plotter_settings() {
		set_output_folder_name("mjj_vs_ISR");	
	}


	void define_plots(){
		add_new_TH2D("m_isrE", new TH2D("m_isrE", "w/ cuts; m_{jj} [GeV];  E_{ISR in Detector}", 45, 30, 120, 80, 0, 80) );
		add_new_TH2D("m_isrP", new TH2D("m_isrP", "w/ cuts; m_{jj} [GeV];  P_{ISR in Detector}", 45, 30, 120, 80, 0, 80) );
		add_new_TH2D("m_isrTheta", new TH2D("m_isrTheta", "w/ cuts; m_{jj} [GeV];  #Theta_{ISR in Detector}", 45, 30, 120, 32, 0, 3.2) );

	}

	void fill_plots(){
		float weight = get_current_weight();

		// This is the loop over all events
		while ( get_next_event() ) {
			if ( true_evt_type == 0 ) {continue;} // Here only interested in WW/ZZ as defined on generator level
			if ( pass_selection != 1 ) {continue;}
			
			//if ( ( (pair1_mass < 65) || (pair1_mass > 105) ) ) { 
			get_TH2D("m_isrE")->Fill(pair1_mass, true_ISR_inJets_E, weight);			
			if ( true_ISR_inJets_E > 0 ) {
				get_TH2D("m_isrP")->Fill(pair1_mass, true_ISR_inJets_p, weight);			
				get_TH2D("m_isrTheta")->Fill(pair1_mass, true_ISR_inJets_theta, weight);			
			}
			//}
			//if ( ( (pair2_mass < 65) || (pair2_mass > 105) ) ) { 
			get_TH2D("m_isrE")->Fill(pair2_mass, true_ISR_inJets_E, weight);			
			if ( true_ISR_inJets_E > 0 ) {
				get_TH2D("m_isrP")->Fill(pair2_mass, true_ISR_inJets_p, weight);			
				get_TH2D("m_isrTheta")->Fill(pair2_mass, true_ISR_inJets_theta, weight);			
			}
			//}
		}
	}

	void draw_plots(){
		vector<string> observable_names = {"E", "P", "Theta"};
		vector<string> h2_plot_name = {"mjj_isrE", "mjj_isrP", "mjj_isrTheta"};	
	
		for (int l=0; l<observable_names.size(); l++) {
			TCanvas *canvas_h2 = new TCanvas("can_h2", "", 0, 0, 800, 800);
			get_TH2D("m_isr" + observable_names[l])->Draw("colz");
			canvas_h2->SetRightMargin(0.22); // Adjust right margin so that color scheme is shown fully
			string plot_name_h2 = get_output_directory() + "/" + h2_plot_name[l] + ".pdf";
			canvas_h2->Print(plot_name_h2.c_str());
			canvas_h2->Close(); // Avoid memory leaks
		}
	}
};
