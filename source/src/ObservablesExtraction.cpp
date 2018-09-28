#include "aQGCObservablesProcessor.h"

//-------------------------------------------------------------------------------------------------

void aQGCObservablesProcessor::findVectorBosonObservables( TLorentzVector &V1_jet1_tlv, TLorentzVector &V1_jet2_tlv, TLorentzVector &V2_jet1_tlv, TLorentzVector &V2_jet2_tlv  ) {
  // Single VB observables
  TLorentzVector V1_tlv = V1_jet1_tlv + V1_jet2_tlv;
  TLorentzVector V2_tlv = V2_jet1_tlv + V2_jet2_tlv;
  
  m_V1_m        = V1_tlv.M();
  m_V1_pT       = V1_tlv.Pt();
  m_V1_cosTheta = V1_tlv.CosTheta();
  
  m_V2_m        = V2_tlv.M();
  m_V2_pT       = V2_tlv.Pt();
  m_V2_cosTheta = V2_tlv.CosTheta();

  // Observables in boosted single V frame
  TLorentzVector V1_jet1_V1boosted_tlv = getBoostedVector( V1_jet1_tlv, V1_tlv );
  TLorentzVector V2_jet1_V2boosted_tlv = getBoostedVector( V2_jet1_tlv, V2_tlv );
  
  m_V1_jet_absCosThetaStar = fabs( V1_jet1_V1boosted_tlv.Angle( V1_tlv.Vect() ) );
  m_V2_jet_absCosThetaStar = fabs( V2_jet1_V2boosted_tlv.Angle( V2_tlv.Vect() ) );
  
  // Combined VB observables
  TLorentzVector VV_tlv = V1_tlv + V2_tlv;
  
  m_VV_m      = VV_tlv.M();
  m_VV_pT     = VV_tlv.Pt();
  m_VV_ET     = VV_tlv.Et();
  
  // Observables in boosted combined VV frame
  TLorentzVector VV_V1_VVboosted_tlv = getBoostedVector( V1_tlv, VV_tlv );
  m_VV_V_absCosThetaStar = fabs( VV_V1_VVboosted_tlv.Angle( VV_tlv.Vect() ) );
  
  // Observables from recoil against VBs
  TLorentzVector collision_system_tlv ( m_com_E*sin(0.014/2.0), 0, 0, m_com_E );
  TLorentzVector recoil_tlv  = collision_system_tlv - VV_tlv;
  m_recoil_m  = recoil_tlv.M();
}

//-------------------------------------------------------------------------------------------------
