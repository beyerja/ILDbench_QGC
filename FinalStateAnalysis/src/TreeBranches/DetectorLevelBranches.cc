#include "JakobsVBSProcessor.h"



void JakobsVBSProcessor::SetDetectorLevelBranches( EventInfo &info ) {
	// Actual observables as they can be used in analysis of ILC data 
	
	CreateObservablesBranch( _datatrain, &info.observ, "observables" );
	//_datatrain->Branch( "observables", &info.observ, obervable_variables.c_str() );

	std::string jet_observables = 
					std::string("E:") +
					"theta:" +
					"mass";	

	for (int i=0; i<__Njets; i++) {
		std::string index_string = std::to_string(i+1);

		_datatrain->Branch( ("jet_" + index_string).c_str(), &info.observ.jets[i],
							jet_observables.c_str() ); 
	}

}


