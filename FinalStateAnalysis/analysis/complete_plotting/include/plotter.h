#ifndef PLOTTER_h
#define PLOTTER_h
#include "InfoStorage.h"
#include "../../../include/jet_corrections.h"

class Plotter {
	/* Superclass for all Plotters, contains basic functionalities,
 	details must then be specified in implementation of subclass.
	-> Looping etc. provided, but what, how and where to plot custom
	*/

  public:

	// File and directory information
	InfoStorage info_storage;

	string output_folder_name;

	void set_output_folder_name(string _output_folder_name) {
		// Set the name of the folder in the output directory for this specific plotter
		output_folder_name = _output_folder_name;
	}

	string get_output_folder_name() { return output_folder_name; }

	string get_output_directory() {
		// Return the plotter specific output directory
		return info_storage.get_output_directory() + "/" + get_output_folder_name() ;
	}

	// Vectors for the possible diagrams (if new type needed: add it)
	vector<TH1D*> TH1D_vector;
	vector<TH2D*> TH2D_vector;
	vector<TProfile*> TProfile_vector;
	vector<TProfile2D*> TProfile2D_vector;
	vector<TH3D*> TH3D_vector;

	// Maps for comprehensive storage of the diagrams by a name
	// -> connects index of histogram in vector with its name
	map<string, int> TH1D_map;
	map<string, int> TH2D_map;
	map<string, int> TProfile_map;
	map<string, int> TProfile2D_map;
	map<string, int> TH3D_map;

	// Functions to store and get the histograms by name (copy-paste for new types)
	void add_new_TH1D (string name, TH1D* hist) {
		TH1D_vector.push_back(hist);										// push histo into vector
		int hist_index = TH1D_vector.size() - 1;							// get its index
		TH1D_map.insert( pair<string,int>( name.c_str(), hist_index ) );	// connect index with name
	}

	void add_new_TH2D (string name, TH2D* hist) {
		TH2D_vector.push_back(hist);										// push histo into vector
		int hist_index = TH2D_vector.size() - 1;							// get its index
		TH2D_map.insert( pair<string,int>( name.c_str(), hist_index ) );	// connect index with name
	}

	void add_new_TProfile (string name, TProfile* hist) {
		TProfile_vector.push_back(hist);										// push histo into vector
		int hist_index = TProfile_vector.size() - 1;							// get its index
		TProfile_map.insert( pair<string,int>( name.c_str(), hist_index ) );	// connect index with name
	}

	void add_new_TProfile2D (string name, TProfile2D* hist) {
		TProfile2D_vector.push_back(hist);										// push histo into vector
		int hist_index = TProfile2D_vector.size() - 1;							// get its index
		TProfile2D_map.insert( pair<string,int>( name.c_str(), hist_index ) );	// connect index with name
	}

	void add_new_TH3D (string name, TH3D* hist) {
		TH3D_vector.push_back(hist);										// push histo into vector
		int hist_index = TH3D_vector.size() - 1;							// get its index
		TH3D_map.insert( pair<string,int>( name.c_str(), hist_index ) );	// connect index with name
	}


	TH1D* get_TH1D (string name) {
		int hist_index = TH1D_map.find(name.c_str())->second;	// Get the index of hist called name
		return TH1D_vector[hist_index];
	}

	TH2D* get_TH2D (string name) {
		int hist_index = TH2D_map.find(name.c_str())->second;	// Get the index of hist called name
		return TH2D_vector[hist_index];
	}

	TProfile* get_TProfile (string name) {
		int hist_index = TProfile_map.find(name.c_str())->second;	// Get the index of hist called name
		return TProfile_vector[hist_index];
	}

	TProfile2D* get_TProfile2D (string name) {
		int hist_index = TProfile2D_map.find(name.c_str())->second;	// Get the index of hist called name
		return TProfile2D_vector[hist_index];
	}

	TH3D* get_TH3D (string name) {
		int hist_index = TH3D_map.find(name.c_str())->second;	// Get the index of hist called name
		return TH3D_vector[hist_index];
	}


