#include "../include/plotter.h"

// Very simple example

class SeparationCurvePlotter : public Plotter {

	void set_plotter_settings() {
		set_output_folder_name("separation_curves");
	}

	double cut_min = 30;
	double cut_max = 250;
	static const int N_steps = 4400;
    double step_size = (cut_max - cut_min) / double(N_steps); 
    vector<double> separator_masses;

    vector<double> N_correct_ID_WWs_genlevel, N_correct_ID_ZZs_genlevel;
    vector<double> genlevel_WW_eff, genlevel_ZZ_eff;

    vector<double> N_correct_ID_WWs_reco, N_correct_ID_ZZs_reco;
    vector<double> reco_WW_eff, reco_ZZ_eff;

    vector<double> N_correct_ID_WWs_cheatjet, N_correct_ID_ZZs_cheatjet;
    vector<double> cheatjet_WW_eff, cheatjet_ZZ_eff;
    
    vector<double> N_correct_ID_WWs_cheatjet_custompairing, N_correct_ID_ZZs_cheatjet_custompairing;
    vector<double> cheatjet_custompairing_WW_eff, cheatjet_custompairing_ZZ_eff;

    vector<double> N_correct_ID_WWs_cheated_overlay, N_correct_ID_ZZs_cheated_overlay;
    vector<double> cheated_overlay_WW_eff, cheated_overlay_ZZ_eff;

    vector<double> N_correct_ID_WWs_cheatjet_noSLDs, N_correct_ID_ZZs_cheatjet_noSLDs;
    vector<double> cheatjet_noSLDs_WW_eff, cheatjet_noSLDs_ZZ_eff;

    vector<double> N_correct_ID_WWs_reco_perfectbtag, N_correct_ID_ZZs_reco_perfectbtag;
    vector<double> reco_perfectbtag_WW_eff, reco_perfectbtag_ZZ_eff;

    vector<double> N_correct_ID_WWs_cheatjet_perfectbtag, N_correct_ID_ZZs_cheatjet_perfectbtag;
    vector<double> cheatjet_perfectbtag_WW_eff, cheatjet_perfectbtag_ZZ_eff;
    
    vector<double> N_correct_ID_WWs_cheatjet_custompairing_perfectbtag, N_correct_ID_ZZs_cheatjet_custompairing_perfectbtag;
    vector<double> cheatjet_custompairing_perfectbtag_WW_eff, cheatjet_custompairing_perfectbtag_ZZ_eff;

	void define_plots(){
        separator_masses = vector<double> (N_steps, 0);
        N_correct_ID_WWs_genlevel = N_correct_ID_ZZs_genlevel = genlevel_WW_eff = genlevel_ZZ_eff = vector<double> (N_steps, 0);
        N_correct_ID_WWs_reco = N_correct_ID_ZZs_reco = reco_WW_eff = reco_ZZ_eff = vector<double> (N_steps, 0);
        N_correct_ID_WWs_cheatjet = N_correct_ID_ZZs_cheatjet = cheatjet_WW_eff = cheatjet_ZZ_eff = vector<double> (N_steps, 0);
        N_correct_ID_WWs_cheatjet_custompairing = N_correct_ID_ZZs_cheatjet_custompairing = cheatjet_custompairing_WW_eff = cheatjet_custompairing_ZZ_eff = vector<double> (N_steps, 0);
        N_correct_ID_WWs_cheated_overlay = N_correct_ID_ZZs_cheated_overlay = cheated_overlay_WW_eff = cheated_overlay_ZZ_eff = vector<double> (N_steps, 0);
        N_correct_ID_WWs_cheatjet_noSLDs = N_correct_ID_ZZs_cheatjet_noSLDs = cheatjet_noSLDs_WW_eff = cheatjet_noSLDs_ZZ_eff = vector<double> (N_steps, 0);
        N_correct_ID_WWs_reco_perfectbtag = N_correct_ID_ZZs_reco_perfectbtag = reco_perfectbtag_WW_eff = reco_perfectbtag_ZZ_eff = vector<double> (N_steps, 0);
        N_correct_ID_WWs_cheatjet_perfectbtag = N_correct_ID_ZZs_cheatjet_perfectbtag = cheatjet_perfectbtag_WW_eff = cheatjet_perfectbtag_ZZ_eff = vector<double> (N_steps, 0);
        N_correct_ID_WWs_cheatjet_custompairing_perfectbtag = N_correct_ID_ZZs_cheatjet_custompairing_perfectbtag = cheatjet_custompairing_perfectbtag_WW_eff = cheatjet_custompairing_perfectbtag_ZZ_eff = vector<double> (N_steps, 0);
		for ( int i_step = 0; i_step<N_steps; i_step++ ) { separator_masses[i_step] = cut_min + step_size*i_step; } 
	}


