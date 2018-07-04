#include "eventClass.C"

void dont_pass_cut ( TreeEvent &event, int &pass_cut ) {
	pass_cut = 0;
	event.pass_selection = 0;
}

void eventSelection(TreeEvent &event) {
	/* Takes eventClass instance, test if the included values 
 	pass the cuts, and sets the pass_selection flag to 0 (not 
	passed) or 1 (passed).
	
	Cuts are currently taken from Notes to ILD LoI (2010).
	 */

	event.pass_selection = 1;

	// Start all cuts as passed, then change if it doesn't pass 
	event.pass_isolep_cut = event.pass_min_chargedparticles_jet_cut = event.pass_min_particles_jet_cut 
	= event.pass_min_Ejet_cut = event.pass_y_34_cut = event.pass_miss_mass_cut = event.pass_E_T_vis_cut 
	= event.pass_p_T_vis_cut = event.pass_cosTheta_miss_cut = event.pass_cosTheta_EmaxTrack_cut 
	= event.pass_E_10cone_EmaxTrack_cut = 1;		



	// Starts out with flag being set to 0 (= selection not passed)
	// If any of the cuts are failed: return before flag can be set to 1
	if ( event.num_isolated_leptons > 0 )			{ dont_pass_cut(event, event.pass_isolep_cut); }
	if ( event.min_num_chargedparticles_jet < 2 )	{ dont_pass_cut(event, event.pass_min_chargedparticles_jet_cut); } 
	if ( event.min_num_particles_jet < 3 ) 			{ dont_pass_cut(event, event.pass_min_particles_jet_cut); }
	if ( event.min_Ejet < 10 )						{ dont_pass_cut(event, event.pass_min_Ejet_cut); }
	if ( event.y_34 < 0.0001 ) 						{ dont_pass_cut(event, event.pass_y_34_cut); }
	if ( event.miss_mass < 200 )					{ dont_pass_cut(event, event.pass_miss_mass_cut); }
	if ( event.E_T_vis < 150 )						{ dont_pass_cut(event, event.pass_E_T_vis_cut); }
	if ( event.p_T_vis < 40 )						{ dont_pass_cut(event, event.pass_p_T_vis_cut); }
	if ( fabs(event.cosTheta_miss) > 0.99 )			{ dont_pass_cut(event, event.pass_cosTheta_miss_cut); }
	if ( fabs(event.cosTheta_EmaxTrack) > 0.99)		{ dont_pass_cut(event, event.pass_cosTheta_EmaxTrack_cut); }
	if ( event.E_10cone_EmaxTrack < 2.0 ) 			{ dont_pass_cut(event, event.pass_E_10cone_EmaxTrack_cut); }


}
