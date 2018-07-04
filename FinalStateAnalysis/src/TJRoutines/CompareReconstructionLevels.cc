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


// TODO Maybe move this to a different source file. It is too general to be stuck here
// Also as a template it should be moved to the header
template <typename Type>
void JakobsVBSProcessor::findVectorDifferences(	std::vector<Type> vector1, std::vector<Type> vector2,
												std::vector<Type> &out_in1notin2, std::vector<Type> &out_in2notin1 ) {
	/* General function that compares two vectors and writes into the output vectors
	those elements which are in one of the vectors but not in the other one.
	If one wants all in same output vector just set both output to same vector.
	Output vector only contains unique elements. */

	int N1 = vector1.size();
	int N2 = vector2.size();

	// Those loops are as inefficient as possible and probably could be done faster
	// but at least their functionality should be clear
	for ( int i1=0; i1<N1; i1++ ) {
		// if element already in output vector skip second comparison (-> unique elements in output)
		if(std::find(out_in1notin2.begin(), out_in1notin2.end(), vector1[i1]) != out_in1notin2.end()) {
			continue;
		}
		bool element_i1_in_vector2 = false;
		for ( int i2=0; i2<N2; i2++ ) {
			// If the element is in vector 2, break the inner loop because of no need to keep looking
			if (vector1[i1] == vector2[i2]) {
				element_i1_in_vector2 = true;
				break;
			}
		}
		if ( ! element_i1_in_vector2 ) {
			// Put element which is not in v2 but is in v1 into first output vector
			out_in1notin2.push_back(vector1[i1]);
		}
	}


	for ( int i2=0; i2<N2; i2++ ) {
		if(std::find(out_in2notin1.begin(), out_in2notin1.end(), vector2[i2]) != out_in2notin1.end()) {
			continue;
		}
		bool element_i2_in_vector1 = false;
		for ( int i1=0; i1<N1; i1++ ) {
			if (vector2[i2] == vector1[i1]) {
				element_i2_in_vector1 = true;
				break;
			}
		}
		if ( ! element_i2_in_vector1 ) {
			out_in2notin1.push_back(vector2[i2]);
		}
	}


}


void JakobsVBSProcessor::compareActualToClusteredJets( std::vector<ReconstructedParticle*> jets_clustered,
		std::vector<ReconstructedParticleVec> TJ_jetPFOs_correct, EventInfo &info ) {
	/* Function to compare the correctly clustered jets without overlay (TJ_jetPFOs_correct)
 	- which are found from the MC truth using the TrueJet processor - to the jets clustered
	in my own analysis.
	Keep in mind the different Types of the jets in the two vectors!
 	*/

	int Njets_correct	= TJ_jetPFOs_correct.size();
	int Njets_clustered	= jets_clustered.size();

	// Vectors containing the objectIDs (NOT pdgID) of the objects in all jets
	std::vector<int> objectIDs_correct;
	std::vector<int> objectIDs_clustered;

	// Fill vectors with all objects from all jets
	for (int ijet = 0; ijet<Njets_correct; ijet++) {
		ReconstructedParticleVec jet_i = TJ_jetPFOs_correct[ijet];
		for ( int iPFO = 0; iPFO<jet_i.size(); iPFO++ ) {
			// This is the ID of the ReconstructedParticle in the slcio file
			objectIDs_correct.push_back(jet_i[iPFO]->id());
		}
	}

	for (int ijet = 0; ijet<Njets_clustered; ijet++) {
		ReconstructedParticleVec jet_i = jets_clustered[ijet]->getParticles();
		for ( int iPFO = 0; iPFO<jet_i.size(); iPFO++ ) {
			// This is the ID of the ReconstructedParticle in the slcio file
			objectIDs_clustered.push_back(jet_i[iPFO]->id());
		}
	}

	std::vector<int> objectIDs_incorrect_notinclustered;
	std::vector<int> objectIDs_inclustered_notincorrect;

	// Compare the two objectID arrays and write the differences (contained in one but not the other)
	// into the appropriate output files
	findVectorDifferences( 	objectIDs_correct, objectIDs_clustered,
							objectIDs_incorrect_notinclustered, objectIDs_inclustered_notincorrect );

	info.correctRecoJets_to_ownClusteredJets.N_PFOs_inLevel1 = objectIDs_correct.size();
	info.correctRecoJets_to_ownClusteredJets.N_PFOs_inLevel2 = objectIDs_clustered.size();

	info.correctRecoJets_to_ownClusteredJets.N_PFOs_inLevel1_notinLevel2 = objectIDs_incorrect_notinclustered.size();
	info.correctRecoJets_to_ownClusteredJets.N_PFOs_inLevel2_notinLevel1 = objectIDs_inclustered_notincorrect.size();

}


