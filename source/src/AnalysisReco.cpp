#include "aQGCObservablesProcessor.h"

//-------------------------------------------------------------------------------------------------

void aQGCObservablesProcessor::getJetVector( ReconstructedParticleVec &output_vector ) {
  try {
    // Get the jet collection from the current event
    LCCollection* collection = m_event->getCollection(m_jetsCollectionName);
    streamlog_out(DEBUG) << "Number of jets: " << collection->getNumberOfElements() << std::endl;
    
    for(int e=0 ; e<collection->getNumberOfElements() ; e++) {
      // Get an object from the collection and convert it to a reconstructed particle
      ReconstructedParticle* particle = static_cast<EVENT::ReconstructedParticle*>(collection->getElementAt(e));
      
      // If the collection type is wrong you end up with a null pointer here.
      // Always check it !
      if(nullptr == particle) {
        streamlog_out(ERROR) << "Wrong object type in collection '" << m_jetsCollectionName << "'" << std::endl;
        continue;
      }
      
      output_vector.push_back(particle);
    }
  }
  catch(EVENT::DataNotAvailableException &) {
    // You end up here if the collection m_pfoCollectionName is not available in this event
    streamlog_out(WARNING) << "Jet collection '" << m_jetsCollectionName << "' is not available !" << std::endl;
  }
}

//-------------------------------------------------------------------------------------------------

void aQGCObservablesProcessor::analyseReconstructed() {
  ReconstructedParticleVec jets{};
  this->getJetVector( jets );
  if ( ! jets.empty() ){
    this->findBosonPairObservables( jets );
    
    m_y_34 = m_event->getCollection(m_jetsCollectionName)->getParameters().getFloatVal( "y_{n-1,n}" );
  }
}

//-------------------------------------------------------------------------------------------------











