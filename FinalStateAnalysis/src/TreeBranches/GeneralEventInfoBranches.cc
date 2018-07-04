#include "JakobsVBSProcessor.h"


void JakobsVBSProcessor::SetGeneralEventInfoBranches( EventInfo &info ) {
	// Event-specfic
	_datatrain->Branch( "evt"     , 		&info.evtN, 			"evt/I" );
}


