#include "JakobsVBSProcessor.h"

using namespace lcio;
using namespace marlin;

/* Functions that read in TrueJet_Parser output and return
   vector containing the Particles in their corresponding
	LCIO format and with the required information */


std::vector<ReconstructedParticleVec> JakobsVBSProcessor::getTJAllJets() {	

	std::vector<ReconstructedParticleVec> TJ_all_jetPFOs_in_detector;

	for (int ijet=0 ; ijet<njets() ; ijet++ ) {
		if (  seen_partics(ijet).size() > 0 ) {
			ReconstructedParticleVec ijetPFOs = seen_partics(ijet); 
			TJ_all_jetPFOs_in_detector.push_back(ijetPFOs);
		}
	}

	return TJ_all_jetPFOs_in_detector;
}



std::vector<TLorentzVector*> JakobsVBSProcessor::getTJTrueValueTLVsOfAllJets() {
	std::vector<TLorentzVector*> true_value_jet_tlvs;
	for (int ijet=0 ; ijet<njets() ; ijet++ ) {
		const double* p4 = p4seen(ijet);
		TLorentzVector* true_value_ijet_tlv = new TLorentzVector( p4[0], p4[1], p4[2], p4[3]);
		true_value_jet_tlvs.push_back(true_value_ijet_tlv);	
	}
	return true_value_jet_tlvs;
}

