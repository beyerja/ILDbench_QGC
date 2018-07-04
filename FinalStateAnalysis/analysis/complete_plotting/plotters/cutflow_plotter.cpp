#include "../include/plotter.h"

// Very simple example

class CutflowPlotter : public Plotter {

	void set_plotter_settings() {
		set_output_folder_name("cutflow");	
	}

	void define_plots(){
		add_new_TH1D("cutflow", new TH1D("cutflow", "cutflow; ;Events", 12, -0.5, 11.5) );
		add_new_TH1D("cutflow_WW", new TH1D("cutflow_WW", "cutflow; ;Events", 12, -0.5, 11.5) );
		add_new_TH1D("cutflow_ZZ", new TH1D("cutflow_ZZ", "cutflow; ;Events", 12, -0.5, 11.5) );
	}

	void fill_plots(){
		float weight = get_current_weight();

		// This is the loop over all events
		while ( get_next_event() ) {
			if ( true_evt_type == 0 ) {continue;}
			get_TH1D("cutflow")->Fill( 0., weight);
			if ( true_evt_type == 1 ) { get_TH1D("cutflow_WW")->Fill( 0., weight); }
			else if ( true_evt_type == 2 ) { get_TH1D("cutflow_ZZ")->Fill( 0., weight); }

			if ( pass_isolep_cut == 1 ) {
				get_TH1D("cutflow")->Fill( 1., weight);
				if ( true_evt_type == 1 ) { get_TH1D("cutflow_WW")->Fill( 1., weight); }
				else if ( true_evt_type == 2 ) { get_TH1D("cutflow_ZZ")->Fill( 1., weight); }
			} else { continue; }
			if ( pass_min_chargedparticles_jet_cut == 1 ) {
				get_TH1D("cutflow")->Fill( 2., weight);
				if ( true_evt_type == 1 ) { get_TH1D("cutflow_WW")->Fill( 2., weight); }
				else if ( true_evt_type == 2 ) { get_TH1D("cutflow_ZZ")->Fill( 2., weight); }
			} else { continue; }
			if ( pass_min_particles_jet_cut == 1 ) {
				get_TH1D("cutflow")->Fill( 3., weight);
				if ( true_evt_type == 1 ) { get_TH1D("cutflow_WW")->Fill( 3., weight); }
				else if ( true_evt_type == 2 ) { get_TH1D("cutflow_ZZ")->Fill( 3., weight); }
			} else { continue; }
			if ( pass_min_Ejet_cut == 1 ) {
				get_TH1D("cutflow")->Fill( 4., weight);
				if ( true_evt_type == 1 ) { get_TH1D("cutflow_WW")->Fill( 4., weight); }
				else if ( true_evt_type == 2 ) { get_TH1D("cutflow_ZZ")->Fill( 4., weight); }
			} else { continue; }
			if ( pass_y_34_cut == 1 ) {
				get_TH1D("cutflow")->Fill( 5., weight);
				if ( true_evt_type == 1 ) { get_TH1D("cutflow_WW")->Fill( 5., weight); }
				else if ( true_evt_type == 2 ) { get_TH1D("cutflow_ZZ")->Fill( 5., weight); }
			} else { continue; }
			if ( pass_miss_mass_cut == 1 ) {
				get_TH1D("cutflow")->Fill( 6., weight);
				if ( true_evt_type == 1 ) { get_TH1D("cutflow_WW")->Fill( 6., weight); }
				else if ( true_evt_type == 2 ) { get_TH1D("cutflow_ZZ")->Fill( 6., weight); }
			} else { continue; }
			if ( pass_E_T_vis_cut == 1 ) {
				get_TH1D("cutflow")->Fill( 7., weight);
				if ( true_evt_type == 1 ) { get_TH1D("cutflow_WW")->Fill( 7., weight); }
				else if ( true_evt_type == 2 ) { get_TH1D("cutflow_ZZ")->Fill( 7., weight); }
			} else { continue; }
			if ( pass_p_T_vis_cut == 1 ) {
				get_TH1D("cutflow")->Fill( 8., weight);
				if ( true_evt_type == 1 ) { get_TH1D("cutflow_WW")->Fill( 8., weight); }
				else if ( true_evt_type == 2 ) { get_TH1D("cutflow_ZZ")->Fill( 8., weight); }
			} else { continue; }
			if ( pass_cosTheta_miss_cut == 1 ) {
				get_TH1D("cutflow")->Fill( 9., weight);
				if ( true_evt_type == 1 ) { get_TH1D("cutflow_WW")->Fill( 9., weight); }
				else if ( true_evt_type == 2 ) { get_TH1D("cutflow_ZZ")->Fill( 9., weight); }
			} else { continue; }
			if ( pass_cosTheta_EmaxTrack_cut == 1 ) {
				get_TH1D("cutflow")->Fill( 10., weight);
				if ( true_evt_type == 1 ) { get_TH1D("cutflow_WW")->Fill( 10., weight); }
				else if ( true_evt_type == 2 ) { get_TH1D("cutflow_ZZ")->Fill( 10., weight); }
			} else { continue; }
			if ( pass_E_10cone_EmaxTrack_cut == 1 ) {
				get_TH1D("cutflow")->Fill( 11., weight);
				if ( true_evt_type == 1 ) { get_TH1D("cutflow_WW")->Fill( 11., weight); }
				else if ( true_evt_type == 2 ) { get_TH1D("cutflow_ZZ")->Fill( 11., weight); }
			} else { continue; }

		}
	}

	void draw_plots(){

		vector<string> cut_labels = {	"no cuts", "isolep", "min_chargedparticles_jet", "min_particles_jet", "min_Ejet", 
										"y_34", "miss_mass", "E_T_vis", "p_T_vis", "cosTheta_miss", "cosTheta_EmaxTrack", 
										"E_10cone_EmaxTrack" };

		TCanvas* c1 = new TCanvas("c_cutflow", "", 0, 0, 900, 950);

		get_TH1D("cutflow")->SetMinimum(0);

		TAxis* x_axis = get_TH1D("cutflow")->GetXaxis();
		for (int i = 0; i<cut_labels.size(); i++) {
			x_axis->SetBinLabel(x_axis->FindBin(i), cut_labels[i].c_str());
			x_axis->ChangeLabel(x_axis->FindBin(i), 0.8);
		}
		x_axis->SetLabelOffset(0.004);
		x_axis->SetLabelSize(0.03);

		get_TH1D("cutflow")->Draw("hist");

		get_TH1D("cutflow_WW")->SetLineColor(4);	
		get_TH1D("cutflow_ZZ")->SetLineColor(2);	


		TLegend* legend = new TLegend(0.7, 0.6, 0.9, 0.9);
		legend->AddEntry(get_TH1D("cutflow"), "total", "l"); 
		legend->AddEntry(get_TH1D("cutflow_WW"), "WW", "l"); 
		legend->AddEntry(get_TH1D("cutflow_ZZ"), "ZZ", "l"); 
		legend->Draw();

		get_TH1D("cutflow_WW")->Draw("hist same");
		get_TH1D("cutflow_ZZ")->Draw("hist same");

		c1->SetBottomMargin(0.111);
		c1->SetRightMargin(0.158);

		c1->Print((get_output_directory() + "/cutflow.pdf").c_str());
	};
};
