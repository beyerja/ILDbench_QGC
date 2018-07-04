#include "JakobsVBSProcessor.h"


void JakobsVBSProcessor::SetTJMCsInJetsBranches(EventInfo &info) {

	std::vector<std::string> short_names = {
		"Lep",
		"ChHad",
		"NeHad",
		"Gamma"
	};

	std::vector<JetParticleTypeInfo*> type_infos = {
		&info.TJ_jetinfo.jet_leptons,
		&info.TJ_jetinfo.jet_charged_hadrons,
		&info.TJ_jetinfo.jet_neutral_hadrons,
		&info.TJ_jetinfo.jet_photons
	};

	
	for (int i; i<short_names.size(); i++) {
		std::string short_name = short_names[i];
		JetParticleTypeInfo* type_info = type_infos[i];
		
		_datatrain->Branch( ("TJ_MC" + short_name + "_exists").c_str(),
			(int*)&(type_info->type_MCs.exists),
			"exists[max_matrix_size]/I" );
		
		_datatrain->Branch( ("TJ_MC" + short_name + "_E").c_str(), 
			&(type_info->type_MCs.MC_E), 
			"MC_E[max_matrix_size]/F" );
		
		_datatrain->Branch( ("TJ_MC" + short_name + "_pdgID").c_str(), 
			&(type_info->type_MCs.MC_pdgID), 
			"MC_pdgID[max_matrix_size]/I" );
		
		_datatrain->Branch( ("TJ_MC" + short_name + "_highest_trck_weight_in_this_jet").c_str(), 
			&(type_info->type_MCs.highest_trck_weight_in_this_jet), 
			"highest_trck_weight_in_this_jet[max_matrix_size]/F" );
		
		_datatrain->Branch( ("TJ_MC" + short_name + "_highest_calo_weight_in_this_jet").c_str(), 
			&(type_info->type_MCs.highest_calo_weight_in_this_jet), 
			"highest_calo_weight_in_this_jet[max_matrix_size]/F" );
		
		_datatrain->Branch( ("TJ_MC" + short_name + 
							"_highest_trck_weight_in_any_recoparticle").c_str(), 
			&(type_info->type_MCs.highest_trck_weight_in_any_recoparticle), 
			"highest_trck_weight_in_any_recoparticle[max_matrix_size]/F" );
		
		_datatrain->Branch( ("TJ_MC" + short_name + 
							"_highest_calo_weight_in_any_recoparticle").c_str(), 
			&(type_info->type_MCs.highest_calo_weight_in_any_recoparticle), 
			"highest_calo_weight_in_any_recoparticle[max_matrix_size]/F" );
		
		_datatrain->Branch( ("TJ_MC" + short_name + 
							"_highest_trck_weight_recoparticle_IDd_as_" + short_name).c_str(), 
			&(type_info->type_MCs.highest_trck_weight_recoparticle_IDd_as_type),
			"highest_trck_weight_recoparticle_IDd_as_type[max_matrix_size]/I" );
		
		_datatrain->Branch( ("TJ_MC" + short_name + 
							"_highest_calo_weight_recoparticle_IDd_as_" + short_name).c_str(), 
			&(type_info->type_MCs.highest_calo_weight_recoparticle_IDd_as_type),
			"highest_calo_weight_recoparticle_IDd_as_type[max_matrix_size]/I" );
	}

}