	// For looping also allow access by index (bc it's generic)
	int get_number_TH1Ds() { return TH1D_vector.size(); }
	int get_number_TH2Ds() { return TH2D_vector.size(); }
	int get_number_TProfiles() { return TProfile_vector.size(); }
	int get_number_TProfile2Ds() { return TProfile2D_vector.size(); }
	int get_number_TH3Ds() { return TH3D_vector.size(); }

	TH1D* get_TH1D_i (int i) { return TH1D_vector[i]; }
	TH2D* get_TH2D_i (int i) { return TH2D_vector[i]; }
	TProfile* get_TProfile_i (int i) { return TProfile_vector[i]; }
	TProfile2D* get_TProfile2D_i (int i) { return TProfile2D_vector[i]; }
	TH3D* get_TH3D_i (int i) { return TH3D_vector[i]; }

	// Functions to automatically save all histograms to a rootfile in the output directory
	void save_all_TH1Ds_in_rootfile() {
		if ( get_number_TH1Ds() == 0 ) { return; }	// Do only if histograms available

		TFile* TH1D_file = new TFile( (get_output_directory() + "/_all_TH1Ds.root").c_str() ,"RECREATE");
		for ( int i=0; i<get_number_TH1Ds(); i++ ) {
			TH1D* th1d_clone = (TH1D*)get_TH1D_i(i)->Clone();
			th1d_clone->Draw();
		}
		TH1D_file->Write();
		TH1D_file->Close();
	}

	void save_all_TH2Ds_in_rootfile() {
		if ( get_number_TH2Ds() == 0 ) { return; }	// Do only if histograms available

		TFile* TH2D_file = new TFile( (get_output_directory() + "/_all_TH2Ds.root").c_str() ,"RECREATE");
		for ( int i=0; i<get_number_TH2Ds(); i++ ) {
			TH2D* th2d_clone = (TH2D*)get_TH2D_i(i)->Clone();
			th2d_clone->Draw();
		}
		TH2D_file->Write();
		TH2D_file->Close();
	}

	void save_all_TProfiles_in_rootfile() {
		if ( get_number_TProfiles() == 0 ) { return; }	// Do only if histograms available

		TFile* TProfile_file = new TFile( (get_output_directory() + "/_all_TProfiles.root").c_str() ,"RECREATE");
		for ( int i=0; i<get_number_TProfiles(); i++ ) {
			TProfile* tprofile_clone = (TProfile*)get_TProfile_i(i)->Clone();
			tprofile_clone->Draw();
		}
		TProfile_file->Write();
		TProfile_file->Close();
	}

	void save_all_TProfile2Ds_in_rootfile() {
		if ( get_number_TProfile2Ds() == 0 ) { return; }	// Do only if histograms available

		TFile* TProfile2D_file = new TFile( (get_output_directory() + "/_all_TProfile2Ds.root").c_str() ,"RECREATE");
		for ( int i=0; i<get_number_TProfile2Ds(); i++ ) {
			TProfile2D* tprofile_clone = (TProfile2D*)get_TProfile2D_i(i)->Clone();
			tprofile_clone->Draw();
		}
		TProfile2D_file->Write();
		TProfile2D_file->Close();
	}

	void save_all_TH3Ds_in_rootfile() {
		if ( get_number_TH3Ds() == 0 ) { return; }	// Do only if histograms available

		TFile* TH3D_file = new TFile( (get_output_directory() + "/_all_TH3Ds.root").c_str() ,"RECREATE");
		for ( int i=0; i<get_number_TH3Ds(); i++ ) {
			TH3D* th3d_clone = (TH3D*)get_TH3D_i(i)->Clone();
			th3d_clone->Draw();
		}
		TH3D_file->Write();
		TH3D_file->Close();
	}



	void save_all_histograms_in_rootfiles() {
		// Execute all histograms to rootfile functions
		save_all_TH1Ds_in_rootfile();
		save_all_TH2Ds_in_rootfile();
		save_all_TProfiles_in_rootfile();
		save_all_TProfile2Ds_in_rootfile();
		save_all_TH3Ds_in_rootfile();
	}

	// Specifics for current file
	float current_weight;
	float get_current_weight() { return current_weight; }

	TTree* current_tree;
	// For looping over tree:
	int current_event_number;

