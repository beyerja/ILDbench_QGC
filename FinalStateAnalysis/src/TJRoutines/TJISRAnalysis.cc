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


void JakobsVBSProcessor::analyseGeneratorLevelISR(std::vector<ReconstructedParticleVec> TJ_jetPFOs_fromISR, EventInfo &info) {
	/* Analyse jet PFOs originating from ISR using the generator level information gained  through TrueJet.
	*/

	int Njets_fromISR = TJ_jetPFOs_fromISR.size();

	// Add TLorentzvectors of Jet particles to TLV of total ISR
	for (int ijet = 0; ijet<Njets_fromISR; ijet++) {
		ReconstructedParticleVec jet_i = TJ_jetPFOs_fromISR[ijet];
		for ( int iPFO = 0; iPFO<jet_i.size(); iPFO++ ) {
			TLorentzVector jetPFO_fromISR = TLorentzVector( jet_i[iPFO]->getMomentum(), jet_i[iPFO]->getEnergy() );
			
			info.gen_level.isr_inJets.ISR_tlv += jetPFO_fromISR;
		}
	}
	
	info.gen_level.isr_inJets.processTLV();
}

