#include "aQGCObservablesProcessor.h"


//-------------------------------------------------------------------------------------------------

ReconstructedParticleVec aQGCObservablesProcessor::getRecoCollectionVector( std::string collectionName ) {
  ReconstructedParticleVec output_vector {};
  
  try {
    // Get the jet collection from the current event
    LCCollection* collection = m_event->getCollection(collectionName);
    streamlog_out(DEBUG)  << "In getRecoCollectionVector: Number of particles in collection " << collectionName << ": " 
                          << collection->getNumberOfElements() << std::endl;
    
    for(int e=0 ; e<collection->getNumberOfElements() ; e++) {
      // Get an object from the collection and convert it to a reconstructed particle
      ReconstructedParticle* particle = static_cast<EVENT::ReconstructedParticle*>(collection->getElementAt(e));
      
      // If the collection type is wrong you end up with a null pointer here.
      // Always check it !
      if(nullptr == particle) {
        streamlog_out(ERROR) << "ERROR in getRecoCollectionVector: Wrong object type in collection '" << collectionName << "'" << std::endl;
        continue;
      }
      
      output_vector.push_back(particle);
    }
  }
  catch(EVENT::DataNotAvailableException &) {
    // You end up here if the collection m_pfoCollectionName is not available in this event
    streamlog_out(WARNING) << "In getRecoCollectionVector: Collection '" << collectionName << "' is not available !" << std::endl;
  }
  
  return output_vector;
}

//-------------------------------------------------------------------------------------------------

void aQGCObservablesProcessor::analyseReconstructed() {
  
  // Try to find jet observables
  ReconstructedParticleVec jets = this->getRecoCollectionVector( m_jetsCollectionName );
  if ( ! jets.empty() ){
    this->findJetObservables( jets );
    
    m_y_34 = m_event->getCollection(m_jetsCollectionName)->getParameters().getFloatVal( "y_{n-1,n}" );
  }
  
  // Try to find observables that consider complete event
  ReconstructedParticleVec reco_particles = this->getRecoCollectionVector( m_pfoCollectionName );
  if ( ! reco_particles.empty() ){
    this->findParticleObservables( reco_particles );
  }
  
  // Test if any isolated leptons were found
  ReconstructedParticleVec isolated_leptons = this->getRecoCollectionVector( m_isolatedLeptonsCollectionName );
  if ( ! isolated_leptons.empty() ){
    m_found_isolep = true;
  }
  
  
  //TODO: If flavour information available: Set Vector Boson type 
}

//-------------------------------------------------------------------------------------------------