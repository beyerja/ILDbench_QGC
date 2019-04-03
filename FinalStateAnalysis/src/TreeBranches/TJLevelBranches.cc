#include "JakobsVBSProcessor.h"


void JakobsVBSProcessor::SetTrueJetLevelBranches( EventInfo &info ) {

	// ISR from TJ
	_datatrain->Branch( "true_ISR_inJets_E",		&info.gen_level.isr_inJets.E ,		"true_ISR_inJets_E/F" );
	_datatrain->Branch( "true_ISR_inJets_p",		&info.gen_level.isr_inJets.p ,		"true_ISR_inJets_p/F" );
	_datatrain->Branch( "true_ISR_inJets_theta",	&info.gen_level.isr_inJets.theta ,	"true_ISR_inJets_theta/F" );


	// Information for total level:
	_datatrain->Branch( "TJ_final_cn_total_E", &info.TJ_eventinfo.final_cn_system.total_E,
						"TJ_final_cn_total_E" );
	_datatrain->Branch( "TJ_initial_cn_total_E", &info.TJ_eventinfo.initial_cn_system.total_E,
						"TJ_initial_cn_total_E" );
	_datatrain->Branch( "TJ_reco_total_E", &info.TJ_eventinfo.reco_system.total_E,
						"TJ_reco_total_E" );



	// Store my used maximum number of jets so that I can store the observables as arrays
	_datatrain->Branch( "max_Njets", &info.max_Njets, "max_Njets/I" );
	_datatrain->Branch( "max_Nparticles", &info.max_Nparticles, "max_Nparticles/I" );
	_datatrain->Branch( "max_matrix_size", &info.max_matrix_size, "max_matrix_size/I" );


	_datatrain->Branch( "TJ_jets_exist",
						&info.TJ_jetinfo.exists,
						"exists[max_Njets]/I");
	_datatrain->Branch( "TJ_jets_final_elementon_pdgID",
						&info.TJ_jetinfo.final_elementon_pdgID,
						"final_elementon_pdgID[max_Njets]/I" );


	std::vector<std::string> level_names = {
							"final_elementon",
							"true_of_detectable",
							"detected",
							"true_jet"
							};

	std::vector<TrueJetLevel*> levels = {
							&info.TJ_jetinfo.final_elementon,
							&info.TJ_jetinfo.true_value_of_detectable_jet,
							&info.TJ_jetinfo.detected,
							&info.TJ_jetinfo.true_jet
							};

	for ( int i = 0; i<level_names.size(); i++ ) {
		_datatrain->Branch( ( "TJ_jets_" + level_names[i] + "_E" ).c_str(),
							&levels[i]->E,
							"E[max_Njets]/F" );
		_datatrain->Branch( ( "TJ_jets_" + level_names[i] + "_p" ).c_str(),
							&levels[i]->p,
							"p[max_Njets]/F" );
		_datatrain->Branch( ( "TJ_jets_" + level_names[i] + "_mass" ).c_str(),
							&levels[i]->mass,
							"mass[max_Njets]/F" );
	}

	// Comparison of jets that are correctly clustered from reco w/o overlay (using TrueJet) to those I clustered myself
	_datatrain->Branch( "NPFOs_inCorrectJets",	&info.correctRecoJets_to_ownClusteredJets.N_PFOs_inLevel1, 	"NPFOs_inCorrectJets/I");
	_datatrain->Branch( "NPFOs_inClusteredJets",&info.correctRecoJets_to_ownClusteredJets.N_PFOs_inLevel2, 	"NPFOs_inClusteredJets/I");

	_datatrain->Branch( "NPFOs_inCorrectJets_notinClusteredJets",
						&info.correctRecoJets_to_ownClusteredJets.N_PFOs_inLevel1_notinLevel2,
						"NPFOs_inCorrectJets_notinClusteredJets/I" );
	_datatrain->Branch( "NPFOs_inClusteredJets_notinCorrectJets",
						&info.correctRecoJets_to_ownClusteredJets.N_PFOs_inLevel2_notinLevel1,
						"NPFOs_inClusteredJets_notinCorrectJets/I" );


	// Branches from using TJ to cheat certains steps of processor
	_datatrain->Branch( "masses_cheated_clustering_and_pairing",	&info.TJ_eventinfo.TJjets_from_initital_cn,
											"pair1_mass/F:pair2_mass/F");
	_datatrain->Branch( "masses_cheated_clustering",	&info.TJ_eventinfo.TJjets_custom_pairing,
											"pair1_mass/F:pair2_mass/F");
	_datatrain->Branch( "masses_cheated_overlay_removal",	&info.observ_cheated_overlay,
											"pair1_mass/F:pair2_mass/F");
}













		/*_datatrain->Branch( ( "TJ_jets_" + level_names[i] + "_theta" ).c_str(),
							&levels[i]->theta,
							"theta[__max_Njets]/F" );
		_datatrain->Branch( ( "TJ_jets_" + level_names[i] + "_num_electrons" ).c_str(),
							&levels[i]->num_electrons,
							"num_electrons[__max_Njets]/I" );
		_datatrain->Branch( ( "TJ_jets_" + level_names[i] + "_num_muons" ).c_str(),
							&levels[i]->num_muons,
							"num_muons[__max_Njets]/I" );
		_datatrain->Branch( ( "TJ_jets_" + level_names[i] + "_num_taus" ).c_str(),
							&levels[i]->num_taus,
							"num_taus[__max_Njets]/I" );
		_datatrain->Branch( ( "TJ_jets_" + level_names[i] + "_num_neutrinos" ).c_str(),
							&levels[i]->num_neutrinos,
							"num_neutrinos[__max_Njets]/I" );
		_datatrain->Branch( ( "TJ_jets_" + level_names[i] + "_charged_leptons_E" ).c_str(),
							&levels[i]->charged_leptons_E,
							"charged_leptons_E[__max_Njets]/F" );
		_datatrain->Branch( ( "TJ_jets_" + level_names[i] + "_charged_leptons_p" ).c_str(),
							&levels[i]->charged_leptons_p,
							"charged_leptons_p[__max_Njets]/F" );
		_datatrain->Branch( ( "TJ_jets_" + level_names[i] + "_charged_leptons_mass" ).c_str(),
							&levels[i]->charged_leptons_mass,
							"charged_leptons_mass[__max_Njets]/F" );
		_datatrain->Branch( ( "TJ_jets_" + level_names[i] + "_neutrinos_E" ).c_str(),
							&levels[i]->neutrinos_E,
							"neutrinos_E[__max_Njets]/F" );
		_datatrain->Branch( ( "TJ_jets_" + level_names[i] + "_neutrinos_p" ).c_str(),
							&levels[i]->neutrinos_p,
							"neutrinos_p[__max_Njets]/F" );
		_datatrain->Branch( ( "TJ_jets_" + level_names[i] + "_neutrinos_mass" ).c_str(),
							&levels[i]->neutrinos_mass,
							"neutrinos_mass[__max_Njets]/F" );*/
