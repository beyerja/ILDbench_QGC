#include "aQGCObservablesProcessor.h"


//-------------------------------------------------------------------------------------------------

MCParticleVec aQGCObservablesProcessor::findVisibleMC( MCParticleVec &mc_particles ) {
    // Find daughters after hard interaction
    MCParticleVec initial_ee = this->findLastInitialee( mc_particles );
    MCParticleSet ee_vis_daughters {};
    for(auto const& e: initial_ee ) {
      MCParticleVec e_daughters = e->getDaughters();
      for ( unsigned int i_daughter = 0; i_daughter<e_daughters.size(); i_daughter++ ){
        // Only take detectable particles:
        //  -> Not neutrinos
        //  -> Not going into beampipe
        MCParticle* e_daughter = e_daughters[i_daughter];
        TLorentzVector e_daughter_tlv = TLorentzVector( e_daughter->getMomentum(), e_daughter->getEnergy() ); 
        if ( (! PDGIDChecks::isNeutrinoID(e_daughter->getPDG())) &&
              e_daughter_tlv.CosTheta() < 0.99 ) {
          ee_vis_daughters.insert( e_daughters[i_daughter] );
        }
      }
    }
    
    return MCParticleVec( ee_vis_daughters.begin(), ee_vis_daughters.end() );
}

//-------------------------------------------------------------------------------------------------

void aQGCObservablesProcessor::calculateMCObservables( MCParticleVec &mc_particles ) {
  /** Goal: 
        Find values on MC level comparable to the ones on reconstructed level.
        Then we can test what influence cuts have - kind of - on theory level.
      Method:
        - Find visible particles after hard interaction
        - Cluster them (if necessary) to 4 "jets" using same algorithm as reco jet clustering
        - Find observables on those clustered "MC jets"
        - If visible contains a charged lepton set tag as found, then influence of lepton tagging can be tested by ignore/using flag
  */
  
  MCParticleVec visible_initial_state = this->findVisibleMC( mc_particles );
  streamlog_out(DEBUG) << "In calculateMCObservables: Found " << visible_initial_state.size() << " visible initial particles!" << std::endl;
  
  // TODO THIS SHOULD NOT BE HERE BUT IN MCTRUTH
  // Look if there's a charged lepton
  if ( this->getChargedLeptons(visible_initial_state).size() > 0 ) {
    m_found_isolep = true;
  }
  
  // Do jet clustering
  JetClusterer <MCParticle> jet_clusterer;
  jet_clusterer.setInputParticles( visible_initial_state );
  jet_clusterer.setClusteringAlgorithm( "ee_kt_algorithm" );
  jet_clusterer.setNFinalParticles( 4 );
  jet_clusterer.runClustering();
  MCParticleVec clustered_particles = jet_clusterer.getClusteredParticles();
  
  // Get Jet observables (gooooood enough!)
  if ( clustered_particles.size() > 3 ) {
    this->findJetObservables( clustered_particles );
  } else {
    streamlog_out(DEBUG) << "In calculateMCObservables: Found less than 4 visible MC after hard scatter." << std::endl;
  }
      
  jet_clusterer.Clean(); // Remove new pointers created in clustering process
}
  
//-------------------------------------------------------------------------------------------------

void aQGCObservablesProcessor::analyseMCObservables() {
  /** Analysing generator level.
  */
  
  MCParticleVec mc_particles{};
  this->getMCParticleVector( mc_particles );
  this->calculateMCObservables( mc_particles );
}

//-------------------------------------------------------------------------------------------------