	bool get_next_event() {
		/* Allows running over all events of the file using a simple while loop */
		// If counter within size of tree -> get next event
		// Otherwise return false -> stop
		if ( current_event_number < current_tree->GetEntries() ) {

			// Get new array sizes
			current_tree->GetBranch("max_Njets")->GetEntry(current_event_number);
			current_tree->GetBranch("max_Nparticles")->GetEntry(current_event_number);
			current_tree->GetBranch("max_matrix_size")->GetEntry(current_event_number);

			// Set addresses for arrays with event specific length
			// Includes global GetEntry for all variables
			set_dynamic_addresses();

			current_event_number++; // Increment to next event
			return true;
		} else {
			return false;
		}
	}

	TTree* get_current_tree(){
		return current_tree;
	}



	/* --------------------------------------------------------*/
	// Because reading arrays in takes a lot of time only do it when necessary
	// => Set flags that need to be activated in case arrats should be used

	bool need_1D_arrays = false;
	bool need_2D_arrays = false;

	void set_1D_array_reading( bool need ) { need_1D_arrays = need; }
	void set_2D_array_reading( bool need ) { need_2D_arrays = need; }

	/* --------------------------------------------------------*/
	// All variables that are read out of the tree
	float xsection;

	int evt;

	int true_evt_type;

	float true_pair1_mass;
	float true_pair2_mass;
	float true_pair1_theta;
	float true_pair2_theta;
	float true_miss_mass;
	float true_sum_E_q;
	int partner_of_truthV1;
	int partner_of_truthV2;
	float true_ISR_inJets_E;
	float true_ISR_inJets_p;
	float true_ISR_inJets_theta;

	float TJ_final_cn_total_E;
	float TJ_initial_cn_total_E;
	float TJ_reco_total_E;

	int true_quarks_pdg_id[4];
	float true_quarks_E[4];
	float true_quarks_theta[4];

	int true_nu1_pdg_id;
	float true_nu1_E;
	float true_nu1_theta;

	int true_nu2_pdg_id;
	float true_nu2_E;
	float true_nu2_theta;

	int NPFOs_inCorrectJets;
	int NPFOs_inClusteredJets;
	int NPFOs_inCorrectJets_notinClusteredJets;
	int NPFOs_inClusteredJets_notinCorrectJets;

	float pair1_mass;
	float pair2_mass;
	float pair1_theta;
	float pair2_theta;
	float pair1_p;
	float pair2_p;
	float miss_mass;
	float sum_E_q;
	float E_T_vis;
	float p_T_vis;
	float E_10cone_EmaxTrack;
	float cosTheta_miss;
	float cosTheta_EmaxTrack;
	float y_34;
	float min_Ejet;
	int min_num_particles_jet;
	int min_num_chargedparticles_jet;
	int num_isolated_leptons;

	float jets_E[4];
	float jets_theta[4];
	float jets_mass[4];

	int max_Njets;
	int* TJ_jets_exists;
	int* TJ_jets_final_elementon_pdgID;
	float* TJ_jets_final_elementon_mass;
	float* TJ_jets_final_elementon_E;
	float* TJ_jets_final_elementon_p;
	float* TJ_jets_true_mass;
	float* TJ_jets_true_E;
	float* TJ_jets_true_p;
	float* TJ_jets_true_of_detectable_mass;
	float* TJ_jets_true_of_detectable_E;
	float* TJ_jets_true_of_detectable_p;
	float* TJ_jets_detected_mass;
	float* TJ_jets_detected_E;
	float* TJ_jets_detected_p;

  float TJ_pair1_mass_from_icn;
  float TJ_pair2_mass_from_icn;

	int max_Nparticles;

	int** TJ_RecoLep_exists;
	float** TJ_RecoLep_E;
	int** TJ_MCLep_exists;
	float** TJ_MCLep_E;
	int** TJ_MCLep_pdgID;
	float** TJ_MCLep_highest_trck_weight_in_this_jet;
	float** TJ_MCLep_highest_calo_weight_in_this_jet;

