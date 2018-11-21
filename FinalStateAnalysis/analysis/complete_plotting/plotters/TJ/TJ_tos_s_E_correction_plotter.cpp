#include "../../include/plotter.h"

Double_t fit(Double_t *coord, Double_t* par) {
	double x1 = coord[1];
	double x2 = coord[1];
	double x1_prop_corr = ( par[1] + pow(par[3], 2) * x1 ) * x1;
	double x2_prop_corr = ( par[2] + pow(par[4], 2) * x2 ) * x2 ;
	double x1_x2_corr = pow(par[5], 2) * x1 * x2;
	double offset = par[0];
	return x1_prop_corr + x2_prop_corr + x1_x2_corr + offset;
}


class TJSeenTrueOfSeenECorrectionPlotter : public Plotter {

	void set_plotter_settings() {
		set_output_folder_name("TJ/TJ_s_tos_correction");	
		set_1D_array_reading( true );
		set_2D_array_reading( true );

	}

	void define_plots(){

		add_new_TH3D( "Ejet_s_VS_Ejet_tos_N_chhad",
					new TH3D( "Ejet_tos_VS_Ejet_s_N_chhad",   
					"True of detected E_{jet} VS detected E_{jet} & detected N_{charged hadrons};\
					E_{detected}^{jet}; N_{charged hadrons, E>5GeV}",
					150, 0, 600, 21, -0.5, 20.5, 150, 0, 600) );
	
		//get_TH3D( "Ejet_s_VS_Ejet_tos_N_chhad" )->SetErrorOption( "" );

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

				get_TH3D( "Ejet_s_VS_Ejet_tos_N_chhad" ) 
				->Fill( TJ_jets_detected_E[ijet], N_highE_reco_chhad, 
						TJ_jets_true_of_detectable_E[ijet], weight );

			}
							

		}
	}


	void draw_plots(){
		const Int_t N_par = 6;

		TH3D* hist = get_TH3D( "Ejet_s_VS_Ejet_tos_N_chhad" );

		Double_t pars[N_par] = {1, 1.2, -1, 0, 0, 0};
		TF3 *f3 = new TF3("f3",fit,0,400,-0.5,16.5,0,400, N_par);
		f3->SetParameters(pars);
		//f3->SetParLimits(1, 1, 1.5);

		//ROOT::Math::MinimizerOptions::SetDefaultMinimizer("Minuit2");

		//hist->Fit("f3", "WQN");
		cout << " E fit " << endl;
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
/*
		TCanvas* p_can = new TCanvas( "fit_proj", "", 0, 0, 600, 600 );


		TProfile2D* distr = get_TH3D( "Ejet_s_VS_Ejet_tos_N_chhad" )->Project3DProfile("yx");

		TH2D* fit_2D = new TH2D("fit_proj", "True of detected E_{jet} VS detected E_{jet} & detected N_{charged hadrons}; E_{detected}^{jet}; N_{charged hadrons, E>5GeV}", 150, 0, 600, 21, -0.5, 20.5);
		Double_t* result_pars = f3->GetParameters();

		int N_xbins = fit_2D->GetNbinsX();
		int N_ybins = fit_2D->GetNbinsY();

		Double_t x[2];
		for (int xbin=1; xbin<N_xbins; xbin++) {
			for (int ybin=1; ybin<N_ybins; ybin++) {
				x[0] = fit_2D->GetXaxis()->GetBinCenter(xbin);
				x[1] = fit_2D->GetYaxis()->GetBinCenter(ybin);

				double z = result_pars[0] + result_pars[1] * x[0] + result_pars[2] * x[1];
				//if ( distr->GetBinContent(xbin, ybin) !=0 ) {
					fit_2D->SetBinContent(xbin, ybin, z);
				//} else {
				//	fit_2D->SetBinContent(xbin, ybin, 0);
				//}
			}
		}

		distr->SetLineColor(1);
		fit_2D->SetLineColor(2);
		fit_2D->Rebin2D(5,1);
		distr->Rebin2D(5,1);
		fit_2D->Draw("box");
		distr->Draw("box same");
		fit_2D->Draw("box same");

		p_can->Print((get_output_directory() + "/E_fit.pdf").c_str());
		p_can->Close();
*/
	}
};