	double N_true_WWs_weighted {};
	double N_true_ZZs_weighted {};

	double N_true_WWs_weighted_noSLDevents {};
	double N_true_ZZs_weighted_noSLDevents {};

	void fill_plots(){
		float weight = get_current_weight();

		// This is the loop over all events
		while ( get_next_event() ) {
		//	if ( pass_selection == 1 ) {
				double genlevel_mass_average = (true_pair1_mass+true_pair2_mass)/2.;
				double reco_mass_average = (pair1_mass+pair2_mass)/2.;
				double cheatjet_mass_average = (TJ_pair1_mass_from_icn+TJ_pair2_mass_from_icn)/2.;
				double cheatjet_custompairing_mass_average = (TJ_pair1_mass_from_icn_custom_pairing+TJ_pair2_mass_from_icn_custom_pairing)/2.;
				double cheated_overlay_mass_average = (TJ_pair1_mass_cheated_overlay+TJ_pair2_mass_cheated_overlay)/2.;
        
				int number_c_quarks = 0;
				int number_b_quarks = 0;
				for ( int i=0; i<4; i++ ) {
					if ( fabs(true_quarks_pdg_id[i]) == 4 )  { number_c_quarks++; }
					if ( fabs(true_quarks_pdg_id[i]) == 5 )  { number_b_quarks++; }
				}  


				if ( true_evt_type == 1 ) {
					N_true_WWs_weighted+=weight;
          if (n_BSLD == 0 && n_CSLD == 0) { N_true_WWs_weighted_noSLDevents+=weight; }
					for ( int i_step = 0; i_step<N_steps; i_step++ ) {
						if ( genlevel_mass_average < separator_masses[i_step] ) { N_correct_ID_WWs_genlevel[i_step]+=weight; } 
						if ( reco_mass_average < separator_masses[i_step] ) { 
							N_correct_ID_WWs_reco[i_step]+=weight; 
							if ( number_b_quarks == 0 ) { // If b there assume as b-tagged and therefore ZZ -> for WW no bs allowed!
								N_correct_ID_WWs_reco_perfectbtag[i_step]+=weight;
							}
						}
						if ( cheatjet_mass_average < separator_masses[i_step] ) { 
							N_correct_ID_WWs_cheatjet[i_step]+=weight; 
							if ( number_b_quarks == 0 ) { // If b there assume as b-tagged and therefore ZZ -> for WW no bs allowed!
								N_correct_ID_WWs_cheatjet_perfectbtag[i_step]+=weight;
							}
              if (n_BSLD == 0 && n_CSLD == 0) {
                N_correct_ID_WWs_cheatjet_noSLDs[i_step]+=weight; 
              }
						}
						if ( cheatjet_custompairing_mass_average < separator_masses[i_step] ) { 
							N_correct_ID_WWs_cheatjet_custompairing[i_step]+=weight; 
							if ( number_b_quarks == 0 ) { // If b there assume as b-tagged and therefore ZZ -> for WW no bs allowed!
								N_correct_ID_WWs_cheatjet_custompairing_perfectbtag[i_step]+=weight;
							}
						}
						if ( cheated_overlay_mass_average < separator_masses[i_step] ) { 
							N_correct_ID_WWs_cheated_overlay[i_step]+=weight; 
						}
					}
				}
				else if ( true_evt_type == 2 ) {
					N_true_ZZs_weighted+=weight;
          if (n_BSLD == 0 && n_CSLD == 0) { N_true_ZZs_weighted_noSLDevents+=weight; }
					for ( int i_step = 0; i_step<N_steps; i_step++ ) {
						if ( genlevel_mass_average > separator_masses[i_step] ) { N_correct_ID_ZZs_genlevel[i_step]+=weight; } 
						if ( reco_mass_average > separator_masses[i_step] ) { N_correct_ID_ZZs_reco[i_step]+=weight; }
						if ( cheatjet_mass_average > separator_masses[i_step] ) {
              N_correct_ID_ZZs_cheatjet[i_step]+=weight;
              if (n_BSLD == 0 && n_CSLD == 0) { N_correct_ID_ZZs_cheatjet_noSLDs[i_step]+=weight; }
            }
						if ( cheatjet_custompairing_mass_average > separator_masses[i_step] ) { N_correct_ID_ZZs_cheatjet_custompairing[i_step]+=weight; }
						if ( cheated_overlay_mass_average > separator_masses[i_step] ) { N_correct_ID_ZZs_cheated_overlay[i_step]+=weight; }
						if ( number_b_quarks > 0 || reco_mass_average > separator_masses[i_step] ) { N_correct_ID_ZZs_reco_perfectbtag[i_step]+=weight; }
						if ( number_b_quarks > 0 || cheatjet_mass_average > separator_masses[i_step] ) { N_correct_ID_ZZs_cheatjet_perfectbtag[i_step]+=weight; }
						if ( number_b_quarks > 0 || cheatjet_custompairing_mass_average > separator_masses[i_step] ) { N_correct_ID_ZZs_cheatjet_custompairing_perfectbtag[i_step]+=weight; }
					}
				}
			}
	//	}

	}
  
