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

void aQGCObservablesProcessor::analyseMC() {
  MCParticleVec mc_particles{};
  this->getMCParticleVector( mc_particles );
  if ( ! mc_particles.empty() ){
    // this->findParticleObservables( mc_particles );
  }
}