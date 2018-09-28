#ifndef OBSERVABLESEXTRACTION_H
#define OBSERVABLESEXTRACTION_H 1

#include "aQGCObservablesProcessor.h"

//-------------------------------------------------------------------------------------------------

template <class ParticleClass> 
void aQGCObservablesProcessor::findJetObservables( std::vector<ParticleClass*> &jet_vector ) {
  VectorBosonPairFinder <ParticleClass> VBpair_finder;
  VBpair_finder.setParticleVector(jet_vector);
  VBpair_finder.findBestCandidate();
          
  ParticleClass* boson1_jet1 = VBpair_finder.getBoson1Particle1();
  ParticleClass* boson1_jet2 = VBpair_finder.getBoson1Particle2();
  ParticleClass* boson2_jet1 = VBpair_finder.getBoson2Particle1();
  ParticleClass* boson2_jet2 = VBpair_finder.getBoson2Particle2();
  
  TLorentzVector V1_jet1_tlv ( boson1_jet1->getMomentum(),  boson1_jet1->getEnergy() );
	TLorentzVector V1_jet2_tlv ( boson1_jet2->getMomentum(), 	boson1_jet2->getEnergy() );
	TLorentzVector V2_jet1_tlv ( boson2_jet1->getMomentum(), 	boson2_jet1->getEnergy() );
	TLorentzVector V2_jet2_tlv ( boson2_jet2->getMomentum(), 	boson2_jet2->getEnergy() );
  
  this->findVectorBosonObservables( V1_jet1_tlv, V1_jet2_tlv, V2_jet1_tlv, V2_jet2_tlv  );
  
  // General jet observables
  m_min_jetE          = std::numeric_limits<float>::infinity(); // Start out with maximal minimum values -> Must be overwritten
  m_min_jetNparticles = std::numeric_limits<float>::infinity();
  m_min_jetNcharged   = std::numeric_limits<float>::infinity();
  
  for(unsigned int i_jet=0; i_jet<jet_vector.size(); i_jet++){
    TLorentzVector jet_tlv = TLorentzVector( jet_vector[i_jet]->getMomentum(), jet_vector[i_jet]->getEnergy() );
  
    float jet_E = jet_tlv.E();
    if ( jet_E < m_min_jetE ) { m_min_jetE = jet_E; }

    // Try to investigate jet content ( only works on recos and if it is set... )
    ReconstructedParticle* ijet_reco = dynamic_cast<ReconstructedParticle*>(jet_vector[i_jet]);
    if ( nullptr != ijet_reco ){
      const std::vector<ReconstructedParticle*>& jet_particles = ijet_reco->getParticles();
      
      int n_jet_particles = jet_particles.size();
      int n_charged = 0;
      for( int i_particle = 0; i_particle < n_jet_particles; i_particle++ ){
        ReconstructedParticle* jet_particle = jet_particles[i_particle];
        if ( jet_particle->getCharge() != 0 ) {
          n_charged += 1;
        }
      }
      
      if ( n_jet_particles < m_min_jetNparticles )  { m_min_jetNparticles  = n_jet_particles; } 
      if ( n_charged < m_min_jetNcharged )          { m_min_jetNcharged    = n_charged;       }
    } else { // If particle are not reco
      m_min_jetNparticles = -1;
      m_min_jetNcharged   = -1;
      streamlog_out(DEBUG) << "In findJetObservables: No particles within jet found." << std::endl;
      return;
    }
  }
}

//-------------------------------------------------------------------------------------------------

template <class ParticleClass> 
void aQGCObservablesProcessor::findParticleObservables( std::vector<ParticleClass*> &particle_vector ) {
  unsigned int n_particles = particle_vector.size();
  
  // Find highest energy track of the event
  TLorentzVector leadEtrack_tlv (-1, 0, 0, 0);
  for(unsigned int i_particle=0; i_particle<n_particles; i_particle++){
    TLorentzVector particle_tlv ( particle_vector[i_particle]->getMomentum(), particle_vector[i_particle]->getEnergy() );
    
    if ( particle_tlv.E() > leadEtrack_tlv.E() ) {
      leadEtrack_tlv = particle_tlv;
    }
  }
  
  m_leadEtrack_cosTheta = leadEtrack_tlv.CosTheta();
  
  
  // Find all particles within 10degree cone around highest energy track,
  // cluster them together and save the combined energy
  TLorentzVector leadEtrack_10cone_tlv {};
  for(unsigned int i_particle=0; i_particle<n_particles; i_particle++){
    TLorentzVector particle_tlv ( particle_vector[i_particle]->getMomentum(), particle_vector[i_particle]->getEnergy() );
    float angle_to_leadEtrack = leadEtrack_tlv.Angle(particle_tlv.Vect());
  
    // If angle is smaller than 10 degree
    if ( angle_to_leadEtrack < (10.0/360.0 * 2.0*TMath::Pi()) ) {
      leadEtrack_10cone_tlv += particle_tlv;
    }
  }
  leadEtrack_10cone_tlv -= leadEtrack_tlv; // Do not count Emax-track into cone
  
  m_leadEtrack_coneE = leadEtrack_10cone_tlv.E();
}

//-------------------------------------------------------------------------------------------------

  #endif