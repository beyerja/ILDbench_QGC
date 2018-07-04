#include "../../include/plotter.h"

class TJTotalEPlotter : public Plotter {

	void set_plotter_settings() {
		set_output_folder_name("TJ/TJ_total_E");	
	}

	void define_plots(){

		add_new_TH1D("TJ_initial_cn_total_E", 
			new TH1D( "TJ_initial_cn_total_E", 
			"TJ inital colour neutrals; #sum_{TJ initial cn} E [GeV]; Events", 50, 550, 1050)); 
		add_new_TH1D("TJ_final_cn_total_E", 
			new TH1D( "TJ_final_cn_total_E", 
			"TJ inital colour neutrals; #sum_{TJ final cn} E [GeV]; Events", 50, 550, 1050)); 
		add_new_TH1D("TJ_reco_total_E", 
			new TH1D( "TJ_reco_total_E", 
			"TJ reconstructed jets; #sum_{TJ reco jets} E [GeV]; Events", 50, 550, 1050)); 
		add_new_TH1D("TJ_reco_plus_nus_total_E", 
			new TH1D( "TJ_reco_plus_nus_total_E", 
			"TJ reconstructed jets + generator level neutrinos; #sum_{TJ reco jets, #nu} E [GeV]; Events"
			, 50, 550, 1050)); 



		add_new_TH1D("TJ_true_total_E", 
			new TH1D( "TJ_true_total_E", 
			"TJ true level; #sum_{TJ jet true} E [GeV]; Events", 50, 550, 1050)); 

	}

	void fill_plots(){
		float weight = get_current_weight();

		// TODO For all samles: collect all masses of FEs -> draw lines in TH1D

		// This is the loop over all events
		while ( get_next_event() ) {

			if ( pass_selection == 0 ) { continue; }

			get_TH1D("TJ_initial_cn_total_E")->Fill( TJ_initial_cn_total_E, weight );
			get_TH1D("TJ_final_cn_total_E")->Fill( TJ_final_cn_total_E, weight );
			get_TH1D("TJ_reco_total_E")->Fill( TJ_reco_total_E, weight );

			get_TH1D("TJ_reco_plus_nus_total_E")->Fill( TJ_reco_total_E + true_nu1_E + true_nu2_E, weight );



			get_TH1D("TJ_true_total_E")->Fill( TJ_initial_cn_total_E, weight );

		}
	}

	void draw_plots(){

		vector<array<float, 3>> colors {{27,158,119}, {217,95,2}, {117,112,179}, {231,41,138}, {102,166,30}, {230,171,2}};
		for (int j=0; j<colors.size(); j++) {
		    TColor *h2_color = new TColor(9100+j, colors[j][0]/256.0, colors[j][1]/256.0, colors[j][2]/256.0);
    	}                                                                                              


		TCanvas *canvas_h1 = new TCanvas("can_h1", "", 0, 0, 800, 800);

		THStack* stack = new THStack("TJ_levels_total_E", "TJ total E, different levels; #sum_{TJ level, all} E [GeV]; Events"); 

		TLegend* leg = new TLegend(0.25, 0.5, 0.6, 0.9);

		TH1D* h_icn = get_TH1D("TJ_initial_cn_total_E");
		h_icn->SetLineColor(9100);
		h_icn->SetLineWidth(4);
		stack->Add(h_icn);
		leg->AddEntry(h_icn, "#splitline{inital colour neutrals}{w/o ISR}", "l");

		TH1D* h_fcn = get_TH1D("TJ_final_cn_total_E");
		h_fcn->SetLineColor(9101);
		h_fcn->SetLineWidth(3);
		stack->Add(h_fcn);
		leg->AddEntry(h_fcn, "#splitline{final colour neutrals}{(w/ ISR and w/o FSR)}", "l");

		TH1D* h_reco = get_TH1D("TJ_reco_total_E");
		h_reco->SetLineColor(9102);
		h_reco->SetLineWidth(2);
		stack->Add(h_reco);
		leg->AddEntry(h_reco, "reconstructed particles", "l");

		TH1D* h_reco_plus_nus = get_TH1D("TJ_reco_plus_nus_total_E");
		h_reco_plus_nus->SetLineColor(9103);
		h_reco_plus_nus->SetLineWidth(2);
		stack->Add(h_reco_plus_nus);
		leg->AddEntry(h_reco_plus_nus, "#splitline{reconstructed particles}{+#nu_{true}s}", "l");

		gStyle->SetTitleSize(0.045, "x");
		gStyle->SetTitleOffset(1.65, "x");
		gStyle->SetTitleOffset(1.5, "y");

		canvas_h1->SetLeftMargin(0.22);

		stack->Draw("nostack hist");
		leg->Draw();


		string plot_name_h1 = get_output_directory() + "/TJ_total_E_spectra.pdf";
		canvas_h1->Print(plot_name_h1.c_str());
		canvas_h1->Close();
	};
};
