#include "eventClass.C"


void eventSelection(TreeEvent &event) {
	/* Takes eventClass instance, test if the included values 
 	pass the cuts, and sets the pass_selection flag to 0 (not 
	passed) or 1 (passed).
	
	Cuts are currently taken from Notes to ILD LoI (2010).
	 */

	event.pass_selection = 0;

	if ( event.min_num_chargedparticles_jet < 2 ) 	{ return; } 
	else if ( event.min_num_particles_jet < 3 ) 	{ return; }
	else if ( event.min_Ejet < 10 )					{ return; }
	else if ( event.y_34 < 0.0001 ) 				{ return; }
	else if ( event.miss_mass < 200 )				{ return; }
	else if ( event.E_T_vis < 150 )					{ return; }
	else if ( event.p_T_vis < 40 )					{ return; }
	else if ( fabs(event.cosTheta_miss) > 0.99 )	{ return; }
	else if ( fabs(event.cosTheta_EmaxTrack) > 0.99){ return; }
	else if ( event.E_10cone_EmaxTrack < 2.0 ) 		{ return; }
		



	event.pass_selection = 1;

}
