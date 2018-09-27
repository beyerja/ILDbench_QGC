#include "aQGCObservablesProcessor.h"

//-------------------------------------------------------------------------------------------------

void aQGCObservablesProcessor::getMCParticleVector( MCParticleVec &output_vector ) {
  try {
    // Get the MC particle collection from the current event
    LCCollection* collection = m_event->getCollection(m_mcCollectionName);
    streamlog_out(DEBUG) << "Number of mc particles: " << collection->getNumberOfElements() << std::endl;

    for(int e=0 ; e<collection->getNumberOfElements() ; e++) {
      // Get an object from the collection and convert it to a MC particle
      MCParticle* particle = static_cast<EVENT::MCParticle*>(collection->getElementAt(e));

      // If the collection type is wrong you end up with a null pointer here.
      // Always check it !
      if(nullptr == particle) {
        streamlog_out(ERROR) << "Wrong object type in collection '" << m_mcCollectionName << "'" << std::endl;
        continue;
      }

      output_vector.push_back(particle);
    }
  }
  catch(EVENT::DataNotAvailableException &) {
    // You end up here if the collection m_pfoCollectionName is not available in this event
    streamlog_out(WARNING) << "MC collection '" << m_pfoCollectionName << "' is not available !" << std::endl;
  }
}

//-------------------------------------------------------------------------------------------------

MCParticleVec aQGCObservablesProcessor::findLastInitialee( MCParticleVec &mc_particles ) {
  /** Find initial e+ e- in MC particles before interaction (but after ISR).
      Calls itself recursively if multiple versions of the initial e+e- exist
      in the MCParticles.
      Recursion ends when hard scatter is found.
  */
  
  MCParticleVec initial_ee{};
  
  MCParticle* first_electron {};
  MCParticle* first_positron {};
  
  // First get first e+ and e- in MC list
  for ( auto & mc: mc_particles ) {//unsigned int i_mc = 0; i_mc<mc_particles.size(); i_mc++ ) {
    // MCParticle* mc = mc_particles[i_mc];
    int mc_pdg = mc->getPDG();
    
    if ( mc_pdg == -11 )  { first_electron = mc; }
    if ( mc_pdg == 11 )   { first_positron = mc; }
  
    // Stop when both found
    if ( nullptr != first_electron && nullptr != first_positron ) break;
  }
  
  if ( nullptr == first_electron || nullptr == first_positron ) {
    streamlog_out(ERROR) << "ERROR in findLastInitialee: No initial e+e- particles found!" << std::endl;
  }
  
  // Then go through electron daughters and try to see if it consists
  // of e+e- + some gammas -> If so take next step e+e- and test again
  // Everything else would already be hard interaction
  MCParticleVec electron_daughters ( first_electron->getDaughters() );
  MCParticleVec positron_daughters ( first_positron->getDaughters() );
  MCParticleVec ee_daughters {};
  ee_daughters.insert( ee_daughters.end(), electron_daughters.begin(), electron_daughters.end() );
  ee_daughters.insert( ee_daughters.end(), positron_daughters.begin(), positron_daughters.end() );
  MCParticle* daughter_electron {};
  MCParticle* daughter_positron {};
  bool found_hardscatter_particles = false;
  // TODO GO OVER BOTH DAUGHTERS
  for ( auto & daughter: ee_daughters ) {
    int daughter_pdg = daughter->getPDG();
    
    if      ( daughter_pdg == -11 ) { daughter_electron = daughter; }
    else if ( daughter_pdg == 11 )  { daughter_positron = daughter; }
    else if ( daughter_pdg == 22 )  { continue; }
    else { 
      /* Found some non-{e+,e-,gamma} particle, break and return first e+e- */
      initial_ee = { first_electron, first_positron };
      found_hardscatter_particles = true;
    }
  }
  if ( !found_hardscatter_particles ) {
    // Have not found hard scatter yet, try next level recursively
    streamlog_out(DEBUG) << "In findLastInitialee: Didn't end at hard scatter yet, try another level." << std::endl;
    MCParticleVec new_level_ee {daughter_electron, daughter_positron};
    initial_ee = this->findLastInitialee( new_level_ee );
  } else {
    streamlog_out(DEBUG) << "In findLastInitialee: Found hard scatter, taking ee directly before it." << std::endl;
  }
  
  // If final e+e- not already found set here
  if ( initial_ee.empty() ) {
    // If daughter e+e- found use them, else use first
    if ( nullptr != daughter_electron && nullptr != daughter_positron ) {
      initial_ee = { daughter_electron, daughter_positron };
    }
    else {
      initial_ee = { first_electron, first_positron };
    }
  }
  
  return initial_ee;
}

//-------------------------------------------------------------------------------------------------

