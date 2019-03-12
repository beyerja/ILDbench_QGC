#ifndef JakobsVBSProcessor_h
#define JakobsVBSProcessor_h 1

#include "marlin/Processor.h"
#include "lcio.h"
#include <string>
#include <vector>

//CUSTOM
#include "EventInformation.h"
#include "FileInformation.h"
//#include "jet_corrections.h"

//ROOT
#include "TFile.h"
#include "TTree.h"
#include "TMath.h"
#include "TVector3.h"
#include "TLorentzVector.h"

//LCIO
#include <EVENT/LCCollection.h>
#include <EVENT/MCParticle.h>
#include <EVENT/ReconstructedParticle.h>
#include <EVENT/Vertex.h>

// TrueJet_Parser can link generator level color singlets (quarks,gluons)
// to the jet particles they produced, thereby allowing comparison of
// correct jet with own clustered one.
#include "Adjusted_TrueJet_Parser.h"

using namespace lcio ;
using namespace marlin ;



class JakobsVBSProcessor : public Processor , public Adjusted_TrueJet_Parser {

 public:

  virtual Processor*  newProcessor() { return new JakobsVBSProcessor ; }


  JakobsVBSProcessor() ;

  /** Called at the begin of the job before anything is read.
   * Use to initialize the processor, e.g. book histograms.
   */
  virtual void init() ;

  virtual void makeNTuple() ;

  virtual void SetFileInfoBranches( FileInfo &file_info );
  virtual void SetGeneralEventInfoBranches( EventInfo &info );
  virtual void SetGeneratorLevelBranches( EventInfo &info );
  virtual void SetTrueJetLevelBranches( EventInfo &info );
  virtual void SetDetectorLevelBranches( EventInfo &info );
  virtual void CreateObservablesBranch( TTree* tree, Observables* observ_ptr, std::string branch_name );

  virtual void SetTJRecosInJetsBranches(EventInfo &info);
  virtual void SetTJMCsInJetsBranches(EventInfo &info);

  /** Called for every run.
   */
  virtual void processRunHeader( LCRunHeader* run ) ;

  /** Called for every event - the working horse.
   */
  virtual void processEvent( LCEvent * evt ) ;

    /** Subroutines
     */
    virtual void analyseEvent(LCCollection* colMC, LCCollection* colPFOs, LCCollection* colJets, LCCollection* colPFOsFromFastJet, LCCollection* colIsoLeps, std::vector<ReconstructedParticleVec> TJ_jetPFOs_correct, std::vector<ReconstructedParticleVec> TJ_jetPFOs_fromISR, LCRelationNavigator* relation_recoMCtruth, EventInfo &info);

	std::vector<MCParticle*> checkMCFinalState(LCCollection* AllMC);
	std::vector<ReconstructedParticle*> checkPOFinalState(LCCollection* AllPOs) ;
    std::vector<ReconstructedParticle*> checkPOParticleType(std::vector<ReconstructedParticle*> POs,int PDG) ;
    std::vector<ReconstructedParticle*> CheckPOParticlePairType( std::vector<ReconstructedParticle*> PFOs,int PDG, int counter_minus    , int counter_plus, int counter_pair) ;

	// TrueJet readout
	virtual std::vector<ReconstructedParticleVec> getTJAllJets();
	virtual std::vector<TLorentzVector*> getTJTrueValueTLVsOfAllJets();

	// Using Generator level information
	virtual std::vector<MCParticle*> findTrueGenLevelNeutrinos(std::vector<MCParticle*> MCparticles);
	virtual std::vector<MCParticle*> findTrueGenLevelQuarks(std::vector<MCParticle*> MCparticles);
	virtual	void findAllowedVVCandidates(std::vector<std::vector<std::vector<MCParticle*>>> all_VV_candidates, std::vector<std::vector<std::vector<MCParticle*>>> &possible_VV_candidates, std::string VV_name );
	virtual void findTrueEventType(std::vector<MCParticle*> MCparticles, EventInfo &info);
	virtual void analyseGeneratorLevelISR(std::vector<ReconstructedParticleVec> TJ_jetPFOs_fromISR, EventInfo &info);
	virtual void analyseGeneratorLevelQuarks(std::vector<MCParticle*> MCparticles, EventInfo &info);
	virtual void analyseGeneratorLevelNeutrinos(std::vector<MCParticle*> MCparticles, EventInfo &info);
	virtual void findBestGeneratorToRecoVPairing(EventInfo &info);

	template<typename LCIOParticleType> void fillVPairInformation( std::vector<LCIOParticleType*> V1, std::vector<LCIOParticleType*> V2, Observables &observ);
	template<typename LCIOParticleType>	void getAllVVTo4ParticleCombinations ( std::vector<LCIOParticleType*> particles, std::vector<std::vector<std::vector<LCIOParticleType*>>> &VV_candidates );
	template<typename LCIOParticleType>	void getVVCandidatesMasses( std::vector<std::vector<std::vector<LCIOParticleType*>>> VV_candidates, std::vector<std::vector<float>> &candidates_masses ) ;
	template<typename LCIOParticleType>	int findIndexBestPairInVVCandidates( std::vector<std::vector<std::vector<LCIOParticleType*>>> VV_candidates );

	template<typename LCIOParticleType> void findVpair(std::vector<LCIOParticleType*> jets, Observables &observ);

