
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
    
  registerInputCollection( LCIO::MCPARTICLE,
    "MCParticleCollection",
    "Name of the MC particle collection",
    m_mcCollectionName,
    std::string("MCParticle")
  );
         
  // Register input parameters
  registerProcessorParameter("OutputRootFileName",
    "Path of output rootfile",
    m_rootfilename,
    std::string("test.root"));
    
    
}

//-------------------------------------------------------------------------------------------------

void aQGCObservablesProcessor::init() {
  // Usually a good idea to print parameters
  printParameters(); // method from Processor
  
  //TODO Fill meta info in TNamed!!
  
  m_rootfile = new TFile(m_rootfilename.c_str(), "recreate");
  m_mctree = new TTree("mcObservablesTree", "mcObservablesTree");
  m_recotree = new TTree("recoObservablesTree", "recoObservablesTree");
  
  this->setTFileContent();
}

//-------------------------------------------------------------------------------------------------



void aQGCObservablesProcessor::processRunHeader( EVENT::LCRunHeader* run ) {
  streamlog_out(MESSAGE) << "Starting run no " << run->getRunNumber() << std::endl;
  // LCRunHeader objects can be printed using LCTOOLS class
  UTIL::LCTOOLS::dumpRunHeader(run);
}

//-------------------------------------------------------------------------------------------------

void aQGCObservablesProcessor::processEvent( EVENT::LCEvent * event ) {
  
  m_event = event;
  
  // Basic idea: perform same analysis first on MC then on Recos
  // -> will have some obvious differences, but will in same output observables
  // So running structure: 
  //  Empty variables -> Fill & Write MC -> Empty variables -> Fill & Write Reco
  
  this->CleanEvent();
  // MC
  m_mctree->Fill();
  this->CleanEvent();
  // Reco
  m_recotree->Fill();
  
  
  streamlog_out(DEBUG) << "Processing event no " << m_event->getEventNumber() << " - run " << m_event->getEventNumber() << std::endl;
  
  try {
    // Get the reconstructed particle collection from the current event
    EVENT::LCCollection* collection = m_event->getCollection(m_pfoCollectionName);
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
  
  std::cout << "aQGCAnalysis: end() " << this->name() // << " processed " << m_nEvtSum << " events in " << m_nRunSum << " runs " << std::endl
  << "Rootfile: " << m_rootfilename.c_str() << std::endl;

  // Write to .root file
  m_rootfile->cd();
  
  m_detector_model.Write();
  m_e_polarization.Write();
  m_p_polarization.Write();
  m_process_name.Write();
  m_cross_section.Write();
  m_cross_section_error.Write();
  
  
  m_mctree->Write();
  m_recotree->Write();
  
  m_rootfile->Close();
  
  delete m_rootfile;
  
  delete m_mctree;
  delete m_recotree;
}

