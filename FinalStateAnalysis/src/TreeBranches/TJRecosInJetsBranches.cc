#include "JakobsVBSProcessor.h"


void JakobsVBSProcessor::SetTJRecosInJetsBranches(EventInfo &info) {

	// Feed array into tree as 1D -> read out again as 1D anyway

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

		_datatrain->Branch( ("TJ_Reco" + short_name + "_exists").c_str(),
						&(type_info->recos_IDd_as_type.exists),
						"exists[max_matrix_size]/I" );
		
		_datatrain->Branch( ("TJ_Reco" + short_name + "_E").c_str(), 
						&(type_info->recos_IDd_as_type.reco_E), 
						"reco_E[max_matrix_size]/F" );
		_datatrain->Branch( ("TJ_Reco" + short_name + "_highest_MC" 
							+ short_name + "_trck_weight").c_str(), 
						&(type_info->recos_IDd_as_type.highest_type_trck_weight), 
						"highest_type_trck_weight[max_matrix_size]/F" );
		_datatrain->Branch( ("TJ_Reco" + short_name + "_highest_MC" 
							+ short_name + "_calo_weight").c_str(), 
						&(type_info->recos_IDd_as_type.highest_type_calo_weight), 
						"highest_type_calo_weight[max_matrix_size]/F" );
	}
}

