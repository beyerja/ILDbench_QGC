#include "aQGCObservablesProcessor.h"

//-------------------------------------------------------------------------------------------------

void aQGCObservablesProcessor::getRecoParticleVector( ReconstructedParticleVec &output_vector ) {
  try {
    // Get the reconstructed particle collection from the current event
    LCCollection* collection = m_event->getCollection(m_pfoCollectionName);
    streamlog_out(DEBUG) << "Number of reco particles: " << collection->getNumberOfElements() << std::endl;
    
    for(int e=0 ; e<collection->getNumberOfElements() ; e++) {
      // Get an object from the collection and convert it to a reconstructed particle
      ReconstructedParticle* particle = static_cast<EVENT::ReconstructedParticle*>(collection->getElementAt(e));
      
      // If the collection type is wrong you end up with a null pointer here.
      // Always check it !
      if(nullptr == particle) {
        streamlog_out(ERROR) << "Wrong object type in collection '" << m_pfoCollectionName << "'" << std::endl;
        continue;
      }
      
      output_vector.push_back(particle);
    }
  }
  catch(EVENT::DataNotAvailableException &) {
    // You end up here if the collection m_pfoCollectionName is not available in this event
    streamlog_out(WARNING) << "Pfo collection '" << m_pfoCollectionName << "' is not available !" << std::endl;
  }
}

//-------------------------------------------------------------------------------------------------

void aQGCObservablesProcessor::analyseReconstructed() {
  ReconstructedParticleVec reco_particles{};
  this->getRecoParticleVector( reco_particles );
  this->findObservables( reco_particles );
}

//-------------------------------------------------------------------------------------------------











