#include "JakobsVBSProcessor.h"


void JakobsVBSProcessor::SetGeneratorLevelBranches( EventInfo &info ) {
	// Naming 'true' is a bit unfortunate as we can't actually know the intermediate process
	// 'generator_level' would be more appropriate
	
	_datatrain->Branch( "true_evt_type",	&info.gen_level.eventType,	"true_evt_type/I");

	CreateObservablesBranch( _datatrain, &info.gen_level.observ, "true_observables" );	

	_datatrain->Branch( "partner_of_truthV1",	&info.Vpair_link.partner_of_truthpair1,		"partner_of_truthV1/I");
	_datatrain->Branch( "partner_of_truthV2",	&info.Vpair_link.partner_of_truthpair2,		"partner_of_truthV2/I");


	std::string true_particle_observables = 
					std::string("pdg_id/I:") +
					"E/F:" +
					"theta/F";

	for (int i=0; i<4; i++) {
		std::string index_string = std::to_string(i+1);

		_datatrain->Branch( ("true_quark" + index_string).c_str(), &info.gen_level.quarks[i],
							true_particle_observables.c_str() );
	}	


	_datatrain->Branch( "true_nu1", &info.gen_level.nu1, true_particle_observables.c_str() ); 
	_datatrain->Branch( "true_nu2", &info.gen_level.nu2, true_particle_observables.c_str() ); 
}


