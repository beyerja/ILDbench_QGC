#include "../../include/plotter.h"

class TJQuarkJetsPlusNeutrinosPlotter : public Plotter {

	void set_plotter_settings() {
		set_output_folder_name("TJ/TJ_jets_plus_true_TJisr_and_nus");	
		set_1D_array_reading( true );
	}

	void define_plots(){

		add_new_TH1D("TJ_tos_qjets_plus_TJ_t_ISR_plus_nus_E", 
			new TH1D( "TJ_tos_qjets_plus_TJ_t_ISR_plus_nus_E", "TJ jets + TJ ISR + #nu_{true}s, jets = true of detected; #sum_{TJ jets true of detected, #nu, ISR} E [GeV]; Events", 75, 500, 1400)); 
		add_new_TH1D("TJ_s_qjets_plus_TJ_t_ISR_plus_nus_E", 
			new TH1D( "TJ_s_qjets_plus_TJ_t_ISR_plus_nus_E", "TJ jets + TJ ISR + #nu_{true}s, jets = detected; #sum_{TJ jets detected, #nu, ISR} E [GeV]; Events", 75, 500, 1400)); 

	}

	void fill_plots(){
		float weight = get_current_weight();

		// TODO For all samles: collect all masses of FEs -> draw lines in TH1D

		// This is the loop over all events
		while ( get_next_event() ) {

			if ( pass_selection == 0 ) { continue; }

			float TJ_tos_qjets_E = 0;
			float TJ_s_qjets_E = 0;
			float TJ_t_ISRjets_E = 0;

			for ( int i=0; i<max_Njets; i++ ) {
				if (TJ_jets_exists[i] == 0) { continue; }

				float abs_final_elementon_pdgID = fabs( TJ_jets_final_elementon_pdgID[i] );

				if ( abs_final_elementon_pdgID == 22 ) {	
					TJ_t_ISRjets_E += TJ_jets_true_E[i];
				}
				else if ( abs_final_elementon_pdgID == 0 ||  abs_final_elementon_pdgID > 9 ) {
					continue;
				}

				TJ_tos_qjets_E += TJ_jets_true_of_detectable_E[i]; 
				TJ_s_qjets_E += TJ_jets_detected_E[i]; 
			}  

			float TJ_tos_qjets_plus_TJISR_plus_nus_E = 
					TJ_tos_qjets_E + TJ_t_ISRjets_E + true_nu1_E + true_nu2_E;
		
			float TJ_s_qjets_plus_TJISR_plus_nus_E = 
					TJ_s_qjets_E + TJ_t_ISRjets_E + true_nu1_E + true_nu2_E;


			get_TH1D("TJ_tos_qjets_plus_TJ_t_ISR_plus_nus_E")->Fill( TJ_tos_qjets_plus_TJISR_plus_nus_E, weight );

			get_TH1D("TJ_s_qjets_plus_TJ_t_ISR_plus_nus_E")->Fill( TJ_s_qjets_plus_TJISR_plus_nus_E, weight );
		}
	}

	void draw_plots(){

		TCanvas *canvas_h1 = new TCanvas("can_h1", "", 0, 0, 800, 800);

		THStack* stack = new THStack("TJ_qjets_plus_TJ_t_ISR_plus_nus_E", "TJ jets + TJ ISR_{true} + #nu_{true}s, different jet levels; #sum_{TJ jets (level), #nu, ISR} E [GeV]; Events"); 

		TLegend* leg = new TLegend(0.25, 0.8, 0.55, 0.9);

		TH1D* h_tos = get_TH1D("TJ_tos_qjets_plus_TJ_t_ISR_plus_nus_E");
		h_tos->SetLineColor(36);
		stack->Add(h_tos);
		leg->AddEntry(h_tos, "jets: true of detected", "l");

		TH1D* h_s = get_TH1D("TJ_s_qjets_plus_TJ_t_ISR_plus_nus_E");
		h_s->SetLineColor(45);
		stack->Add(h_s);
		leg->AddEntry(h_s, "jets: detected", "l");

		gStyle->SetTitleSize(0.045, "x");
		gStyle->SetTitleOffset(1.6, "x");

		stack->Draw("nostack hist");
		leg->Draw();


		string plot_name_h1 = get_output_directory() + "/TJ_qjet_plus_ISR_and_nus_E_spectra.pdf";
		canvas_h1->Print(plot_name_h1.c_str());
		canvas_h1->Close();
	};
};