	virtual void calculateCutParameters(std::vector<ReconstructedParticle*> PFOs, std::vector<ReconstructedParticle*> jets,	EventInfo &info);

	// Subroutines using TrueJet information
	template <typename Type> void findVectorDifferences(	std::vector<Type> vector1, std::vector<Type> vector2, std::vector<Type> &out_in1notin2, std::vector<Type> &out_in2notin1 );
	virtual void compareActualToClusteredJets( std::vector<ReconstructedParticle*> jets_clustered, std::vector<ReconstructedParticleVec> TJ_jetPFOs_correct, EventInfo &info );
	virtual void LevelComparisonIndividualJets(LCRelationNavigator* relation_recoMCtruth, EventInfo &info);

	virtual void SplitWeight( FloatVec &combined_weights, FloatVec &single_weights, std::string weight_name );
	virtual void GetMCParticlesLinkedToPFO( ReconstructedParticle* PFO, LCRelationNavigator* relation_recoMCtruth, MCParticleVec &MCParticle_vector );

	virtual void SearchSingleTJJetMC( MCParticle* MC, LCRelationNavigator* relation_recoMCtruth,  TrueJetSingleJetInfo &TJ_jetinfo );
	virtual void FindLowestLevelRecoParticles( ReconstructedParticle* reco, std::set<ReconstructedParticle*> &jet_recos_set );
	virtual void SearchSingleTJJetPFO( ReconstructedParticle* PFO, LCRelationNavigator* relation_recoMCtruth, std::set<MCParticle*> &jet_MCs_set, TrueJetSingleJetInfo &TJ_jetinfo );
	virtual void SearchSingleTJJetParticles( LCRelationNavigator* relation_recoMCtruth, TrueJetSingleJetInfo &TJ_jetinfo );
	virtual void FindLowestLevelMCParticles( MCParticle* mc, std::set<MCParticle*> &jet_MCs_set );
	virtual bool IsIntTypePDGID( int pdgID, float charge, std::string type );
	virtual bool IdentifiedAsType( ReconstructedParticle* PFO, std::string type );
	virtual bool MCParticleIsType( MCParticle* MCparticle, std::string type );
	virtual int GetIndexHighestWeightTypeMC( LCObjectVec &MCsToThisPFO, FloatVec &MCweightsToThisPFO, std::string );
	virtual int GetIndexHighestWeightReco( FloatVec &PFOweightsToThisMC);
	virtual void TJProcessSingleTypePFO( ReconstructedParticle* PFO, LCRelationNavigator* relation_recoMCtruth, JetParticleTypeInfo &type_particles ) ;
	virtual void TJProcessSingleTypeMC( MCParticle* MC, LCRelationNavigator* relation_recoMCtruth, JetParticleTypeInfo &type_particles ) ;

	virtual void calculateTJTotalLevel( ReconstructedParticleVec* particles, TJLevelInfo &level_info );
	virtual void calculateTJTotalLevels( EventInfo &info );

  void TJGetInitialCNRecoMasses(EventInfo &info); 
  void TJGetCustomPairingRecoMasses(EventInfo &info); 

  /** For TrueJet_Parser -> see its documentation
  */
  std::string get_recoMCTruthLink(){ return _recoMCTruthLink  ; } ;

  virtual void check( LCEvent * evt ) ;


  /** Called after data processing for clean up.
   */
  virtual void end() ;


  /**
  General Helper Functions
  */

  struct compareSecondArgument;

  template<class Type> std::vector<int> FindCommonParticleIndices( std::vector<Type> v1, std::vector<Type> v2 );
  template<class Type> std::vector<ReconstructedParticle*> RecosFromLCIOVec ( Type &LCIOVec );

 protected:

  int __max_Njets = 20;
  int __Njets = 4;
  int __max_Nparticles = 50;

  /** Input collection name.
   */
  std::string _colAllPFOs;
  std::string _colFastJets;
  std::string _colPFOsFromFastJet;
  std::string _colIsoleps;
  std::string _colSLDecays;
  std::string _colMC;

  std::string _mcpfoRelation;

  // Input collections for TrueJet
  std::string _recoMCTruthLink;

  // This are NOT declared here! They are already declared in TrueJet_Parser.h and repeated delaration will lead to error!
  //std::string _trueJetCollectionName;
  //std::string _finalColourNeutralCollectionName;
  //std::string _initialColourNeutralCollectionName;
  //std::string _trueJetPFOLink;
  //std::string _trueJetMCParticleLink;
  //std::string _initialElementonLink;
  //std::string _finalElementonLink;
  //std::string _finalColourNeutralLink;
  //std::string _initialColourNeutralLink;


  //std::string get_recoMCTruthLink(){ return _recoMCTruthLink  ; } ;

  /** Parameters during Marlin analysis
   */

  int _nRun ;
  int _nEvt ;

  EventInfo info;
  FileInfo file_info;
  
  // tj is a pointer to a Trujet_Parser, with the data of this processor object:
  Adjusted_TrueJet_Parser* tj {};

  /** Output File specifics
   */
  TFile* _otfile;
  TTree* _datatrain;
  //TTree* _TJRecoLeptonsInJets_tree;
  //TTree* _TJMCLeptonsInJets_tree;

  std::string _rootfilename;
} ;

#include "VpairFinder.h"  // This needs to be at end of this header, otherwise template functionality doesn
#include "LCIOHelpers.h"
#endif
