#include "JakobsVBSProcessor.h"
#include <iostream>
#include <vector>

#include <EVENT/LCCollection.h>
#include <EVENT/MCParticle.h>

// ----- include for verbosity dependend logging ---------
#include "marlin/VerbosityLevels.h"


#include "TFile.h"
#include "TTree.h"
#include "TMath.h"
#include "TVector3.h"
#include "TLorentzVector.h"




using namespace lcio ;
using namespace marlin ;

//LCRelationNavigator* reltrue_tj =0;

JakobsVBSProcessor aJakobsVBSProcessor ;


JakobsVBSProcessor::JakobsVBSProcessor() : Processor("JakobsVBSProcessor") {

    _description = "JakobsVBSProcessor: Search for VBS at the ILD detector" ;


    // Standard input collections
    registerInputCollection( LCIO::RECONSTRUCTEDPARTICLE,
            "InputAllPFOsCollection",
            "Name of the PFOs collection",
            _colAllPFOs,
            std::string("PandoraPFOs")
    );

    registerInputCollection( LCIO::MCPARTICLE,
            "MCParticleCollection",
            "Name of the MC particle collection",
            _colMC,
            std::string("MCParticlesSkimmed2")
    );


	// Process specific input collections
    registerInputCollection( LCIO::RECONSTRUCTEDPARTICLE,
            "InputFastJetsCollection",
            "Name of the jet collection",
            _colFastJets,
            std::string("FastJets")
    );

    registerInputCollection( LCIO::RECONSTRUCTEDPARTICLE,
            "InputPFOsFromFastJetCollection",
            "Name of the collection holding the PFOs in the FastJets",
            _colPFOsFromFastJet,
            std::string("PFOsFromFastJet")
    );

	registerInputCollection( LCIO::RECONSTRUCTEDPARTICLE,
            "InputIsolepsCollection",
            "Name of the isolated lepton collection",
            _colIsoleps,
            std::string("Isoleps")
    );


	registerInputCollection( LCIO::LCRELATION,
			"MCPFORelation",
			"Relation between MC and PFO particles",
			_mcpfoRelation,
			std::string("RecoMCTruthLink2") );


	// Inputs from TrueJet processor
	//WARNING Remove the "2"s when possible

	registerInputCollection( LCIO::LCRELATION,
    		"RecoMCTruthLink",
    		"Name of the RecoMCTruthLink input collection"  ,
			_recoMCTruthLink,
			std::string("RecoMCTruthLink2") ) ;

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



	// Technical
	// -> e.g. rootfile names

    registerProcessorParameter( "RootFileName",
            "Name of Root file (default: output.root)",
            _rootfilename,
            std::string("/nfs/dust/ilc/group/ild/beyerjac/VBS/nunu_hadrons/output/root_files/output.root")
    );


}



void JakobsVBSProcessor::init() {

    streamlog_out(DEBUG) << "   init called  " << std::endl ;

    // usually a good idea to
    printParameters() ;

    makeNTuple() ;

    _nRun = 0 ;
    _nEvt = 0 ;

	file_info.init();

}


void JakobsVBSProcessor::makeNTuple() {
    _otfile = new TFile( _rootfilename.c_str() , "RECREATE" );
    _datatrain= new TTree( "datatrain" , "events" );

	SetFileInfoBranches( file_info );
	SetGeneralEventInfoBranches( info );
	SetGeneratorLevelBranches( info );
	SetTrueJetLevelBranches( info );
	SetDetectorLevelBranches( info );

	SetTJRecosInJetsBranches( info );
	SetTJMCsInJetsBranches( info );
}



void JakobsVBSProcessor::processRunHeader( LCRunHeader* run) {

    _nRun++ ;
}




