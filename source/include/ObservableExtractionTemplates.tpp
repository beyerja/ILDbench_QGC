#ifndef OBSERVABLESEXTRACTION_H
#define OBSERVABLESEXTRACTION_H 1

#include "aQGCObservablesProcessor.h"



// // Total event observables
// tree->Branch( "m_recoil", &m_m_recoil,  "m_recoil" );
// tree->Branch( "vis_ET",   &m_vis_ET,    "vis_ET" );
// tree->Branch( "vis_pT",   &m_vis_pT,    "vis_pT" );
// 
// // Vector boson observables
// tree->Branch( "V1_type",      &m_V1_type,     "V1_type/I"); 
// tree->Branch( "V1_m",         &m_V1_m,        "V1_m/F");
// tree->Branch( "V1_pT",        &m_V1_pT,       "V1_pT/F");
// tree->Branch( "V1_cosTheta",  &m_V1_cosTheta, "V1_cosTheta/F");
// tree->Branch( "V2_type",      &m_V2_type,     "V2_type/I"); 
// tree->Branch( "V2_m",         &m_V2_m,        "V2_m/F");
// tree->Branch( "V2_pT",        &m_V2_pT,       "V2_pT/F");
// tree->Branch( "V2_cosTheta",  &m_V2_cosTheta, "V2_cosTheta/F");
// 
// // Combined VB (= WW/ZZ) properties
// tree->Branch( "VV_m",   &m_VV_m,  "VV_m/F");
// tree->Branch( "VV_pT",  &m_VV_pT, "VV_pT/F");
// 
// // Jet property variables
// tree->Branch( "y_34",               &m_y_34,              "y_34/F");
// tree->Branch( "min_jetE",           &m_min_jetE,          "min_jetE/F"); // minimum over all four jets
// tree->Branch( "min_jetNparticles",  &m_min_jetNparticles, "min_jetNparticles/F");
// tree->Branch( "min_jetNcharged",    &m_min_jetNcharged,   "min_jetNcharged/F");
// 
// // Properties of most energetic track
// tree->Branch( "leadEtrack_cosTheta",  &m_leadEtrack_cosTheta, "leadEtrack_cosTheta/F");
// tree->Branch( "leadEtrack_coneE",     &m_leadEtrack_coneE,    "leadEtrack_coneE/F");


template <class ParticleClass> void aQGCObservablesProcessor::findObservables( std::vector<ParticleClass*> &particle_vector ) {
  
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