#include "JakobsVBSProcessor.h"
#include <iostream>
#include <fstream>
#include <set>

#include <vector>
#include <set>
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


// TODO Move this to different File!!!
void JakobsVBSProcessor::SplitWeight( 
		FloatVec &combined_weights, FloatVec &single_weights, 
		std::string weight_name ) {

	for (int i=0; i<combined_weights.size(); i++) {
		if ( weight_name == "trck" ) {
			single_weights.push_back(double((int(combined_weights[i])%10000)/1000.));
		}
		else if ( weight_name == "calo" ) {
			single_weights.push_back(double((int(combined_weights[i])/10000)/1000.));
		} 
	} 
}


void JakobsVBSProcessor::GetMCParticlesLinkedToPFO( 
		ReconstructedParticle* PFO, LCRelationNavigator* relation_recoMCtruth, 
		MCParticleVec &MCParticle_vector) {

	LCObjectVec object_vector	= relation_recoMCtruth->getRelatedToObjects( PFO );
	for ( int iMC = 0; iMC<object_vector.size(); iMC++ ) {
		MCParticle* MC = dynamic_cast<MCParticle*>(object_vector[iMC]);
		MCParticle_vector.push_back(MC);
	}
}


void JakobsVBSProcessor::SearchSingleTJJetMC (
		MCParticle* MC,	LCRelationNavigator* relation_recoMCtruth,
		TrueJetSingleJetInfo &TJ_jetinfo ) {

	/* ------------------------------------------------ */
	/* HERE Functions to act on single TJJet MCParticle */
	streamlog_out(DEBUG1) << "In SearchSingleTJJetMC: Processing Single MC " << std::endl; 
	streamlog_out(DEBUG1) << "In SearchSingleTJJetMC: Looking at leptons. " << std::endl; 
	TJProcessSingleTypeMC( MC, relation_recoMCtruth, TJ_jetinfo.jet_leptons );
  streamlog_out(DEBUG1) << "In SearchSingleTJJetMC: Looking at charged hadrons. " << std::endl; 
	TJProcessSingleTypeMC( MC, relation_recoMCtruth, TJ_jetinfo.jet_charged_hadrons );
  streamlog_out(DEBUG1) << "In SearchSingleTJJetMC: Looking at neutral hadrons. " << std::endl; 
	TJProcessSingleTypeMC( MC, relation_recoMCtruth, TJ_jetinfo.jet_neutral_hadrons );
  streamlog_out(DEBUG1) << "In SearchSingleTJJetMC: Looking at photons. " << std::endl; 
	TJProcessSingleTypeMC( MC, relation_recoMCtruth, TJ_jetinfo.jet_photons );

	/* ------------------------------------------------ */
}


void JakobsVBSProcessor::FindLowestLevelMCParticles( 
		MCParticle* mc, std::set<MCParticle*> &jet_MCs_set ) {

	// Recursively search for lowest level of daughters and insert them in the set
	MCParticleVec daughters = mc->getDaughters();
	if ( daughters.size() > 0 ) {
		for ( int idaughter=0; idaughter<daughters.size(); idaughter++ ) {
			MCParticle* daughter = daughters[idaughter];
			// Very dirty fix to never accidentaly end up at initial particles
			if ( daughter->getEnergy() > 450 ) {continue;} 

			FindLowestLevelMCParticles( daughter, jet_MCs_set );			
		}
	}
	else {
		jet_MCs_set.insert(mc);
	}

}


