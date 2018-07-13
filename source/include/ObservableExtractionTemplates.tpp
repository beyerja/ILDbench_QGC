#ifndef OBSERVABLESEXTRACTION_H
#define OBSERVABLESEXTRACTION_H 1

#include "aQGCObservablesProcessor.h"



// // Vector boson observables
// tree->Branch( "V1_type",      &m_V1_type,     "V1_type/I"); 
// tree->Branch( "V2_type",      &m_V2_type,     "V2_type/I"); 

//-------------------------------------------------------------------------------------------------

template <class ParticleClass> void aQGCObservablesProcessor::findJetObservables( std::vector<ParticleClass*> &jet_vector ) {
  VectorBosonPairFinder <ParticleClass> VBpair_finder;
  VBpair_finder.setParticleVector(jet_vector);
  VBpair_finder.findBestCandidate();
          
  ParticleClass* boson1_p1 = VBpair_finder.getBoson1Particle1();
  ParticleClass* boson1_p2 = VBpair_finder.getBoson1Particle2();
  ParticleClass* boson2_p1 = VBpair_finder.getBoson2Particle1();
  ParticleClass* boson2_p2 = VBpair_finder.getBoson2Particle2();
  
  TLorentzVector V1_p1_tlv ( boson1_p1->getMomentum(),  boson1_p1->getEnergy() );
	TLorentzVector V1_p2_tlv ( boson1_p2->getMomentum(), 	boson1_p2->getEnergy() );
	TLorentzVector V2_p1_tlv ( boson2_p1->getMomentum(), 	boson2_p1->getEnergy() );
  
	TLorentzVector V2_p2_tlv ( boson2_p2->getMomentum(), 	boson2_p2->getEnergy() );
  
  // Single VB observables
  TLorentzVector V1_tlv = V1_p1_tlv + V1_p2_tlv;
  TLorentzVector V2_tlv = V2_p1_tlv + V2_p2_tlv;
  
  m_V1_m        = V1_tlv.M();
  m_V1_pT       = V1_tlv.Pt();
  m_V1_cosTheta = V1_tlv.CosTheta();
  
  m_V2_m        = V2_tlv.M();
  m_V2_pT       = V2_tlv.Pt();
  m_V2_cosTheta = V2_tlv.CosTheta();
  
  // Combined VB observables
  TLorentzVector VV_tlv = V1_tlv + V2_tlv;
  
  m_VV_m      = VV_tlv.M();
  m_VV_pT     = VV_tlv.Pt();
  m_VV_ET     = VV_tlv.Et();
  
  // Observables from recoil against VBs
  TLorentzVector collision_system_tlv ( m_com_E*sin(0.014/2.0), 0, 0, m_com_E );
  TLorentzVector recoil_tlv  = collision_system_tlv - VV_tlv;
  m_recoil_m  = recoil_tlv.M();
  
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

template <class ParticleClass> void aQGCObservablesProcessor::findParticleObservables( std::vector<ParticleClass*> &particle_vector ) {
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