	int** TJ_RecoChHad_exists;
	float** TJ_RecoChHad_E;
	int** TJ_MCChHad_exists;
	float** TJ_MCChHad_E;
	int** TJ_MCChHad_pdgID;
	float** TJ_MCChHad_highest_trck_weight_in_this_jet;
	float** TJ_MCChHad_highest_calo_weight_in_this_jet;

	int** TJ_RecoNeHad_exists;
	float** TJ_RecoNeHad_E;
	int** TJ_MCNeHad_exists;
	float** TJ_MCNeHad_E;
	int** TJ_MCNeHad_pdgID;
	float** TJ_MCNeHad_highest_trck_weight_in_this_jet;
	float** TJ_MCNeHad_highest_calo_weight_in_this_jet;

	int** TJ_RecoGamma_exists;
	float** TJ_RecoGamma_E;
	int** TJ_MCGamma_exists;
	float** TJ_MCGamma_E;
	int** TJ_MCGamma_pdgID;
	float** TJ_MCGamma_highest_trck_weight_in_this_jet;
	float** TJ_MCGamma_highest_calo_weight_in_this_jet;


	int max_matrix_size;

	int pass_isolep_cut, pass_min_chargedparticles_jet_cut, pass_min_particles_jet_cut,
		pass_min_Ejet_cut, pass_y_34_cut, pass_miss_mass_cut, pass_E_T_vis_cut,
		pass_p_T_vis_cut, pass_cosTheta_miss_cut, pass_cosTheta_EmaxTrack_cut,
		pass_E_10cone_EmaxTrack_cut;

	int pass_selection;