void JakobsVBSProcessor::SearchSingleTJJetPFO ( 
		ReconstructedParticle* PFO, LCRelationNavigator* relation_recoMCtruth, 
		std::set<MCParticle*> &jet_MCs_set, TrueJetSingleJetInfo &TJ_jetinfo ) { 
	
	/* ----------------------------------------- */
	/* HERE Functions to act on single TJJet PFO */
	TJProcessSingleTypePFO( PFO, relation_recoMCtruth, TJ_jetinfo.jet_leptons );
	TJProcessSingleTypePFO( PFO, relation_recoMCtruth, TJ_jetinfo.jet_charged_hadrons );
	TJProcessSingleTypePFO( PFO, relation_recoMCtruth, TJ_jetinfo.jet_neutral_hadrons );
	TJProcessSingleTypePFO( PFO, relation_recoMCtruth, TJ_jetinfo.jet_photons );

	/* ----------------------------------------- */

	// Get MC Particles to this PFO and loop over them:
	MCParticleVec MCParticle_vector;
	GetMCParticlesLinkedToPFO( PFO, relation_recoMCtruth, MCParticle_vector); 
	
	for ( int iMC = 0; iMC<MCParticle_vector.size(); iMC++ ) {
		// Find lowest level MCParticles and insert them into the set
		FindLowestLevelMCParticles(MCParticle_vector[iMC], jet_MCs_set);
	}
}


void JakobsVBSProcessor::FindLowestLevelRecoParticles( 
		ReconstructedParticle* reco, std::set<ReconstructedParticle*> &jet_recos_set ) {

	// Recursively search for lowest level of reco daughters and insert them in the set
	if ( reco->isCompound() ) {
		ReconstructedParticleVec daughters = reco->getParticles();	
		for ( int idaughter=0; idaughter<daughters.size(); idaughter++ ) {
			ReconstructedParticle* daughter = daughters[idaughter];
			FindLowestLevelRecoParticles( daughter, jet_recos_set );			
		}
	}
	else {
		jet_recos_set.insert(reco);
	}

}

void JakobsVBSProcessor::SearchSingleTJJetParticles( 
		LCRelationNavigator* relation_recoMCtruth, TrueJetSingleJetInfo &TJ_jetinfo ) {

	int ijet = TJ_jetinfo.current_jet();

	// Set to prevent double-counting -> Only allows unique entries
	std::set<ReconstructedParticle*> jet_recos_set;
	std::set<MCParticle*> jet_MCs_set;

	streamlog_out(DEBUG3) << "In SearchSingleTJJetParticles: Start treating jet " << ijet << " recos \n";

	// Search for all (lowest level) reco-particles in the jet
	if (  seen_partics(ijet).size() > 0 ) {

		// seen_partics(...) is function of TrueJet_Parser, see header
		ReconstructedParticleVec ijet_PFOs = seen_partics(ijet); 

		for ( int ijetPFO = 0; ijetPFO<ijet_PFOs.size(); ijetPFO++ ) {
			ReconstructedParticle* jetPFO_i = ijet_PFOs[ijetPFO];
			
			FindLowestLevelRecoParticles(jetPFO_i, jet_recos_set);
		}
	
	}

	// Recos must be treated first because it includes finding the MCs
	streamlog_out(DEBUG3) << "In SearchSingleTJJetParticles: Start treating jet " << ijet << " recos \n";
	// The Iterator is a Pointer to my pointer to a ReconstructedParticle
	for (	std::set<ReconstructedParticle*>::iterator it=jet_recos_set.begin(); 
			it!=jet_recos_set.end(); ++it) {

		SearchSingleTJJetPFO( *it, relation_recoMCtruth, jet_MCs_set, TJ_jetinfo );
	}

	streamlog_out(DEBUG3) << "In SearchSingleTJJetParticles: Start treating jet " << ijet << " MCs \n";
	// The Iterator is a Pointer to my pointer to an MCParticle
	for (std::set<MCParticle*>::iterator it=jet_MCs_set.begin(); it!=jet_MCs_set.end(); ++it) {
		SearchSingleTJJetMC( *it, relation_recoMCtruth, TJ_jetinfo );
	}
  streamlog_out(DEBUG3) << "In SearchSingleTJJetParticles: Done with jet " << ijet << " MCs \n";

}  
