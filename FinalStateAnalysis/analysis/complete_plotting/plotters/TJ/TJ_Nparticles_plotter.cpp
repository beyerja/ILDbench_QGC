#include "../../include/plotter.h"


struct JetLevel {

	string descr;

	float** E;
	float** p;	

	JetLevel( string _descr, float** _E, float** _p ) :
		descr(_descr),
		E(_E), p(_p)
	{}

};

struct LevelStep {

	JetLevel level1;
	JetLevel level2;

	string name;
	string titel;

	LevelStep( JetLevel _level1, JetLevel _level2, string _name, string _titel ) :
		level1(_level1),  level2(_level2), 
		name(_name),  titel(_titel) 
	{}

};


struct NRange {

	string descr;
	int N_min;
	int N_max;

	NRange( string _descr, int _N_min, int _N_max ) :
		descr(_descr),  N_min(_N_min),  N_max(_N_max) 
	{}

	bool in_range(int N) {
		if ( (N >= N_min) && (N < N_max) ) { return true; }
		else { return false; }
	}

};





class TJNParticlesPlotter : public Plotter {

	void set_plotter_settings() {
		set_output_folder_name("TJ/TJ_N_particles_in_jets");	
	}

	vector<string> particle_types = {"all_q", "udsc", "b"};

	JetLevel true_of_seen = JetLevel("detected particles true values",
							&TJ_jets_true_of_detectable_E, &TJ_jets_true_of_detectable_p) ;
	JetLevel seen =  		JetLevel("detected particles reco values",
							&TJ_jets_detected_E, &TJ_jets_detected_p) ;

	vector<LevelStep> steps;
	vector<NRange> ranges;

	void create_levels() {
		steps.push_back( LevelStep( true_of_seen, seen, "tos_s", "jet reconstruction" ) );
		
		ranges.push_back( NRange( "0_10", 0, 10) );
		ranges.push_back( NRange( "10_15", 10, 15) );
		ranges.push_back( NRange( "15_20", 15, 20) );
		ranges.push_back( NRange( "21_50", 20, 50) );
		ranges.push_back( NRange( "geq51", 50, 2000000000) );
	}

	void define_plots(){

	  create_levels();

	  for (int istep=0; istep<steps.size(); istep++) {
		LevelStep step = steps[istep];

		for (int itype=0; itype<particle_types.size(); itype++) {
		  string type = particle_types[itype];

		  for (int irange=0; irange<ranges.size(); irange++) { 
			NRange range = ranges[irange];

			add_new_TProfile(( 	step.name +"_Ejet_"+ type+ "_jets_NMCparticles_" 
								+ range.descr ).c_str(), 
				new TProfile((	step.name +"_Ejet_"+ type+ "_jets_NMCpartciles_" 
								+ range.descr ).c_str(),
							(type + " " + step.titel + 
							", varying N_{particles} in true-of-detected;\
							E_{" + step.level1.descr + "}^{jet} [GeV]; \
							E_{" + step.level2.descr + "}^{jet} [GeV]").c_str(),
							50, 0, 200) ); 

		  }
		}
	  }

	}

	bool is_good_lepton( int ijet, int ilep ) {
		if (TJ_MCLep_exists[ijet][ilep] == 0) { return false; }
		else if ( 	(TJ_MCLep_highest_trck_weight_in_this_jet[ijet][ilep] < 0.5) &&
					(TJ_MCLep_highest_calo_weight_in_this_jet[ijet][ilep] < 0.5) ) {
			return false; 
		}
		else { return true; }
	}

	bool is_good_charged_hadron( int ijet, int ichhad ) {
		if (TJ_MCChHad_exists[ijet][ichhad] == 0) { return false; }
		else if ( 	(TJ_MCChHad_highest_trck_weight_in_this_jet[ijet][ichhad] < 0.5) &&
					(TJ_MCChHad_highest_calo_weight_in_this_jet[ijet][ichhad] < 0.5) ) {
			return false; 
		}
		else { return true; }
	}


	bool is_good_neutral_hadron( int ijet, int inehad ) {
		if (TJ_MCNeHad_exists[ijet][inehad] == 0) { return false; }
		else if ( 	(TJ_MCNeHad_highest_trck_weight_in_this_jet[ijet][inehad] < 0.5) &&
					(TJ_MCNeHad_highest_calo_weight_in_this_jet[ijet][inehad] < 0.5) ) {
			return false; 
		}
		else { return true; }
	}

	bool is_good_gamma( int ijet, int igamma ) {
		if (TJ_MCGamma_exists[ijet][igamma] == 0) { return false; }
		else if ( 	(TJ_MCGamma_highest_trck_weight_in_this_jet[ijet][igamma] < 0.5) &&
					(TJ_MCGamma_highest_calo_weight_in_this_jet[ijet][igamma] < 0.5) ) {
			return false; 
		}
		else { return true; }
	}


	NRange* find_range( int N ) {
		for ( int irange=0; irange<ranges.size(); irange++ ) {
			if (ranges[irange].in_range(N)) { return &ranges[irange]; }
		}

		return NULL;
	}


	void test() {
		cout << " n ";
		for ( int irange=0; irange<ranges.size(); irange++ ) {
			cout << ranges[irange].N_max << " ";
		}
	}

