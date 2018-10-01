#include "aQGCObservablesProcessor.h"

//-------------------------------------------------------------------------------------------------

void aQGCObservablesProcessor::calculateMCTruth( MCParticleVec &mc_particles ) {
  /** Check if mc particles contain hard interaction that could correspond to a
      e+e- -> nu nu WW/ZZ -> nu nu q q q q
      hard interaction.
      Cuts for the signal definition are taken from previous ILD analyses.
  */

  MCParticleVec initial_4q2nu   = this->findInitial4q2nu(mc_particles);
  MCParticleVec initial_quarks  = this->getQuarks( initial_4q2nu );
  MCParticleVec initial_nus     = this->getNeutrinos( initial_4q2nu );
  
  if ( initial_4q2nu.empty() ){
    streamlog_out(DEBUG) << "In analyseMC: Event not of 4q2nu type!" << std::endl;
    return;
  } 

  VectorBosonPairFinder <MCParticle> VBpair_finder;
  VBpair_finder.setParticleVector(initial_quarks);
  VBpair_finder.setMinimizationCriterium("MC_VV_likeness");
  VBpair_finder.findBestCandidate();
  
  // If pair finder found possible WW/ZZ candidate check neutrinos inv. mass 
  //  => exclude neutrinos that could come from Z
  //  -> those with combined invariant mass < 100GeV
  //  -> those with generations other than electron neutrino bc e->W+nu always ends up with electron neutrino
  if ( VBpair_finder.getVBPairType() == 0 ) {
    streamlog_out(DEBUG) << "In analyseMC: 4q state not signal like!" << std::endl;
    return;
  }
  
  MCParticle* nu1 = initial_nus[0];
  MCParticle* nu2 = initial_nus[1];
  
  TLorentzVector nu1_tlv = TLorentzVector( nu1->getMomentum(), nu1->getEnergy() ); 
  TLorentzVector nu2_tlv = TLorentzVector( nu2->getMomentum(), nu2->getEnergy() ); 
  float m_nunu = (nu1_tlv+nu2_tlv).M();
  
  if ( m_nunu<100 && fabs(nu1->getPDG())!=12 && fabs(nu2->getPDG())!=12 ) {
    streamlog_out(DEBUG) << "In analyseMC: 2nu state not signal like!" << std::endl;
    return;
  } 
  
  // Event is signal-like, now get the MC parameters
  m_signal_type = VBpair_finder.getVBPairType();
  streamlog_out(DEBUG) << "In analyseMC: Event is of VB type " << m_signal_type << std::endl;
  this->findVectorBosonObservables( VBpair_finder );
}

//-------------------------------------------------------------------------------------------------

void aQGCObservablesProcessor::analyseMCTruth() {
  /** Analysing generator level.
  */
  
  MCParticleVec mc_particles{};
  this->getMCParticleVector( mc_particles );
  this->calculateMCTruth( mc_particles );
}

//-------------------------------------------------------------------------------------------------
