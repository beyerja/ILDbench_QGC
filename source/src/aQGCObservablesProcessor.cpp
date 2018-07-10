
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
    std::string("MCParticle") );
  
  registerInputCollection( 
    LCIO::RECONSTRUCTEDPARTICLE,         
    "ClusteredJets",                     
    "The collection name for the clustered jets from reco",   
    m_jetsCollectionName,                 
    std::string("FastJets") );        
  
  registerInputCollection( 
    LCIO::RECONSTRUCTEDPARTICLE,         
    "PFOsFromClusteredJets",                     
    "The collection name for PFOs from clustered jets",   
    m_jetPFOsCollectionName,                 
    std::string("PFOsFromFastJet") );        


    
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
  
  this->setTTreeBranches();
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
  
  streamlog_out(DEBUG) << "Processing event no " << m_event->getEventNumber() << " - run " << m_event->getEventNumber() << std::endl;
  
  this->CleanEvent();
  streamlog_out(DEBUG) << "Processing MC info " << std::endl;
  m_mctree->Fill();
  
  this->CleanEvent();
  streamlog_out(DEBUG) << "Processing reconstructed info " << std::endl;
  this->analyseReconstructed();
  m_recotree->Fill();
  
}

//-------------------------------------------------------------------------------------------------

void aQGCObservablesProcessor::end() {
  // Cleanup your mess here !
  
  std::cout << "aQGCAnalysis: end() " << this->name() // << " processed " << m_nEvtSum << " events in " << m_nRunSum << " runs " << std::endl
  << "Rootfile: " << m_rootfilename.c_str() << std::endl;

  this->setInputInfoStrings(); // Use last event to read file parameters
  
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
}