	void fill_plots(){
		float weight = get_current_weight();

		// This is the loop over all events
		while ( get_next_event() ) {
			if ( pass_selection == 0 ) { continue; } 

			for (int ijet=0; ijet<max_Njets; ijet++) {

				if ( ! TJ_jets_exists[ijet] ) { continue; } 

				if ( fabs(TJ_jets_final_elementon_pdgID[ijet]) > 5 ) { continue; }
				if ( TJ_jets_true_of_detectable_E[ijet] == 0 ) { continue; }

				string type;

				if ( fabs(TJ_jets_final_elementon_pdgID[ijet]) == 5 ) {
					type = "b";
				} else {
					type = "udsc";
				}

				float total_N = 0;
				for (int iparticle=0; iparticle<max_Nparticles; iparticle++) {
					if ( is_good_lepton(ijet, iparticle) ) 			{ total_N++; }
					if ( is_good_charged_hadron(ijet, iparticle) ) 	{ total_N++; }
					if ( is_good_neutral_hadron(ijet, iparticle) ) 	{ total_N++; }
					if ( is_good_gamma(ijet, iparticle) ) 			{ total_N++; }
				}

				NRange range = *find_range(total_N);

				for (int istep=0; istep<steps.size(); istep++) {
					LevelStep step = steps[istep];

					get_TProfile(( step.name +"_Ejet_all_q_jets_NMCparticles_" + range.descr ).c_str())
						->Fill( (*step.level1.E)[ijet], (*step.level2.E)[ijet], weight);

					get_TProfile((	step.name +"_Ejet_"+ type + "_jets_NMCparticles_" 
									+ range.descr ).c_str())
						->Fill( (*step.level1.E)[ijet], (*step.level2.E)[ijet], weight);
				}

			}
							

		}
	}


	void draw_plots(){
		string output_dir = get_output_directory();

		vector<array<float, 3>> colors {{27,158,119}, {217,95,2}, {117,112,179}, {231,41,138}, {102,166,30}, {230,171,2}};
		for (int j=0; j<colors.size(); j++) {
		    TColor *h2_color = new TColor(9100+j, colors[j][0]/256.0, colors[j][1]/256.0, colors[j][2]/256.0);
    	}                                                                                              
		for (int istep=0; istep<steps.size(); istep++) {
			LevelStep step = steps[istep];

			for (int itype=0; itype<particle_types.size(); itype++) {
				string type = particle_types[itype];

				TCanvas* can = new TCanvas( ("can_" + type + "_" + step.name).c_str(), 
											"", 0, 0, 600, 600); 	
				TPad* pad_up 	= new TPad( ("p1_" + type + "_" + step.name).c_str(), 
											"", 0.,0.35,1.,1.,0); 	
				pad_up->Draw();

				TPad* pad_down 	= new TPad( ("p2_" + type + "_" + step.name).c_str(), 
											"", 0.,0.,1.,0.4005,0); 	
				pad_down->Draw();
				pad_down->SetTopMargin(0);

				THStack* profile_stack = new THStack(("can_" + type +"_"+ step.name).c_str(),
							(type + " " + step.titel + ", varying N_{particles} in true-of-detected;\
							;E_{" + step.level2.descr + "}^{jet} [GeV]").c_str() );


				THStack* prjctn_stack = new THStack(("proj_can_" + type +"_"+ step.name).c_str(),
											(";	E_{" + step.level2.descr + "}^{jet} [GeV]; \
											Events").c_str());

				TLegend* leg = new TLegend(0.25, 0.6, 0.45, 0.9);



		  		for (int irange=0; irange<ranges.size(); irange++) { 
					NRange range = ranges[irange];

					TProfile* profile = get_TProfile( (	step.name +"_Ejet_"+ type 
											+ "_jets_NMCparticles_" + range.descr ).c_str() );
					profile->SetLineColor(9100+irange);
					profile->SetMarkerColor(9100+irange);

					leg->AddEntry(profile, (to_string(range.N_min)+"-"+to_string(range.N_max)).c_str());
					profile_stack->Add(profile);


					TH1D* entries_h1 = new TH1D((string(profile->GetName()) + "_pX").c_str(),
						(string(" ;") + profile->GetXaxis()->GetTitle() + "; Events" ).c_str(),
						profile->GetNbinsX(), profile->GetXaxis()->GetXmin(),
						profile->GetXaxis()->GetXmax() );
					for (int ibin=0; ibin<profile->GetNbinsX(); ibin++) {
						entries_h1->SetBinContent(ibin+1, profile->GetBinEntries(ibin+1));
					}
					entries_h1->SetLineColor(9100+irange);
					entries_h1->SetMarkerColor(9100+irange);
					entries_h1->SetMarkerSize(0.1);
					prjctn_stack->Add(entries_h1);

				}

				pad_up->cd();
				profile_stack->Draw("p nostack");
				gPad->GetFrame()->SetY1(0);
				gPad->Update();
				float axis_max = gPad->GetFrame()->GetY2();
				if ( axis_max > 200 ) { axis_max = 200; }
				TLine* profile_line = new TLine(0, 0, axis_max, axis_max);

				profile_line->SetLineWidth(1); 
				profile_line->Draw("same");
				leg->Draw();

				pad_down->cd();
				prjctn_stack->Draw("hist nostack");

				string plot_name_profile = output_dir + "/" + step.name +"/"+ "Ejet_"+ type 
													+ "_jets.pdf";
				can->Print(plot_name_profile.c_str());
				can->Close();
//				delete profile_line;

			}
		}

	}
			
};