	void set_tree_adresses () {
		/* Link the variables used in the plotting to the current tree */
		current_tree->SetBranchAddress( "xsection" , &xsection);

		current_tree->SetBranchAddress( "evt" , &evt);

		current_tree->SetBranchAddress( "true_evt_type", &true_evt_type);

		TBranch* truth_branch = current_tree->GetBranch("true_observables");
		truth_branch->GetLeaf( "pair1_mass" )->SetAddress(&true_pair1_mass);
		truth_branch->GetLeaf( "pair2_mass" )->SetAddress(&true_pair2_mass);
		truth_branch->GetLeaf( "pair1_theta" )->SetAddress(&true_pair1_theta);
		truth_branch->GetLeaf( "pair2_theta" )->SetAddress(&true_pair2_theta);
		truth_branch->GetLeaf( "miss_mass" )->SetAddress(&true_miss_mass);
		truth_branch->GetLeaf( "sum_E_q" )->SetAddress(&true_sum_E_q);

		current_tree->SetBranchAddress( "partner_of_truthV1" , &partner_of_truthV1);
		current_tree->SetBranchAddress( "partner_of_truthV2" , &partner_of_truthV2);
		current_tree->SetBranchAddress( "true_ISR_inJets_E" , &true_ISR_inJets_E);
		current_tree->SetBranchAddress( "true_ISR_inJets_p" , &true_ISR_inJets_p);
		current_tree->SetBranchAddress( "true_ISR_inJets_theta" , &true_ISR_inJets_theta);

		current_tree->SetBranchAddress( "TJ_final_cn_total_E" , &TJ_final_cn_total_E);
		current_tree->SetBranchAddress( "TJ_initial_cn_total_E" , &TJ_initial_cn_total_E);
		current_tree->SetBranchAddress( "TJ_reco_total_E" , &TJ_reco_total_E);

    TBranch* TJ_observ_from_icns = current_tree->GetBranch("masses_cheated_clustering_and_pairing");
    TJ_observ_from_icns->GetLeaf("pair1_mass")->SetAddress(&TJ_pair1_mass_from_icn);
    TJ_observ_from_icns->GetLeaf("pair2_mass")->SetAddress(&TJ_pair2_mass_from_icn); 

		for (int i=0; i<4; i++) {
			std::string index_string = std::to_string(i+1);
			TBranch* true_quark_branch =
				current_tree->GetBranch(("true_quark" + index_string).c_str());

			true_quark_branch->GetLeaf("pdg_id")->SetAddress(&true_quarks_pdg_id[i] );
			true_quark_branch->GetLeaf("E")->SetAddress(&true_quarks_E[i] );
			true_quark_branch->GetLeaf("theta")->SetAddress(&true_quarks_theta[i] );
		}

		TBranch* true_nu1_branch = current_tree->GetBranch("true_nu1");
		true_nu1_branch->GetLeaf("pdg_id")->SetAddress(&true_nu1_pdg_id);
		true_nu1_branch->GetLeaf("E")->SetAddress(&true_nu1_E);
		true_nu1_branch->GetLeaf("theta")->SetAddress(&true_nu1_theta);

		TBranch* true_nu2_branch = current_tree->GetBranch("true_nu2");
		true_nu2_branch->GetLeaf("pdg_id")->SetAddress(&true_nu2_pdg_id);
		true_nu2_branch->GetLeaf("E")->SetAddress(&true_nu2_E);
		true_nu2_branch->GetLeaf("theta")->SetAddress(&true_nu2_theta);

		current_tree->SetBranchAddress("max_Njets", &max_Njets);
		current_tree->SetBranchAddress("max_Nparticles", &max_Nparticles);
		current_tree->SetBranchAddress("max_matrix_size", &max_matrix_size);

		current_tree->SetBranchAddress( "NPFOs_inCorrectJets", &NPFOs_inCorrectJets);
		current_tree->SetBranchAddress( "NPFOs_inClusteredJets" , &NPFOs_inClusteredJets);
		current_tree->SetBranchAddress( "NPFOs_inCorrectJets_notinClusteredJets", &NPFOs_inCorrectJets_notinClusteredJets);
		current_tree->SetBranchAddress( "NPFOs_inClusteredJets_notinCorrectJets", &NPFOs_inClusteredJets_notinCorrectJets);

		TBranch* observ_branch = current_tree->GetBranch("observables");
		observ_branch->GetLeaf( "pair1_mass" )->SetAddress(&pair1_mass);
		observ_branch->GetLeaf( "pair2_mass" )->SetAddress(&pair2_mass);
		observ_branch->GetLeaf( "pair1_theta" )->SetAddress(&pair1_theta);
		observ_branch->GetLeaf( "pair2_theta" )->SetAddress(&pair2_theta);
		observ_branch->GetLeaf( "pair1_p" )->SetAddress(&pair1_p);
		observ_branch->GetLeaf( "pair2_p" )->SetAddress(&pair2_p);
		observ_branch->GetLeaf( "miss_mass" )->SetAddress(&miss_mass);
		observ_branch->GetLeaf( "sum_E_q" )->SetAddress(&sum_E_q);
		observ_branch->GetLeaf( "E_T_vis" )->SetAddress(&E_T_vis);
		observ_branch->GetLeaf( "p_T_vis" )->SetAddress(&p_T_vis);
		observ_branch->GetLeaf( "E_10cone_EmaxTrack" )->SetAddress(&E_10cone_EmaxTrack);
		observ_branch->GetLeaf( "cosTheta_miss" )->SetAddress(&cosTheta_miss);
		observ_branch->GetLeaf( "cosTheta_EmaxTrack" )->SetAddress(&cosTheta_EmaxTrack);
		observ_branch->GetLeaf( "y_34" )->SetAddress(&y_34);
		observ_branch->GetLeaf( "min_Ejet" )->SetAddress(&min_Ejet);
		observ_branch->GetLeaf( "min_num_particles_jet" )->SetAddress(&min_num_particles_jet);
		observ_branch->GetLeaf( "min_num_chargedparticles_jet" )->SetAddress(&min_num_chargedparticles_jet);
		observ_branch->GetLeaf( "num_isolated_leptons" )->SetAddress(&num_isolated_leptons);

		for (int i=0; i<4; i++) {
			std::string index_string = std::to_string(i+1);

			TBranch* jet_branch = current_tree->GetBranch( ("jet_" + index_string).c_str() );
			jet_branch->GetLeaf("E")->SetAddress(&jets_E[i] );
			jet_branch->GetLeaf("theta")->SetAddress(&jets_theta[i] );
			jet_branch->GetLeaf("mass")->SetAddress(&jets_mass[i] );
		}

		current_tree->SetBranchAddress( "pass_isolep_cut", &pass_isolep_cut);
		current_tree->SetBranchAddress( "pass_min_chargedparticles_jet_cut", &pass_min_chargedparticles_jet_cut);
		current_tree->SetBranchAddress( "pass_min_particles_jet_cut", &pass_min_particles_jet_cut);
		current_tree->SetBranchAddress( "pass_min_Ejet_cut", &pass_min_Ejet_cut);
		current_tree->SetBranchAddress( "pass_y_34_cut", &pass_y_34_cut);
		current_tree->SetBranchAddress( "pass_miss_mass_cut", &pass_miss_mass_cut);
		current_tree->SetBranchAddress( "pass_E_T_vis_cut", &pass_E_T_vis_cut);
		current_tree->SetBranchAddress( "pass_p_T_vis_cut", &pass_p_T_vis_cut);
		current_tree->SetBranchAddress( "pass_cosTheta_miss_cut", &pass_cosTheta_miss_cut);
		current_tree->SetBranchAddress( "pass_cosTheta_EmaxTrack_cut", &pass_cosTheta_EmaxTrack_cut);
		current_tree->SetBranchAddress( "pass_E_10cone_EmaxTrack_cut", &pass_E_10cone_EmaxTrack_cut);

		current_tree->SetBranchAddress( "pass_selection", &pass_selection);

	}


