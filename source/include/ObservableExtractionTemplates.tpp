#ifndef OBSERVABLESEXTRACTION_H
#define OBSERVABLESEXTRACTION_H 1

#include "aQGCObservablesProcessor.h"



// // Vector boson observables
// tree->Branch( "V1_type",      &m_V1_type,     "V1_type/I"); 
// tree->Branch( "V2_type",      &m_V2_type,     "V2_type/I"); 
// 
// // Jet property variables
// tree->Branch( "min_jetE",           &m_min_jetE,          "min_jetE/F"); // minimum over all four jets
// tree->Branch( "min_jetNparticles",  &m_min_jetNparticles, "min_jetNparticles/F");
// tree->Branch( "min_jetNcharged",    &m_min_jetNcharged,   "min_jetNcharged/F");
// 
// // Properties of most energetic track
// tree->Branch( "leadEtrack_cosTheta",  &m_leadEtrack_cosTheta, "leadEtrack_cosTheta/F");
// tree->Branch( "leadEtrack_coneE",     &m_leadEtrack_coneE,    "leadEtrack_coneE/F");


template <class ParticleClass> void aQGCObservablesProcessor::findBosonPairObservables( std::vector<ParticleClass*> &particle_vector ) {
  VectorBosonPairFinder <ParticleClass> VBpair_finder;
  VBpair_finder.setParticleVector(particle_vector);
  VBpair_finder.findBestCandidate();
          
  ParticleClass* boson1_p1 = VBpair_finder.getBoson1Particle1();
  ParticleClass* boson1_p2 = VBpair_finder.getBoson1Particle2();
  ParticleClass* boson2_p1 = VBpair_finder.getBoson2Particle1();
  ParticleClass* boson2_p2 = VBpair_finder.getBoson2Particle2();
  
  TLorentzVector V1_p1_tlv ( boson1_p1->getMomentum(),  boson1_p1->getEnergy() );
	TLorentzVector V1_p2_tlv ( boson1_p2->getMomentum(), 	boson1_p2->getEnergy() );
	TLorentzVector V2_p1_tlv ( boson2_p1->getMomentum(), 	boson2_p1->getEnergy() );
	TLorentzVector V2_p2_tlv ( boson2_p2->getMomentum(), 	boson2_p2->getEnergy() );
  
  TLorentzVector V1_tlv = V1_p1_tlv + V1_p2_tlv;
  TLorentzVector V2_tlv = V2_p1_tlv + V2_p2_tlv;
  TLorentzVector VV_tlv = V1_tlv + V2_tlv;
  
  m_V1_m        = V1_tlv.M();
  m_V1_pT       = V1_tlv.Pt();
  m_V1_cosTheta = V1_tlv.CosTheta();
  
  m_V2_m        = V2_tlv.M();
  m_V2_pT       = V2_tlv.Pt();
  m_V2_cosTheta = V2_tlv.CosTheta();
  
  //TODO Extract CMS
  
  m_VV_m      = VV_tlv.M();
  m_VV_pT     = VV_tlv.Pt();
  m_VV_ET     = VV_tlv.Et();
  //m_m_recoil  = VV_tlv
  
}


  //       // Find most VV like 2 x dijet pair combination
  //     findVpair(jets, info.observ);
  // 
  //     int nJets = jets.size();
  // 
  //     // Find within the four jets:
  //     //      - minimum occuring jet energy
  //     //      - minimum number of particles in one jet
  //     //      - minimum number of charged particles in one jet
  //     float min_Ejet = 99999;
  // int min_num_particles_jet                   = 99999;
  // int min_num_chargedparticles_jet    = 99999;
  // 
  //     for(int i=0; i<nJets; i++){
  //         TLorentzVector jet_tlv = TLorentzVector( jets[i]->getMomentum(), jets[i]->getEnergy() );
  // 
  //             info.observ.jets[i].tlv = jet_tlv;
  //             info.observ.jets[i].process_tlv();
  // 
  //             const ReconstructedParticleVec& jetPFOs = jets[i]->getParticles();
  //             int nJetPFOs = jetPFOs.size(); 
  // 
  //             int nCharged = 0;
  //             for( unsigned int k = 0; k < nJetPFOs; k++ ){
  //                     ReconstructedParticle* jetparticle = jetPFOs[k];
  //                     if ( jetparticle->getCharge() != 0 ) {
  //                             nCharged += 1;
  //                     }
  //             }
  // 
  //             if ( min_Ejet > jet_tlv.E() ) { min_Ejet = jet_tlv.E(); }
  //             if ( min_num_particles_jet > nJetPFOs ) { min_num_particles_jet = nJetPFOs; }
  //             if ( min_num_chargedparticles_jet > nCharged ) { min_num_chargedparticles_jet = nCharged; }
  //     }
  // 
  //     info.observ.min_Ejet = min_Ejet;        
  //     info.observ.min_num_particles_jet = min_num_particles_jet;
  //     info.observ.min_num_chargedparticles_jet = min_num_chargedparticles_jet;
  // 
  // 
  //     // Find highest energy track of the event, from its TLorentzVector calculate
  //     // its cos(Theta)
  //     int nPFOs = PFOs.size();
  //     float max_track_E = 0;
  //     TLorentzVector maxE_particle_tlv;
  //     for(int i=0; i<nPFOs; i++){
  //             TLorentzVector particle_tlv = TLorentzVector( PFOs[i]->getMomentum(), PFOs[i]->getEnergy() );
  //             if ( particle_tlv.E() > max_track_E ) {
  //                     maxE_particle_tlv = particle_tlv;
  //             }       
  //     }
  // 
  //     info.observ.cosTheta_EmaxTrack = maxE_particle_tlv.CosTheta();
  // 
  //     // Find all particles within 10degree cone around highest energy track,
  //     // cluster them together and save the combined energy
  //     TLorentzVector EmaxTrack_10cone_tlv = TLorentzVector( 0, 0, 0, 0 );
  //     for(int i=0; i<nPFOs; i++){
  //             TLorentzVector particle_tlv = TLorentzVector( PFOs[i]->getMomentum(), PFOs[i]->getEnergy() );
  //             float angle_to_EmaxTrack = maxE_particle_tlv.Angle(particle_tlv.Vect());
  //             if ( angle_to_EmaxTrack < (10.0/360.0 * 2.0*TMath::Pi()) ) {
  //                     EmaxTrack_10cone_tlv += particle_tlv;
  //             }       
  //     }
  //     // Do not count Emax-track into cone
  //     EmaxTrack_10cone_tlv -= maxE_particle_tlv;
  // 
  //     info.observ.E_10cone_EmaxTrack = EmaxTrack_10cone_tlv.E();
  
  #endif