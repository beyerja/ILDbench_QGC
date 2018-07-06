
// -- your process header
#include "aQGCObservablesProcessor.h"


// This line allows to register your processor in marlin when calling "Marlin steeringFile.xml"
aQGCObservablesProcessor anaQGCObservablesProcessor;

//-------------------------------------------------------------------------------------------------

aQGCObservablesProcessor::aQGCObservablesProcessor() : Processor("aQGCObservablesProcessor") {
  // _description comes from Processor
  _description = "Processor searching the event for variables relevant for an aQGC analysis";
  
  // Register input collections
  registerInputCollection( 
    LCIO::RECONSTRUCTEDPARTICLE,         // The collection type. Checkout the LCIO documentation for other types
    "PfoCollection",                     // The parameter name to read from steering file 
    "The Pandora PFO collection name",   // A parameter description. Please fill this correctly
    m_pfoCollectionName,                 // Your variable to store the result after steering file parsing
    std::string("PandoraPFOs") );        // That's the default value, in case 
         
  // Register input parameters
  registerProcessorParameter("OutputRootFileName",
    "Path of output rootfile",
    m_rootfilename,
    "test.root");
    
    
}

//-------------------------------------------------------------------------------------------------

void aQGCObservablesProcessor::init() {
  // Usually a good idea to print parameters
  printParameters(); // method from Processor
}

//-------------------------------------------------------------------------------------------------

void aQGCObservablesProcessor::processRunHeader( EVENT::LCRunHeader* run ) {
  streamlog_out(MESSAGE) << "Starting run no " << run->getRunNumber() << std::endl;
  // LCRunHeader objects can be printed using LCTOOLS class
  UTIL::LCTOOLS::dumpRunHeader(run);
}

//-------------------------------------------------------------------------------------------------

void aQGCObservablesProcessor::processEvent( EVENT::LCEvent * event ) {
  streamlog_out(DEBUG) << "Processing event no " << event->getEventNumber() << " - run " << event->getEventNumber() << std::endl;
  
  try {
    // Get the reconstructed particle collection from the current event
    EVENT::LCCollection* collection = event->getCollection(m_pfoCollectionName);
    streamlog_out(DEBUG) << "Number of reco particles: " << collection->getNumberOfElements() << std::endl;
    
    for(int e=0 ; e<collection->getNumberOfElements() ; e++) {
      // Get an object from the collection and convert it to a reconstructed particle
      EVENT::ReconstructedParticle* particle = static_cast<EVENT::ReconstructedParticle*>(collection->getElementAt(e));
      
      // If the collection type is wrong you end up with a null pointer here.
      // Always check it !
      if(nullptr == particle) {
        streamlog_out(ERROR) << "Wrong object type in collection '" << m_pfoCollectionName << "'" << std::endl;
        continue;
      }
      
      // Start analysing your particle here ! Enjoy !
      // Example:
      if(particle->getEnergy() < m_pfoEnergyCut) {
        continue;
      }
      
      streamlog_out(DEBUG) << "Particle passes the energy cut !" << std::endl;
    }
  }
  catch(EVENT::DataNotAvailableException &) {
    // You end up here if the collection m_pfoCollectionName is not available in this event
    streamlog_out(WARNING) << "Pfo collection '" << m_pfoCollectionName << "' is not available !" << std::endl;
  }
}

//-------------------------------------------------------------------------------------------------

void aQGCObservablesProcessor::end() {
  // Cleanup your mess here !
}