	void set_dynamic_1D_array_addresses() {
		vector<int**> int_1D_arrays = {
			&TJ_jets_exists,
			&TJ_jets_final_elementon_pdgID
		};

		vector<string> int_1D_names = {
			"TJ_jets_exist",
			"TJ_jets_final_elementon_pdgID"
		};

		vector<float**> float_1D_arrays = {
			&TJ_jets_final_elementon_mass,
			&TJ_jets_final_elementon_E,
			&TJ_jets_final_elementon_p,
			&TJ_jets_true_mass,
			&TJ_jets_true_E,
			&TJ_jets_true_p,
			&TJ_jets_true_of_detectable_mass,
			&TJ_jets_true_of_detectable_E,
			&TJ_jets_true_of_detectable_p,
			&TJ_jets_detected_mass,
			&TJ_jets_detected_E,
			&TJ_jets_detected_p
		};

		vector<string> float_1D_names = {
			"TJ_jets_final_elementon_mass",
			"TJ_jets_final_elementon_E",
			"TJ_jets_final_elementon_p",
			"TJ_jets_true_jet_mass",
			"TJ_jets_true_jet_E",
			"TJ_jets_true_jet_p",
			"TJ_jets_true_of_detectable_mass",
			"TJ_jets_true_of_detectable_E",
			"TJ_jets_true_of_detectable_p",
			"TJ_jets_detected_mass",
			"TJ_jets_detected_E",
			"TJ_jets_detected_p"
		};

		// Clean up memory
		if ( current_event_number != 0 ) {
			for (int i=0; i<int_1D_arrays.size(); i++) 		{ delete *(int_1D_arrays[i]); }
			for (int i=0; i<float_1D_arrays.size(); i++)	{ delete *(float_1D_arrays[i]); }
		}

		for (int i=0; i<int_1D_arrays.size(); i++) {
			*(int_1D_arrays[i]) = new int [max_Njets];
			current_tree->SetBranchAddress( int_1D_names[i].c_str(), *(int_1D_arrays[i]) );
		}

		for (int i=0; i<float_1D_arrays.size(); i++) {
			*(float_1D_arrays[i]) = new float [max_Njets];
			current_tree->SetBranchAddress( float_1D_names[i].c_str(), *(float_1D_arrays[i]) );
		}
	}


	vector<int*> helper_int_arrays;
	vector<float*> helper_float_arrays;

