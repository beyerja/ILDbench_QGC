#include "../../include/plotter.h"

class TJSeenTrueOfSeenPCorrectionPlotter : public Plotter {

	void set_plotter_settings() {
		set_output_folder_name("TJ/TJ_s_tos_correction");	
	}

	void define_plots(){

		add_new_TH3D( "pjet_s_VS_pjet_tos_N_chhad",
					new TH3D( "pjet_tos_VS_pjet_s_N_chhad",   
					"True of detected p_{jet} VS detected p_{jet} & detected N_{charged hadrons};\
					p_{detected}^{jet}; N_{charged hadrons, E>5GeV}",
					150, 0, 600, 21, -0.5, 20.5, 150, 0, 600) );
	

	}

	bool is_good_charged_hadron( int ijet, int ichhad ) {
		if (TJ_RecoChHad_exists[ijet][ichhad] == 0) { return false; }
		if (TJ_RecoChHad_E[ijet][ichhad] < 5) { return false; }
		else { return true; }

	}

	void fill_plots(){
		float weight = get_current_weight();

		// This is the loop over all events
		while ( get_next_event() ) {
			//if ( pass_selection == 0 ) { continue; } 
		
			if ( ! (pass_isolep_cut == 1 &&
					pass_min_chargedparticles_jet_cut == 1 &&
					pass_min_particles_jet_cut == 1 &&
					pass_y_34_cut == 1 &&
					pass_miss_mass_cut == 1 &&
					pass_cosTheta_miss_cut == 1 &&
					pass_cosTheta_EmaxTrack_cut == 1 &&
					pass_E_10cone_EmaxTrack_cut == 1 ) ) { continue; } 

			for (int ijet=0; ijet<max_Njets; ijet++) {

				if ( ! TJ_jets_exists[ijet] ) { continue; } 


				if ( fabs(TJ_jets_final_elementon_pdgID[ijet]) > 5 ) { continue; }

				int N_highE_reco_chhad = 0;

				for (int ichhad=0; ichhad<max_Nparticles; ichhad++) {
					if ( ! is_good_charged_hadron(ijet, ichhad) ) { continue; }
					else { N_highE_reco_chhad++; }
				}

				get_TH3D( "pjet_s_VS_pjet_tos_N_chhad" ) 
				->Fill( TJ_jets_detected_p[ijet], N_highE_reco_chhad, 
						TJ_jets_true_of_detectable_p[ijet], weight );

			}
							

		}
	}


	void draw_plots(){
		const Int_t N_par = 6;

		TH3D* hist = get_TH3D( "pjet_s_VS_pjet_tos_N_chhad" );

		Double_t pars[N_par] = {1, 1.2, -1, 0, 0, 0};
		TF3 *f3 = new TF3("f3",fit,0,400,-0.5,16.5,0,400, N_par);
		f3->SetParameters(pars);
		//f3->SetParLimits(1, 1, 1.5);

		//ROOT::Math::MinimizerOptions::SetDefaultMinimizer("Minuit2");

		//hist->Fit("f3", "WQN");
		cout << " p fit " << endl;
		hist->Fit("f3", "+");


		TCanvas* can = new TCanvas( "can", "", 0, 0, 600, 600);				

		//hist->Draw("lego2");
		//f3->Draw("same");
		can->Print((get_output_directory() + "/Fit.pdf").c_str());
		can->Close();


		TFile *file = TFile::Open((get_output_directory() + "/Fit.root").c_str(), "RECREATE");
		hist->Write();
		//f3->Write();
		file->Close();
	}
};
