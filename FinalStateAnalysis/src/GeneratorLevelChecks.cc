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


// "TrueGenLevel" is a bit redundant
std::vector<MCParticle*> JakobsVBSProcessor::findTrueGenLevelNeutrinos(std::vector<MCParticle*> MCparticles) {
	/* 	General assumption here: I can just take the first two neutrinos that occur.
 	*/
	int nMC = MCparticles.size();
	int nu_pdgs[] = {12, 14, 16, 18};
	std::vector<MCParticle*> gen_level_nus;
	
	for(int i=0; i<nMC; i++){
		int particlePDG = MCparticles[i]->getPDG();
		if ( std::find(std::begin(nu_pdgs), std::end(nu_pdgs), fabs(particlePDG)) != std::end(nu_pdgs)) {
			gen_level_nus.push_back(MCparticles[i]);
		}
		if ( gen_level_nus.size() == 2 ) {break;}
	}
	return gen_level_nus;
}


std::vector<MCParticle*> JakobsVBSProcessor::findTrueGenLevelQuarks(std::vector<MCParticle*> MCparticles) {
	int nMC = MCparticles.size();

	std::vector<MCParticle*> gen_level_quarks;
	
	for(int i=0; i<nMC; i++){
		if ( fabs(MCparticles[i]->getPDG()) < 9 ) {
			gen_level_quarks.push_back(MCparticles[i]);
		}
		if ( gen_level_quarks.size() == 4 ) {break;}
	}
	return gen_level_quarks;
}


void JakobsVBSProcessor::findAllowedVVCandidates ( 
		std::vector<std::vector<std::vector<MCParticle*>>> all_VV_candidates , 
		std::vector<std::vector<std::vector<MCParticle*>>> &allowed_VV_candidates , 
		std::string VV_name ) {
	/* Find those pairs in all (q q) (q q) combinations that by flavour, charge
 	and masses may have come from a W or Z 
 	*/

	std::vector<std::vector<float>> candidates_masses;
	getVVCandidatesMasses( all_VV_candidates, candidates_masses );

	int n_VV_candidates = all_VV_candidates.size();
	for(int i=0; i<n_VV_candidates; i++){

		MCParticle*	V1_q1 = all_VV_candidates[i][0][0];
		MCParticle*	V1_q2 = all_VV_candidates[i][0][1];
		MCParticle*	V2_q1 = all_VV_candidates[i][1][0];
		MCParticle*	V2_q2 = all_VV_candidates[i][1][1];
		
		std::vector<MCParticle*> V1_candidate {V1_q1, V1_q2};
		std::vector<MCParticle*> V2_candidate {V2_q1, V2_q2};
		std::vector<std::vector<MCParticle*>> VV_candidate {V1_candidate, V2_candidate}; 
		
		float mV1 = candidates_masses[i][0];
		float mV2 = candidates_masses[i][1];
		
		// Check that V masses are not more than 20GeV apart
		if ( fabs(mV1 - mV2) > 20 ) { continue; }
		
		if ( VV_name == "ZZ" ) {
			// Is ZZ candidate if each pair consists of q and qbar of same flavour
			if ( (V1_q1->getPDG() == -1*V1_q2->getPDG()) && (V2_q1->getPDG() == -1*V2_q2->getPDG()) ) {
				// Check if V within Z mass window
				if ( ( mV1 + mV2 > 171.0 ) && ( mV1 + mV2 < 195 ) ) { 
					allowed_VV_candidates.push_back(VV_candidate);	
				}
			}
		}
		
		else if ( VV_name == "WW" ) {
			// Is WW candidate if pair consists of (q anti-q) (q anti-q) and in each 
			// (q anti-q) pair one is up-type and one down-type
			if (( V1_q1->getPDG() * V1_q2->getPDG() < 0 ) && ( V2_q1->getPDG() * V2_q2->getPDG() < 0 ) &&
				(	( (V1_q1->getPDG() % 2 == 0) && (V1_q2->getPDG() % 2 != 0 ) ) ||
					( (V1_q1->getPDG() % 2 != 0) && (V1_q2->getPDG() % 2 == 0 ) )		) &&
				(	( (V2_q1->getPDG() % 2 == 0) && (V2_q2->getPDG() % 2 != 0 ) ) ||
					( (V2_q1->getPDG() % 2 != 0) && (V2_q2->getPDG() % 2 == 0 ) )		) ) { 				
				// Check if V within W mass window
				if ( ( mV1 + mV2 > 147.0 ) && ( mV1 + mV2 < 171.0 ) ) {
					allowed_VV_candidates.push_back(VV_candidate);
				}
			}
		}
	}
}


