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



int JakobsVBSProcessor::GetIndexHighestWeightTypeMC(
		LCObjectVec &MCs_to_PFO, FloatVec &MC_weights_to_PFO, std::string type ) {

	std::vector<float> weights_in_PFO;
	std::vector<int> indices_in_PFO;

	for ( int iMC = 0; iMC<MCs_to_PFO.size(); iMC++ ) {

		MCParticle* MC_i = dynamic_cast<MCParticle*>( MCs_to_PFO[iMC] );
		float MC_weighti = MC_weights_to_PFO[iMC];

		if ( MCParticleIsType( MC_i, type ) ) {
			weights_in_PFO.push_back( MC_weighti );
			indices_in_PFO.push_back( iMC );
		}
	} 

	if ( weights_in_PFO.size() > 0 ) {
		int maximum_weight_index = std::distance(weights_in_PFO.begin(), std::max_element(weights_in_PFO.begin(), weights_in_PFO.end()));
		return indices_in_PFO[maximum_weight_index];
	}
	else { return -1; }
}

int JakobsVBSProcessor::GetIndexHighestWeightReco( FloatVec &PFO_weights_to_MC ) {

	std::vector<float> weight_of_recos_in_MC;
	std::vector<int> index_of_recos_in_MC;

	for ( int iPFO = 0; iPFO<PFO_weights_to_MC.size(); iPFO++ ) {
		float PFO_weighti = PFO_weights_to_MC[iPFO];
		weight_of_recos_in_MC.push_back( PFO_weighti );
		index_of_recos_in_MC.push_back( iPFO );
	} 

	if ( weight_of_recos_in_MC.size() > 0 ) {
		int maximum_weight_index = std::distance(weight_of_recos_in_MC.begin(), std::max_element(weight_of_recos_in_MC.begin(), weight_of_recos_in_MC.end()));

		return index_of_recos_in_MC[maximum_weight_index];
	}
	else { return -1; }
}



void JakobsVBSProcessor::TJProcessSingleTypePFO( 
		ReconstructedParticle* PFO, LCRelationNavigator* relation_recoMCtruth, 
		JetParticleTypeInfo &type_particles ) { 

	LCObjectVec MCsToThisPFO		= relation_recoMCtruth->getRelatedToObjects( PFO );
	FloatVec MCweightsToThisPFO 	= relation_recoMCtruth->getRelatedToWeights( PFO );

	FloatVec MCtrckweightsToThisPFO;
	SplitWeight( MCweightsToThisPFO, MCtrckweightsToThisPFO, "trck" );

	FloatVec MCcaloweightsToThisPFO;
	SplitWeight( MCweightsToThisPFO, MCcaloweightsToThisPFO, "calo" );


	if ( IdentifiedAsType( PFO, type_particles.type ) ) {

		int i_jet = type_particles.jet_counter->count();
		int i_reco = type_particles.next_reco();

		type_particles.recos_IDd_as_type.exists[i_jet][i_reco] = 1;

		type_particles.recos_IDd_as_type.reco_E[i_jet][i_reco] = PFO->getEnergy();

		int highest_trck_weight_type_index = 
				GetIndexHighestWeightTypeMC( MCsToThisPFO, MCtrckweightsToThisPFO, 
											type_particles.type);
		if ( highest_trck_weight_type_index == -1 ) {
			type_particles.recos_IDd_as_type.highest_type_trck_weight[i_jet][i_reco] = 0;
		} else {
			type_particles.recos_IDd_as_type.highest_type_trck_weight[i_jet][i_reco] = 
				MCtrckweightsToThisPFO[highest_trck_weight_type_index];
		}

		
		int highest_calo_weight_type_index = 
				GetIndexHighestWeightTypeMC( MCsToThisPFO, MCcaloweightsToThisPFO, 
											type_particles.type);
		if ( highest_calo_weight_type_index == -1 ) {
			type_particles.recos_IDd_as_type.highest_type_calo_weight[i_jet][i_reco] = 0;
		} else {
			type_particles.recos_IDd_as_type.highest_type_calo_weight[i_jet][i_reco] = 
				MCcaloweightsToThisPFO[highest_calo_weight_type_index];
		}
		
	}
}