  int index_diagonal_point(TGraph* graph){
    /** Finds the point of the TGraph closest to the (0,0)-(1,1) diagonal and 
        returns the index of that point
    */
    double minimal_x_y_distance = 1.1;
    int index_minimal_distance  = graph->GetN() + 1; // To make sure it throws an error if rest fails
    
    Double_t x, y;
    for( int i = 0; i < graph->GetN(); i++ ) {
      graph->GetPoint(i, x, y);
      if ( fabs(x-y) < minimal_x_y_distance ) {
        minimal_x_y_distance = fabs(x-y);
        index_minimal_distance = i;
      }
    }
    
    if (index_minimal_distance == graph->GetN() + 1) {
      std::cout << "Something went wrong in index_diagonal_point!" << std::endl;
    }
    return index_minimal_distance;
  }

  void cout_diagonal_point(TGraph* graph){
    double x,y;
    graph->GetPoint(index_diagonal_point(graph),x,y);
    std::cout << graph->GetName() << " " << x << " " << y << std::endl;
  }

	void draw_plots(){
		for ( int i_step = 0; i_step<N_steps; i_step++ ) {
			genlevel_WW_eff[i_step] = N_correct_ID_WWs_genlevel[i_step] / N_true_WWs_weighted;
			genlevel_ZZ_eff[i_step] = N_correct_ID_ZZs_genlevel[i_step] / N_true_ZZs_weighted;
			reco_WW_eff[i_step] = N_correct_ID_WWs_reco[i_step] / N_true_WWs_weighted;
			reco_ZZ_eff[i_step] = N_correct_ID_ZZs_reco[i_step] / N_true_ZZs_weighted;
			cheatjet_WW_eff[i_step] = N_correct_ID_WWs_cheatjet[i_step] / N_true_WWs_weighted;
			cheatjet_ZZ_eff[i_step] = N_correct_ID_ZZs_cheatjet[i_step] / N_true_ZZs_weighted;
			cheatjet_custompairing_WW_eff[i_step] = N_correct_ID_WWs_cheatjet_custompairing[i_step] / N_true_WWs_weighted;
			cheatjet_custompairing_ZZ_eff[i_step] = N_correct_ID_ZZs_cheatjet_custompairing[i_step] / N_true_ZZs_weighted;
			cheated_overlay_WW_eff[i_step] = N_correct_ID_WWs_cheated_overlay[i_step] / N_true_WWs_weighted;
			cheated_overlay_ZZ_eff[i_step] = N_correct_ID_ZZs_cheated_overlay[i_step] / N_true_ZZs_weighted;
			cheatjet_noSLDs_WW_eff[i_step] = N_correct_ID_WWs_cheatjet_noSLDs[i_step] / N_true_WWs_weighted_noSLDevents;
			cheatjet_noSLDs_ZZ_eff[i_step] = N_correct_ID_ZZs_cheatjet_noSLDs[i_step] / N_true_ZZs_weighted_noSLDevents;
			reco_perfectbtag_WW_eff[i_step] = N_correct_ID_WWs_reco_perfectbtag[i_step] / N_true_WWs_weighted;
			reco_perfectbtag_ZZ_eff[i_step] = N_correct_ID_ZZs_reco_perfectbtag[i_step] / N_true_ZZs_weighted;
			cheatjet_perfectbtag_WW_eff[i_step] = N_correct_ID_WWs_cheatjet_perfectbtag[i_step] / N_true_WWs_weighted;
			cheatjet_perfectbtag_ZZ_eff[i_step] = N_correct_ID_ZZs_cheatjet_perfectbtag[i_step] / N_true_ZZs_weighted;
			cheatjet_custompairing_perfectbtag_WW_eff[i_step] = N_correct_ID_WWs_cheatjet_custompairing_perfectbtag[i_step] / N_true_WWs_weighted;
			cheatjet_custompairing_perfectbtag_ZZ_eff[i_step] = N_correct_ID_ZZs_cheatjet_custompairing_perfectbtag[i_step] / N_true_ZZs_weighted;
		}


		TFile* TGraph_file = new TFile( (get_output_directory() + "/_all_TGraphs.root").c_str() ,"RECREATE");
		add_new_TGraph("separation_curve_genlevel", new TGraph(N_steps, &genlevel_WW_eff[0], &genlevel_ZZ_eff[0]));
		get_TGraph("separation_curve_genlevel")->SetName("separation_curve_genlevel");
		get_TGraph("separation_curve_genlevel")->SetTitle("; WW ID eff.; ZZ ID eff.");
		get_TGraph("separation_curve_genlevel")->Draw();
    cout_diagonal_point(get_TGraph("separation_curve_genlevel"));
    

		add_new_TGraph("separation_curve_reco", new TGraph(N_steps, &reco_WW_eff[0], &reco_ZZ_eff[0]));
		get_TGraph("separation_curve_reco")->SetName("separation_curve_reco");
		get_TGraph("separation_curve_reco")->SetTitle("; WW ID eff.; ZZ ID eff.");
		get_TGraph("separation_curve_reco")->Draw();
    cout_diagonal_point(get_TGraph("separation_curve_reco"));

		add_new_TGraph("separation_curve_cheatjet", new TGraph(N_steps, &cheatjet_WW_eff[0], &cheatjet_ZZ_eff[0]));
		get_TGraph("separation_curve_cheatjet")->SetName("separation_curve_cheatjet");
		get_TGraph("separation_curve_cheatjet")->SetTitle("; WW ID eff.; ZZ ID eff.");
		get_TGraph("separation_curve_cheatjet")->Draw();
    cout_diagonal_point(get_TGraph("separation_curve_cheatjet"));
    
		add_new_TGraph("separation_curve_cheatjet_custompairing", new TGraph(N_steps, &cheatjet_custompairing_WW_eff[0], &cheatjet_custompairing_ZZ_eff[0]));
		get_TGraph("separation_curve_cheatjet_custompairing")->SetName("separation_curve_cheatjet_custompairing");
		get_TGraph("separation_curve_cheatjet_custompairing")->SetTitle("; WW ID eff.; ZZ ID eff.");
		get_TGraph("separation_curve_cheatjet_custompairing")->Draw();
    cout_diagonal_point(get_TGraph("separation_curve_cheatjet_custompairing"));
    
		add_new_TGraph("separation_curve_cheated_overlay", new TGraph(N_steps, &cheated_overlay_WW_eff[0], &cheated_overlay_ZZ_eff[0]));
		get_TGraph("separation_curve_cheated_overlay")->SetName("separation_curve_cheated_overlay");
		get_TGraph("separation_curve_cheated_overlay")->SetTitle("; WW ID eff.; ZZ ID eff.");
		get_TGraph("separation_curve_cheated_overlay")->Draw();
    cout_diagonal_point(get_TGraph("separation_curve_cheated_overlay"));
    
		add_new_TGraph("separation_curve_cheatjet_noSLDs", new TGraph(N_steps, &cheatjet_noSLDs_WW_eff[0], &cheatjet_noSLDs_ZZ_eff[0]));
		get_TGraph("separation_curve_cheatjet_noSLDs")->SetName("separation_curve_cheatjet_noSLDs");
		get_TGraph("separation_curve_cheatjet_noSLDs")->SetTitle("; WW ID eff.; ZZ ID eff.");
		get_TGraph("separation_curve_cheatjet_noSLDs")->Draw();
    cout_diagonal_point(get_TGraph("separation_curve_cheatjet_noSLDs"));

		add_new_TGraph("separation_curve_reco_perfectbtag", new TGraph(N_steps, &reco_perfectbtag_WW_eff[0], &reco_perfectbtag_ZZ_eff[0]));
		get_TGraph("separation_curve_reco_perfectbtag")->SetName("separation_curve_reco_perfectbtag");
		get_TGraph("separation_curve_reco_perfectbtag")->SetTitle("; WW ID eff.; ZZ ID eff.");
		get_TGraph("separation_curve_reco_perfectbtag")->Draw();
    cout_diagonal_point(get_TGraph("separation_curve_reco_perfectbtag"));

		add_new_TGraph("separation_curve_cheatjet_perfectbtag", new TGraph(N_steps, &cheatjet_perfectbtag_WW_eff[0], &cheatjet_perfectbtag_ZZ_eff[0]));
		get_TGraph("separation_curve_cheatjet_perfectbtag")->SetName("separation_curve_cheatjet_perfectbtag");
		get_TGraph("separation_curve_cheatjet_perfectbtag")->SetTitle("; WW ID eff.; ZZ ID eff.");
		get_TGraph("separation_curve_cheatjet_perfectbtag")->Draw();
    cout_diagonal_point(get_TGraph("separation_curve_cheatjet_perfectbtag"));
    
		add_new_TGraph("separation_curve_cheatjet_custompairing_perfectbtag", new TGraph(N_steps, &cheatjet_custompairing_perfectbtag_WW_eff[0], &cheatjet_custompairing_perfectbtag_ZZ_eff[0]));
		get_TGraph("separation_curve_cheatjet_custompairing_perfectbtag")->SetName("separation_curve_cheatjet_custompairing_perfectbtag");
		get_TGraph("separation_curve_cheatjet_custompairing_perfectbtag")->SetTitle("; WW ID eff.; ZZ ID eff.");
		get_TGraph("separation_curve_cheatjet_custompairing_perfectbtag")->Draw();
    cout_diagonal_point(get_TGraph("separation_curve_cheatjet_custompairing_perfectbtag"));


		get_TGraph("separation_curve_genlevel")->Write();
		get_TGraph("separation_curve_reco")->Write();
		get_TGraph("separation_curve_cheatjet")->Write();
		get_TGraph("separation_curve_cheatjet_custompairing")->Write();
    get_TGraph("separation_curve_cheated_overlay")->Write();
    get_TGraph("separation_curve_cheatjet_noSLDs")->Write();
		get_TGraph("separation_curve_reco_perfectbtag")->Write();
		get_TGraph("separation_curve_cheatjet_perfectbtag")->Write();
		get_TGraph("separation_curve_cheatjet_custompairing_perfectbtag")->Write();
		TGraph_file->Write();
		TGraph_file->Close();

	};
};
