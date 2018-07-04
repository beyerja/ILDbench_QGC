#ifndef TJSINGLEJET_INFORMATION_H
#define TJSINGLEJET_INFORMATION_H 1 

#include "TTree.h"
#include "TLorentzVector.h"

#include <vector>

static const int __max_Njets = 25;
static const int __Njets = 4;
static const int __max_Nparticles = 150;

class Counter{

  int counter;

  public:
	
	void init() {
		counter = -1;
	}

	Counter& operator++(){
		counter++;
		return *this;
	}
	Counter operator++(int) {
		Counter tmp(*this);
		++(*this);
		return tmp;
	}
	
	int count(){
		return counter;
	}

	void reset(){
		counter = -1;
	}

};


class TJRecoParticleAsTypeIDd {

  public:

	int exists[__max_Njets][__max_Nparticles];

	float reco_E[__max_Njets][__max_Nparticles];

	float highest_type_trck_weight[__max_Njets][__max_Nparticles];	
	float highest_type_calo_weight[__max_Njets][__max_Nparticles];	

	void init() {

		for (int i=0; i<__max_Njets; i++) {
			for (int j=0; j<__max_Nparticles; j++) {
				exists[i][j] = 0;

				reco_E[i][j] = -10;
			
				highest_type_trck_weight[i][j] = -1;
				highest_type_calo_weight[i][j] = -1;
			}
		}
	}

};


class TJTypeMC {

  public:

	int exists[__max_Njets][__max_Nparticles];

	float MC_E[__max_Njets][__max_Nparticles];

	int MC_pdgID[__max_Njets][__max_Nparticles];

	float highest_trck_weight_in_this_jet[__max_Njets][__max_Nparticles];
	float highest_calo_weight_in_this_jet[__max_Njets][__max_Nparticles];

	float highest_trck_weight_in_this_jet_IDd_as_type[__max_Njets][__max_Nparticles];
    float highest_calo_weight_in_this_jet_IDd_as_type[__max_Njets][__max_Nparticles];

	float highest_trck_weight_in_any_recoparticle[__max_Njets][__max_Nparticles];
	float highest_calo_weight_in_any_recoparticle[__max_Njets][__max_Nparticles];
	
	int highest_trck_weight_recoparticle_IDd_as_type[__max_Njets][__max_Nparticles];
	int highest_calo_weight_recoparticle_IDd_as_type[__max_Njets][__max_Nparticles];

	void init() {

		for (int i=0; i<__max_Njets; i++) {
			for (int j=0; j<__max_Nparticles; j++) {
				exists[i][j] = 0;

				MC_E[i][j] = -10;

				MC_pdgID[i][j] = 0;

				highest_trck_weight_in_this_jet[i][j] = -1;
			    highest_calo_weight_in_this_jet[i][j] = -1;

				highest_trck_weight_in_this_jet_IDd_as_type[i][j] = -1;
			    highest_calo_weight_in_this_jet_IDd_as_type[i][j] = -1;

				highest_trck_weight_in_any_recoparticle[i][j] = -1;
				highest_calo_weight_in_any_recoparticle[i][j] = -1;
			
				highest_trck_weight_recoparticle_IDd_as_type[i][j] = -1;
				highest_calo_weight_recoparticle_IDd_as_type[i][j] = -1;
			}
		}
	}

};


class JetParticleTypeInfo {

  public:

	char* type;

	TJRecoParticleAsTypeIDd recos_IDd_as_type;
	TJTypeMC type_MCs;

	Counter reco_counter;
	Counter mc_counter;

	Counter* jet_counter;

	void init(Counter* _jet_counter, char* _type) {

		type = _type;

		jet_counter = _jet_counter;	

		recos_IDd_as_type.init();
		type_MCs.init();
	}
	
	int next_reco() {
		reco_counter++;
		return reco_counter.count(); 
	}

	int next_mc() {
		mc_counter++;
		return mc_counter.count();
	}

	void reset_type_counters() {
		reco_counter.reset();
		mc_counter.reset();
	}

};


class TrueJetLevel {

	public:
		float E[__max_Njets];
		float p[__max_Njets];
		float mass[__max_Njets];
		float theta[__max_Njets];

		Counter* jet_counter;

		void init(Counter* _jet_counter) {

			jet_counter = _jet_counter;

			for (int i=0; i<__max_Njets; i++){
				E[i] = -10;
				p[i] = -10;
				mass[i] = -10;
				theta[i] = -10;
			}
		}

};

class TrueJetSingleJetInfo {

	public:
		int exists[__max_Njets];
		int final_elementon_pdgID[__max_Njets];

		Counter jet_counter;

		TrueJetLevel true_jet;
		TrueJetLevel final_elementon;
		TrueJetLevel true_value_of_detectable_jet;
		TrueJetLevel detected;
		
		JetParticleTypeInfo jet_leptons;
		JetParticleTypeInfo jet_charged_hadrons;
		JetParticleTypeInfo jet_neutral_hadrons;
		JetParticleTypeInfo jet_photons;

		void init() {
			for (int i=0; i<__max_Njets; i++) {
				exists[i] = 0;
				final_elementon_pdgID[i] = 0;
			}

			jet_counter.init();

			true_jet.init(&jet_counter);
			final_elementon.init(&jet_counter);
			true_value_of_detectable_jet.init(&jet_counter);
			detected.init(&jet_counter);
			
			jet_leptons.init(&jet_counter, "lepton");			
			jet_charged_hadrons.init(&jet_counter, "chargedhadron");			
			jet_neutral_hadrons.init(&jet_counter, "neutralhadron");			
			jet_photons.init(&jet_counter, "photon");			
		}

		int next_jet() {
			jet_counter++;
			jet_leptons.reset_type_counters(); // lepton index start from 0 again
			jet_charged_hadrons.reset_type_counters(); 
			jet_neutral_hadrons.reset_type_counters(); 
			jet_photons.reset_type_counters(); 
			return jet_counter.count();
		}

		int current_jet() {
			return jet_counter.count();
		}

};



#endif