void JakobsVBSProcessor::processEvent( LCEvent * evt ) {

    // this gets called for every event
    // usually the working horse ...

    // Clear memory
    memset( &info, 0, sizeof(info) );
	info.init();
	info.evtN = _nEvt;

	// Get information from the event header (e.g. cross-section)
	file_info.xsection = evt->getParameters().getFloatVal("CrossSection_fb");

	// tj is a pointer to a Trujet_Parser, with the data of this processor object:
	Adjusted_TrueJet_Parser* tj= this ;
	// this method gets all the collections needed + initialises a few convienent variables.
	tj->getall(evt);

	// Use TrueJet to get those reconstructed particles that actually originate from a parton jet,
	// those from ISR and those from overlay
	int Njets_correct=tj->njets();  // True number of jets (=number of FS partons)

//TODO Move PFO readout from TJ to something similar to collection reader and do reading in VBSAnalysis
	std::vector<ReconstructedParticleVec> TJ_jetPFOs_correct;
	std::vector<ReconstructedParticleVec> TJ_jetPFOs_fromISR;
	std::vector<ReconstructedParticleVec> TJ_jetPFOs_fromOverlay;
	std::vector<ReconstructedParticleVec> TJ_jetPFOs_allseen;

	for (int ijet=0 ; ijet< Njets_correct; ijet++ ) {
		streamlog_out(DEBUG5) << " Number of PFOs used : " << seen_partics(ijet).size()   << std::endl;
		if (  seen_partics(ijet).size() > 0 ) {
			ReconstructedParticleVec ijetPFOs_correct = seen_partics(ijet); // seen_partics(...) is function of TrueJet_Parser, see header
			TJ_jetPFOs_allseen.push_back(ijetPFOs_correct);

			// Jet types: 1 string, 2 lepton, 3 cluster, 4 ISR, 5 overlay.
			int jet_type =  type_jet(ijet);
			if ( (jet_type == 1) || (jet_type == 3) ) {
				TJ_jetPFOs_correct.push_back(ijetPFOs_correct);
			}
			else if ( jet_type == 4 ) {
				TJ_jetPFOs_fromISR.push_back(ijetPFOs_correct);
			}
			else if ( jet_type == 5 ) {
				TJ_jetPFOs_fromOverlay.push_back(ijetPFOs_correct);
			}
		}

	}


	// Relation between ReconstructedParticles and their corresponding MCParticles
	// -> tells you which MC Particles contribute to ReconstructedParticle 4momentum
	LCRelationNavigator* relation_recoMCtruth = new LCRelationNavigator( evt->getCollection( _recoMCTruthLink ) );


    // try to get lcio collection (exits if collection is not available which is good bc they definitely should exist!!!))
    LCCollection* colAllPFOs 			= evt->getCollection( _colAllPFOs 		) ;
	LCCollection* colFastJets 			= evt->getCollection( _colFastJets 	) ;
	LCCollection* colPFOsFromFastJet	= evt->getCollection( _colPFOsFromFastJet 	) ;
    LCCollection* colMC 				= evt->getCollection( _colMC 			) ;

    // Alternativelly if you do not want Marlin to exit in case of a non-existing collection
    // use the following (commented out) code:
    //LCCollection* col = NULL;
    //try{
    //    col = evt->getCollection( _colName );
    //}
    //catch( lcio::DataNotAvailableException e )
    //{
    //    streamlog_out(WARNING) << _colName << " collection not available" << std::endl;
    //    col = NULL;
    //}

	LCCollection* colIsoleps = NULL;
    try{
        colIsoleps = evt->getCollection( _colIsoleps );
    }
    catch( lcio::DataNotAvailableException e )
    {
        streamlog_out(WARNING) << _colIsoleps << " collection not available" << std::endl;
        colIsoleps = NULL;
    }


    // this will only be entered if the collection is available
    /**if( colMC != NULL ){
        int nMCP = colMC->getNumberOfElements()  ;
        for(int i=0; i< nMCP ; i++){
            MCParticle* p = dynamic_cast<MCParticle*>( colMC->getElementAt( i ) ) ;
        }
    }*/


	/** Call analysis script on event
 	*/

    //-- note: this will not be printed if compiled w/o MARLINDEBUG=1 !

    streamlog_out(DEBUG) << "   processing event: " << evt->getEventNumber()
       << "   in run:  " << evt->getRunNumber() << std::endl ;


	analyseEvent(	colMC, colAllPFOs, colFastJets, colPFOsFromFastJet, colIsoleps,
					TJ_jetPFOs_correct, TJ_jetPFOs_fromISR,
					relation_recoMCtruth,
					info);

	// Update array sizes for TTree
	info.findNs();

	/*// Test if matrices are okay
	for (int i=0; i<__max_Njets; i++) {
		if ( info.TJ_jetinfo.exists[i] ==0 ) {continue;}
		for(int j=0; j<__max_Nparticles; j++) {
			if ( ((int*)&info.TJ_jetinfo.jet_leptons.type_MCs.exists)[i*__max_Nparticles+j] == 0 ) {continue;}
			streamlog_out(WARNING) << "jet"<< i << " lep" << j << " " ;
		}
		streamlog_out(WARNING) << "\n next ";
	}*/


	_datatrain->Fill();

  if ( info.TJ_eventinfo.TJjets_from_initital_cn.pair1_mass > 200 && info.gen_level.eventType > 0) {
    streamlog_out(WARNING) << "High mass event, pair1mass: " << info.TJ_eventinfo.TJjets_from_initital_cn.pair1_mass << " evtN: " << _nEvt << std::endl;
  }

    _nEvt ++ ;
}



void JakobsVBSProcessor::check( LCEvent * evt ) {
    // nothing to check here - could be used to fill checkplots in reconstruction processor
}


void JakobsVBSProcessor::end(){

    _otfile->cd();
    _datatrain->Write();
    _otfile->Close();

}
