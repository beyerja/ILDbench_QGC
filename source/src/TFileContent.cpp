#include "aQGCObservablesProcessor.h"


//-------------------------------------------------------------------------------------------------

void aQGCObservablesProcessor::setObservablesBranches( TTree* tree ){
  
  // If new variable is added remember to clean it up in the CleanEvent function!!
  
  // Vector boson observables
  tree->Branch( "V1_type",      &m_V1_type,     "V1_type/I"); 
  tree->Branch( "V1_m",         &m_V1_m,        "V1_m/F");
  tree->Branch( "V1_pT",        &m_V1_pT,       "V1_pT/F");
  tree->Branch( "V1_cosTheta",  &m_V1_cosTheta, "V1_cosTheta/F");
  tree->Branch( "V2_type",      &m_V2_type,     "V2_type/I"); 
  tree->Branch( "V2_m",         &m_V2_m,        "V2_m/F");
  tree->Branch( "V2_pT",        &m_V2_pT,       "V2_pT/F");
  tree->Branch( "V2_cosTheta",  &m_V2_cosTheta, "V2_cosTheta/F");

  // Combined VB (= WW/ZZ) properties
  tree->Branch( "VV_m",   &m_VV_m,  "VV_m/F");
  tree->Branch( "VV_pT",  &m_VV_pT, "VV_pT/F");
  tree->Branch( "VV_ET",   &m_VV_ET,    "VV_ET/F" );
  tree->Branch( "m_recoil", &m_recoil_m,  "m_recoil/F" );

  // Jet property variables
  tree->Branch( "y_34",               &m_y_34,              "y_34/F");
  tree->Branch( "min_jetE",           &m_min_jetE,          "min_jetE/F"); // minimum over all four jets
  tree->Branch( "min_jetNparticles",  &m_min_jetNparticles, "min_jetNparticles/F");
  tree->Branch( "min_jetNcharged",    &m_min_jetNcharged,   "min_jetNcharged/F");
  
  // Properties of most energetic track
  tree->Branch( "leadEtrack_cosTheta",  &m_leadEtrack_cosTheta, "leadEtrack_cosTheta/F");
  tree->Branch( "leadEtrack_coneE",     &m_leadEtrack_coneE,    "leadEtrack_coneE/F");
  
  // Properties of most energetic track
  tree->Branch( "found_isolep",     &m_found_isolep,    "found_isolep/O"); // O means boolean (obviously :P)
  
  
  // Signal type branch, in this processor used for MC level, can later be used for cuts
  // 0 = not signal, 
  tree->Branch( "signal_type",     &m_signal_type,    "signal_type/I"); 
  
}

//-------------------------------------------------------------------------------------------------

void aQGCObservablesProcessor::setTTreeBranches() {
  this->setObservablesBranches(m_mctree);
  this->setObservablesBranches(m_recotree);
}

//-------------------------------------------------------------------------------------------------

void aQGCObservablesProcessor::setInputInfoStrings() {
  m_detector_model        = TNamed( "detector_model",       m_event->getDetectorName()  );
  m_process_name          = TNamed( "process_name",         m_event->getParameters().getStringVal( "processName" )                      );
  
  m_e_polarization        = TNamed( "e_polarization",       std::to_string(m_event->getParameters().getFloatVal( "Pol0" ))              );
  m_p_polarization        = TNamed( "p_polarization",       std::to_string(m_event->getParameters().getFloatVal( "Pol1" ))              );
  m_cross_section         = TNamed( "cross_section",        std::to_string(m_event->getParameters().getFloatVal( "crossSection" ))      );
  m_cross_section_error   = TNamed( "cross_section_error",  std::to_string(m_event->getParameters().getFloatVal( "crossSectionError" )) );
}

//-------------------------------------------------------------------------------------------------







