#include "TJJetRecoParticleFinderProcessor.h"

TJJetRecoParticleFinder aTJJetRecoParticleFinder ;

TJJetRecoParticleFinder::TJJetRecoParticleFinder() : Processor("TJJetRecoParticleFinder") {

  _description = "TJJetRecoParticleFinder: Use TrueJet output (based on MC info) to collect ReconstructedParticles originating only from hadronic jets";

  // Standard input collections
  registerInputCollection( LCIO::RECONSTRUCTEDPARTICLE,
    "InputAllPFOsCollection",
    "Name of the PFOs collection",
    _colAllPFOs,
    std::string("PandoraPFOs") ) ;

  registerInputCollection( LCIO::MCPARTICLE,
    "MCParticleCollection",
    "Name of the MC particle collection",
    _colMC,
    std::string("MCParticle") ) ;

	registerInputCollection( LCIO::LCRELATION,
		"MCPFORelation",
		"Relation between MC and PFO particles",
		_mcpfoRelation,
		std::string("RecoMCTruthLink") );

	registerInputCollection( LCIO::LCRELATION,
		"RecoMCTruthLink",
		"Name of the RecoMCTruthLink input collection"  ,
		_recoMCTruthLink,
		std::string("RecoMCTruthLink") ) ;

	registerInputCollection( LCIO::RECONSTRUCTEDPARTICLE,
		"TrueJets" ,
		"Name of the TrueJetCollection output collection"  ,
		_trueJetCollectionName ,
		std::string("TrueJets") ) ;

	registerInputCollection( LCIO::RECONSTRUCTEDPARTICLE,
		"FinalColourNeutrals" ,
		"Name of the FinalColourNeutralCollection output collection"  ,
		_finalColourNeutralCollectionName ,
		std::string("FinalColourNeutrals") ) ;

	registerInputCollection( LCIO::RECONSTRUCTEDPARTICLE,
		"InitialColourNeutrals" ,
		"Name of the InitialColourNeutralCollection output collection"  ,
		_initialColourNeutralCollectionName ,
		std::string("InitialColourNeutrals") ) ;

	 registerInputCollection( LCIO::LCRELATION,
		"TrueJetPFOLink" ,
		"Name of the TrueJetPFOLink output collection"  ,
		_trueJetPFOLink,
		std::string("TrueJetPFOLink") ) ;

	registerInputCollection( LCIO::LCRELATION,
		"TrueJetMCParticleLink" ,
		"Name of the TrueJetMCParticleLink output collection"  ,
		_trueJetMCParticleLink,
		std::string("TrueJetMCParticleLink") ) ;

	registerInputCollection( LCIO::LCRELATION,
		"FinalElementonLink" ,
		"Name of the  FinalElementonLink output collection"  ,
		_finalElementonLink,
		std::string("FinalElementonLink") ) ;

	registerInputCollection( LCIO::LCRELATION,
		"InitialElementonLink" ,
		"Name of the  InitialElementonLink output collection"  ,
		_initialElementonLink,
		std::string("InitialElementonLink") ) ;

	registerInputCollection( LCIO::LCRELATION,
		"FinalColourNeutralLink" ,
		"Name of the  FinalColourNeutralLink output collection"  ,
		_finalColourNeutralLink,
		std::string("FinalColourNeutralLink") ) ;

	registerInputCollection( LCIO::LCRELATION,
		"InitialColourNeutralLink" ,
		"Name of the  InitialColourNeutralLink output collection"  ,
		_initialColourNeutralLink,
		std::string("InitialColourNeutralLink") ) ;

	registerOutputCollection( LCIO::RECONSTRUCTEDPARTICLE,
		"RecosFromHadronicJets" ,
		"Name of the output collection containing reconstructed particles from hadronic jets (found with TrueJet)"  ,
		_RecosFromHadronicJetsName,
		std::string("RecosFromHadronicJets") ) ;
}

