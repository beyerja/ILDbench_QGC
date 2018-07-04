#include "JakobsVBSProcessor.h"
#include <iostream>
#include <fstream>
#include <set>

#include <vector>
#include <stack>
#include <algorithm> // for std::find
#include <iterator> // for std::begin, std::end

//LCIO
#include <EVENT/LCCollection.h>
#include <EVENT/ReconstructedParticle.h>
#include <EVENT/Vertex.h>
#include "UTIL/PIDHandler.h"
#include "LCIOSTLTypes.h"

//----- include for verbosity dependend logging ---------
#include "marlin/VerbosityLevels.h"

//ROOT
#include "TFile.h"
#include "TTree.h"
#include "TMath.h"
#include "TVector3.h"
#include "TLorentzVector.h"


using namespace lcio;
using namespace marlin;



void JakobsVBSProcessor::calculateTJTotalLevel( 
		ReconstructedParticleVec* particles, TJLevelInfo &level_info ) {

	float total_E = 0;
	for (int i=0; i<particles->size(); i++) {
		ReconstructedParticle* particle = particles->at(i);

		total_E += particle->getEnergy();
	}

	level_info.total_E = total_E;
}


void JakobsVBSProcessor::calculateTJTotalLevels( EventInfo &info ) {

	ReconstructedParticleVec* final_colour_neutrals = getFinalcn();
	calculateTJTotalLevel( final_colour_neutrals, info.TJ_eventinfo.final_cn_system );

	ReconstructedParticleVec* initial_colour_neutrals = getInitialcn();
	calculateTJTotalLevel( initial_colour_neutrals, info.TJ_eventinfo.initial_cn_system );

	ReconstructedParticleVec* TJ_jets = getJets();
	calculateTJTotalLevel( TJ_jets, info.TJ_eventinfo.reco_system );
}