	void set_dynamic_2D_array_addresses() {

		vector<int***> int_2D_arrays = {
			&TJ_RecoLep_exists,
			&TJ_MCLep_exists,
			&TJ_MCLep_pdgID,
			&TJ_RecoChHad_exists,
			&TJ_MCChHad_exists,
			&TJ_MCChHad_pdgID,
			&TJ_RecoNeHad_exists,
			&TJ_MCNeHad_exists,
			&TJ_MCNeHad_pdgID,
			&TJ_RecoGamma_exists,
			&TJ_MCGamma_exists,
			&TJ_MCGamma_pdgID

		};

		vector<string> int_2D_names = {
			"TJ_RecoLep_exists",
			"TJ_MCLep_exists",
			"TJ_MCLep_pdgID",
			"TJ_RecoChHad_exists",
			"TJ_MCChHad_exists",
			"TJ_MCChHad_pdgID",
			"TJ_RecoNeHad_exists",
			"TJ_MCNeHad_exists",
			"TJ_MCNeHad_pdgID",
			"TJ_RecoGamma_exists",
			"TJ_MCGamma_exists",
			"TJ_MCGamma_pdgID"
		};

		vector<float***> float_2D_arrays = {
			&TJ_RecoLep_E,
			&TJ_MCLep_E,
			&TJ_MCLep_highest_trck_weight_in_this_jet,
			&TJ_MCLep_highest_calo_weight_in_this_jet,
			&TJ_RecoChHad_E,
			&TJ_MCChHad_E,
			&TJ_MCChHad_highest_trck_weight_in_this_jet,
			&TJ_MCChHad_highest_calo_weight_in_this_jet,
			&TJ_RecoNeHad_E,
			&TJ_MCNeHad_E,
			&TJ_MCNeHad_highest_trck_weight_in_this_jet,
			&TJ_MCNeHad_highest_calo_weight_in_this_jet,
			&TJ_RecoGamma_E,
			&TJ_MCGamma_E,
			&TJ_MCGamma_highest_trck_weight_in_this_jet,
			&TJ_MCGamma_highest_calo_weight_in_this_jet
		};

		vector<string> float_2D_names = {
			"TJ_RecoLep_E",
			"TJ_MCLep_E",
			"TJ_MCLep_highest_trck_weight_in_this_jet",
			"TJ_MCLep_highest_calo_weight_in_this_jet",
			"TJ_RecoChHad_E",
			"TJ_MCChHad_E",
			"TJ_MCChHad_highest_trck_weight_in_this_jet",
			"TJ_MCChHad_highest_calo_weight_in_this_jet",
			"TJ_RecoNeHad_E",
			"TJ_MCNeHad_E",
			"TJ_MCNeHad_highest_trck_weight_in_this_jet",
			"TJ_MCNeHad_highest_calo_weight_in_this_jet",
			"TJ_RecoGamma_E",
			"TJ_MCGamma_E",
			"TJ_MCGamma_highest_trck_weight_in_this_jet",
			"TJ_MCGamma_highest_calo_weight_in_this_jet"
		};

		//Problem w/ 2D Branches: (next time use vector.............)
		// - Are saved as 1D array in ROOT
		// - To read out as 2D: First create 1D and couple to Branch
		// 						Then create 2D and couple to 1D

		for (int i=0; i<helper_int_arrays.size(); i++){
			delete helper_int_arrays[i];
		}
		helper_int_arrays.resize(0);

		for (int i=0; i<helper_float_arrays.size(); i++){
			delete helper_float_arrays[i];
		}
		helper_float_arrays.resize(0);


		for (int i=0; i<int_2D_arrays.size(); i++) {
			helper_int_arrays.push_back( new int [max_matrix_size] );
			current_tree->SetBranchAddress( int_2D_names[i].c_str(), helper_int_arrays[i] );
		}

		for (int i=0; i<float_2D_arrays.size(); i++) {
			helper_float_arrays.push_back( new float [max_matrix_size] );
			current_tree->SetBranchAddress( float_2D_names[i].c_str(), helper_float_arrays[i] );
		}

		current_tree->GetEntry(current_event_number); // Load again for newly set arrays

		for (int i=0; i<int_2D_arrays.size(); i++) {
			*(int_2D_arrays[i]) = new int* [max_Njets];
			for (int ijet=0; ijet<max_Njets; ijet++) {
				(*(int_2D_arrays[i]))[ijet] = &helper_int_arrays[i][ijet*max_Nparticles];
			}
		}

		for (int i=0; i<float_2D_arrays.size(); i++) {
			*(float_2D_arrays[i]) = new float* [max_Njets];
			for (int ijet=0; ijet<max_Njets; ijet++) {
				(*(float_2D_arrays[i]))[ijet] = &helper_float_arrays[i][ijet*max_Nparticles];
			}
		}
	}

