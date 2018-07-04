#include "JakobsVBSProcessor.h"
#include <iostream>
#include <fstream>
#include <set>
#include <math.h>

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


void JakobsVBSProcessor::findBestGeneratorToRecoVPairing(EventInfo &info) {
	/* Find which reconstructed vector boson fits best to which generator level vector boson 
 	using angular distance parameter DeltaR */

	TLorentzVector true_pair1_tlv = info.gen_level.observ.pair1_tlv;
	TLorentzVector true_pair2_tlv = info.gen_level.observ.pair2_tlv;

	TLorentzVector reco_pair1_tlv = info.observ.pair1_tlv;
	TLorentzVector reco_pair2_tlv = info.observ.pair2_tlv;

	// DeltaR^2 + DeltaR^2
	float distance_I	= std::pow(true_pair1_tlv.DeltaR(reco_pair1_tlv), 2) + std::pow(true_pair2_tlv.DeltaR(reco_pair2_tlv), 2); 
	float distance_II 	= std::pow(true_pair1_tlv.DeltaR(reco_pair2_tlv), 2) + std::pow(true_pair2_tlv.DeltaR(reco_pair1_tlv), 2); 
	
	// Combination with smaller distances taken as better fit
	if ( distance_I < distance_II ) {
		info.Vpair_link.partner_of_truthpair1 = 1;
		info.Vpair_link.partner_of_truthpair2 = 2;
	} else {
		info.Vpair_link.partner_of_truthpair1 = 2;
		info.Vpair_link.partner_of_truthpair2 = 1;
	}
} 
