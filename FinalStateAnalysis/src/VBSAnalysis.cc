#include "JakobsVBSProcessor.h"
#include <iostream>
#include <fstream>
#include <set>

#include <vector>
#include <stack>

//LCIO
#include <EVENT/LCCollection.h>
#include <EVENT/MCParticle.h>
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

void JakobsVBSProcessor::analyseEvent(	
		LCCollection* colMC, LCCollection* colPFOs, LCCollection* colJets, LCCollection* colPFOsFromFastJet,
		LCCollection* colIsoleps, 
    LCCollection* colCheatedNoOverlayJets,  LCCollection* colCheatedNoOverlayJetsPFOsFromFastJet,
		std::vector<ReconstructedParticleVec> TJ_jetPFOs_correct, std::vector<ReconstructedParticleVec> TJ_jetPFOs_fromISR,
		LCRelationNavigator* relation_recoMCtruth,
		EventInfo &info) {

	// Read out the particle from the Collections into C++ vectors
	std::vector<MCParticle*> MCparticles=checkMCFinalState(colMC);
	std::vector<ReconstructedParticle*> PFOs=checkPOFinalState(colPFOs);
	std::vector<ReconstructedParticle*> jets=checkPOFinalState(colJets);
	std::vector<ReconstructedParticle*> PFOs_from_jets=checkPOFinalState(colPFOsFromFastJet);
  std::vector<ReconstructedParticle*> isoleps=checkPOFinalState(colIsoleps);
  std::vector<ReconstructedParticle*> jets_cheated_overlay=checkPOFinalState(colCheatedNoOverlayJets);
  std::vector<ReconstructedParticle*> PFOs_cheated_overlay=checkPOFinalState(colCheatedNoOverlayJetsPFOsFromFastJet);

	// Categorize Event on generator level
	findTrueEventType(MCparticles, info);

	// Look at ISR as known from TrueJet
	analyseGeneratorLevelISR(TJ_jetPFOs_fromISR, info);

	// Get information about the 4 individual quarks and the 2 neutrinos on Generator Level
	analyseGeneratorLevelQuarks(MCparticles, info);
	analyseGeneratorLevelNeutrinos(MCparticles, info);

	// Calculate Parameters that I later make a cut on,
	// y_34 has to be done separate because its a parameter of the collection	
	info.observ.y_34= colJets->getParameters().getFloatVal("y_{n-1,n}");
	calculateCutParameters( PFOs, jets, info.observ );

	// nunu+4q FS does not contain isolated leptons
	// Maybe put into Preselection, but might save time to put earlier
	info.observ.num_isolated_leptons = isoleps.size();


	/* ------------------- TJ -----------------------*/
	// Also get information about individual jets on each level of reconstruction
	LevelComparisonIndividualJets(relation_recoMCtruth, info); 
	calculateTJTotalLevels( info );
  findVpair(jets_cheated_overlay, info.observ_cheated_overlay);

	/* ------------------- OLD ----------------------*/
	// For optimization purposes: Compare correct Reco-Jets (from TrueJet) with own clustered ones
	compareActualToClusteredJets( jets, TJ_jetPFOs_correct, info);

	// For optimization purposes: Find which reconstructed V fits best to which generator level V
	findBestGeneratorToRecoVPairing(info);
	/* ----------------------------------------------*/

}       
