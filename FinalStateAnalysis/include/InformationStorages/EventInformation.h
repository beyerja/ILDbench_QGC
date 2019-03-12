#ifndef EVENT_INFORMATION_H
#define EVENT_INFORMATION_H 1 

#include "TTree.h"
#include "TLorentzVector.h"
#include <math.h>

#include "TJInformation.h"
#include "TJTotalSingleJetInformation.h"

// IMPORTANT NOTE TO EVERYTHING IN HERE:
// If something should be written into rootfile later it needs to be public
// so that the branch can be connected to the variable address.

//WARNING: For the way I read my variables into my tree:
//	- The variables read into the tree must ALWAYS be the first thing declared in the class
//	- The variables must be in the same order as I name the TLeafes of my tree

class JetInfo {
	// Information about one of the jets
	public:
		float E;
		float theta;
		float mass;

		TLorentzVector tlv;

		void init() {
			E = -10;
			theta = -10;
			mass = -10;

			tlv = TLorentzVector(0.0, 0.0, 0.0, 0.0);
		}

		void process_tlv() {
			E = tlv.E();
			theta = tlv.Theta();
			mass = tlv.M();
		}


};


class Observables {
	/* Class that holds all information, that can be measured by the detector
 	and I make a cut on */ 

	public:

		float pair1_mass;
		float pair2_mass;
		float pair1_theta;
		float pair2_theta;
		float pair1_p;
		float pair2_p;

		float miss_mass; // Also called: recoil mass

		float sum_E_q; // should be called sum_E_vis ...
		float E_T_vis;
		float p_T_vis;
		float E_10cone_EmaxTrack;

		float cosTheta_miss;
		float cosTheta_EmaxTrack;

		// Distance parameter when clustering jets from 4 to 3 jets
		float y_34;
	
		// Each of those is the minimum over all 4 jets 
		// -> Cut on them means cut on all jets
		float min_Ejet;
		int min_num_particles_jet;
		int min_num_chargedparticles_jet;

		int num_isolated_leptons;

		TLorentzVector pair1_tlv;
		TLorentzVector pair2_tlv;
		TLorentzVector visible_tlv;
		TLorentzVector miss_tlv;

		JetInfo jets[__Njets];

		void init() {
			pair1_tlv 	= TLorentzVector(0.0, 0.0, 0.0, 0.0);
			pair2_tlv 	= TLorentzVector(0.0, 0.0, 0.0, 0.0);
			visible_tlv = TLorentzVector(0.0, 0.0, 0.0, 0.0);
			miss_tlv 	= TLorentzVector(0.0, 0.0, 0.0, 0.0);

			pair1_mass	= -10;
			pair2_mass	= -10;
			pair1_theta = -10;
			pair2_theta = -10;
			pair1_p = -10;
			pair2_p = -10;

			miss_mass	= -10;

			sum_E_q		= -10;
			E_T_vis		= -10;
			p_T_vis		= -10;
			E_10cone_EmaxTrack	= -10;

			cosTheta_miss	= -2;
			cosTheta_EmaxTrack	= -2;

			for (int i=0; i<__Njets; i++) { 
				jets[i].init();
			}

			y_34	= -1;
	
			min_Ejet	= -10;
			min_num_particles_jet			= -1;
			min_num_chargedparticles_jet	= -1;

			num_isolated_leptons = -1;

		}

		void processTLVs() {
			pair1_mass = pair1_tlv.M();
			pair2_mass = pair2_tlv.M();
			pair1_theta = pair1_tlv.Theta();
			pair2_theta = pair2_tlv.Theta();
			pair1_p = pair1_tlv.P();
			pair2_p = pair2_tlv.P();


			visible_tlv = pair1_tlv + pair2_tlv;
			sum_E_q		= visible_tlv.E();
			E_T_vis		= visible_tlv.Et();
			p_T_vis		= visible_tlv.Pt();

			TLorentzVector initial = TLorentzVector(1000*sin(0.014/2.0),0,0,1000);	
			miss_tlv 	= initial - pair1_tlv - pair2_tlv;
			miss_mass 	= miss_tlv.M();
			cosTheta_miss	= miss_tlv.CosTheta();
		}

};

class ISR {
	/* Class to store information about the ISR (could be reco, true,...) */
	
	public:
		TLorentzVector ISR_tlv;
		
		float E;
		float p;
		float theta;
		
		void init() {
			   ISR_tlv = TLorentzVector(0.0, 0.0, 0.0, 0.0);
		
				E = -10;
				p = -10;
				theta = -10;
		}

		void processTLV() {
			E = ISR_tlv.E();
			if ( E > 0 ) {
				// only fill other variables if ISR at all
				p = ISR_tlv.P();
				theta = ISR_tlv.Theta();
			}
		}

};