void JakobsVBSProcessor::LevelComparisonIndividualJets(LCRelationNavigator* relation_recoMCtruth, EventInfo &info) {

	if ( njets() > __max_Njets ) {
		streamlog_out(WARNING) << " More actual jets than anticipated : " << njets()   << std::endl;
	}

	TJGetInitialCNRecoMasses(info);


	for ( int i=0; i<njets(); i++ ) {
		int ijet = info.TJ_jetinfo.next_jet();
		streamlog_out(DEBUG) << " Treating jet " << ijet   << std::endl;

		SearchSingleTJJetParticles( relation_recoMCtruth, info.TJ_jetinfo );

		TLorentzVector seen_jet_tlv = TLorentzVector( p4seen(ijet)[1],p4seen(ijet)[2],p4seen(ijet)[3], p4seen(ijet)[0] );
		float detected_mass = seen_jet_tlv.M();
		float detected_E	= seen_jet_tlv.E();
		float detected_p	= seen_jet_tlv.P();

		ReconstructedParticleVec ijetPFOs = seen_partics(ijet); // seen_partics(...) is function of TrueJet_Parser, see header

		TLorentzVector true_of_seen_tlv = TLorentzVector( p4trueseen(ijet)[1], p4trueseen(ijet)[2], p4trueseen(ijet)[3], p4trueseen(ijet)[0]);
		float true_mass_of_detectable_jet	= true_of_seen_tlv.M();
		float true_E_of_detectable_jet		= true_of_seen_tlv.E();
		float true_p_of_detectable_jet		= true_of_seen_tlv.P();

		// The final elementon is the q / g / gamma / ... after parton shower
		MCParticle* final_elementon_particle = final_elementon(ijet);
		int final_elementon_pdgID = final_elementon_particle->getPDG();
		TLorentzVector final_elementon_tlv = TLorentzVector( final_elementon_particle->getMomentum(), final_elementon_particle->getEnergy() );
		float final_elementon_mass = final_elementon_tlv.M();
		float final_elementon_E = final_elementon_tlv.E();
		float final_elementon_p = final_elementon_tlv.P();


		TLorentzVector true_tlv = TLorentzVector( p4true(ijet)[1], p4true(ijet)[2], p4true(ijet)[3], p4true(ijet)[0]);
		float true_mass	= true_tlv.M();
		float true_E	= true_tlv.E();
		float true_p	= true_tlv.P();

		streamlog_out(DEBUG) << "evt: " << _nEvt << " E_t: " << true_E
							<< " E_tos: " << true_E_of_detectable_jet
							<< " E_s: " << detected_E << "\n";

		info.TJ_jetinfo.exists[ijet] = 1;
		info.TJ_jetinfo.final_elementon_pdgID[ijet] = final_elementon_pdgID;
		info.TJ_jetinfo.final_elementon.mass[ijet] = final_elementon_mass;
		info.TJ_jetinfo.final_elementon.E[ijet] = final_elementon_E;
		info.TJ_jetinfo.final_elementon.p[ijet] = final_elementon_p;
		info.TJ_jetinfo.detected.mass[ijet] = detected_mass;
		info.TJ_jetinfo.detected.E[ijet] = detected_E;
		info.TJ_jetinfo.detected.p[ijet] = detected_p;
		info.TJ_jetinfo.true_value_of_detectable_jet.mass[ijet] = true_mass_of_detectable_jet;
		info.TJ_jetinfo.true_value_of_detectable_jet.E[ijet] = true_E_of_detectable_jet;
		info.TJ_jetinfo.true_value_of_detectable_jet.p[ijet] = true_p_of_detectable_jet;
		info.TJ_jetinfo.true_jet.mass[ijet] = true_mass;
		info.TJ_jetinfo.true_jet.E[ijet] = true_E;
		info.TJ_jetinfo.true_jet.p[ijet] = true_p;

	}

}
