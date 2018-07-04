#include "JakobsVBSProcessor.h"
#include <iostream>
#include <fstream>
#include <set>

#include <vector>
#include <stack>
#include <math.h>
#include <algorithm>

//LCIO
#include <EVENT/LCCollection.h>
#include <EVENT/ReconstructedParticle.h>
#include <EVENT/Vertex.h>
#include <IMPL/ReconstructedParticleImpl.h>
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

#include "TRandom.h"

using namespace lcio;
using namespace marlin;

/* Functions that read in LCCollections and return
   vector containing the Particles in their corresponding
	LCIO format */


std::vector<MCParticle*> JakobsVBSProcessor::checkMCFinalState(LCCollection* AllMC) {
	std::vector<MCParticle*> FS_all;
	int nMC = AllMC->getNumberOfElements();
	for( int i = 0; i < nMC; i++ ){
		MCParticle* MC = dynamic_cast< MCParticle* >( AllMC->getElementAt(i) );
		FS_all.push_back(MC);
	}
	return(FS_all);
}


std::vector<ReconstructedParticle*> JakobsVBSProcessor::checkPOFinalState(LCCollection* AllPOs) {
	std::vector<ReconstructedParticle*> FS;
	int npo;
    try{
	   npo = AllPOs->getNumberOfElements();
	} catch(...) {
           npo = 0;
	}
	if ( npo != 0 ){
	for( int i = 0; i < npo; i++ ){
		ReconstructedParticle* po= dynamic_cast< ReconstructedParticle* >( AllPOs->getElementAt(i) );
		FS.push_back(po);
	}
	}
	return(FS);
}




