#ifndef _aQGC_PROCESSOR_H_
#define _aQGC_PROCESSOR_H_ 1

#include "StandardIncludes.h"

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
  // Additional class member functions for analysis
  void setInputInfoStrings();
  void setObservablesBranches( TTree* tree );
  void setTTreeBranches();
  
  void CleanEvent();
  
  // Initialize your members in the class definition to 
  // be more efficient and avoid compiler warnings
  LCEvent*  m_event {};
  
  std::string         m_pfoCollectionName {};
  std::string         m_mcCollectionName {};
  
  TTree*              m_mctree {};
  TTree*              m_recotree {};
  
  TFile*              m_rootfile {};
  std::string         m_rootfilename {};
  
  TNamed  m_detector_model {};
  TNamed  m_e_polarization {};
  TNamed  m_p_polarization {};
  TNamed  m_process_name {};
  TNamed  m_cross_section {};
  TNamed  m_cross_section_error {};
  
  
  // Observables 
  float m_m_recoil {};
  float m_vis_ET {};
  float m_vis_pT {};
  
  int   m_V1_type {};
  float m_V1_m {};
  float m_V1_pT {};
  float m_V1_cosTheta {};
  
  int   m_V2_type {};
  float m_V2_m {};
  float m_V2_pT {};
  float m_V2_cosTheta {};
  
  float m_VV_m {};
  float m_VV_pT {};
  
  float m_y_34 {};
  float m_min_jetE {};
  float m_min_jetNparticles {};
  float m_min_jetNcharged {};
  
  float m_leadEtrack_cosTheta {};
  float m_leadEtrack_coneE {};
};

#endif