	void set_dynamic_addresses() {
		// c++ doesn't allow dynamic 2D memory allocation
		// -> Trick it into doing that anyway by first assigning 1D memory for pointers
		// 		and then 2D memory by assigning memory to those pointers

		if ( need_1D_arrays ) {
			set_dynamic_1D_array_addresses();
		}

		if ( need_2D_arrays ) {
			set_dynamic_2D_array_addresses();
		}
		else {
			current_tree->GetEntry(current_event_number);
		}


	}

	/* --------------------------------------------------------*/


	/* --------------------------------------------------------*/
	// Physical constants
	float mW_DBD = 80.419000;
	float mZ_DBD = 91.188200;
	/* --------------------------------------------------------*/


	/* --------------------------------------------------------*/
	// Functions that need to be specified in the subclass
	virtual void set_plotter_settings() = 0;
	virtual void define_plots() = 0;
	virtual void fill_plots() = 0;
	virtual void draw_plots() = 0;
	/* --------------------------------------------------------*/


	void loop_over_files() {
		/* Perfom standard loop over all files in info_storage */
		int N_files = info_storage.get_number_of_files();
		for (int file_i=0; file_i<N_files; file_i++) {
			current_weight=info_storage.get_file_weight_i(file_i);

			// Open the tree of the new file
			string current_file_path = info_storage.get_file_path_i(file_i);
			TFile *file = TFile::Open( current_file_path.c_str() );
			current_tree = (TTree*)file->Get( ( info_storage.get_tree_name() ).c_str() );
			current_event_number=0; // Reset counter for event loops

			// Link the variables to the current tree
			set_tree_adresses();
			fill_plots();
			file->Close();
		}

	}


	/* --------------------------------------------------------*/
	// Functions to help with plotting.
	//
	void draw_mass_line ( string particle, string measurement_version, string direction ) {
		// In the current histogram draw a line in the given direction at the point of
		// the particles mass.
		// CAREFUL: Can only be called AFTER other histos are already drawn!

		float mass = -9999999;
		int line_color = 0;

		if ( particle == "W" ) {
			line_color = 9;
			if ( measurement_version == "DBD" ) { mass = mW_DBD; }
		}
		else if ( particle == "Z" ) {
			line_color = 46;
			if ( measurement_version == "DBD" ) { mass = mZ_DBD; }
		}

		if ( mass<0 ) { return; }

		if ( direction == "x" ) {
			gPad->Update(); // Some weird ROOT thing. First need to update the current Pad before able to read axis range.

			// Get Minimum and Maximum of axis from current TPad
			float x_min = gPad->GetFrame()->GetX1();
			float x_max = gPad->GetFrame()->GetX2();

			TLine* mass_line = new TLine(x_min, mass, x_max, mass);
			mass_line->SetLineColorAlpha(line_color, 0.5);
			mass_line->Draw("same");
		}
		else if ( direction == "y" ) {
			gPad->Update(); // Some weird ROOT thing. First need to update the current Pad before able to read axis range.

			// Get Minimum and Maximum of axis from current TPad
			float y_min = gPad->GetFrame()->GetY1();
			float y_max = gPad->GetFrame()->GetY2();

			TLine* mass_line = new TLine(mass, y_min, mass, y_max);
			mass_line->SetLineColorAlpha(line_color, 0.5);
			mass_line->Draw("same");
		}
	}

	/* --------------------------------------------------------*/


	/* --------------------------------------------------------*/
	//Functions to be called on instance (additional to virtuals).
	//
	void init ( InfoStorage _info_storage ) {
		info_storage = _info_storage;
		gROOT->Reset();
		gStyle->SetOptStat(0);
		TH1D::SetDefaultSumw2();
		TH2D::SetDefaultSumw2();
		TProfile::SetDefaultSumw2();
		TProfile2D::SetDefaultSumw2();
		TH3D::SetDefaultSumw2();
	}

	void run() {
		set_plotter_settings();
		define_plots();
		loop_over_files();
		save_all_histograms_in_rootfiles();
		draw_plots();
	}
	/* --------------------------------------------------------*/
};

#endif