void JakobsVBSProcessor::findTrueEventType(std::vector<MCParticle*> MCparticles, EventInfo &info) {
	/* Read MC particles of event and (more or less) guess the intermediate state which occured.
 	MC generator produces final state with full matrix element, so intermediate particles can not
	be read since they are theoretically unknown. 
	-> Search for vector boson pair by looking at quark flavour and pair masses */
	/* Identification based on cuts in 2009 ILD Workshop by Ward&Yan and modified
 	to take into which quark flavour final states are possible for W and Z decays*/

	/* First exclude neutrinos that could come from Z
	-> those with combined invariant mass < 100GeV
	-> those with generations other than electron neutrino bc e->W+nu always ends up with electron neutrino */
	std::vector<MCParticle*> gen_level_nus = findTrueGenLevelNeutrinos(MCparticles);
	TLorentzVector nu1_tlv = TLorentzVector(gen_level_nus[0]->getMomentum(),gen_level_nus[0]->getEnergy());	
	TLorentzVector nu2_tlv = TLorentzVector(gen_level_nus[1]->getMomentum(),gen_level_nus[1]->getEnergy());	
	float m_nunu = (nu1_tlv+nu2_tlv).M();
	int nu1_pdgID = gen_level_nus[0]->getPDG();
	int nu2_pdgID = gen_level_nus[1]->getPDG();
	if ( (m_nunu < 100) || (fabs(nu1_pdgID) != 12) || (fabs(nu2_pdgID) != 12) ) {
		info.gen_level.eventType = 0;
		return;
	}

	// Based on quark flavour: 
	// 	- Z can only decay to same flavour quark antiquark pair
	// 	- W decay to any quark antiquark pair where one is down and one is up
	// and invariant masses of the quark-antiquark combinations (= V candidate)
	//
	//	-> Search for all possible WW and ZZ candidates using the flavour and mass criteria
	//	-> Then search for best pair using pair finder and classify event as WW or ZZ according
	//		to whether best pair is WW or ZZ 
	//		(however ambiguity after looking at flavours and masses should almost never appear)

	std::vector<MCParticle*> gen_level_quarks = findTrueGenLevelQuarks(MCparticles);

	std::vector<std::vector<std::vector<MCParticle*>>> all_VV_candidates;
	getAllVVTo4ParticleCombinations( gen_level_quarks , all_VV_candidates );

	std::vector<std::vector<std::vector<MCParticle*>>> ZZ_candidates;
	findAllowedVVCandidates( all_VV_candidates, ZZ_candidates, "ZZ" );

	std::vector<std::vector<std::vector<MCParticle*>>> WW_candidates;
	findAllowedVVCandidates( all_VV_candidates, WW_candidates, "WW" );

	if ( ( ZZ_candidates.size() != 0) && ( WW_candidates.size() != 0) ) { streamlog_out(DEBUG) << " BOTH NOT EMPTY!!!" << std::endl;}

	if ( ( ZZ_candidates.size() == 0) && ( WW_candidates.size() == 0) ) {
		// Does not fulfill requirements to be WW or ZZ
		info.gen_level.eventType = 0;
		return;
	}
	else {
		// Is either WW or ZZ.
		// Now search within all possible pairs for best VV combination (by mass criteria).
		// -> Then by index check if it was WW or ZZ and fill event information

		// Combine to all allowed VV candidates
		std::vector<std::vector<std::vector<MCParticle*>>> allowed_VV_candidates;
		allowed_VV_candidates.reserve( ZZ_candidates.size() + WW_candidates.size() ); // preallocate memory
		allowed_VV_candidates.insert( allowed_VV_candidates.end(), ZZ_candidates.begin(), ZZ_candidates.end() );
		allowed_VV_candidates.insert( allowed_VV_candidates.end(), WW_candidates.begin(), WW_candidates.end() );

		int best_pair_index = findIndexBestPairInVVCandidates( allowed_VV_candidates );

		if ( best_pair_index < ZZ_candidates.size() ) {
			info.gen_level.eventType = 2; // is ZZ
		}
		else {
			info.gen_level.eventType = 1; // is WW
		}

		fillVPairInformation(allowed_VV_candidates[best_pair_index][0], allowed_VV_candidates[best_pair_index][1], info.gen_level.observ);
	}
	

}


void  JakobsVBSProcessor::analyseGeneratorLevelQuarks(std::vector<MCParticle*> MCparticles, EventInfo &info) {
	std::vector<MCParticle*> gen_level_quarks = findTrueGenLevelQuarks(MCparticles);
	
	for (int i=0; i<4; i++) {
		MCParticle* quark = gen_level_quarks[i];
		info.gen_level.quarks[i].pdg_id	= quark->getPDG();	
		info.gen_level.quarks[i].tlv = TLorentzVector( quark->getMomentum(), quark->getEnergy() ); 
		info.gen_level.quarks[i].process_tlv();
	}		
}


void JakobsVBSProcessor::analyseGeneratorLevelNeutrinos(std::vector<MCParticle*> MCparticles,EventInfo &info) {
	std::vector<MCParticle*> gen_level_nus = findTrueGenLevelNeutrinos(MCparticles);
	info.gen_level.nu1.tlv = TLorentzVector(gen_level_nus[0]->getMomentum(),
											gen_level_nus[0]->getEnergy());
	info.gen_level.nu2.tlv = TLorentzVector(gen_level_nus[1]->getMomentum(),
											gen_level_nus[1]->getEnergy());

	info.gen_level.nu1.pdg_id = gen_level_nus[0]->getPDG();
	info.gen_level.nu2.pdg_id = gen_level_nus[1]->getPDG();

	info.gen_level.nu1.process_tlv();
	info.gen_level.nu2.process_tlv();
}
