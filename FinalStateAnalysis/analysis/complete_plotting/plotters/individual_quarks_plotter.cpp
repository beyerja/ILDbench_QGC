#include "../include/plotter.h"

class IndividualJetsPlotter : public Plotter {

	void set_plotter_settings() {
		set_output_folder_name("individual_quarks");	
	}

	void define_plots(){
		add_new_TH1D("jet_E", new TH1D("jet_E", "w/ cuts; E_{jet} [GeV]; Events", 500, 0, 500));
		add_new_TH1D("jet_theta", new TH1D("jet_theta", "w/ cuts; #theta_{jet}; Events", 32, 0, 3.2));
		add_new_TH1D("jet_mass", new TH1D("jet_mass", "w/ cuts; m_{jet} [GeV]; Events", 100, 0, 100));

		add_new_TH1D("q_E", new TH1D("q_E", "w/ cuts; E_{q} [GeV]; Events", 500, 0, 500));
		add_new_TH1D("q_theta", new TH1D("q_theta", "w/ cuts; #theta_{q}; Events", 32, 0, 3.2));
		add_new_TH1D("q_E_nocuts", new TH1D("q_E_nocuts", "; E_{q} [GeV]; Events", 500, 0, 500));
		add_new_TH1D("q_theta_nocuts", new TH1D("q_theta_nocuts", "; #theta_{q}; Events", 32, 0, 3.2));

	}

	void fill_plots(){
		float weight = get_current_weight()/4.;

		// This is the loop over all events
		while ( get_next_event() ) {

			for ( int i=0; i<4; i++ ) {
				get_TH1D( "q_E_nocuts" )->Fill( true_quarks_E[i] , weight );
				get_TH1D( "q_theta_nocuts" )->Fill( true_quarks_theta[i] , weight );
			}

			if ( pass_selection == 0 ) { continue; }

			for ( int i=0; i<4; i++ ) {
				get_TH1D( "jet_E" )->Fill( jets_E[i] , weight );
				get_TH1D( "jet_theta" )->Fill( jets_theta[i] , weight );
				get_TH1D( "jet_mass" )->Fill( jets_mass[i] , weight );

				get_TH1D( "q_E" )->Fill( true_quarks_E[i] , weight );
				get_TH1D( "q_theta" )->Fill( true_quarks_theta[i] , weight );
			
			}  
		
		}
	}

	void draw_plots(){

		for (int i=0; i<get_number_TH1Ds(); i++) {

			TH1D* h1 = get_TH1D_i(i);

			string h1_name = h1->GetName();
			TCanvas *canvas_h1 = new TCanvas( ("can_h1_" + h1_name).c_str(), "", 0, 0, 600, 600);

			h1->Draw("hist");
			h1->GetYaxis()->SetTitleOffset(1.55);
		    canvas_h1->SetLeftMargin(0.2);

			string plot_name = get_output_directory() + "/" + h1_name + ".pdf";
			canvas_h1->Print(plot_name.c_str());
			canvas_h1->Close();
		}

	};
};
