#ifndef LCIOHELPER_H
#define LCIOHELPER_H 1

#include "JakobsVBSProcessor.h"
#include <iostream>
#include <fstream>
#include <set>
#include <typeinfo>

#include <vector>
#include <stack>

//LCIO
#include <EVENT/LCCollection.h>
#include <EVENT/ReconstructedParticle.h>
#include <IMPL/ReconstructedParticleImpl.h>
#include <EVENT/Vertex.h>
#include "UTIL/PIDHandler.h"
#include "LCIOSTLTypes.h"

//----- include for verbosity dependend logging ---------
#include "marlin/VerbosityLevels.h"


template<class Type>
std::vector<int> JakobsVBSProcessor::FindCommonParticleIndices( 
						std::vector<Type> v1, std::vector<Type> v2 ) {

	std::vector<int> common_indices;

	for (int i=0; i<v1.size(); i++) {
		if ( std::find( v2.begin(), v2.end(), v1[i] ) != v2.end() ) {
			common_indices.push_back(i);
		}	
	}
		
	return common_indices;
}


template<class Type>
std::vector<ReconstructedParticle*> JakobsVBSProcessor::RecosFromLCIOVec ( Type &LCIOVec ){

	std::vector<ReconstructedParticle*> out_vec;
	
	for (int i=0; i<LCIOVec.size(); i++) {
		out_vec.push_back(dynamic_cast<ReconstructedParticle*>(LCIOVec[i]));
	}

	return out_vec;
}





#endif