MCParticleVec aQGCObservablesProcessor::findInitial4q2nu( MCParticleVec &mc_particles ) {
  /** Try to find initial state 4q+2neutrino initial state if available,
      otherwise leave output_vector empty
  */
  
  MCParticleVec initial_4q2nu {};
  
  MCParticleVec initial_ee = this->findLastInitialee( mc_particles );

  // Find daughters after hard interaction
  MCParticleSet ee_daughters {};
  for(auto const& e: initial_ee ) {
    MCParticleVec e_daughters = e->getDaughters();
    for ( auto & e_daughter: e_daughters ) {//unsigned int i_daughter = 0; i_daughter<e_daughters.size(); i_daughter++ ){
      // Special case: intermediate vector bosons inserted by WHIZARD 
      // -> ignore them, go over their daughters instead
      int daughter_pdg = e_daughter->getPDG();
      if ( PDGIDChecks::isVectorBosonID(daughter_pdg) ) {
        MCParticleVec V_daughters = e_daughter->getDaughters();
        for ( auto & V_daughter: V_daughters ) {
          streamlog_out(DEBUG) << "In findInitial4q2nu: Found intermediate vector boson. Ignoring it and taking daughters instead." << std::endl;
          ee_daughters.insert( V_daughter );
        }
      } else {
        ee_daughters.insert( e_daughter );
      }
    }
  }
  
  streamlog_out(DEBUG) << "In findInitial4q2nu: ee_daughter IDs: ";
  for ( MCParticleSet::iterator mc_it=ee_daughters.begin(); mc_it!=ee_daughters.end(); ++mc_it ){
    streamlog_out(DEBUG) << (*mc_it)->getPDG() << " " ;
  }
  streamlog_out(DEBUG) << std::endl;
  
  
  // Find 4q2nu topology if available
  if ( ee_daughters.size() == 6 ){
    int n_quarks {};
    int n_nus {};
    for ( MCParticleSet::iterator mc_it=ee_daughters.begin(); mc_it!=ee_daughters.end(); ++mc_it ){
      int daughter_pdg = (*mc_it)->getPDG();
      if ( PDGIDChecks::isNeutrinoID(daughter_pdg) ) n_nus++;
      if ( PDGIDChecks::isQuarkID(daughter_pdg)    ) n_quarks++;
    }
    
    if ( n_quarks==4 && n_nus==2 ){
      initial_4q2nu = MCParticleVec( ee_daughters.begin(), ee_daughters.end() );
    }  
    else {
      streamlog_out(DEBUG) << "In findInitial4q2nu: e+e- don't have 4q + 2nu daughters. N quarks: " << n_quarks << ", N nus: " << n_nus << std::endl;
    }
  } else {
    streamlog_out(DEBUG) << "In findInitial4q2nu: e+e- don't have 6 daughters. N daughters: " << ee_daughters.size() << std::endl;
  }
  
  return initial_4q2nu;
}

//-------------------------------------------------------------------------------------------------

MCParticleVec aQGCObservablesProcessor::getQuarks( MCParticleVec &mc_particles ) {
  MCParticleVec mc_quarks;
  for ( unsigned int i_mc=0; i_mc<mc_particles.size(); i_mc++ ) {
    if ( PDGIDChecks::isQuarkID( mc_particles[i_mc]->getPDG() ) ) {
      mc_quarks.push_back(mc_particles[i_mc]);
    }
  }
  return mc_quarks;
}

//-------------------------------------------------------------------------------------------------

MCParticleVec aQGCObservablesProcessor::getNeutrinos( MCParticleVec &mc_particles ) {
  MCParticleVec mc_nus;
  for ( unsigned int i_mc=0; i_mc<mc_particles.size(); i_mc++ ) {
    if ( PDGIDChecks::isNeutrinoID( mc_particles[i_mc]->getPDG() ) ) {
      mc_nus.push_back(mc_particles[i_mc]);
    }
  }
  return mc_nus;
}

//-------------------------------------------------------------------------------------------------

MCParticleVec aQGCObservablesProcessor::getChargedLeptons( MCParticleVec &mc_particles ) {
  MCParticleVec mc_charged_leptons;
  for ( unsigned int i_mc=0; i_mc<mc_particles.size(); i_mc++ ) {
    if ( PDGIDChecks::isChargedLeptonID( mc_particles[i_mc]->getPDG() ) ) {
      mc_charged_leptons.push_back(mc_particles[i_mc]);
    }
  }
  return mc_charged_leptons;
}

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

void aQGCObservablesProcessor::analyseMC() {
  /** Analysing generator level.
  */
  
  MCParticleVec mc_particles{};
  this->getMCParticleVector( mc_particles );
  this->checkMCSignalLikeness( mc_particles );
  this->calculateMCObservables( mc_particles );
}

//-------------------------------------------------------------------------------------------------

