class RecoTruthLink {
	/* Class containing relations between Generator level and Reco level */
	public:
		int partner_of_truthpair1;
		int partner_of_truthpair2;

		void init() {
			partner_of_truthpair1 = 0;
			partner_of_truthpair2 = 0;
		}
};




class PFOLevelComparison {
	public:
		int N_PFOs_inLevel1;
		int N_PFOs_inLevel2;

		int N_PFOs_inLevel1_notinLevel2;
		int N_PFOs_inLevel2_notinLevel1;
		
		void init() {
			N_PFOs_inLevel1=-1;
			N_PFOs_inLevel2=-1;

			N_PFOs_inLevel1_notinLevel2=-1;
			N_PFOs_inLevel2_notinLevel1=-1;
		}
};


class TrueParticleInfo {
	// Information about one of the generator level particle 
	public:
		int pdg_id;
		float E;
		float theta;

		TLorentzVector tlv;

		void init() {
			pdg_id = 0;
			E = -10;
			theta = -10;

			tlv = TLorentzVector(0.0, 0.0, 0.0, 0.0);
		}

		void process_tlv() {
			E = tlv.E();
			theta = tlv.Theta();
		}


};

class Truth {
	
	public:
		// Type: 1=WW, 2=ZZ, 0=others
		int eventType;

		Observables observ;
		ISR isr;
		ISR isr_inJets;

		// Use array to allow iteration
		TrueParticleInfo quarks[__Njets];

		TrueParticleInfo nu1;
		TrueParticleInfo nu2;

		int n_CSLDecays;
		int n_BSLDecays;

		void init() {
			eventType = 0;

			observ.init();
			isr.init();
			isr_inJets.init();
			for (int i=0; i<__Njets; i++) { 
				quarks[i].init();
			}

			nu1.init();
			nu2.init();

			n_CSLDecays = 0;
			n_BSLDecays = 0;
		} 

};



class EventInfo {
	/* General class that holds all information about the event 
 	on all the levels of the generation */

	public:
		int evtN;
		int pass_dijet_mass_cuts;

		int Njets;
		int max_Njets;
		int max_Nparticles;
		int max_matrix_size;

		Truth gen_level;

		Observables observ;
		Observables corr_observ; // After jet corrections

		RecoTruthLink Vpair_link;

		PFOLevelComparison correctRecoJets_to_ownClusteredJets;

		TJEventInfo TJ_eventinfo;	
		TrueJetSingleJetInfo TJ_jetinfo;

		void init() {
			pass_dijet_mass_cuts = 0;

			// Even if something goes wrong avoid array sizes smaller 1
			Njets 		= 1;
			max_Njets	= 1;
			max_Nparticles 	= 1;
			max_matrix_size = 1;
		
			gen_level.init();
	
			observ.init();
			corr_observ.init();

			Vpair_link.init();

			correctRecoJets_to_ownClusteredJets.init();

			TJ_eventinfo.init();
			TJ_jetinfo.init();
		}

		void findNs(){
			// TODO If possible find a way to make this work
			// -> Problem: root doesn't realize now it's not supposed to take with full matrix,
			// 				so it ends up stopping at element max_matrix_size, which could be anything...
			
			max_Njets = __max_Njets;
			max_Nparticles = __max_Nparticles;
			max_matrix_size = max_Njets * max_Nparticles; 


			/*
			// Find maximum lengths of arrays -> reduce file size by varying saved array size
			
			// TJ_jets 
			for ( int i=0; i<__max_Njets; i++ ) {
				// Find first index where no jet exists -> i = #jets
				if ( TJ_jetinfo.exists[i] != 1 ) { 
					max_Njets = i; 
					break;
				}

				// For all existing jets check if the number of existing leptons is greater 
				// than current maximum value
				for ( int j=0; j<__max_Nparticles; j++ ) {
					if (	(TJ_jetinfo.jet_leptons.recos_IDd_as_type.exists[i][j] != 1) &&
							(TJ_jetinfo.jet_leptons.type_MCs.exists[i][j] != 1) && 
							(TJ_jetinfo.jet_charged_hadrons.recos_IDd_as_type.exists[i][j] != 1)&&
							(TJ_jetinfo.jet_charged_hadrons.type_MCs.exists[i][j] != 1) && 
							(TJ_jetinfo.jet_neutral_hadrons.recos_IDd_as_type.exists[i][j] != 1)&&
							(TJ_jetinfo.jet_neutral_hadrons.type_MCs.exists[i][j] != 1) &&
							(TJ_jetinfo.jet_photons.recos_IDd_as_type.exists[i][j] != 1)&&
							(TJ_jetinfo.jet_photons.type_MCs.exists[i][j] != 1) ) {
						if ( j > max_Nparticles ) { max_Nparticles = j; }
						break; 
					}
				}

			}

			max_matrix_size = max_Njets * max_Nparticles;
			*/
		}

};

#endif


