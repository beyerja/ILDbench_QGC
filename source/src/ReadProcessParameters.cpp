#include "aQGCObservablesProcessor.h"

//-------------------------------------------------------------------------------------------------

void aQGCObservablesProcessor::readInputInfo(){
  m_detector_model        = m_event->getDetectorName();
  m_process_name          = m_event->getParameters().getStringVal( "processName" );

  m_e_polarization        = m_event->getParameters().getFloatVal( "Pol0" );
  m_p_polarization        = m_event->getParameters().getFloatVal( "Pol1" );
  m_cross_section         = m_event->getParameters().getFloatVal( "crossSection" );
  m_cross_section_error   = m_event->getParameters().getFloatVal( "crossSectionError" );
  
  m_processinfotree->Fill();
}

//-------------------------------------------------------------------------------------------------