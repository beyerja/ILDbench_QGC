#include "aQGCObservablesProcessor.h"

//-------------------------------------------------------------------------------------------------

void aQGCObservablesProcessor::checkMCSignalLikeness( MCParticleVec &mc_particles ) {
  /** Check if mc particles contain hard interaction that could correspond to a
      e+e- -> nu nu WW/ZZ -> nu nu q q q q
      hard interaction.
      Cuts for the signal definition are taken from previous ILD analyses.
  */

  MCParticleVec initial_4q2nu   = this->findInitial4q2nu(mc_particles);
  MCParticleVec initial_quarks  = this->getQuarks( initial_4q2nu );
  MCParticleVec initial_nus     = this->getNeutrinos( initial_4q2nu );
  if ( ! initial_4q2nu.empty() ){
    VectorBosonPairFinder <MCParticle> VBpair_finder;
    VBpair_finder.setParticleVector(initial_quarks);
    VBpair_finder.setMinimizationCriterium("MC_VV_likeness");
    VBpair_finder.findBestCandidate();
    
    // If pair finder found possible WW/ZZ candidate check neutrinos inv. mass 
    //  => exclude neutrinos that could come from Z
    //  -> those with combined invariant mass < 100GeV
    //  -> those with generations other than electron neutrino bc e->W+nu always ends up with electron neutrino
    if ( VBpair_finder.getVBPairType() != 0 ) {
      MCParticle* nu1 = initial_nus[0];
      MCParticle* nu2 = initial_nus[1];
      
      TLorentzVector nu1_tlv = TLorentzVector( nu1->getMomentum(), nu1->getEnergy() ); 
      TLorentzVector nu2_tlv = TLorentzVector( nu2->getMomentum(), nu2->getEnergy() ); 
      float m_nunu = (nu1_tlv+nu2_tlv).M();
      
      if ( m_nunu>100 && fabs(nu1->getPDG())==12 && fabs(nu2->getPDG())==12 ) {
        m_signal_type = VBpair_finder.getVBPairType();
      }
    }
    
  } else {
    streamlog_out(DEBUG) << "In analyseMC: Event not of 4q2nu type!" << std::endl;
  }
}

//-------------------------------------------------------------------------------------------------

void aQGCObservablesProcessor::calculateMCTruth( MCParticleVec &mc_particles ) {
  /** Goal: 
  */
  jet_clusterer.Clean(); // Remove new pointers created in clustering process
}

//-------------------------------------------------------------------------------------------------

void aQGCObservablesProcessor::analyseMCTruth() {
  /** Analysing generator level.
  */
  
  // TODO Shift this to different file and change logik of what I'm doing here
  // -> Move everything that explicitly requires MC knowledge to MCTruth
  // -> In MCObservables only keep things that I can "see" as if my MC kinematics where visible in the detector (but ONLY the kinematics)
  
  MCParticleVec mc_particles{};
  this->getMCParticleVector( mc_particles );
  this->checkMCSignalLikeness( mc_particles );
  // this->calculateMCObservables( mc_particles );
}

//-------------------------------------------------------------------------------------------------
