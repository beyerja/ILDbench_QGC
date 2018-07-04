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



bool JakobsVBSProcessor::IsIntTypePDGID( int pdgID, float charge, std::string type ) {

	streamlog_out(DEBUG) << " Got: pdgID:" << pdgID << " charge:" << charge
						<< " searching for: " << type << " ->";

	if(	(type == "lepton") && (
		fabs(pdgID) == 11 ||
		fabs(pdgID) == 13 ||
		fabs(pdgID) == 15 ||
		fabs(pdgID) == 17 ) )	 {
			streamlog_out(DEBUG) << " found it.\n";
			return true;

	} else if(
		(type == "chargedhadron") && 
		(fabs(pdgID) > 37) && // Everything above that should be hadrons in our case
		(int(charge) != 0) ) { 
			streamlog_out(DEBUG) << " found it.\n";
			return true;

	} else if(
		(type == "neutralhadron") && 
		(fabs(pdgID) > 37) && // Everything above that should be hadrons in our case
		(int(charge) == 0) ) { 
			streamlog_out(DEBUG) << " found it.\n";
			return true;
	} else if(	
		(type == "photon") && (	fabs(pdgID) == 22 )	) {
			streamlog_out(DEBUG) << " found it.\n";
			return true;

	} else {
		streamlog_out(DEBUG) << " did not find it.\n";
		return false;
	}
}

bool JakobsVBSProcessor::IdentifiedAsType( ReconstructedParticle* PFO, std::string type ) {
	float charge = PFO->getCharge();


	if ( PFO->getParticleIDs().size() > 0 ) {
		if(	IsIntTypePDGID( fabs(PFO->getParticleIDs()[0]->getPDG()), charge, type ) ) {
			return true;
		} else {
			return false;
		}
	} else if ( PFO->getParticleIDUsed() != 0 ) {
		if ( IsIntTypePDGID( PFO->getParticleIDUsed()->getPDG(), charge, type ) ) {
			return true;
		} else {
			return false;
		}
	} else if ( IsIntTypePDGID( PFO->getType(), charge, type ) ) {
		return true;
	} else {
		return false;
	}
}


bool JakobsVBSProcessor::MCParticleIsType( MCParticle* MCparticle, std::string type ) {
	if ( IsIntTypePDGID( fabs(MCparticle->getPDG()), MCparticle->getCharge(), type ) ) {
		return true;
	} else {
		return false;
	}
}
