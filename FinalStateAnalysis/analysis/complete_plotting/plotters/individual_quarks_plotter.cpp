#include "../include/plotter.h"

class IndividualJetsPlotter : public Plotter {

	void set_plotter_settings() {
		set_output_folder_name("individual_quarks");	
	}

	void define_plots(){
		add_new_TH1D("jet_E", new TH1D("jet_E", "w/ cuts; E_{jet} [GeV]; Events", 500, 0, 500));
		add_new_TH1D("jet_theta", new TH1D("jet_theta", "w/ cuts; #theta_{jet} [GeV]; Events", 32, 0, 3.2));
		add_new_TH1D("jet_mass", new TH1D("jet_mass", "w/ cuts; m_{jet} [GeV]; Events", 100, 0, 100));
	}

	void fill_plots(){
		float weight = get_current_weight();

		// This is the loop over all events
		while ( get_next_event() ) {

			if ( pass_selection == 0 ) { continue; }

			int number_heavy_quarks = 0;
			for ( int i=0; i<4; i++ ) {
				get_TH1D( "jet_E" )->Fill( jets_E[i] , weight );
				get_TH1D( "jet_theta" )->Fill( jets_theta[i] , weight );
				get_TH1D( "jet_mass" )->Fill( jets_mass[i] , weight );
			}  
		
		}
	}

	void draw_plots(){

		for (int i=0; i<get_number_TH1Ds(); i++) {

			TH1D* h1 = get_TH1D_i(i);

			string h1_name = h1->GetName();
			TCanvas *canvas_h1 = new TCanvas( ("can_h1_" + h1_name).c_str(), "", 0, 0, 600, 600);

			h1->Draw("hist");

			string plot_name = get_output_directory() + "/" + h1_name + ".pdf";
			canvas_h1->Print(plot_name.c_str());
			canvas_h1->Close();
		}

	};
};
