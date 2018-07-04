
class TreeEvent {
	public:
		// Declare variables corresponding to branches of TTree
		// from Marlin output
		float pair1_mass;
		float pair2_mass;
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

		// New branch variable to be set in current selection
		// TODO This clearly at some point needs to be generalized
		int	pass_isolep_cut, pass_min_chargedparticles_jet_cut, pass_min_particles_jet_cut, 
			pass_min_Ejet_cut, pass_y_34_cut, pass_miss_mass_cut, pass_E_T_vis_cut, 
			pass_p_T_vis_cut, pass_cosTheta_miss_cut, pass_cosTheta_EmaxTrack_cut, 
			pass_E_10cone_EmaxTrack_cut;		

		

		int pass_selection;

		// Save new TBranch so that they can be filled separately 
		// (Have to ->Fill() TBranch and not TTree bc otherwise new event
		// will be added instead of just updating old one)
		TBranch	*pass_isolep_cut_branch, *pass_min_chargedparticles_jet_cut_branch, *pass_min_particles_jet_cut_branch, 
				*pass_min_Ejet_cut_branch, *pass_y_34_cut_branch, *pass_miss_mass_cut_branch, *pass_E_T_vis_cut_branch, 
				*pass_p_T_vis_cut_branch, *pass_cosTheta_miss_cut_branch, *pass_cosTheta_EmaxTrack_cut_branch, 
				*pass_E_10cone_EmaxTrack_cut_branch;		

		TBranch* pass_selection_branch;


		void set_addresses(TTree* new_tree) {
			/* Variables from current event in new_tree are read into variables above */
			TBranch* observ_branch = new_tree->GetBranch("observables");

			observ_branch->GetLeaf("pair1_mass")->SetAddress(&pair1_mass);
			observ_branch->GetLeaf("pair2_mass")->SetAddress(&pair2_mass);
			observ_branch->GetLeaf("miss_mass")->SetAddress(&miss_mass);
			observ_branch->GetLeaf("sum_E_q")->SetAddress(&sum_E_q);
			observ_branch->GetLeaf("E_T_vis")->SetAddress(&E_T_vis);
			observ_branch->GetLeaf("p_T_vis")->SetAddress(&p_T_vis);
			observ_branch->GetLeaf("E_10cone_EmaxTrack")->SetAddress(&E_10cone_EmaxTrack);
			observ_branch->GetLeaf("cosTheta_miss")->SetAddress(&cosTheta_miss);
			observ_branch->GetLeaf("cosTheta_EmaxTrack")->SetAddress(&cosTheta_EmaxTrack);
			observ_branch->GetLeaf("y_34")->SetAddress(&y_34);
			observ_branch->GetLeaf("min_Ejet")->SetAddress(&min_Ejet);
			observ_branch->GetLeaf("min_num_particles_jet")->SetAddress(&min_num_particles_jet);
			observ_branch->GetLeaf("min_num_chargedparticles_jet")->SetAddress(&min_num_chargedparticles_jet);
			observ_branch->GetLeaf("num_isolated_leptons")->SetAddress(&num_isolated_leptons);




			// Create new branches that hold info if event passed selection cuts
			pass_isolep_cut_branch = new_tree->Branch( "pass_isolep_cut", &pass_isolep_cut, "pass_isolep_cut/I");
			pass_min_chargedparticles_jet_cut_branch = new_tree->Branch( "pass_min_chargedparticles_jet_cut", &pass_min_chargedparticles_jet_cut, "pass_min_chargedparticles_jet_cut/I");
			pass_min_particles_jet_cut_branch = new_tree->Branch( "pass_min_particles_jet_cut", &pass_min_particles_jet_cut, "pass_min_particles_jet_cut/I");
			pass_min_Ejet_cut_branch = new_tree->Branch( "pass_min_Ejet_cut", &pass_min_Ejet_cut, "pass_min_Ejet_cut/I");
			pass_y_34_cut_branch = new_tree->Branch( "pass_y_34_cut", &pass_y_34_cut, "pass_y_34_cut/I");
			pass_miss_mass_cut_branch = new_tree->Branch( "pass_miss_mass_cut", &pass_miss_mass_cut, "pass_miss_mass_cut/I");
			pass_E_T_vis_cut_branch = new_tree->Branch( "pass_E_T_vis_cut", &pass_E_T_vis_cut, "pass_E_T_vis_cut/I");
			pass_p_T_vis_cut_branch = new_tree->Branch( "pass_p_T_vis_cut", &pass_p_T_vis_cut, "pass_p_T_vis_cut/I");
			pass_cosTheta_miss_cut_branch = new_tree->Branch( "pass_cosTheta_miss_cut", &pass_cosTheta_miss_cut, "pass_cosTheta_miss_cut/I");
			pass_cosTheta_EmaxTrack_cut_branch = new_tree->Branch( "pass_cosTheta_EmaxTrack_cut", &pass_cosTheta_EmaxTrack_cut, "pass_cosTheta_EmaxTrack_cut/I");
			pass_E_10cone_EmaxTrack_cut_branch = new_tree->Branch( "pass_E_10cone_EmaxTrack_cut", &pass_E_10cone_EmaxTrack_cut, "pass_E_10cone_EmaxTrack_cut/I");
			 
			pass_selection_branch = new_tree->Branch( "pass_selection", &pass_selection, "pass_selection/I");
			
		}

		void fill_new_branches(){
			// Only new branches need to be filled!
			// Don't do Fill on TTree, bc then event will be dublicated!
			pass_isolep_cut_branch->Fill();
			pass_min_chargedparticles_jet_cut_branch->Fill();
			pass_min_particles_jet_cut_branch->Fill();
			pass_min_Ejet_cut_branch->Fill();
			pass_y_34_cut_branch->Fill();
			pass_miss_mass_cut_branch->Fill();
			pass_E_T_vis_cut_branch->Fill();
			pass_p_T_vis_cut_branch->Fill();
			pass_cosTheta_miss_cut_branch->Fill();
			pass_cosTheta_EmaxTrack_cut_branch->Fill();
			pass_E_10cone_EmaxTrack_cut_branch->Fill();
			 
			pass_selection_branch->Fill();
		}

};