void JakobsVBSProcessor::TJProcessSingleTypeMC( 
		MCParticle* MC,	LCRelationNavigator* relation_recoMCtruth, JetParticleTypeInfo &type_particles ) {


	// Check that MCParticle not already handled
	if ( MCParticleIsType( MC, type_particles.type ) ) {

		int i_jet = type_particles.jet_counter->count();
		int i_mc = type_particles.next_mc();

		type_particles.type_MCs.exists[i_jet][i_mc] = 1;

		type_particles.type_MCs.MC_E[i_jet][i_mc] = MC->getEnergy();
		type_particles.type_MCs.MC_pdgID[i_jet][i_mc] = MC->getPDG();

		// Connection to PFOs in all TJJets

		LCObjectVec PFOsToThisMC = relation_recoMCtruth->getRelatedFromObjects( MC );
		FloatVec PFOweightsToThisMC = relation_recoMCtruth->getRelatedFromWeights( MC );
		
		FloatVec PFOtrckweightsToThisMC;
		SplitWeight( PFOweightsToThisMC, PFOtrckweightsToThisMC, "trck" );

		FloatVec PFOcaloweightsToThisMC;
		SplitWeight( PFOweightsToThisMC, PFOcaloweightsToThisMC, "calo" );

		int highest_trck_weight_reco_type_index = GetIndexHighestWeightReco( PFOtrckweightsToThisMC);
		int highest_calo_weight_reco_type_index = GetIndexHighestWeightReco( PFOcaloweightsToThisMC);

		if ( highest_trck_weight_reco_type_index == -1 ) {
			type_particles.type_MCs.highest_trck_weight_in_any_recoparticle[i_jet][i_mc] = 0;
		} else {
			type_particles.type_MCs.highest_trck_weight_in_any_recoparticle[i_jet][i_mc] = PFOtrckweightsToThisMC[highest_trck_weight_reco_type_index];
			ReconstructedParticle* highest_trck_weight_reco = dynamic_cast<ReconstructedParticle*>( PFOsToThisMC[highest_trck_weight_reco_type_index] );
			if ( IdentifiedAsType( highest_trck_weight_reco, type_particles.type ) ) {
				type_particles.type_MCs.highest_trck_weight_recoparticle_IDd_as_type[i_jet][i_mc] = 1;
			} else {
				type_particles.type_MCs.highest_trck_weight_recoparticle_IDd_as_type[i_jet][i_mc] = 0;
			}
		}

		if ( highest_calo_weight_reco_type_index == -1 ) {
			type_particles.type_MCs.highest_calo_weight_in_any_recoparticle[i_jet][i_mc] = 0;
		} else {
			type_particles.type_MCs.highest_calo_weight_in_any_recoparticle[i_jet][i_mc] = PFOcaloweightsToThisMC[highest_calo_weight_reco_type_index];
			ReconstructedParticle* highest_calo_weight_reco = dynamic_cast<ReconstructedParticle*>( PFOsToThisMC[highest_calo_weight_reco_type_index] );
			if ( IdentifiedAsType( highest_calo_weight_reco, type_particles.type ) ) {
				type_particles.type_MCs.highest_calo_weight_recoparticle_IDd_as_type[i_jet][i_mc] = 1;
			} else {
				type_particles.type_MCs.highest_calo_weight_recoparticle_IDd_as_type[i_jet][i_mc] = 0;
			}
		}


		// Connection to PFOs in this TJJet
		ReconstructedParticleVec ijet_PFOs = seen_partics( i_jet );
		std::vector<int> indices_thisJetPFOs_to_MC = 
				FindCommonParticleIndices( 	RecosFromLCIOVec(PFOsToThisMC) , 
											RecosFromLCIOVec(ijet_PFOs) ); 

		FloatVec ThisJetPFOtrckweightsToThisMC;
		FloatVec ThisJetPFOcaloweightsToThisMC;

		for (int k=0; k<indices_thisJetPFOs_to_MC.size(); k++) {
			ThisJetPFOtrckweightsToThisMC.push_back(PFOtrckweightsToThisMC[k]);				
			ThisJetPFOcaloweightsToThisMC.push_back(PFOcaloweightsToThisMC[k]);				
		}	

		int highest_trck_weight_index = GetIndexHighestWeightReco( ThisJetPFOtrckweightsToThisMC);
		int highest_calo_weight_index = GetIndexHighestWeightReco( ThisJetPFOcaloweightsToThisMC);

		if ( highest_trck_weight_index == -1 ) {
			type_particles.type_MCs.highest_trck_weight_in_this_jet[i_jet][i_mc] = 0;
		} else {
			type_particles.type_MCs.highest_trck_weight_in_this_jet[i_jet][i_mc] = ThisJetPFOtrckweightsToThisMC[highest_trck_weight_index];
		}

		if ( highest_calo_weight_index == -1 ) {
			type_particles.type_MCs.highest_calo_weight_in_this_jet[i_jet][i_mc] = 0;
		} else {
			type_particles.type_MCs.highest_calo_weight_in_this_jet[i_jet][i_mc] = ThisJetPFOcaloweightsToThisMC[highest_calo_weight_index];
		}
	}
}

