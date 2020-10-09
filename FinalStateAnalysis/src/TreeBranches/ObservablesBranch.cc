#include "JakobsVBSProcessor.h"

void JakobsVBSProcessor::CreateObservablesBranch( TTree* tree, Observables* observ_ptr, std::string branch_name ) {
	// Standardized way to read all variables from Observables class into TBranch

	std::string obervable_variables = 
					std::string("pair1_mass/F:") +
					"pair2_mass/F:" +
					"pair1_theta/F:" +
					"pair2_theta/F:" +
					"pair1_p/F:" +
					"pair2_p/F:" +
					"VV_mass/F:" +
					"miss_mass/F:" +
					"sum_E_q/F:" +
					"E_T_vis/F:" +
					"p_T_vis/F:" +
					"E_10cone_EmaxTrack/F:" +
					"cosTheta_miss/F:" +
					"cosTheta_EmaxTrack/F:" +
					"y_34/F:" +
					"min_Ejet/F:" +
					"min_num_particles_jet/I:" +
					"min_num_chargedparticles_jet/I:" +
					"num_isolated_leptons/I";

	tree->Branch( branch_name.c_str(), observ_ptr, obervable_variables.c_str() );
}
