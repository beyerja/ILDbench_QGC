#include "../../include/plotter.h"

class TJJetEnergyResolutionPlotter : public Plotter {

	void set_plotter_settings() {
		set_output_folder_name("TJ/TJ_JER");
		set_1D_array_reading( true );
	}

	void define_plots(){
		vector<string> particle_names = {"light_quarks", "charm", "bottom", "photon", "gluon", "lepton"};


		for (int i=0; i<particle_names.size(); i++) {
			string particle_name = particle_names[i];

			add_new_TProfile(("profile_s_t_E_" + particle_name).c_str(),
					new TProfile(("profile_detected_vs_true_E_" + particle_name).c_str(), "Jet Energy Resolution w/ TJ jets at #sqrt{s}=1TeV; E_{j}^{true} [GeV]; (RMS(E_{j})/Mean(E_{j}))_{reco} [%]", 25, 0, 500));

		}
	}

	void fill_plots(){
		float weight = get_current_weight();

		// TODO For all samles: collect all masses of FEs -> draw lines in TH1D

		// This is the loop over all events
		while ( get_next_event() ) {

			//if ( pass_selection == 0 ) { continue; }

			for ( int i=0; i<max_Njets; i++ ) {
				if (TJ_jets_exists[i] == 0) { break; }

				float abs_final_elementon_pdgID = fabs( TJ_jets_final_elementon_pdgID[i] );

				string particle_name;
				if ( (abs_final_elementon_pdgID == 1) ||  (abs_final_elementon_pdgID == 2) || (abs_final_elementon_pdgID == 3) ) {
					particle_name = "light_quarks";
				}
				else if ( abs_final_elementon_pdgID == 4 ) { particle_name = "charm"; }
				else if ( abs_final_elementon_pdgID == 5 ) { particle_name = "bottom"; }
				else if ( abs_final_elementon_pdgID == 22 ) { particle_name = "photon"; }
				else if ( abs_final_elementon_pdgID == 21 ) { particle_name = "gluon"; }
				else if ( (abs_final_elementon_pdgID > 10) && (abs_final_elementon_pdgID < 19) ) {
					particle_name = "lepton";
				}
				else { particle_name = "other"; continue; }

				get_TProfile( ("profile_s_t_E_" + particle_name).c_str() )->Fill( TJ_jets_true_E[i], TJ_jets_detected_E[i], weight);

			}
		}
	}


	void get_resolution_projection ( TProfile* plot, TH1D* error_clone ){
		plot->SetErrorOption("s");
		for (int i=1; i<plot->GetNbinsX()+2; i++) {
			if (plot->GetBinContent(i) > 0){
				error_clone->SetBinContent( i, plot->GetBinError(i)/plot->GetBinContent(i)*100.0 );
			} else {
				error_clone->SetBinContent(i, 0);
			}
			error_clone->SetBinError( i, 0.000001 );
		}

	}

	void draw_plots(){

		vector<string> particle_names = {"light_quarks", "charm", "bottom"};

		vector<string> variables = {"E"};

		vector<array<float, 3>> colors {{27,158,119}, {217,95,2}, {117,112,179}, {231,41,138}};
//{27,158,119}, {217,95,2}, {117,112,179}, {231,41,138}, {102,166,30}, {230,171,2}};
		for (int j=0; j<colors.size(); j++) {
		    TColor *h2_color = new TColor(9100+j, colors[j][0]/256.0, colors[j][1]/256.0, colors[j][2]/256.0);
    	}


		for (int j=0; j<variables.size(); j++) {

			TCanvas *canvas_profile_s_t = new TCanvas(("can_profile_s_t" + variables[j]).c_str(), "", 0, 0, 600, 600);
			TLegend* legend_profile_s_t = new TLegend(0.25, 0.6, 0.45, 0.9);

			for (int i=0; i<particle_names.size(); i++) {
				string particle_name = particle_names[i];


				canvas_profile_s_t->cd();
				TProfile* profile_s_t = get_TProfile(("profile_s_t_" + variables[j] + "_" + particle_name).c_str());

				TH1D* resolution_plot =  (TH1D*)profile_s_t->ProjectionX(("resolution_" + variables[j] + "_" + particle_name).c_str());

				get_resolution_projection(profile_s_t, resolution_plot);

				resolution_plot->SetMinimum(0);
				resolution_plot->SetMaximum(100);
				resolution_plot->SetLineColor(9100+i);
				resolution_plot->SetLineWidth(5);
				resolution_plot->SetMarkerColor(9100+i);
				resolution_plot->SetMarkerSize(0.1);
				legend_profile_s_t->AddEntry(resolution_plot, particle_name.c_str(), "l");
				resolution_plot->Draw("p same");

			}

			legend_profile_s_t->Draw();
			string plot_name_profile_s_t = get_output_directory() + "/JER_detected_vs_true_" +  variables[j] + ".pdf";
			canvas_profile_s_t->Print(plot_name_profile_s_t.c_str());
			canvas_profile_s_t->Close();

		}

	};
};
