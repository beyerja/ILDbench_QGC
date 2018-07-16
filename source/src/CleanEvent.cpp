#include "aQGCObservablesProcessor.h"

void aQGCObservablesProcessor::CleanEvent() {

  m_V1_type     = 0;
  m_V1_m        = -999;
  m_V1_pT       = 0;
  m_V1_cosTheta = -999;

  m_V2_type     = 0;
  m_V2_m        = -999;
  m_V2_pT       = 0;
  m_V2_cosTheta = 999;

  m_V1_jet_absCosThetaStar = 999;
  m_V2_jet_absCosThetaStar = 999;

  m_VV_m  = -999;
  m_VV_pT = 0;
  m_VV_ET    = -999;
  m_recoil_m  = -999;
  
  m_VV_V_absCosThetaStar = 999;

  m_y_34              = -999;
  m_min_jetE          = -999;
  m_min_jetNparticles = -999;
  m_min_jetNcharged   = -999;

  m_leadEtrack_cosTheta = -999;
  m_leadEtrack_coneE    = -999;
  
  m_found_isolep = false;
  
  m_signal_type = 0;
}