#include "../include/plotter.h"

// Include the plotter source files
#include "../plotters/test_plotter.cpp"
#include "../plotters/mjjmjj_plotter.cpp"
#include "../plotters/theta_m_plotter.cpp"
#include "../plotters/trueISR_E_over_jetmass.cpp"
#include "../plotters/tail_plotter.cpp"
#include "../plotters/all_selected_plotter.cpp"
#include "../plotters/mjj_reco_quark_pdg_plotter.cpp"
#include "../plotters/individual_quarks_plotter.cpp"
#include "../plotters/cutflow_plotter.cpp"
#include "../plotters/TJ/TJ_jet_level_plotter.cpp"
#include "../plotters/TJ/TJ_jet_mass_investigation_plotter.cpp"
#include "../plotters/TJ/TJ_lepton_in_jet_plotter.cpp"
#include "../plotters/TJ/TJ_charged_hadron_in_jet_plotter.cpp"
#include "../plotters/TJ/TJ_neutral_hadron_in_jet_plotter.cpp"
#include "../plotters/TJ/TJ_photon_in_jet_plotter.cpp"
#include "../plotters/TJ/TJ_Nparticles_plotter.cpp"
#include "../plotters/TJ/TJ_tos_s_E_correction_plotter.cpp"
#include "../plotters/TJ/TJ_tos_s_p_correction_plotter.cpp"
#include "../plotters/TJ/TJ_corrected_jets_plotter.cpp"
#include "../plotters/TJ/TJ_corrected_lowE_jets_plotter.cpp"
#include "../plotters/TJ/TJ_charged_hadron_in_corrected_jet_plotter.cpp"
#include "../plotters/TJ/TJ_2nus4qs_plotter.cpp"
#include "../plotters/TJ/TJ_total_E_plotter.cpp"
#include "../plotters/TJ/TJ_observ_to_icns_plotter.cpp"
#include "../plotters/TJ/TJ_jet_energy_resolution_plotter.cpp"
#include "../plotters/TJ/TJ_observ_to_icns_uds_only_plotter.cpp"
#include "../plotters/TJ/TJ_observ_to_icns_udsc_only_plotter.cpp"

void set_plotters(vector<Plotter*> &plotters) {

	/* ---------------------------------------------------------------------*/
	// INSERT THE PLOTTER MODULES HERE

	//plotters.push_back( new TestPlotter );
	//plotters.push_back( new MjjMjjPlotter );
	//plotters.push_back( new ThetaMPlotter );
	//plotters.push_back( new TrueISRvsMjjPlotter );
	//plotters.push_back( new TailPlotter );
	//plotters.push_back( new AllSelectedPlotter );
	//plotters.push_back( new RecoMjjVSQuarkPDGPlotter );
	//plotters.push_back( new IndividualJetsPlotter );
	//plotters.push_back( new CutflowPlotter );
	plotters.push_back( new TJJetLevelsPlotter );
	//plotters.push_back( new TJJetMassInvestigationPlotter );
	//plotters.push_back( new TJLeptonsInJetsPlotter );
	//plotters.push_back( new TJChargedHadronsInJetsPlotter );
	//plotters.push_back( new TJNeutralHadronsInJetsPlotter );
	//plotters.push_back( new TJPhotonsInJetsPlotter );
	//plotters.push_back( new TJNParticlesPlotter );
	//plotters.push_back( new TJSeenTrueOfSeenECorrectionPlotter );
	//plotters.push_back( new TJSeenTrueOfSeenPCorrectionPlotter );
	//plotters.push_back( new TJCorrectedJetsPlotter );
	//plotters.push_back( new TJCorrectedLowEJetsPlotter );
	//plotters.push_back( new TJChargedHadronsInCorrectedJetsPlotter );
	//plotters.push_back( new TJQuarkJetsPlusNeutrinosPlotter );
	//plotters.push_back( new TJTotalEPlotter );
	//plotters.push_back( new TJObservToICNsPlotter );
	plotters.push_back( new TJJetEnergyResolutionPlotter );
	//plotters.push_back( new TJObservToICNsOnlyUDSPlotter );
	//plotters.push_back( new TJObservToICNsOnlyUDSCPlotter );
	/* ---------------------------------------------------------------------*/
}