void TJJetRecoParticleFinder::init() {
  streamlog_out(DEBUG4) << "   init called  " << std::endl ;

  // usually a good idea to
  printParameters() ;
  
  _nRun = 0 ;
  _nEvt = 0 ;
}

void TJJetRecoParticleFinder::processRunHeader( LCRunHeader* /*run*/) {
  streamlog_out(DEBUG4) << "Run number: " << _nRun << std::endl ;
  _nRun++ ;
}

void TJJetRecoParticleFinder::processEvent( LCEvent * evt ) {
  // this gets called for every event
  // usually the working horse ...
  
  //-- note: this will not be printed if compiled w/o MARLINDEBUG=1 !
  streamlog_out(DEBUG4) << "   processing event: " << evt->getEventNumber()
  << "   in run:  " << evt->getRunNumber() << std::endl ;

  // tj is a pointer to a Trujet_Parser, with the data of this processor object:
  tj= this ;
  // this method gets all the collections needed + initialises a few convienent variables.
  tj->getall(evt);

  // Collect all jets belonging to some initial quark or gluon
  // Do this by looping over the jets (and not the initial quarks) to avoid double counting the jets 
  LCCollectionVec *particles_from_hadronic_jets = new LCCollectionVec(LCIO::RECONSTRUCTEDPARTICLE);
  particles_from_hadronic_jets->setSubset(true);
  streamlog_out(DEBUG4) << "In processEvent: Searching for particles within jets."  << std::endl;
  streamlog_out(DEBUG3) << "In processEvent: Got " << tj->njets() << " jets of types: ";
  for (int ijet=0 ; ijet<tj->njets(); ijet++ ) {
    if (  seen_partics(ijet).size() > 0 ) {
      // Jet types: 1 string, 2 lepton, 3 cluster, 4 ISR, 5 overlay.
      int jet_type =  type_jet(ijet);
      streamlog_out(DEBUG3) << jet_type;
      if ( ( jet_type % 100 == 1 ) || ( jet_type % 100 == 3 ) ) { // According to TrueJet marking of hadronic jets
        streamlog_out(DEBUG3) << "(hadronic)";
        ReconstructedParticle *jet = jets->at(ijet); // jets is defined in TrueJet_Parser header (sorry...)
        std::vector<lcio::ReconstructedParticle*> particles_in_jet = jet->getParticles();
        for ( std::vector<lcio::ReconstructedParticle*>::const_iterator i_part=particles_in_jet.begin(); i_part!=particles_in_jet.end(); ++i_part) {
          particles_from_hadronic_jets->addElement( (*i_part) );
        }
      }
      streamlog_out(DEBUG3) << " ";
    } else {
      streamlog_out(DEBUG3) << type_jet(ijet) << "(No PFOs) ";
    }
  }
  streamlog_out(DEBUG3) << std::endl;
  
  if (particles_from_hadronic_jets->getNumberOfElements() == 0) {
    streamlog_out(WARNING) << "In processEvent: No hadronic jets found!" << std::endl;
  } else {
    streamlog_out(DEBUG4) << "In processEvent: Found " << particles_from_hadronic_jets->getNumberOfElements() << " reco-particles from hadronic jets." << std::endl;
    streamlog_out(DEBUG4) << "                 (Total number of reco-particles in event was " << evt->getCollection(_colAllPFOs)->getNumberOfElements() << " )." << std::endl;
  }
  
  streamlog_out(DEBUG4) << "In processEvent: Adding collection " << _RecosFromHadronicJetsName << std::endl;
  evt->addCollection( particles_from_hadronic_jets, _RecosFromHadronicJetsName.c_str() );

  _nEvt ++ ;
}



void TJJetRecoParticleFinder::check( LCEvent * evt ) {
    // nothing to check here - could be used to fill checkplots in reconstruction processor
}


void TJJetRecoParticleFinder::end(){
}
