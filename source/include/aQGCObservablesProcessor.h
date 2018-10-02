#ifndef _aQGC_PROCESSOR_H_
#define _aQGC_PROCESSOR_H_ 1

#include "StandardIncludes.h"
#include "VBPairFindingTemplates.h"
#include "JetClusteringTemplate.h"



class aQGCObservablesProcessor : public Processor {
public:
  /**
   *  @brief  Factory method to create a processor
   *
   *  This method is called internally by Marlin to create an instance of your processor
   */
  Processor* newProcessor() { return new aQGCObservablesProcessor(); }
  
  /**
   *  @brief  Constructor
   *
   *  Regiter your parameters in there. See aQGCObservablesProcessor.cc
   */
  aQGCObservablesProcessor();
  
  // These two lines avoid frequent compiler warnings when using -Weffc++
  aQGCObservablesProcessor( const aQGCObservablesProcessor& ) = delete;
  aQGCObservablesProcessor& operator=( const aQGCObservablesProcessor& ) = delete;
  
  /** 
   *  @brief  Called once at the begin of the job before anything is read.
   *  
   *  Use to initialize the processor, e.g. book histograms. The parameters that
   *  you have registered in the constructor are initialized
   */
  void init();

  /** 
   *  Called for every run read from the file. 
   *  
   *  If you use simulation data output from ddsim (e.g from production data on grid),
   *  this will be called before the first event and only once in the job.
   */
  void processRunHeader( EVENT::LCRunHeader* run );
  
  /** 
   *  @brief  Called for every event - the working horse.
   *
   *  Analyze your reconstructed particle objects from the event object here.
   *  See aQGCObservablesProcessor.cc for an example
   */
  void processEvent( EVENT::LCEvent * event );
  
  /** 
   *  @brief  Called after data processing for clean up.
   *
   *  You have allocated memory somewhere in your code ? 
   *  This is the best place to clean your mess !
   */
  void end();
  
private:
  // Typedefs for easier readability
  typedef std::set<MCParticle*> MCParticleSet;
  
  // Additional class member functions for analysis
  void setObservablesBranches( TTree* tree );
  void setInputInfoBranches( TTree* tree );
  void setTTreeBranches();
  
  void readInputInfo();
  
  void CleanEvent();

  TLorentzVector getBoostedVector( TLorentzVector &to_boost, TLorentzVector &boost_system );
  
  void getMCParticleVector( MCParticleVec &output_vector );
  MCParticleVec findLastInitialee( MCParticleVec &mc_particles );
  MCParticleVec findInitial4q2nu( MCParticleVec &mc_particles );
  MCParticleVec getQuarks( MCParticleVec &mc_particles );
  MCParticleVec getNeutrinos( MCParticleVec &mc_particles );
  MCParticleVec getChargedLeptons( MCParticleVec &mc_particles );
  MCParticleVec findVisibleMC( MCParticleVec &mc_particles );
  void calculateMCObservables( MCParticleVec &mc_particles );
  void analyseMCObservables();
  void calculateMCTruth( MCParticleVec &mc_particles );
  void analyseMCTruth();
  
  ReconstructedParticleVec getRecoCollectionVector( std::string collectionName );
  void analyseReconstructed();
  
  
  // Member function templates -> definitions need to be included at end of this file
  template <class ParticleClass> void findVectorBosonObservables( VectorBosonPairFinder<ParticleClass> &VBpair_finder );
  template <class ParticleClass> void findJetObservables( std::vector<ParticleClass*> &jet_vector );
  template <class ParticleClass> void findParticleObservables( std::vector<ParticleClass*> &particle_vector );

  
  // Initialize your members in the class definition to 
  // be more efficient and avoid compiler warnings
  LCEvent*  m_event {};
  
  std::string         m_pfoCollectionName {};
  std::string         m_mcCollectionName {};
  std::string         m_jetsCollectionName {};
  std::string         m_jetPFOsCollectionName {};
  std::string         m_isolatedLeptonsCollectionName {};
  
  TTree*              m_processinfotree {};
  TTree*              m_truthtree {};
  TTree*              m_mctree {};
  TTree*              m_recotree {};
  
  TFile*              m_rootfile {};
  std::string         m_rootfilename {};
  
  // Process information
  
  TString  m_detector_model {};
  TString  m_process_name {};
  float  m_e_polarization {};
  float  m_p_polarization {};
  float  m_cross_section {};
  float  m_cross_section_error {};
  
  float m_com_E {};
  
  // Observables 
  
  int   m_V1_type {};
  float m_V1_m {};
  float m_V1_pT {};
  float m_V1_cosTheta {};
  
  int   m_V2_type {};
  float m_V2_m {};
  float m_V2_pT {};
  float m_V2_cosTheta {};
  
  float m_V1_jet_absCosThetaStar {};
  float m_V2_jet_absCosThetaStar {};
  
  float m_VV_m {};
  float m_VV_pT {};
  float m_VV_ET {};
  float m_recoil_m {};
  
  float m_VV_V_absCosThetaStar {};
  
  float m_y_34 {};
  float m_min_jetE {};
  float m_min_jetNparticles {};
  float m_min_jetNcharged {};
  
  float m_leadEtrack_cosTheta {};
  float m_leadEtrack_coneE {};
  
  bool  m_found_isolep {};
  
  int   m_signal_type {};
};

// Include here headers with definitions of templates
#include "ObservableExtractionTemplates.tpp"

#endif
